#include "drawmaster.h"
#include "ui_drawmaster.h"

#include <QPainter>
#include <QtMath>
#include <QDateTime>
#include <QWheelEvent>

#include "zxstockopt.h"
#include "infomaster.h"
#include "stockadddialog.h"
#include "quoteformat.h"

#include <datactrldef.h>
#include <datamodeldefs/stockinfodef.h>
#include <hzlog.h>

#define PAGE_WIDTH          570
#define PAGE_ITEM_HEIGHT    28
#define OFFSET_X            78
#define OFFSET_Y            10
#define OFFSET_RIGHT_X      6

#define TITLE_RADIUS_VALUE  2
#define ROOT_RADIUS_VALUE   14
#define ROOT_WIDTH_VALUE    96
#define ROOT_HEIGHT_VALUE   25

/*
 * 模板：
 * 股票名称 实时涨跌幅 龙虎榜
   股票名称因“XXX”上榜，成交额：XX，合计买入：XX，合计卖出：XX，净额：XX
*/
#define LongHuBang_Title_Plate      "%1 %2 龙虎榜"
#define LongHuBang_Content_Plate    "%1因 \"%2\" 上榜，成交额：%3元，合计买入：%4元，合计卖出：%5元，净额：%6元"

/*
 *--除权除息(分红送转) （当天发生的）
    股票名称 实时涨跌幅 除权除息
    股票名称今日除权除息，每X股派X元
  --除权除息(分红送转) （将要除权除息）
    股票名称 实时涨跌幅 将要除权除息
    股票名称将于X除权除息，每X股派X元
*/
#define FHSZ_Title_GQ_Already_Plate "%1 %2 股权登记"
#define FHSZ_Title_GQ_Will_Plate    "%1 %2 将要股权登记"
#define FHSZ_Title_CQ_Already_Plate "%1 %2 除权除息"
#define FHSZ_Title_CQ_Will_Plate    "%1 %2 将要除权除息"

#define FHSZ_Content_GQ_Already_Plate   "%1 %2今日股权登记，%3"
#define FHSZ_Content_GQ_Will_Plate      "%1将于%2股权登记，%3"
#define FHSZ_Content_CQ_Already_Plate   "%1 %2今日除权除息，%3"
#define FHSZ_Content_CQ_Will_Plate      "%1将于%2除权除息，%3"

/*
--财报（当天发布的）
股票名称 实时涨跌幅 财报发布
股票名称 今日发布财报，财报详情>>(其中“财报详情>>”有数据时，即展示，无数据时则不展示“财报详情>>”，点击“财报详情”即进入财报详情页)

--财报(将要发布财报)
股票名称 实时涨跌幅 将要发布财报
股票名称将于X发布财报
*/
#define CB_Title_CurTime_Plate      "%1 %2 财报发布"
#define CB_Content_CurTime_Plate    "%1 今日发布财报"
#define CB_Title_PreTime_Plate      "%1 %2 将要发布财报"
#define CB_Content_PreTime_Plate    "%1将于%2发布财报"

/** 近30分钟内主动净买 0
股票名称  实时涨跌幅 近30分钟内主动净买
股票名称 近30分钟内出现主动净买，成交额为XX元，发生时股价为XX元
eg:
浦发银行  +1.53% 近30分钟内主动净买
浦发银行 近30分钟内出现主动净买，成交额为223万元，发生时股价为23.30元
*/
#define MB_ThirtyMinBuy_Plate "%1 近30分钟内出现主动净买，成交额为%2元，发生时股价为%3元"

/* 近30分钟内主动净卖 1
股票名称  实时涨跌幅 近30分钟内主动净卖
股票名称 近30分钟内出现主动净卖，成交额为XX元，发生时股价为XX元
eg:
浦发银行  -1.53% 近30分钟内主动净卖
浦发银行 近30分钟内出现主动净卖，成交额为223万元，发生时股价为23.30元
*/
#define MB_ThirtyMinSell_Plate "%1 近30分钟内出现主动净卖，成交额为%2元，发生时股价为%3元"

/** 超大买单 2
股票名称  实时涨跌幅 超大买单
股票名称 出现超大买单，成交量为XX万股，成交额为XX万元，发生时股价为XX元
eg:
上海上港 +1.82% 超大买单
上海上港出现超大买单，成交量为51万股，成交额为321万元，发生时股价为25.20元
*/
#define MB_MoneyMaxOrdBuy_Plate "%1 出现超大买单，成交量为%2股，成交额为%3元，发生时股价为%4元"

/** 大买单 3
股票名称  实时涨跌幅 大买单
股票名称 出现大买单，成交量为XX万股，成交额为XX万元，发生时股价为XX元
eg:
上海上港 +1.11% 大买单
上海上港 出现大买单，成交量为20万股，成交额为99万元，发生时股价为25.20元
*/
#define MB_MoneyBigOrdBuy_Plate "%1 出现大买单，成交量为%2股，成交额为%3元，发生时股价为%4元"

/** 超大卖单 4
股票名称  实时涨跌幅 超大卖单
股票名称 出现超大卖单，成交量为XX万股，成交额为XX万元，发生时股价为XX元
eg:
超频三 -2.31% 超大卖单
超频三出现超大卖单，成交量为51万股，成交额为256万元，发生时股价为18.23元
*/
#define MB_MoneyMaxOrdSell_Plate "%1 出现超大卖单，成交量为%2股，成交额为%3元，发生时股价为%4元"

/** 大卖单 5
股票名称  实时涨跌幅 大卖单
股票名称 出现大卖单，成交量为XX万股，成交额为XX万元，发生时股价为XX元
eg:
超频三 -2.31% 大卖单
超频三出现大卖单，成交量为19万股，成交额为89万元，发生时股价为18.68元
*/
#define MB_MoneyBigOrdSell_Plate "%1 出现大卖单，成交量为%2股，成交额为%3元，发生时股价为%4元"

/** 涨幅 6
股票名称  实时涨跌幅 涨幅
股票名称于HH:MM涨幅+XX%，发生时股价为XX元
eg:
中国软件 +5.35%涨幅
中国软件于10:38涨幅+5.00%，股价为27.80元
*/
#define MB_UpFiveRate_Plate "%1于%2涨幅%3%，发生时股价为%4元"

/** 跌幅 7
股票名称  实时涨跌幅 跌幅
股票名称于HH:MM跌幅-XX%，发生时股价为XX元
eg:
中国软件 -5.21%跌幅
中国软件于10:37跌幅-5.00%，股价为21.22元
*/
#define MB_DownFiveRate_Plate "%1于%2跌幅%3%，发生时股价为%4元"

/** 涨停 8
股票名称  实时涨跌幅 涨停
股票名称于HH:MM涨停+XX%，发生时股价为XX元
eg:
华微电子 +10.02% 涨停
华微电子于14:32涨停，涨幅+10.21%，股价为22.28元
*/
#define MB_UpLimitRate_Plate "%1于%2涨停%3%，发生时股价为%4元"

/** 跌停 9
股票名称  实时涨跌幅 跌停
股票名称于HH:MM跌停-XX%，发生时股价为XX元
eg:
华微电子 -9.99% 跌停
华微电子于14:25跌停，跌幅-9.99%，股价为19.21元
*/
#define MB_DownLimitRate_Plate "%1于%2跌停%3%，发生时股价为%4元"

/** 打开涨停 10
股票名称  实时涨跌幅 打开涨停
股票名称于HH:MM打开涨停+XX%，发生时股价为XX元
eg:
潍柴动力 +9.35% 打开涨停
潍柴动力于14:25打开涨停，涨幅下落至+9.37%，发生时股价为19.21元
*/
#define MB_OpenUpLimitRate_Plate "%1于%2打开涨停%3%，发生时股价为%4元"

/** 打开跌停 11
股票名称  实时涨跌幅 打开跌停
股票名称于HH:MM打开跌停-XX%，发生时股价为XX元
eg:
潍柴动力 -9.35% 打开跌停
潍柴动力于14:25打开跌停，跌幅降至-9.37%，发生时股价为19.21元
*/
#define MB_OpenDownLimitRate_Plate "%1于%2打开跌停%3%，发生时股价为%4元"

/**
 * @brief FormatTime 格式化时间
 * @param time
 * @return
 */
static QString FormatTime(quint64 time)
{
    QString timeStr = QString::number(time);
    QDate dt = QDateTime::fromString(timeStr,"yyyyMMdd").date();
    if(dt.isValid()) {
        return QString("%1年%2月%3日")
                .arg(dt.year()).arg(dt.month()).arg(dt.day());
    }

    return timeStr;
}

/**
 * @brief FormatItemTag 格式化TAG
 * @param id
 * @return
 */
static QString FormatItemTag(int id)
{
    switch(id) {
    case 0: return QStringLiteral("近30分钟主动净买");
    case 1: return QStringLiteral("近30分钟主动净卖");
    case 2: return QStringLiteral("超大买单");
    case 3: return QStringLiteral("大买单");
    case 4: return QStringLiteral("超大卖单");
    case 5: return QStringLiteral("大卖单");
    case 6: return QStringLiteral("涨幅");
    case 7: return QStringLiteral("跌幅");
    case 8: return QStringLiteral("涨停");
    case 9: return QStringLiteral("跌停");
    case 10: return QStringLiteral("打开涨停");
    case 11: return QStringLiteral("打开跌停");
    }

    return "";
}

///
/// \brief DrawMaster::DrawMaster
/// \param pageId
/// \param parent
///
DrawMaster::DrawMaster(int pageId, QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::DrawMaster)
  ,m_pageId(pageId)
  ,m_drawPanel(new DrawPanel(pageId))
  ,m_scrollBar(new QScrollBar(Qt::Horizontal))
{
    setWindowFlags(Qt::SubWindow);

    ui->setupUi(this);
    m_scrollBar->setObjectName("HZSmartPanelScroll");
    m_activeStock = ZXStockOpt::instance()->getZXStock().isEmpty()?"":ZXStockOpt::instance()->getZXStock().first();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(m_drawPanel);
    vLayout->addWidget(m_scrollBar);
    vLayout->setStretch(0,10);
    ui->verticalLayout->addLayout(vLayout);

    connect(m_scrollBar, &QScrollBar::valueChanged, [&](int value){
        //滚动到最后时，分页请求
        if(value == m_scrollBar->maximum()) {
//            m_reqMax = 200;
//            subMidAbnormal(m_activeStock.toStdString());
//            qDebug() <<  Q_FUNC_INFO << m_reqMax;
        }
        m_drawPanel->slotScrollChanged(value);
    });
    connect(m_drawPanel, &DrawPanel::signalStockCodeClicked, this, &DrawMaster::signalStockCodeClicked);
    connect(m_drawPanel, &DrawPanel::signalDataChanged, [&](int value){
        m_itemCnt = value;
        updateScroll();
    });
    connect(m_drawPanel, &DrawPanel::signalAddCustomStock, [=]{
        StockAddDialog dlg;
        dlg.exec();
        //gInfoStyle->signalUpdateCustomStockCache();
        startReq();
    });

    updateScroll();
    resize(900,550);
}

DrawMaster::~DrawMaster()
{
    delete ui;
}

void DrawMaster::updateScroll()
{
    int pageItems = (m_drawPanel->height()-OFFSET_Y)/PAGE_ITEM_HEIGHT;
    int maxPageCnt = m_itemCnt/pageItems+1;
    int maxValue = (maxPageCnt)*PAGE_WIDTH-m_drawPanel->width();
    m_scrollBar->setPageStep(PAGE_WIDTH);
    m_scrollBar->setRange(1,maxValue);
    update();
    //qDebug() << Q_FUNC_INFO << "page item cnt:" << pageItems << "item cnt:" << mItemCnt << "page cnt:" << maxPageCnt << "max value:" << maxValue;
}

void DrawMaster::afterActive()
{
    afterDeactive();
    startReq(m_activeStock);
}

void DrawMaster::afterDeactive()
{
    if(m_pageId == CompanyEvent_Page) {
        if(m_reqId != -1)
            HzInfoCenter::getInstance()->cancle(m_reqId);
    }
    else if(m_pageId == SmartPlate_Page)
    {
        gHZDataController->cancelSub(m_abnormalReqId);
    }
}

void DrawMaster::slotPanelDataChanged(int count)
{
    //TODO
}

void DrawMaster::slotZXStockClicked(const QString &stockcode)
{
    qDebug() << Q_FUNC_INFO << "pageId:" << m_pageId << "stock:" << stockcode;

    if(m_pageId == CompanyEvent_Page) {
        m_activeStock = stockcode;
        startReq();
    } else if(m_pageId == SmartPlate_Page){
        gHZDataController->cancelSub(m_abnormalReqId);
        m_normalInfo = HZData::MidAbnormalInfo();
        startReq(stockcode);
    }
}

void DrawMaster::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateScroll();
}

void DrawMaster::wheelEvent(QWheelEvent *event)
{
    if(m_scrollBar == nullptr) return;
    int numberDegrees = event->delta() / 8;
    int numberSteps = numberDegrees / 15;
    int value = m_scrollBar->value();
    m_scrollBar->setValue(value + numberSteps * PAGE_WIDTH);//滚动一ye
    event->accept();
}


void DrawMaster::startReq(const QString &stockcode)
{
    QVector<QString> stocks = ZXStockOpt::instance()->getZXStock();
    if(stocks.isEmpty()) {//表示自选股列表为空
        QVariantMap tmpMap;
        tmpMap["code"] = -2;//表示自选股列表为空
        tmpMap["data"] = (m_pageId==CompanyEvent_Page)?
                    QVariant::fromValue(CompanyOptionEvents()) :
                    QVariant::fromValue(HZData::MidAbnormalInfo());

        m_drawPanel->setData(tmpMap);
        return;
    }

    if(m_pageId ==SmartPlate_Page) {
        /**
         * @brief req 首次取最大条数，服务器上有多少则返回多少，模拟订阅时取20条
         */
        m_normalInfo = HZData::MidAbnormalInfo();
        qDebug()<< Q_FUNC_INFO << stockcode << m_activeStock;
        if(stockcode != m_activeStock){
            m_activeStock = stockcode;
            QVariantMap tmpMap;
            tmpMap["code"] = 0;
            tmpMap["data"] = QVariant::fromValue(m_normalInfo);
            m_curPage = -1;
            m_drawPanel->setData(tmpMap);
            updateScroll();
        }

        HZData::ReqMidAbnormalInfo req;
        req.index = -1;
        req.count = INT_MAX;
        req.stock_code = m_activeStock.toStdString();
        OnMidAbnormalArrived fun = std::bind(&DrawMaster::panelAbnormalCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        m_abnormalReqId = gHZDataController->requestMidAbnormalInfo(req,fun);
    } else if(m_pageId ==CompanyEvent_Page){
        stocks.clear();
        stocks.push_back(m_activeStock);
        CompanyOptionEventsCallBack fun5 = std::bind(&DrawMaster::companyOptionEventCallBack, this, std::placeholders::_1, std::placeholders::_2);
        m_reqId = HzInfoCenter::getInstance()->reqCompanyOptionEvents(stocks,2,0,fun5);
    }
}

/**
 * @brief DrawMaster::companyOptionEventCallBack 公司大事首次获取数据回调处理
 * @param ret
 * @param data
 */
void DrawMaster::companyOptionEventCallBack(const InfoRet &ret, const CompanyOptionEvents &data)
{
    QVariantMap tmpMap;
    tmpMap["code"] = ret.code;
    tmpMap["data"] = QVariant::fromValue(data);
    m_drawPanel->setData(tmpMap);
    startReqSnapShot(data);
    updateScroll();
}

/**
 * @brief DrawMaster::panelAbnormalCallBack 盘中异动首次获取数据回调处理
 * @param ret
 * @param data
 * @param err
 */
void DrawMaster::panelAbnormalCallBack(ReqId ret,const HZData::MidAbnormalInfo& data,ErrCode err)
{
    m_normalInfo = HZData::MidAbnormalInfo();
    QVariantMap tmpMap;
    tmpMap["code"] = err;
    tmpMap["data"] = QVariant::fromValue(data);
    m_drawPanel->setData(tmpMap);
    m_normalInfo = data;
    updateScroll();

    //首次获取的数据，提取股票代码并订阅，保证数据的实时性
    if(!m_activeStock.isEmpty())
        subMidAbnormal(m_activeStock.toStdString());
}

/**
 * @brief DrawMaster::subMidAbnormal 盘中异动订阅数据
 * @param stocks
 * note:
 * 若自选股列表中已存在股票代码，那么第一次从列表中去一支代码并调用函数，
 * 之后自选股列表被点击一次，就开始订阅该支股票
 */
void DrawMaster::subMidAbnormal(const std::string &stockcode)
{
    if(m_abnormalReqId != -1)
        gHZDataController->cancelSub(m_abnormalReqId);

    if(stockcode.size() == 0)
        return;

    HZData::ReqMidAbnormalInfo req;
    req.stock_code = stockcode;
    req.index = m_curPage;
    req.direction = 0;
    req.count = m_reqMax;//模拟订阅时取20条
    OnMidAbnormalArrived fun = std::bind(&DrawMaster::subAbnormalCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
    m_abnormalReqId = gHZDataController->subMidAbnormalInfo(req, fun);
}

void DrawMaster::subAbnormalCallBack(ReqId ret,const HZData::MidAbnormalInfo& data,ErrCode err)
{
    if(err == 0) {
        if(m_normalInfo.midAbnormals.count() < data.totalCount)
        {
            for(int i=0; i<data.midAbnormals.count(); ++i){
                m_normalInfo.midAbnormals.append(data.midAbnormals[i]);
            }
            m_normalInfo.totalCount += data.midAbnormals.count();

            QVariantMap tmpMap;
            tmpMap["code"] = err;
            tmpMap["data"] = QVariant::fromValue(m_normalInfo);
            m_curPage = -1;
            m_drawPanel->setData(tmpMap);
            updateScroll();
        }
    }
}

void DrawMaster::startReqSnapShot(const CompanyOptionEvents &datas)
{
    if(datas.isEmpty()) return;
    m_srcData = datas;
    QList<QString> lst;
    int i=0;
    for(const CompanyOptionEvent &ev:datas) {
#if 1
        lst.append(ev.stockcode);
#else
        QString vv = ev.stockcode;
        if(i == 0)
            vv.replace("sz","");
        lst.append(vv);
        qDebug() << "vv:" << ev.stockcode;

        i++;
#endif
    }

    reqSnapShot(lst);
}

void DrawMaster::reqSnapShot(const QList<QString> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i) stdlst.push_back(stocks[i].toStdString());
    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::SLStock_Mask;
    auto fun = std::bind(&DrawMaster::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void DrawMaster::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(err != 0) {
        qDebug() << Q_FUNC_INFO << "error:" << err;
        return;
    }

    HZData::SLStock *slStocls = (HZData::SLStock *)data;
    if(slStocls) {
        for(int i=0; i<num; i++){
            QString code = QString::fromStdString(slStocls[i].stock_code);
            for(int i=0; i<m_srcData.count(); ++i) {
                if(m_srcData[i].stockcode == code) {
                    m_srcData[i].change_ratio = slStocls[i].rate;
                }
            }
        }
    }

    QVariantMap tmpMap;
    tmpMap["code"] = 0;
    tmpMap["data"] = QVariant::fromValue(m_srcData);
    m_drawPanel->setData(tmpMap);
}

////
/// \brief DrawPanel::setData
/// \param data
///
void DrawPanel::setData(const QVariantMap &map)
{
    int code = map["code"].toInt();
    if(code == -2) {
        setPanelItems(QVector<PanelItem>{});
        m_dataFlags = Custom_Stock_Null;
        update();
        return;
    }

    if(code != 0) { //code == -1
        m_dataFlags = Request_Exception;
        setPanelItems(QVector<PanelItem>{});
        m_tipText = QStringLiteral("操作失败，未知错误");
        m_tipUrl = ":/skin/default/img/infopage/NEWS02.png";
        update();
        return;
    }else {
        if(m_pageId == CompanyEvent_Page) {
            CompanyOptionEvents data= map["data"].value<QVector<CompanyOptionEvent>>();
            companyOptionEventPageDataDisplay(code,data);
        }
        else if(m_pageId == SmartPlate_Page)
        {
            HZData::MidAbnormalInfo data = map["data"].value<HZData::MidAbnormalInfo>();
            midAbnormalInfoPageDataDisplay(code,data);
        }
    }
}

void DrawPanel::setPanelItems(const QVector<PanelItem> &items)
{
    m_panalItems = items;
    emit signalDataChanged(items.count());
}

void DrawPanel::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    //updateScrollRegion();
}


void DrawPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    switch(m_dataFlags)
    {
    case Custom_Stock_Null:{//        //弹出 '+' 添加自选股
        QPainter p(this);
        p.fillRect(rect(), gInfoStyle->m_invalidBrush);
        p.setFont(gInfoStyle->m_invalidFont);

        QString text(QStringLiteral("添加自选股"));
        int textLen = p.fontMetrics().width(text);
        int textH = p.fontMetrics().height();
        int w = width()/2, h = height()/2;
        QRect rt(w-83/2, h-83/2, 83, 83);//83 83
        bool isHover = rt.contains(m_pressPos);
        p.setPen(QPen(isHover?QColor("#1d3350"):QColor("#2f2f2f"),2));
        p.drawRoundedRect(rt,5,5);
        p.setPen(QPen(isHover?QColor("#4f9bff"):QColor("#2f2f2f"),2));
        p.drawLine(QPoint(rt.x()+rt.width()/2, rt.y()+24), QPoint(rt.x()+rt.width()/2, rt.y()+rt.height()-24));
        p.drawLine(QPoint(rt.x()+24, rt.y()+rt.height()/2), QPoint(rt.x()+rt.width()-24, rt.y()+rt.height()/2));

        QRect textRt(w-textLen/2, rt.y()+rt.height()+17, textLen, textH);
        p.setPen(QColor("#797979"));
        p.drawText(textRt, Qt::AlignCenter, text);
        m_addRect = rt;
    }break;
    case Request_Null:
    case Request_Exception:{
        QPainter p(this);
        p.fillRect(rect(), gInfoStyle->m_invalidBrush);
        p.setFont(gInfoStyle->m_invalidFont);
        int textLen = p.fontMetrics().width(m_tipText);
        int textH = p.fontMetrics().height();
        int w = width()/2, h = height()/2;
        QRect rt(w-73/2, h-69/2, 73, 62);//83 83
        QPixmap pix(m_tipUrl);
        if(!pix.isNull()){
            p.drawPixmap(rt, pix);
            QRect textRt(w-textLen/2, rt.y()+rt.height()+17, textLen, textH);
            p.setPen(gInfoStyle->m_invalidPen);
            p.drawText(textRt, Qt::AlignCenter, m_tipText);
        }
    }break;
    case Requst_Normal:{
        QPainter painter(this);
        painter.fillRect(rect(),gInfoStyle->m_marketCldDetailBrush);
        painter.fillRect(rect(), QColor("#191919"));
        m_itemIndexs.clear();
        int hg(height()), wdt(width());
        int pagew = (wdt*1./PAGE_WIDTH)+1;
        int start = ((m_scrollValue)*1./PAGE_WIDTH);
        for(int p=start; p<start+pagew; ++p){
            int xPos    = p * PAGE_WIDTH - m_scrollValue;
            int itemCnt = (hg-OFFSET_Y)/PAGE_ITEM_HEIGHT;
            QRect rt(xPos, OFFSET_Y, PAGE_WIDTH, hg);

            painter.setPen(gDrawStyle->m_linePen);
            painter.drawLine(rt.x()+OFFSET_X, 0, rt.x()+OFFSET_X, hg);
            painter.setPen(gDrawStyle->m_splitLine);
            painter.drawLine(rt.x()+PAGE_WIDTH, 0, rt.x()+PAGE_WIDTH, hg);
            for(int i=0; i<itemCnt; ++i){
                QRect txRt(rt.x()+OFFSET_X, i*PAGE_ITEM_HEIGHT+OFFSET_Y, PAGE_WIDTH-OFFSET_X, PAGE_ITEM_HEIGHT);
                int index = p * itemCnt + i;
                drawGraphFromIndex(&painter, txRt, index);
                m_itemIndexs[index] = txRt;
            }
    #if 0 //mouse tips
            for(int i=0; i<itemCnt; ++i){
                int index = p * itemCnt + i;
                QRect contentRt(rt.x()+OFFSET_X, i*PAGE_ITEM_HEIGHT+OFFSET_Y, PAGE_WIDTH-OFFSET_X-10, PAGE_ITEM_HEIGHT);
                if(getItem(index).type == Draw_Content && contentRt.contains(mHoverPos))
                    drawTips(&painter, contentRt, index);
            }
    #endif
        }
    }
    }

}

void DrawPanel::mouseMoveEvent(QMouseEvent *e)
{
    if(m_dataFlags == Custom_Stock_Null){
        m_pressPos = e->pos();
        m_isPressed = false;
        update();
    } else {
        m_hoverPos = e->pos();
        update();
    }
}

void DrawPanel::mousePressEvent(QMouseEvent *event)
{
    if(m_dataFlags == Custom_Stock_Null){
        m_pressPos = event->pos();
        m_isPressed = true;
        //add custom stock
        if(m_addRect.contains(event->pos()))
            emit signalAddCustomStock();
        update();
    } else {
        m_pressPos = event->pos();
        int index = findItem(event->pos());
        PanelItem item = getItem(index);
        emit signalItemPressed(item.title);
        if(m_pageId == CompanyEvent_Page && item.type == Draw_Content && item.smFlags && item.msgType == 3)
        {
            //用本地浏览器打开
            QDesktopServices::openUrl(QUrl::fromUserInput(item.url));
        } else if(m_pageId == SmartPlate_Page && item.type == Draw_Time) {
            emit signalStockCodeClicked(item.code);
        }
    }
}

void DrawPanel::slotScrollChanged(int value)
{
    m_scrollValue = value;
    update();
}

QString DrawPanel::FormatString(double val, bool flags)
{
    QString tmpVal;
    if(flags) {
        if(val>0)
          tmpVal.append("+");
        //tmpVal.append(val>0?"+":val==0?"":"");
    }
    tmpVal.append(HzQuoteFormat::StringUnitFormat::toFormatString(val));
    return tmpVal;
}

void DrawPanel::drawGraphFromIndex(QPainter *painter, const QRect &rect, int index)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->save();
    const PanelItem &item = getItem(index);
    quint32 time = item.time;
    QRect rt(rect);
    switch(item.type){
    case Draw_Date:
    {
        //draw date button and date text
        quint64 date = QDateTime::currentDateTime().toString("yyyyMMdd").toULongLong();
        quint64 value = QDateTime::fromTime_t(time).toString("yyyyMMdd").toULongLong();
        bool isDay = date==value;
        QRect dayRt(rt);
        dayRt.adjust(-ROOT_WIDTH_VALUE/2, 0, ROOT_WIDTH_VALUE/2-rt.width(), 0);
        painter->setPen(isDay ? gDrawStyle->m_toDayBorderPen : gDrawStyle->m_yeasterBorderPen);
        painter->setBrush(gDrawStyle->m_toDayBrush);
        painter->drawRoundedRect(dayRt, ROOT_RADIUS_VALUE, ROOT_RADIUS_VALUE);

        QString tmpDay = isDay?QStringLiteral("今天"):QDateTime::fromTime_t(time).toString("yyyy-MM-dd");
        painter->setPen(isDay?gDrawStyle->m_toDayPen:gDrawStyle->m_yeasterDayPen);
        painter->setFont(isDay?gDrawStyle->m_contentFont:gDrawStyle->m_timeFont);
        painter->drawText(dayRt, Qt::AlignCenter, tmpDay);
    }break;
    case Draw_Time:
    {
        //draw title region
        QRect titleRt(rt);
        titleRt.adjust(9, 2, -OFFSET_RIGHT_X, 0);
        bool isHover = titleRt.contains(m_hoverPos);
        painter->setPen(Qt::NoPen);
        painter->setBrush(isHover?gDrawStyle->m_hoverBrush:gDrawStyle->m_hormalBrush);
        painter->drawRoundedRect(titleRt, TITLE_RADIUS_VALUE, TITLE_RADIUS_VALUE);

        //draw ellipse and draw time text
        int textLen = painter->fontMetrics().width(QString::number(time)), fonth=painter->fontMetrics().height();
        QPointF ellipsePt(rt.topLeft());
        ellipsePt.setY(rt.y()+rt.height()/2);
        painter->setPen(isHover?gDrawStyle->m_ellipseHoverPen:gDrawStyle->m_ellipsePen);
        painter->setBrush(gDrawStyle->m_ellipseBrush);
        painter->drawEllipse(ellipsePt, 3.5, 3.5);
        painter->setPen(gDrawStyle->m_timeTextPen);
        painter->setFont(gDrawStyle->m_timeFont);
        painter->drawText(QRect(rt.x()-textLen-9, rt.y()+(rt.height()-fonth)/2, textLen,fonth), Qt::AlignVCenter|Qt::AlignRight, QDateTime::fromTime_t(time).toString("hh:mm"));

        QString disCode(QString("%1(%2)").arg(item.name).arg(item.code));
        auto StringWidth = [&](const QString &text)->int{ return qCeil(painter->fontMetrics().width(text))+14;};
        int xoffset = 5;

        //draw - code name
        painter->setPen(gDrawStyle->m_timeTextPen);
        painter->setFont(gDrawStyle->m_codeAndTipFont);
        painter->drawText(titleRt.adjusted(xoffset,0,0,0), Qt::AlignVCenter |Qt::AlignLeft, disCode);

        //draw - changed chg
        QString chgStr;
        if(m_pageId == CompanyEvent_Page) {
            bool isInvalidChg(item.chg == INVALID_DOUBLE);
            chgStr =  isInvalidChg?"--":QString::number(item.chg,'f',2)+"%";
            if(isInvalidChg){
                painter->setPen(gDrawStyle->m_noUpDownBrPen);
            }else {
                painter->setPen(item.chg>0.? gDrawStyle->m_changedUpPen : item.chg<0.?gDrawStyle->m_changedDownPen:gDrawStyle->m_noUpDownBrPen);
            }
        } else {
            bool isInvalidChg(item.real_chg == INVALID_DOUBLE);
            chgStr =  isInvalidChg?"--":QString::number(item.real_chg,'f',2)+"%";
            if(isInvalidChg){
                painter->setPen(gDrawStyle->m_noUpDownBrPen);
            }else {
                painter->setPen(item.real_chg>0.? gDrawStyle->m_changedUpPen : item.real_chg<0.?gDrawStyle->m_changedDownPen:gDrawStyle->m_noUpDownBrPen);
            }
        }

        int codeNamePos = xoffset + StringWidth(disCode);
        int changePos   = codeNamePos + StringWidth(chgStr);
        painter->setFont(gDrawStyle->m_changedAndChgFont);
        painter->drawText(titleRt.adjusted(codeNamePos+10,0,0,0), Qt::AlignVCenter |Qt::AlignLeft, chgStr);

        //draw - tips
        painter->setPen(gDrawStyle->m_timeTextPen);
        painter->setFont(gDrawStyle->m_contentFont);
        QString typeStr;
        if(m_pageId == CompanyEvent_Page) {
            typeStr = item.msgType==1?QStringLiteral("龙虎榜"):item.msgType==2?QStringLiteral("分红送转"):QStringLiteral("财报");
            if(item.msgType == 2)
                typeStr = item.typeTwoMsg;
            else if(item.msgType == 3) {
                QDate curDate   = QDateTime::fromTime_t(item.time).date();//发布时间
                QDate cbDate    = QDateTime::fromString(QString::number(item.date),"yyyyMMdd").date();
                if(curDate == cbDate) {
                    typeStr = QStringLiteral("财报发布");
                } else {
                    typeStr = QStringLiteral("将要发布财报");
                }
            }
        } else {
            typeStr = FormatItemTag(item.msgType);
        }
        painter->drawText(titleRt.adjusted(changePos+10,0,0,0), Qt::AlignVCenter |Qt::AlignLeft, QString("%1").arg(typeStr));
    }break;
    case Draw_Content:
    {
        //draw content
        QRect contentRt(rt);
        contentRt.adjust(9, 2, -OFFSET_RIGHT_X, 0);
        painter->setPen(gDrawStyle->m_contentPen);
        painter->setBrush(Qt::NoBrush);
        painter->setFont(gDrawStyle->m_contentFont);
        QTextOption opt;
        opt.setAlignment(Qt::AlignVCenter |Qt::AlignLeft);
        opt.setWrapMode(QTextOption::WordWrap);
        painter->drawText(contentRt.adjusted(5,0,0,0), item.content, opt);
    }break;
    case Draw_ContentEx:
    {
        //draw content
        QRect contentRt(rt);
        contentRt.adjust(9, 2, -OFFSET_RIGHT_X, 0);
        painter->setPen(gDrawStyle->m_contentPen);
        painter->setBrush(Qt::NoBrush);
        painter->setFont(gDrawStyle->m_contentFont);
        QTextOption opt;
        opt.setAlignment(Qt::AlignTop |Qt::AlignLeft);
        opt.setWrapMode(QTextOption::WordWrap);
        painter->drawText(contentRt.adjusted(5,0,0,item.contentEx), item.content, opt);
    }break;
    case Draw_None:
    {
        //no draw
    }

    }
    painter->restore();
}

void DrawPanel::drawTips(QPainter *painter, const QRect &rect, int index)
{
    QRect tipRt(rect);
    int leading = painter->fontMetrics().leading();
    qreal height = 0;
    QTextLayout textLayout;
    textLayout.setCacheEnabled(true);
    textLayout.beginLayout();
    while (1) {
        QTextLine line = textLayout.createLine();
        if (!line.isValid()) break;
        line.setLineWidth(tipRt.width());
        height += leading;
        line.setPosition(QPointF(0, height));
        height += line.height();
    }
    textLayout.endLayout();

    painter->setBrush(gDrawStyle->m_tipsBrush);
    painter->setPen(gDrawStyle->m_tipsPen);
    int fontH = painter->fontMetrics().height();
    tipRt.adjust(ROOT_WIDTH_VALUE/2, rect.height(), -10, height*fontH);
    painter->drawRoundedRect(tipRt.adjusted(-10, -10, 5, 0), 3, 3);

    QTextOption opt(Qt::AlignLeft);
    opt.setWrapMode(QTextOption::WordWrap);
    painter->drawText(tipRt, getItem(index).content, opt);
}

PanelItem DrawPanel::getItem(int index)
{
    if(index < 0 || index > m_panalItems.count()-1) {
        PanelItem item;
        item.type = Draw_None;
        return item;
    }
    return m_panalItems[index];
}

int DrawPanel::findItem(const QPoint &point)
{
    if(m_itemIndexs.isEmpty()) return -1;
    QHashIterator<int,QRect> it(m_itemIndexs);
    while(it.hasNext()) {
        it.next();
        if(it.value().contains(point)){
            return it.key();
        }
    }
    return -1;
}

void DrawPanel::companyOptionEventPageDataDisplay(int code, const CompanyOptionEvents &data)
{
    if(code == 0 && !data.isEmpty()) {
        m_dataFlags = Requst_Normal;
        m_tmsNode.clear();
        QVector<PanelItem> lst;
        for(const CompanyOptionEvent &dt:data) {
            PanelItem item;
            item.time = dt.time;
            item.date = dt.date;
            QString timeStr = QDateTime::fromTime_t(dt.time).toString("yyyyMMdd");
            if(!m_tmsNode.contains(timeStr)) {
                item.type = Draw_Date;
                m_tmsNode.push_back(timeStr);
                lst.append(item);
                item.type = Draw_None;
                lst.append(item);
            }

            item.type = Draw_Time;//时间对应的信息
            item.code = dt.stockcode;
            item.name = dt.stockname;
            item.chg = dt.change_ratio;
            item.buy = dt.buy;
            item.sell = dt.sell;
            item.net_buy = dt.net_buy;
            item.reason = dt.reason;
            item.record_date = FormatTime(dt.record_date);
            item.dividend_date = FormatTime(dt.dividend_date);
            item.detail = dt.detail;
            item.url = dt.url;
            item.msgType = dt.type;//1：龙虎榜，2：分红送转，3：财报
            QString titleStr, contentStr;
            switch(dt.type) {
            case LongHuBang:
            {
                titleStr = QString(LongHuBang_Title_Plate)
                        .arg(dt.stockname+"("+dt.stockcode+")")
                        .arg(dt.change_ratio);
                contentStr = QString(LongHuBang_Content_Plate)
                        .arg(dt.stockname)
                        .arg(dt.reason)
                        .arg(HzQuoteFormat::StringUnitFormat::toFormatString(dt.buy+dt.sell))
                        .arg(HzQuoteFormat::StringUnitFormat::toFormatString(dt.buy))
                        .arg(HzQuoteFormat::StringUnitFormat::toFormatString(dt.sell))
                        .arg(HzQuoteFormat::StringUnitFormat::toFormatString(dt.net_buy));
                lst.append(item);

                int contentHight = 0;
                QTextLayout textLayout(contentStr);
                int leading = 4,height = 4;
                textLayout.setCacheEnabled(true);
                textLayout.beginLayout();
                while (1) {
                    QTextLine line = textLayout.createLine();
                    if (!line.isValid()) break;
                    line.setLineWidth(PAGE_WIDTH-OFFSET_X-10);
                    height += leading;
                    line.setPosition(QPointF(12, height));
                    height += line.height();
                }
                textLayout.endLayout();
                int lineCnt = textLayout.lineCount();
                int space = lineCnt * (leading+height);
                int lineHight = (textLayout.boundingRect().height()-space)/lineCnt;
                if(lineHight < PAGE_ITEM_HEIGHT) lineHight = PAGE_ITEM_HEIGHT;
                int rtH = textLayout.boundingRect().height()+PAGE_ITEM_HEIGHT/2;
                contentHight = ((rtH>PAGE_ITEM_HEIGHT?rtH:PAGE_ITEM_HEIGHT)+lineHight);//add one line

                //详情部分
//                item.type = contentHight > PAGE_ITEM_HEIGHT ? Draw_ContentEx : Draw_Content;
//                if(item.type == Draw_ContentEx) item.contentEx = contentHight;
                item.type = Draw_ContentEx;
                item.contentEx = contentHight;
                item.content = contentStr;
                lst.append(item);

                item.type = Draw_None;
                lst.append(item);
            }break;
            case FenHongSongZhuan://只对当前和将要数据显示，过去数据不展示
            {
                /*当前日期与股权登记和除权出息比较，超过在展示”已XX“，未超过则展示”将要“，note:date字段表示数据展示的日期,故：type=2时,须展示两条消息*/
                QDate curDate   = QDateTime::fromTime_t(dt.time).date();//发布时间
                QDate gqDate    = QDateTime::fromString(QString::number(dt.record_date),"yyyyMMdd").date();
                QDate cqDate    = QDateTime::fromString(QString::number(dt.dividend_date),"yyyyMMdd").date();

                //股权登记情况
                if(gqDate>= curDate) {
                    if(gqDate>curDate) {
                        //将要发生股权登记
                        item.typeTwoMsg = QStringLiteral("将要股权登记");
                        titleStr = QString(FHSZ_Title_GQ_Will_Plate)
                                .arg(dt.stockname)
                                .arg(dt.change_ratio);
                        contentStr = QString(FHSZ_Content_GQ_Will_Plate)
                                .arg(dt.stockname)
                                .arg(item.record_date)
                                .arg(dt.detail);
                    } else if(gqDate==curDate) {
                        //已经发生股权登记
                        item.typeTwoMsg = QStringLiteral("股权登记");
                        titleStr = QString(FHSZ_Title_GQ_Already_Plate)
                                .arg(dt.stockname)
                                .arg(dt.change_ratio);
                        contentStr = QString(FHSZ_Content_GQ_Already_Plate)
                                .arg(dt.stockname)
                                .arg(item.record_date)
                                .arg(dt.detail);
                    }

                    item.type = Draw_Time;//时间对应的信息
                    m_tmsNode.push_back(timeStr);
                    lst.append(item);

                    //详情部分
                    item.type = Draw_Content;
                    item.content = contentStr;
                    lst.append(item);

                    item.type = Draw_None;
                    lst.append(item);
                }

                //除权出息情况
                if(cqDate >= curDate) {
                    if(cqDate>curDate) {
                        //将要发生除权出息
                        item.typeTwoMsg = QStringLiteral("将要除权除息");
                        titleStr = QString(FHSZ_Title_CQ_Will_Plate)
                                .arg(dt.stockname)
                                .arg(dt.change_ratio,0,'f',2);
                        contentStr = QString(FHSZ_Content_CQ_Will_Plate)
                                .arg(dt.stockname)
                                .arg(item.dividend_date)
                                .arg(dt.detail);
                    } else if(cqDate==curDate) {
                        //已经发生除权出息
                        item.typeTwoMsg = QStringLiteral("除权除息");
                        titleStr = QString(FHSZ_Title_CQ_Already_Plate)
                                .arg(dt.stockname)
                                .arg(dt.change_ratio,0,'f',2);
                        contentStr = QString(FHSZ_Content_CQ_Already_Plate)
                                .arg(dt.stockname)
                                .arg(item.dividend_date)
                                .arg(dt.detail);
                    }
                    item.type = Draw_Time;//时间对应的信息
                    m_tmsNode.push_back(timeStr);
                    lst.append(item);

                    //详情部分
                    item.type = Draw_Content;
                    item.content = contentStr;
                    lst.append(item);

                    item.type = Draw_None;
                    lst.append(item);
                }
            }break;
            case CaiBao:
            {
                QDate curDate   = QDateTime::fromTime_t(dt.time).date();//发布时间
                QDate cbDate    = QDateTime::fromString(QString::number(dt.date),"yyyyMMdd").date();
                if(curDate == cbDate) {
                    item.smFlags = true;//分开画
                    titleStr = QString(CB_Title_CurTime_Plate)
                            .arg(dt.stockname)
                            .arg(dt.change_ratio);
                    contentStr = QString(CB_Content_CurTime_Plate).arg(dt.stockname);
                    item.url = dt.url;
                    if(!item.url.isEmpty()) {
                        contentStr += QStringLiteral("，财报详情>>");
                    }
                } else {
                    titleStr = QString(CB_Title_PreTime_Plate)
                            .arg(dt.stockname)
                            .arg(dt.change_ratio);
                    contentStr = QString(CB_Content_PreTime_Plate)
                            .arg(dt.stockname)
                            .arg(FormatTime(dt.date));
                }
                lst.append(item);

                //详情部分
                item.type = Draw_Content;
                item.content = contentStr;
                lst.append(item);

                item.type = Draw_None;
                lst.append(item);
            }break;
            }
        }
        setPanelItems(lst);
        update();
    } else {
        m_dataFlags = Request_Null;
        setPanelItems(QVector<PanelItem>{});
        m_tipText = QStringLiteral("当前暂无数据");
        //mTipUrl = "";//不要图标
        m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    }
}

void DrawPanel::midAbnormalInfoPageDataDisplay(int code, const HZData::MidAbnormalInfo &data)
{
    if(code == 0 && data.totalCount!=0) {
        m_dataFlags = Requst_Normal;
        m_tmsNode.clear();
        QVector<PanelItem> lst;
        for(const HZData::MidAbnormal &dt:data.midAbnormals) {
            PanelItem item;
            item.time = dt.update_time;
            QString timeStr = QDateTime::fromTime_t(dt.update_time).toString("yyyyMMdd");
            if(!m_tmsNode.contains(timeStr)) {
                item.type = Draw_Date;
                m_tmsNode.push_back(timeStr);
                lst.append(item);
                item.type = Draw_None;
                lst.append(item);
            }

            item.code   = dt.stock_code.c_str();
            item.name   = dt.stock_name.c_str();

            qDebug() << Q_FUNC_INFO
                     << "name:" << dt.stock_name.c_str()
                     << "code:" << dt.stock_code.c_str()
                     << "update:" << QDateTime::fromTime_t(dt.update_time).toString("hh:mm")
                     << "rate:" << dt.rate*100
                     << "real_rate:" << dt.real_time_rate
                     << "last_price:" << dt.last_price
                     << "turnover:" << dt.turnover
                     << "volume:" << dt.volume;

            item.type       = Draw_Time;//时间对应的信息
            item.chg        = dt.rate*100;
            item.real_chg   = dt.real_time_rate*100;// real chg
            item.turnover   = dt.turnover;
            item.volume     = dt.volume;
            item.msgType    = dt.abnormal_type;
            QString contentStr;
            switch(dt.abnormal_type) {
            case HZData::ThirtyMinBuy:{
                contentStr = QString(MB_ThirtyMinBuy_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::ThirtyMinSell:{
                contentStr = QString(MB_ThirtyMinSell_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::MoneyMaxOrdBuy:{
                contentStr = QString(MB_MoneyMaxOrdBuy_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.volume))
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::MoneyBigOrdBuy:{
                contentStr = QString(MB_MoneyBigOrdBuy_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.volume))
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::MoneyMaxOrdSell:{
                contentStr = QString(MB_MoneyMaxOrdSell_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.volume))
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::MoneyBigOrdSell:{
                contentStr = QString(MB_MoneyBigOrdSell_Plate)
                        .arg(item.name)
                        .arg(FormatString(dt.volume))
                        .arg(FormatString(dt.turnover))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::UpFiveRate:{
                contentStr = QString(MB_UpFiveRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::DownFiveRate:{
                contentStr = QString(MB_DownFiveRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::UpLimitRate:{
                contentStr = QString(MB_UpLimitRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::DownLimitRate:{
                contentStr = QString(MB_DownLimitRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::OpenUpLimitRate:{
                contentStr = QString(MB_OpenUpLimitRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            case HZData::OpenDownLimitRate:{
                contentStr = QString(MB_OpenDownLimitRate_Plate)
                        .arg(item.name)
                        .arg(QDateTime::fromTime_t(dt.update_time).toString("hh:mm"))
                        .arg(FormatString(item.chg,1))
                        .arg(FormatString(dt.last_price));
            }break;
            }

            lst.append(item);

            //详情部分
            item.type = Draw_Content;
            item.content = contentStr;
            lst.append(item);

            item.type = Draw_None;
            lst.append(item);
        }

        setPanelItems(lst);
        update();
    } else {
        m_dataFlags = Request_Null;
        setPanelItems(QVector<PanelItem>{});
        m_tipText = QStringLiteral("当前暂无数据");
        m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    }
}

DrawStyle *DrawStyle::instance()
{
    static DrawStyle inst;
    return &inst;
}
