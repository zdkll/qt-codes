#include "stockdetailklineminwnd.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QKeyEvent>

#include "subtab.h"
#include "chartmodle.h"
#include "mincanvas.h"
#include "klinecanvas.h"
#include "hzcfg.h"
#include "hzline.h"
#include "basepage.h"
#include "pubmsgdef.h"
#include "httpdata.h"
#include "hzauthenopt.h"

#define temp_indi_bdww 0
#define temp_indi_byzs 1
#define temp_indi_dcxg 2
#define temp_indi_hjjj 3


StockDetailKlineMinWnd::StockDetailKlineMinWnd(QWidget *parent, QString instName)
    :StockDetailBaseWnd(parent,instName)
{
    m_period = HZData::KLINE_PERIOD_DAY;
}

void StockDetailKlineMinWnd::slotsSwitchPeriod(int id)
{
    auto it = m_periodIndexTypeMap.find(id);
    if(it != m_periodIndexTypeMap.end())
    {
        m_period = it.value();
        adjustSrvIndiByPeriodAndStock(m_stockCode,m_period,m_klineWnd->getDrType());
        if(m_klineWnd->isVisible())
        {
            m_klineWnd->switchPeriod(m_period);
        }else
        {
            m_minWnd->afterDeactive();
            m_klineWnd->setBaseInfo(m_stockCode,m_period);
            m_pStackWnd->setCurrentWidget(m_klineWnd);
            m_klineWnd->afterActive();
            m_mainIndiTable->show();
        }
    }else
    {
        adjustSrvIndiByPeriodAndStock(m_stockCode,-1,m_klineWnd->getDrType());
        if(!m_minWnd->isVisible())
        {
            m_klineWnd->afterDeactive();
            m_pStackWnd->setCurrentWidget(m_minWnd);
        }
        if(TabMultiMinType == id)
        {
            m_minWnd->setBaseInfo(m_stockCode,-5);
        }else
        {
            m_minWnd->setBaseInfo(m_stockCode,-1);
        }
        m_minWnd->afterActive();
        m_mainIndiTable->hide();
    }
    if(m_periodTable->currentButton() != id)
        m_periodTable->setCurrentButton(id);
}

void StockDetailKlineMinWnd::slotsSwitchMainIndicator(int id)
{
    if(m_klineWnd->isVisible())
        m_klineWnd->switchMainIndicator(id);
}

void StockDetailKlineMinWnd::slotsSwitchAttachIndicator(int id)
{
    m_minWnd->setAttachIndi(id);
    m_klineWnd->setAttachIndi(id);
    if(m_minWnd->isVisible())
        m_minWnd->switchAttachIndicator(id);
    else if(m_klineWnd->isVisible())
        m_klineWnd->switchAttachIndicator(id);
}

void StockDetailKlineMinWnd::slotsSwitchTemplateIndicator(int id)
{
    //测试绘制特色指标
    if(m_templateMap.contains(id))
    {
        HzSpecialIndicator indi = m_templateMap[id];
        m_klineWnd->switchTemplate(indi.list,indi.mainIndiId);
        m_srvIndiId =id;
    }
}

void StockDetailKlineMinWnd::slotsLeftClick(bool bClick)
{
    emit signalHideLeftWnd(bClick);
}

void StockDetailKlineMinWnd::slotsRightClick(bool bClick)
{
    emit signalHideRightWnd(bClick);
}

void StockDetailKlineMinWnd::slotDrTypeChanged(int type, bool bCtrl)
{
    if(bCtrl)
    {
        adjustSrvIndiByPeriodAndStock(m_stockCode,m_period,m_klineWnd->getDrType());
    }
}

void StockDetailKlineMinWnd::OnStockCodeChange()
{
    m_minWnd->setBaseInfo(m_stockCode);
    m_klineWnd->setBaseInfo(m_stockCode,m_period);
    setObjCodeLabel();
}

void StockDetailKlineMinWnd::keyPressEvent(QKeyEvent *event)
{
    StockDetailBaseWnd::keyPressEvent(event);
    if(Qt::Key_Return == event->key())
    {
        switchKlineAndMin();
    }else if(Qt::Key_PageUp == event->key())
    {
        emit signalWheelChangeStock(m_stockCode,true,1);
    }else if(Qt::Key_PageDown == event->key())
    {
        emit signalWheelChangeStock(m_stockCode,false,1);
    }
}

void StockDetailKlineMinWnd::wheelEvent(QWheelEvent *e)
{
    int step = e->delta() / 8/15;
    emit signalWheelChangeStock(m_stockCode,step>0?true:false,abs(step));
}

void StockDetailKlineMinWnd::createWg()
{
    createHztsIndicator();
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    //顶部标题
    QWidget* pWnd = createTitle();
    pWnd->setFixedHeight(26);
    pLayout->addWidget(pWnd);
    pLayout->setSpacing(1);

    //中部
    pLayout->addWidget(createMidWnd());

    //底部
    pWnd = createBottomWnd();
    pWnd->setFixedHeight(19);
    pLayout->addWidget(pWnd);

    this->setLayout(pLayout);
}

void StockDetailKlineMinWnd::afterActive()
{
    BaseWidget* pWnd = (BaseWidget*)m_pStackWnd->currentWidget();
    if(pWnd)
    {
        pWnd->afterActive();
    }
}

void StockDetailKlineMinWnd::afterDeactive()
{
    BaseWidget* pWnd = (BaseWidget*)m_pStackWnd->currentWidget();
    if(pWnd)
    {
        pWnd->afterDeactive();
    }
}

void StockDetailKlineMinWnd::setCurrType(int type)
{
    //清理掉当前资源
    BaseWidget* pWnd = (BaseWidget*)m_pStackWnd->currentWidget();
    if(pWnd)
    {
        pWnd->afterDeactive();
    }
    auto it = m_periodIndexTypeMap.find(type);
    if(it != m_periodIndexTypeMap.end())
    {
        m_period = it.value();
        m_klineWnd->setBaseInfo(m_stockCode,m_period);
        m_pStackWnd->setCurrentWidget(m_klineWnd);
        m_mainIndiTable->show();
    }else
    {
        m_pStackWnd->setCurrentWidget(m_minWnd);
        if(TabMultiMinType == type)
        {
            m_minWnd->setBaseInfo(m_stockCode,-10);
        }else
        {
            m_minWnd->setBaseInfo(m_stockCode,-1);
        }
        m_mainIndiTable->hide();
    }
    m_periodTable->setCurrentButton(type);
}

int StockDetailKlineMinWnd::getCurrType()
{
    return m_periodTable->currentButton();
}

void StockDetailKlineMinWnd::setMainIndi(int indi)
{
    m_mainId = indi;
}

void StockDetailKlineMinWnd::setAttachIndi(int indi)
{
    m_attachId = indi;
}

void StockDetailKlineMinWnd::switchKlineAndMin()
{
    DetialKlineMinType type  = (DetialKlineMinType)m_periodTable->currentButton();
    if(type == TabMinType || type == TabMultiMinType)
    {
        //slotsSwitchPeriod(m_periodToTypeMap[m_period]);
        slotsSwitchPeriod(TabKlineDayType);
        int indi = m_minWnd->getAttachIndi();
        if(m_attachIndiTable->currentButton() != indi)
            m_attachIndiTable->setCurrentButton(indi);

    }else
    {
        slotsSwitchPeriod(TabMinType);
        int indi = m_klineWnd->getAttachIndi();
        if(m_attachIndiTable->currentButton() != indi)
            m_attachIndiTable->setCurrentButton(indi);
    }
}

void StockDetailKlineMinWnd::switchDrType()
{
     if(m_klineWnd->isVisible())
     {
         m_klineWnd->switchDrType();
     }
}

void StockDetailKlineMinWnd::onNetIoReady(bool ready)
{
    if(m_klineWnd && m_klineWnd->isVisible())
    {
        adjustSrvIndiByPeriodAndStock(m_stockCode,m_period,m_klineWnd->getDrType());
    }else if(m_minWnd && m_minWnd->isVisible())
    {
        adjustSrvIndiByPeriodAndStock(m_stockCode,-1,m_klineWnd->getDrType());
    }
}

void StockDetailKlineMinWnd:: slotsObjChange(const QString &obj)
{
    m_stockCode = obj;
    setObjCodeLabel();
    m_objLabel->update();
    if(m_minWnd->isVisible())
    {
        int day = -1;
        if(TabMultiMinType == m_periodTable->currentButton())
        {
            day = -5;
        }
        m_minWnd->switchStock(m_stockCode,day);
    }else if(m_klineWnd->isVisible())
    {
        adjustSrvIndiByPeriodAndStock(m_stockCode,m_period,m_klineWnd->getDrType());
        m_klineWnd->switchStock(m_stockCode,m_period);
    }
}

QWidget *StockDetailKlineMinWnd::createTitle()
{
    //左侧侧盘口隐藏和显示按钮
    QToolButton* pLeftButton = new QToolButton(this);
    pLeftButton->setCheckable(true);
    pLeftButton->setObjectName("leftExpandButton");
    connect(pLeftButton,SIGNAL(clicked(bool)),this,SLOT(slotsLeftClick(bool)));

    //右侧显示和隐藏按钮
    QToolButton* pRightButton = new QToolButton();
    pRightButton->setCheckable(true);
    pRightButton->setObjectName("rightExpandButton");
    connect(pRightButton,SIGNAL(clicked(bool)),this,SLOT(slotsRightClick(bool)));

    m_objLabel = new QLabel;
    m_objLabel->setObjectName("objName");
    m_objLabel->setText(QStringLiteral("上证指数"));
    QFont font = m_objLabel->font();
    font.setStyleStrategy(QFont::NoAntialias);
    m_objLabel->setFont(font);

    QWidget* pPeriodWnd = createPeriodPanel();

    pLeftButton->setFixedWidth(26);
    pRightButton->setFixedWidth(26);
    m_objLabel->setFixedWidth(85);

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    pLayout->addWidget(pLeftButton);
    HZLine* pLine = new HZLine(Qt::Vertical);
    pLine->setFixedSize(1,26);
    pLayout->addWidget(pLine);

    pLayout->addWidget(m_objLabel);
    pLayout->addWidget(pPeriodWnd);
    pLayout->addStretch();
    pLayout->setSpacing(1);
    pLayout->addWidget(pRightButton);

    QWidget* pWnd = new QWidget;
    pWnd->setLayout(pLayout);
    pWnd->setObjectName("titleWnd");
    return pWnd;
}

QWidget *StockDetailKlineMinWnd::createMidWnd()
{
    m_pStackWnd = new QStackedWidget(this);
    m_minWnd = new MinCanvas(m_pStackWnd);
    m_klineWnd = new KlineCanvas(m_pStackWnd);
    connect(m_klineWnd, SIGNAL(signalDrTypeChanged(int,bool)),this, SLOT(slotDrTypeChanged(int,bool)));
    m_pStackWnd->addWidget(m_minWnd);
    m_pStackWnd->addWidget(m_klineWnd);

    m_klineWnd->setBaseInfo(m_stockCode,m_period);
    m_klineWnd->setChartsInfo(getKlineChartInfo());
    m_klineWnd->setCreateCrossDlgFlag(true);
    m_klineWnd->createWg();

    m_minWnd->setBaseInfo(m_stockCode);
    m_minWnd->setChartsInfo(getMinChartInfo());
    m_minWnd->setCreateCrossDlgFlag(true);
    m_minWnd->createWg();


    m_klineWnd->setEnterAndDbClickSwitchKlineMin(false);
    m_minWnd->setEnterAndDbClickSwitchKlineMin(false);
    return m_pStackWnd;
}

QWidget *StockDetailKlineMinWnd::createBottomWnd()
{
    return createMidBottomInidWnd();
}

QWidget *StockDetailKlineMinWnd::createPeriodPanel()
{
    //周期选择TAB
    m_periodIndexTypeMap[TabKlineMinType] = HZData::KLINE_PERIOD_MIN;
    m_periodIndexTypeMap[TabKline5MinType] = HZData::KLINE_PERIOD_5MIN;
    m_periodIndexTypeMap[TabKline15MinType] = HZData::KLINE_PERIOD_15MIN;
    m_periodIndexTypeMap[TabKlineHourType] = HZData::KLINE_PERIOD_HOUR;
    m_periodIndexTypeMap[TabKlineDayType] = HZData::KLINE_PERIOD_DAY;
    m_periodIndexTypeMap[TabKlineWeek] = HZData::KLINE_PERIOD_WEEK;
    m_periodIndexTypeMap[TabKlineMonth] = HZData::KLINE_PERIOD_MONTH;


    m_periodToTypeMap[HZData::KLINE_PERIOD_MIN] = TabKlineMinType;
    m_periodToTypeMap[HZData::KLINE_PERIOD_5MIN] = TabKline5MinType;
    m_periodToTypeMap[HZData::KLINE_PERIOD_15MIN] = TabKline15MinType;
    m_periodToTypeMap[HZData::KLINE_PERIOD_HOUR] = TabKlineHourType;
    m_periodToTypeMap[HZData::KLINE_PERIOD_DAY] = TabKlineDayType;
    m_periodToTypeMap[HZData::KLINE_PERIOD_WEEK] = TabKlineWeek;
    m_periodToTypeMap[HZData::KLINE_PERIOD_MONTH] = TabKlineWeek;

    m_periodTable = new CSubTab(this);
    m_periodTable->setObjectName("periodTab");
    m_periodTable->addButton(QStringLiteral("分时"),TabMinType);
    m_periodTable->addButton(QStringLiteral("多日"),TabMultiMinType);
    m_periodTable->setCurrentButton(TabMinType);
    m_periodTable->setStretchLastType(CSubTab::Empty_Strecth);
    for(int i = TabKlineMinType;i < TabKlineMinUnknown;i++)
    {
        m_periodTable->addButton(ChartModle::getInstance()->getStrPeriod(m_periodIndexTypeMap[i]),i);

    }
    connect(m_periodTable,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchPeriod(int)));
    return m_periodTable;
}

QWidget *StockDetailKlineMinWnd::createMidBottomInidWnd()
{
    QHBoxLayout* pMidBottomLayout = new QHBoxLayout();
    pMidBottomLayout->setSpacing(0);
    pMidBottomLayout->setMargin(0);
    m_mainIndiTable = new CSubTab();
    m_mainIndiTable->setObjectName("indiTab");
    m_mainIndiTable->hide();
    m_attachIndiTable = new CSubTab();
    m_attachIndiTable->setObjectName("indiTab");
    m_templateTable = new CSubTab();
    m_templateTable->hide();
    m_templateTable->setObjectName("indiTab");


//    m_templateTable->addButton(QStringLiteral("波段为王"),temp_indi_bdww);
    m_templateTable->addButton(QStringLiteral("博弈制胜"),temp_indi_byzs);
    m_templateTable->addButton(QStringLiteral("多彩选股"),temp_indi_dcxg);
    m_templateTable->addButton(QStringLiteral("黄金狙击"),temp_indi_hjjj);
    m_templateTable->setCurrentButton(2);
    m_srvIndiId =m_templateTable->currentButton();


    IndiIndexMap mapTab = Hzcfg::getInstance()->getIndiIndexMap();
    CSubTab* pTab = nullptr;

    for(auto it = mapTab.begin();it != mapTab.end();it++)
    {
        if(it->second.drawArea == IndiDrawArea::indi_main)
        {
            pTab = m_mainIndiTable;
        }else{
            pTab = m_attachIndiTable;
        }
        pTab->addButton(it->second.name.c_str(),it->first);
        pTab->addLine(1,9);
    }
    connect(m_attachIndiTable,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchAttachIndicator(int)));
    connect(m_mainIndiTable,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchMainIndicator(int)));
    connect(m_templateTable,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchTemplateIndicator(int)));

    pMidBottomLayout->addWidget(m_templateTable);
    pMidBottomLayout->addWidget(m_mainIndiTable);
    pMidBottomLayout->addWidget(m_attachIndiTable);
    pMidBottomLayout->addStretch();
    m_klineWnd->setAttachIndi(m_attachId);
    m_minWnd->setAttachIndi(m_attachId);
    m_mainIndiTable->setCurrentButton(m_mainId);
    m_attachIndiTable->setCurrentButton(m_attachId);
    QWidget* pPanel = new QWidget();
    pPanel->setObjectName("midIndiPanel");
    pPanel->setLayout(pMidBottomLayout);
    return pPanel;
}

QVector<ChartItemInfo> StockDetailKlineMinWnd::getMinChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    item.attr = MinTitleShowObjName|MinTitleShowAvgData;
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithPlusBt|ChartTitleWithSubBt;
    info.push_back(item);

    item = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithSetting;
    item.chartClass = "IndicatorChart";
    item.bSelectedStyle = true;
    item.padding = 2;
    item.nHeight = 170;
    item.indiId =3;
    info.push_back(item);
    m_attachId = item.indiId;

    item = ChartItemInfo();
    item.padding = 1;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);

    return info;
}

QVector<ChartItemInfo> StockDetailKlineMinWnd::getKlineChartInfo()
{
    m_mainId = 0;
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.titleAttr = ChartTitleWithSetting;
    item.chartClass = "KlineChart";
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithRestorationComBox|
                     ChartTitleWithPlusBt|ChartTitleWithSubBt|
                     ChartTitleWithSetting;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithSetting;
    item.chartClass = "IndicatorChart";
    item.bSelectedStyle = true;
    item.padding = 2;
    item.nHeight = 100;
    item.indiId =2;
    item.type = ChartAttachType;
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 3;
    item.chartClass = "KlineTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

void StockDetailKlineMinWnd::setObjCodeLabel()
{
    if(!m_stockCode.isEmpty())
    {
        QString str = m_stockCode.mid(2)+".";
        QString preStr = m_stockCode.mid(0,2);
        preStr = preStr.toUpper();
        str += preStr;
        m_objLabel->setText(str);
    }
}

void StockDetailKlineMinWnd::adjustSrvIndiByPeriodAndStock(const QString& stock,int period,int drType)
{
    m_klineWnd->removeSrvIndi();
    QString objName = Httpdata::instance()->getCodeNameByStocks(stock);
    bool bHasePermiss = HZAuthenOpt::instance()->hasPermission(HZAuthen::up_zbhjjj);
    if(period == HZData::KLINE_PERIOD_DAY && objName.isEmpty() == false && bHasePermiss && drType == RestorationNone)
    {
        m_templateTable->show();
        slotsSwitchTemplateIndicator(m_srvIndiId);
    }else
    {
        m_templateTable->hide();
    }
}

void StockDetailKlineMinWnd::createHztsIndicator()
{
    HzSpecialIndicator spIndi;
    ChartItemInfo item;
    //黄金狙击
    spIndi = HzSpecialIndicator();
    item  = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithClose;
    item.chartProvider = "IndicatorSrvProvider";
    item.chartClass = "IndicatorChart";
    item.padding = 2;
    item.nHeight = 80;
    item.indiId =HZData::JDCJ;
    item.type = ChartAttachType;
    spIndi.list.push_back(item);
    spIndi.mainIndiId = HZData::CMFB;
    m_templateMap[temp_indi_hjjj] = spIndi;

    //多彩选股
    spIndi = HzSpecialIndicator();
    item = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithClose;
    item.chartProvider = "IndicatorSrvProvider";
    item.chartClass = "IndicatorChart";
    item.padding = 2;
    item.nHeight = 80;
    item.indiId =HZData::CPTX;
    item.type = ChartAttachType;
    spIndi.list.push_back(item);
    spIndi.mainIndiId = HZData::HLQJ;
    m_templateMap[temp_indi_dcxg] = spIndi;


    //博弈致胜
    //心情曲线
    spIndi = HzSpecialIndicator();
    item = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithClose;
    item.chartProvider = "IndicatorSrvProvider";
    item.chartClass = "IndicatorChart";
    item.padding = 2;
    item.nHeight = 80;
    item.indiId =HZData::XQQX;
    item.type = ChartAttachType;
    spIndi.list.push_back(item);

    //海平面
    item = ChartItemInfo();
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithClose;
    item.chartProvider = "IndicatorSrvProvider";
    item.chartClass = "IndicatorChart";
    item.padding = 2;
    item.nHeight = 80;
    item.indiId =HZData::HPM;
    item.type = ChartAttachType;
    spIndi.list.push_back(item);
    m_templateMap[temp_indi_byzs] = spIndi;
}
