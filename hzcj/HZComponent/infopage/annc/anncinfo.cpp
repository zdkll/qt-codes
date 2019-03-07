#include "anncinfo.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>
#include <QEvent>
#include <QKeyEvent>
#include <QFont>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextOption>
#include <QDateTime>
#include <QVBoxLayout>
#include <QWebEngineProfile>

#include <datamodeldefs/stockinfodef.h>
#include <datacontroller.h>
#include "indexreqbutton.h"
#include "infodef.h"
#include "infomaster.h"
#include "stockadddialog.h"
#include "zxstockopt.h"

#define PIXMAP_WIDTH    76
#define PIXMAP_HEIGHT   76


///
/// \brief AnncModel::AnncModel
/// \param parent
///
AnncModel::AnncModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void AnncModel::addRecords(const QList<STAnnc> &items)
{
    m_records += items;
    flush();
}

void AnncModel::addRecord(const STAnnc &item)
{
    if(!m_records.contains(item)) {
        m_records.append(item);
        emit dataChanged(index(m_records.count()-1,0), index(rowCount(),0));
    }
}

void AnncModel::insertRecord(const STAnnc &item, int idx)
{
    if(idx && idx < m_records.count()) {
        m_records.insert(idx, item);
    }else{
        m_records.append(item);
    }

    emit dataChanged(index(0, 0), index(rowCount(),0));
}

QVariant AnncModel::recordByIndex(int index)
{
    if(index < 0 || index > m_records.count())
        return QVariant();

    return QVariant::fromValue(m_records[index]);
}

int AnncModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_records.count();
}

QVariant AnncModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && !m_records.isEmpty() && (index.row() >=0 && index.row() < m_records.count())) {
        if(HZINFOITEM_CUSTOM_ROLE_ITEM == role){
            return QVariant::fromValue(m_records[index.row()]);
        } else if(HZINFOITEM_CUSTOM_ROLE_TILE_URL == role){
            QString urlStr =m_records[index.row()].url;
            //qDebug() << Q_FUNC_INFO << urlStr;
            return QVariant::fromValue(QUrl::fromUserInput(urlStr));
        }
    }

    return QVariant();
}

void AnncModel::flush()
{
    beginResetModel();
    endResetModel();
}

void AnncModel::updateChgs(const QVariant &var)
{
    if(!var.isValid()) return;
    QHash<QString, double> chgs = var.value<QHash<QString, double>>();
    for(int i=0; i<m_records.count(); ++i)m_records[i].chg = chgs[m_records[i].stockcode];
    emit dataChanged(index(0,0),index(m_records.count()-1,0));
}

void AnncModel::resetModel()
{
    m_records.clear();
    flush();
}

QList<STAnnc> AnncModel::getRecores() const
{
    return m_records;
}

QList<QString> AnncModel::getStockCodes(int start, int end)
{
    QList<QString> stocks;
    if(index(start,0).isValid() && index(end,0).isValid()) {
        for(int i=start; i<end; i++) stocks.append(m_records[i].stockcode);
    }

    return stocks;
}
///
/// \brief AnncItemDelegate::AnncItemDelegate
/// \param parent
///
AnncItemDelegate::AnncItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
  ,m_customPix(":/skin/default/img/infopage/zixuan.png")
{

}

void AnncItemDelegate::setWidth(int w)
{
    m_width = w;
}

void AnncItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    if(index.isValid()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, false);
        STAnnc data = index.data(HZINFOITEM_CUSTOM_ROLE_ITEM).value<STAnnc>();
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRect rect = viewOption.rect;
        rect.setWidth(m_width);
        if( viewOption.state.testFlag(QStyle::State_Selected) && viewOption.state.testFlag(QStyle::State_MouseOver)) {
            painter->fillRect(option.rect, gInfoStyle->m_itemMousePressBrush);
        } else if(viewOption.state.testFlag(QStyle::State_MouseOver)){
            painter->fillRect(option.rect, gInfoStyle->m_itemMouseHoverBrush);
        } else {
            painter->fillRect(option.rect, QBrush(Qt::NoBrush));
        }

        painter->setPen(gInfoStyle->m_hLinePen);
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());

        QRect rt(rect.adjusted(106+VLISTITEM_LEFT_SPACE, 17, -VLISTITEM_LEFT_SPACE, 0));
        QRect pixRt(rt.x()-PIXMAP_WIDTH-VLISTITEM_LEFT_SPACE, rt.y(), PIXMAP_WIDTH, PIXMAP_HEIGHT);
        QTextOption opt(Qt::AlignLeft|Qt::AlignTop);
        opt.setWrapMode(QTextOption::WordWrap);
        opt.setUseDesignMetrics(true);

        //无效值检查和显示样式
        QString tmpChg = data.chg == DBL_MAX?"--":QString::number(data.chg,'f',2);
        QPen chgPen(gInfoStyle->m_noUpDown), chgBrPen(gInfoStyle->m_noUpDownBrPen);
        QBrush chgBrush(gInfoStyle->m_noUpDownBrush);
        if(!tmpChg.contains('--')) {
           double chgVal(data.chg);
           if(chgVal > 0.) {
               chgPen = gInfoStyle->m_upPen;
               chgBrPen = gInfoStyle->m_upBrPen;
               chgBrush = gInfoStyle->m_upBrush;
           } else if( chgVal < 0.) {
               chgPen = gInfoStyle->m_downPen;
               chgBrPen = gInfoStyle->m_downBrPen;
               chgBrush = gInfoStyle->m_downBrush;
           } else {
               chgPen = gInfoStyle->m_noUpDown;
               chgBrPen = gInfoStyle->m_noUpDownBrPen;
               chgBrush = gInfoStyle->m_noUpDownBrush;
           }
        }
        //draw title
        painter->setFont(gInfoStyle->m_titleFont);
        painter->setPen(gInfoStyle->m_titlePen);
        painter->drawText(rt, data.title, opt);
        int hPos = painter->fontMetrics().height();

        hPos += (rt.y()+8);
        QRect des(rt.x() , hPos, rt.width(), 28);

        //draw tag ?
        hPos += des.height();
        bool isTagValid = (data.tag != HZINFOITEM_INVALID_VALUE);
        QRect lastRt(rt.x() , hPos, rt.width(), 38);
        painter->setFont(gInfoStyle->m_customSelFont);
        int tagNameW = painter->fontMetrics().width(isTagValid?gTagTexts[data.tag]:gTagTexts[0]);
        int tagNameH = painter->fontMetrics().height();
        QRect tagRt(lastRt.x(), lastRt.y()+13, tagNameW, tagNameH);
        if(isTagValid) {            
            if(data.tag !=0){
                painter->setPen(gInfoStyle->m_idxBtnTxtPens[data.tag]);
                painter->drawRect(tagRt.adjusted(-1,-1,1,1));
                painter->drawText(tagRt, Qt::AlignCenter, gTagTexts[data.tag]);
            }
            painter->setPen(chgBrPen);
            painter->setBrush(chgBrush);
            painter->drawRect(pixRt);

            painter->setPen(QColor("#ffffff"));
            painter->setFont(gInfoStyle->m_titleFont);
            int mid = pixRt.height()/2;
            int nameH = painter->fontMetrics().height();
            QString cdName = painter->fontMetrics().elidedText(data.stockname, Qt::ElideRight, pixRt.width());
            painter->drawText(pixRt.adjusted(0, mid-nameH-5, 0, 0), Qt::AlignHCenter|Qt::AlignTop, cdName);

            painter->setPen(chgPen);
            painter->setFont(gInfoStyle->m_numberFont);
            painter->drawText(pixRt.adjusted(0, mid+5, 0, 0), Qt::AlignHCenter|Qt::AlignTop, tmpChg+"%");
        }

        painter->setFont(gInfoStyle->m_numberFontTwo);
        tagNameW = painter->fontMetrics().width(isTagValid?gTagTexts[data.tag]:gTagTexts[0]);
        tagNameH = painter->fontMetrics().height();

        QString codeName = data.stockname + " " + data.stockcode.mid(2);
        int codeNameW = painter->fontMetrics().width(codeName);
        int codeNameH = painter->fontMetrics().height();
        int xpos = tagRt.x()+tagRt.width()+15;

//        //draw code name ？
//        if(!data.stockcode.isEmpty()) {
//            painter->setFont(gInfoStyle->mDescripFont);
//            painter->setPen(gInfoStyle->mDescriptPen);
//            painter->drawText(QRect(xpos,tagRt.y(),codeNameW,codeNameH), Qt::AlignLeft | Qt::AlignTop, codeName);
//        }

        //draw time ?
        QString formatTime("yyyy-MM-dd hh:mm");
        int timeW = painter->fontMetrics().width(formatTime);
        tagRt = lastRt;
        if(data.time != HZINFOITEM_INVALID_VALUE ) {
            painter->setPen(gInfoStyle->m_descriptPen);
            painter->drawText(tagRt.adjusted(0, 0, -16, 0), Qt::AlignRight|Qt::AlignVCenter, QDateTime::fromTime_t(data.time).toString(formatTime));
        }

        //draw source ?
        if(!data.source.isEmpty()) {
            painter->setPen(gInfoStyle->m_descriptPen);
            painter->drawText(tagRt.adjusted(0, 0, -(timeW+32), 0), Qt::AlignRight|Qt::AlignVCenter, data.source);
        }

        painter->restore();
    }else{
        QStyledItemDelegate::paint(painter,option,index);
    }
}

QSize AnncItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
        return QSize(m_width, HZINFOITEM_HEIGHT_SIZE);
    else
        return QStyledItemDelegate::sizeHint(option, index);
}


///
/// \brief AnncView::AnncView
/// \param parent
///
AnncView::AnncView(QWidget *parent)
    :QListView(parent)
    ,m_pressPos(QPoint(-1,-1))
    ,m_listModel(new AnncModel(this))
    ,m_listDelegate(new AnncItemDelegate(this))
{
    setSpacing(0);
    setViewMode(QListView::IconMode);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeMode(QListView::Adjust);
    setItemDelegate(m_listDelegate);
    setModel(m_listModel);
    connect(verticalScrollBar(),&QScrollBar::valueChanged, [=](int value){
        m_scrollVal = value;
        int max = verticalScrollBar()->maximum();
        if(value == max) emit signalReStartRequest(value);
        //startRequestSnapShot();
    });
    connect(verticalScrollBar(), &QScrollBar::sliderReleased, [=]{
        m_scrollVal = verticalScrollBar()->value();
        //startRequestSnapShot();
    });
    connect(this, &HotView::clicked, this, [=](const QModelIndex &index){
        if(m_listModel) {
            QUrl url = m_listModel->data(index,HZINFOITEM_CUSTOM_ROLE_TILE_URL).toUrl();
            if(url.isValid()) emit signalItemClicked(url);
        }
    });
}

void AnncView::setData(const QVariantMap &data)
{
    QString id          = data["id"].toString();
    QList<STAnnc> lst   = data["data"].value<QList<STAnnc>>();
    int code            = data["code"].toInt();
    int page            = data["page"].toInt();

    if(code == -2) {
        m_dataFlags = Custom_Stock_Null;
        m_listModel->addRecords(QList<STAnnc>{});
        gInfoStyle->signalWebAboutBlank();
        update();
        return;
    }

    if(0!= code){
        m_dataFlags = Request_Exception;
        m_listModel->addRecords(QList<STAnnc>{});
        m_tipText = QStringLiteral("操作失败，未知错误");
        m_tipUrl = ":/skin/default/img/infopage/NEWS02.png";
        gInfoStyle->signalWebAboutBlank();
        update();
    } else {
        if(!lst.isEmpty()) {
            m_dataFlags = Requst_Normal;
            m_listModel->addRecords(lst);
            QList<STAnnc> tmpLst(m_listModel->getRecores());
            if(!tmpLst.isEmpty()){
                QUrl url(tmpLst.first().url);
                emit signalDateChanged(url);
            }
            startRequestSnapShot();
        } else if(m_listModel->getRecores().isEmpty()){
            m_dataFlags = Request_Null;
            m_listModel->addRecords(QList<STAnnc>{});
            m_tipText = QStringLiteral("当前暂无数据");
            m_tipUrl = ":/skin/default/img/infopage/NEWS01.png";
            gInfoStyle->signalWebAboutBlank();
            update();
        }
    }
}

void AnncView::setSelected(const QString &index)
{
    //Non realization
}

void AnncView::resetModel()
{
    m_listModel->resetModel();
}

void AnncView::resizeEvent(QResizeEvent *e)
{
    m_listDelegate->setWidth(e->size().width());
    QListView::resizeEvent(e);
}

void AnncView::paintEvent(QPaintEvent *e)
{
    switch(m_dataFlags)
    {
    case Custom_Stock_Null:{
        //弹出 '+' 添加自选股
        QPainter p(viewport());
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
        QPainter p(viewport());
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
        QPainter p(viewport());
        p.fillRect(rect(),gInfoStyle->m_marketCldDetailBrush);
        QListView::paintEvent(e);
    }
    }
}

void AnncView::mouseMoveEvent(QMouseEvent *e)
{
    if(m_dataFlags == Custom_Stock_Null){
        m_pressPos = e->pos();
        m_isPressed = false;
        update();
    } else {
        QListView::mouseMoveEvent(e);
    }
}

void AnncView::mousePressEvent(QMouseEvent *event)
{
    if(m_dataFlags == Custom_Stock_Null){
        m_pressPos = event->pos();
        m_isPressed = true;

        //add custom stock
        if(m_addRect.contains(event->pos()))
            emit signalAddCustomStock();
        update();
    } else {
        QListView::mousePressEvent(event);
    }
}

void AnncView::startRequestSnapShot()
{
#if 0
    if(m_listModel) {
        int index = mScrollVal/HZINFOITEM_HEIGHT_SIZE;
        bool isValid = m_listModel->index(index,0).isValid();
        if(isValid) {
            int maxIndex = qMin(index+qCeil(height()/HZINFOITEM_HEIGHT_SIZE), m_listModel->rowCount()-1);
            QList<QString> stockcodes = m_listModel->getStockCodes(index, maxIndex);
            reqSnapShot(stockcodes);
        }
    }
#else

    QVector<QString> zxgs = ZXStockOpt::instance()->getZXStock();
    reqSnapShot(zxgs);
#endif
}

void AnncView::reqSnapShot(const QVector<QString> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i)
        stdlst.push_back(stocks[i].toStdString());
    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::SLStock_Mask;
    auto fun = std::bind(&AnncView::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void AnncView::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(err != 0) {
        qDebug() << Q_FUNC_INFO << "error:" << err;
        return;
    }

    QHash<QString, double> chgs;
    HZData::SLStock *slStocls = (HZData::SLStock *)data;
    if(slStocls) {
        for(int i=0; i<num; i++){
            QString code = QString::fromStdString(slStocls[i].stock_code);
            chgs[code] = slStocls[i].rate;
        }
    }

    m_listModel->updateChgs(QVariant::fromValue(chgs));
}


///
/// \brief AnncMarster::AnncMarster
/// \param adapter
/// \param parent
///
AnncMarster::AnncMarster(IUpdateRequest *adapter,QWidget *parent) :
    BasePage(parent)
  ,m_iURequest(adapter)
  ,m_indexBtn(new IndexReqButton(this))
  ,m_leftView(new AnncView(this))
  ,m_webView(new HZInfoWebPageMarster(this))
  ,m_webPage(new HZInfoWebPage(InsertWindow, QWebEngineProfile::defaultProfile()))
{
    setMouseTracking(true);
    m_indexBtn->setFixedHeight(102);
    m_leftView->setFixedWidth(500);
    m_indexBtn->setObjectName("IndexReqButton");
    m_webView->setWebPage(m_webPage);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    vLayout->addWidget(m_indexBtn);
    vLayout->addWidget(m_leftView);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(m_webView);
    hLayout->setStretch(0,0);
    hLayout->setStretch(1,1);
    setLayout(hLayout);

    //installEventFilter(this);
    m_iURequest->setVisitor(this);
    connect(m_indexBtn, &IndexReqButton::signalIndexChanged, [=](int tag){
        if(isRequest()) {
            if(tag != m_defaultTag) {
                m_leftView->m_listModel->resetModel();
                m_defaultTag = tag;
                m_tagChanged = true;
            }
            reqDatas();
        }
    });
    connect(m_leftView, &AnncView::signalReStartRequest, [=](int value){
        Q_UNUSED(value);
        if(isRequest()) {
            reqDatas();
        }
    });
    connect(m_leftView, &AnncView::signalItemClicked, [=](const QUrl &url){
        if(url.isValid()) m_webView->setUrl(url);
    });

    //每次在tag发生变化，且数据model重新更新时，将首项列表的详情页打开
    connect(m_leftView, &AnncView::signalDateChanged, [&](const QUrl &url){
        if(m_tagChanged && url.isValid()){
            m_tagChanged = false;
            if(m_webView) m_webView->setUrl(url);
        }
    });
    connect(m_leftView, &AnncView::signalAddCustomStock, [=]{
        StockAddDialog dlg;
        dlg.exec();

        QVector<QString> zxstocks = ZXStockOpt::instance()->getZXStock();
        if(zxstocks.isEmpty()){
            QVariantMap tmpMap;
            tmpMap["id"] = "";
            tmpMap["code"] = -2;
            m_leftView->setData(tmpMap);
        }

        m_leftView->resetModel();
        m_tagChanged = true;
        m_defaultTag = m_indexBtn->selected();
        qDebug() << "----------------->tag:" << m_defaultTag;
        reqDatas();
    });
    connect(gInfoStyle, &InfoStyle::signalUpdateCustomStockCache, [=]{
        if(isRequest()) {
            reqDatas();
        }
    });
    connect(gInfoStyle, &InfoStyle::signalWebAboutBlank, [=]{
        QString h5Url = qApp->applicationDirPath();
        h5Url+="/cfg/html/black.html";
        if(m_webView) m_webView->setUrl(QUrl(/*"about:blank"*/h5Url));
    });
}

AnncMarster::~AnncMarster()
{
    if(m_webView)
        delete m_webView;
}

void AnncMarster::updateList(const QVariantMap &var)
{
#if 1
    if(var.isEmpty()) return;
    QVariantMap tmpMap(var);
    tmpMap["id"] = m_iURequest->itemId();
    m_leftView->setData(tmpMap);
#else
    int id = mIURequest->itemId();
    QList<STAnnc> lst(var["data"].value<QList<STAnnc>>());
    int code = val["code"].toInt();
    int page = val["page"].toInt();
    mLeftView->m_listModel->addRecords();
    mLeftView->setSelected(id);//--
#endif
}

void AnncMarster::updateDetails(const QVariant &var)
{
    //Non realization
}

void AnncMarster::afterDeactive()
{
    m_iURequest->cancleReq();
}

void AnncMarster::afterActive()
{
    if(isRequest()){
        int tag = m_indexBtn->selected();
        m_defaultTag = tag;
        m_tagChanged = true;
        m_leftView->resetModel();
        reqDatas();
    }
}

bool AnncMarster::eventFilter(QObject *w, QEvent *e)
{
    if(w == this && e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        if(ke->nativeVirtualKey() == 9) {
            m_indexBtn->tabPressed(0);
            update();
        }
    }
    return false;
}

void AnncMarster::reqDatas()
{
    QVariantMap map;
    map["clear"]        = m_tagChanged;
    map["tag"]          = m_defaultTag;
    map["source"]       = InsertWindow;
    map["stockcode"]    = QVariant::fromValue(InfoMaster::mZXStocks);
    m_iURequest->requestListForParam(map);
}

/**
 * @brief AnncMarster::isRequest 请求前判断当前环境
 * @return
 * code = -2 表示自选股列表为空
 */
bool AnncMarster::isRequest()
{
    InfoMaster::updateOfOptionalStocks();
    if(InfoMaster::mZXStocks.isEmpty()){
        QVariantMap tmpMap;
        tmpMap["id"] = "";
        tmpMap["code"] = -2;
        m_leftView->setData(tmpMap);
        return false;
    }

    return true;
}

