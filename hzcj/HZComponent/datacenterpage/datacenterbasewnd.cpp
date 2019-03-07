#include "datacenterbasewnd.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include "hzline.h"
#include "pubmsgdef.h"
#include "datacenterpagepubdef.h"
#include "datapagetableview.h"
#include "blocktradetopwnd.h"
#include "sldatapagetableview.h"

#include "lhtopwidget.h"
#include "fundchangechartwidget.h"
#include "setbiddingwidget.h"
#include "calandarbar.h"
#include "httpdata.h"
#include "smtcurvegraph.h"
#include "smtlsgraphtitle.h"

///////////////////////集合竞价类///////////////////
//主力资金
#define ZLZJ_TAB 0
//量比异动
#define LBYD_TAB 1
//高开
#define GK_TAB 2
//昨日涨跌
#define ZRZD_TAB 3
//自选
#define ZX_TAB 4

DatacenterBaseWnd::DatacenterBaseWnd(QWidget *parent, QString instName)
    :BaseWidget(parent,instName)
{

}

NormalChildWnd::NormalChildWnd(QWidget *parent, QString instName)
    :DatacenterBaseWnd(parent,instName)
{

}

void NormalChildWnd::afterActive()
{
    if(m_topWnd)
        m_topWnd->afterActive();
    int id =  m_tabSub->currentButton();
    auto it = m_listTabMap.find(id);
    if(it != m_listTabMap.end() && it.value().pObj)
    {
        it.value().pObj->afterActive();
    }
}

void NormalChildWnd::afterDeactive()
{
    if(m_topWnd)
        m_topWnd->afterDeactive();
    int id =  m_tabSub->currentButton();
    auto it = m_listTabMap.find(id);
    if(it != m_listTabMap.end() && it.value().pObj)
    {
        it.value().pObj->afterDeactive();
    }
}

void NormalChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_topWnd = createTopWnd();
    pLayout->addWidget(m_topWnd);
    pLayout->addSpacing(1);
    pLayout->addWidget(createTab());
    pLayout->addSpacing(1);
    pLayout->addWidget(createBottomWnd());
    this->setLayout(pLayout);
}

void NormalChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{

}

BaseWidget *NormalChildWnd::createTopWnd()
{
    m_topWnd = new BaseWidget;
    return m_topWnd;
}

CSubTab *NormalChildWnd::createTab()
{
    int currTab = 0;
    getChildWndItems(m_childWndItems,currTab);
    m_tabSub = new CSubTab;
    m_tabSub->setPacing(0);
    m_tabSub->setObjectName("roottab");
    m_tabSub->setFixedHeight(24);
    for(int i=0;i < m_childWndItems.size();i++)
    {
        m_tabSub->addButton(m_childWndItems[i].name,m_childWndItems[i].tabId);
        m_listTabMap[m_childWndItems[i].tabId] = m_childWndItems[i];
        m_tabSub->addLine(1,24);
    }
    m_tabSub->setCurrentButton(currTab);
    m_tabSub->setStretchLastType(CSubTab::Empty_Strecth);
    connect(m_tabSub,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsTabSwtichWnd(int)));
    return m_tabSub;
}

QWidget *NormalChildWnd::createBottomWnd()
{
    m_pstackWnd = new QStackedWidget(this);
    for(int i=0;i < m_childWndItems.size();i++)
    {
        m_pstackWnd->addWidget(m_childWndItems[i].pWnd);
        m_listIndexMap[i] = m_childWndItems[i];
    }
    int tabId = m_tabSub->currentButton();
    auto it = m_listTabMap.find(tabId);
    if(it != m_listTabMap.end())
    {
        m_pstackWnd->setCurrentWidget(it.value().pWnd);
    }
    return m_pstackWnd;
}

void NormalChildWnd::slotsTabSwtichWnd(int id)
{
    auto it = m_listTabMap.find(id);
    if(it != m_listTabMap.end() && it.value().pObj)
    {
        auto iter = m_listIndexMap.find(m_pstackWnd->currentIndex());
        if(iter != m_listIndexMap.end())
            iter.value().pObj->afterDeactive();
        m_pstackWnd->setCurrentWidget(it.value().pWnd);
        it.value().pObj->afterActive();
    }
}


CallAuctionChildWnd::CallAuctionChildWnd(QWidget *parent, QString instName)
    :NormalChildWnd(parent,instName)
{

}

void CallAuctionChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab = ZLZJ_TAB;
    BiddingTableView *leaderFundView = new BiddingTableView(BiddingTableView::Bid_LeaderFund,this);
    connect(leaderFundView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(leaderFundView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(ZLZJ_TAB,QStringLiteral("主力资金"),leaderFundView,leaderFundView));

    BiddingTableView *quanRatioView = new BiddingTableView(BiddingTableView::Bid_QuantityRatio,this);
    connect(quanRatioView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(quanRatioView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(LBYD_TAB,QStringLiteral("量比异动"),quanRatioView,quanRatioView));

    BiddingTableView *rateView = new BiddingTableView(BiddingTableView::Bid_Rate,this);
    connect(rateView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(rateView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(GK_TAB,QStringLiteral("高开"),rateView,rateView));

    BiddingTableView *preRateView = new BiddingTableView(BiddingTableView::Bid_PreRate,this);
    connect(preRateView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(preRateView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(ZRZD_TAB,QStringLiteral("昨日涨跌"),preRateView,preRateView));

    BiddingTableView *zxstockView = new BiddingTableView(BiddingTableView::Bid_ZXStock,this);
    connect(zxstockView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(zxstockView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(ZX_TAB,QStringLiteral("自选"),zxstockView,zxstockView));
}

BaseWidget* CallAuctionChildWnd::createTopWnd()
{
    m_topWnd = new SetBiddingWidget(this);
    m_topWnd->setFixedHeight(270);
    //    m_topWnd->setObjectName("CallAuctionTopWnd");
    return m_topWnd;
}

LimitUpMonitorChildWnd::LimitUpMonitorChildWnd(QWidget *parent, QString instName)
    :NormalChildWnd(parent,instName)
{

}

void LimitUpMonitorChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab =2;
    RiseFallMonitorView *ztkbView = new RiseFallMonitorView(HZData::RFM_type_zthb);
    connect(ztkbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(ztkbView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(0,QStringLiteral("涨停开板池"),ztkbView,ztkbView));

    RiseFallMonitorView *dtkbView = new RiseFallMonitorView(HZData::RFM_type_dtkb);
    connect(dtkbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(dtkbView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(1,QStringLiteral("跌停开板池"),dtkbView,dtkbView));

    RiseFallMonitorView *ztView = new RiseFallMonitorView(HZData::RFM_type_zt);
    connect(ztView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(ztView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(2,QStringLiteral("涨停池"),ztView,ztView));

    RiseFallMonitorView *lbView = new RiseFallMonitorView(HZData::RFM_type_lb);
    connect(lbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(lbView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(3,QStringLiteral("连板池"),lbView,lbView));

    RiseFallMonitorView *zbView = new RiseFallMonitorView(HZData::RFM_type_zb);
    connect(zbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(zbView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(4,QStringLiteral("炸板池"),zbView,zbView));

    RiseFallMonitorView *dtView = new RiseFallMonitorView(HZData::RFM_type_dt);
    connect(dtView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(dtView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(5,QStringLiteral("跌停池"),dtView,dtView));

    RiseFallMonitorView *yztView = new RiseFallMonitorView(HZData::RFM_type_yestoday_zt);
    connect(yztView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(yztView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(6,QStringLiteral("昨日涨停池"),yztView,yztView));
}

BaseWidget *LimitUpMonitorChildWnd::createTopWnd()
{
    m_topWnd = new HZUpDownGraphMarster;
    m_topWnd->setFixedHeight(270);
    m_topWnd->setObjectName("LimitUpMonitorTopWnd");
    return m_topWnd;
}

WinnersListChildWnd::WinnersListChildWnd(QWidget *parent, QString instName)
    :NormalChildWnd(parent,instName)
{

}

void WinnersListChildWnd::afterActive()
{
    if(m_topWnd)
        m_topWnd->afterActive();
    //    int id =  m_tabSub->currentButton();
    //    if(id > 3)
    //    {
    //        auto it = m_listTabMap.find(id);
    //        if(it != m_listTabMap.end() && it.value().pObj)
    //        {
    //            it.value().pObj->afterActive();
    //        }
    //    }
}

//void WinnersListChildWnd::afterDeactive()
//{
//    if(m_topWnd)
//        m_topWnd->afterDeactive();
//    int id =  m_tabSub->currentButton();
//    if(id > 3)
//    {
//        auto it = m_listTabMap.find(id);
//        if(it != m_listTabMap.end() && it.value().pObj)
//        {
//            it.value().pObj->afterDeactive();
//        }
//    }
//}

void WinnersListChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab =0;
    //lhbStockView1  lhbStockView2 lhbStockView3 外部控制传递Type 和Time_t
    m_lhbStockView = new LhbStockView(LhbtTotalList,this);
    connect(m_lhbStockView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(m_lhbStockView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(0,QStringLiteral("全部个股榜单"),m_lhbStockView,m_lhbStockView));

    list.push_back(NormalTabStatckItem(1,QStringLiteral("机构个股榜单"),m_lhbStockView,m_lhbStockView));

    list.push_back(NormalTabStatckItem(2,QStringLiteral("营业部个股榜单"),m_lhbStockView,m_lhbStockView));

    m_lhbYybDrbView = new LhbYybDrbView(this);
    connect(m_lhbYybDrbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(m_lhbYybDrbView,SIGNAL(itemEntered(const QString &)),this,SLOT(slotsSwitchYybDetail(const QString &)));
    list.push_back(NormalTabStatckItem(3,QStringLiteral("当日营业部"),m_lhbYybDrbView,m_lhbYybDrbView));

    LhbYybSlbView *lhbYybSlbView = new LhbYybSlbView(this);
    connect(lhbYybSlbView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(lhbYybSlbView,SIGNAL(itemEntered(const QString &)),this,SLOT(slotsSwitchYybDetail(const QString &)));
    list.push_back(NormalTabStatckItem(4,QStringLiteral("实力榜"),lhbYybSlbView,lhbYybSlbView));

    LhbYybHybView *lhbYybHybView = new LhbYybHybView(this);
    connect(lhbYybHybView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(lhbYybHybView,SIGNAL(itemEntered(const QString &)),this,SLOT(slotsSwitchYybDetail(const QString &)));
    list.push_back(NormalTabStatckItem(5,QStringLiteral("活跃榜"),lhbYybHybView,lhbYybHybView));
}

BaseWidget *WinnersListChildWnd::createTopWnd()
{
    m_topWnd = new LhTopWidget;
    m_topWnd->setFixedHeight(324);
    connect(m_topWnd,SIGNAL(signalTimeSelectedChanged(qint64)),this,SLOT(slotsTimeSelected(qint64)));
    return m_topWnd;
}

qint64 WinnersListChildWnd::getTime()
{
    return m_currTime;
}

void WinnersListChildWnd::slotsTabSwtichWnd(int id)
{
    if(id ==0) //全部个股榜单
    {
        m_lhbStockView->setStockLhbType(LhbtTotalList);
    }else if(id == 1) //机构个股榜单
    {
        m_lhbStockView->setStockLhbType(LhbtOrganization);
    }else if(id == 2)//营业部个股榜单
    {
        m_lhbStockView->setStockLhbType(LhbBusinessDepartmentStock);
    }
    qDebug()<<"id == :'"<<id;
    NormalChildWnd::slotsTabSwtichWnd(id);
}

void WinnersListChildWnd::slotsTimeSelected(qint64 time)
{
    m_currTime =time;
    m_lhbStockView->setTime(time);
    m_lhbYybDrbView->setTime(time);
    int id =  m_tabSub->currentButton();
    auto it = m_listTabMap.find(id);
    if(it != m_listTabMap.end() && it.value().pObj)
    {
        it.value().pObj->afterDeactive();
        it.value().pObj->afterActive();
    }
}

void WinnersListChildWnd::slotsSwitchYybDetail(const QString &id)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(WINNERS_LIST_ID,id);    //营业部ID
    msg.emplace(CHILD_WND_ID,QString::number(WinnersListSalesOfficeDetailWnd));                  //可选
    msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
    sendMessage(ROOT_FRAME,msg,true);
}

FundAbnormalChildWnd::FundAbnormalChildWnd(QWidget *parent, QString instName)
    :NormalChildWnd(parent,instName)
{

}

void FundAbnormalChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab =0;
    FundAbnormalView *abmlView = new FundAbnormalView(HZData::fat_abnormal,this);
    connect(abmlView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(abmlView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(0,QStringLiteral("实时异动"),abmlView,abmlView));

    FundAbnormalView *fibView = new FundAbnormalView(HZData::fat_first_increase_buy,this);
    connect(fibView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(fibView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(1,QStringLiteral("首次增仓"),fibView,fibView));

    FundAbnormalView *lrView = new FundAbnormalView(HZData::fat_low_rise,this);
    connect(lrView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(lrView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(2,QStringLiteral("低涨幅"),lrView,lrView));

    FundAbnormalView *zxView = new FundAbnormalView(HZData::fat_zxstock,this);
    connect(zxView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(zxView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    list.push_back(NormalTabStatckItem(3,QStringLiteral("自选股"),zxView,zxView));
}

BaseWidget *FundAbnormalChildWnd::createTopWnd()
{
    m_topWnd = new FundChangeChartWidget;
    m_topWnd->setFixedHeight(270);
    //    m_topWnd->setObjectName("FundAbnormalTopWnd");
    return m_topWnd;
}

WinnersListSalesOfficeChildWnd::WinnersListSalesOfficeChildWnd(QWidget *parent, QString instName)
    :DatacenterBaseWnd(parent,instName),
      m_id(-1)
{

}

void WinnersListSalesOfficeChildWnd::setSalesOfficeId(const QString& id)
{
    m_currSalesOfficeId = id;
}

void WinnersListSalesOfficeChildWnd::afterActive()
{
    auto fun = std::bind(&WinnersListSalesOfficeChildWnd::onDataCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_id = HzInfoCenter::getInstance()->reqStockYybDetail(m_currSalesOfficeId,fun);
}

void WinnersListSalesOfficeChildWnd::afterDeactive()
{
    if(m_id != -1)
        HzInfoCenter::getInstance()->cancle(m_id);
}

void WinnersListSalesOfficeChildWnd::onDataCallBack(const InfoRet &id, const StockYybDetail &data)
{
    m_id = -1;
    m_pLabelSoName->setText(data.name);
    m_pLabelSoName->update();

    m_yybSbView->setYybDetailStatistics(data.statistics);
    m_yybHoView->setYybDetaiHistory(data.history);
}

void WinnersListSalesOfficeChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    //创建营业部标题
    m_pLabelSoName = new QLabel(this);
    m_pLabelSoName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    m_pLabelSoName->setObjectName("labelTitle");
    m_pLabelSoName->setFixedHeight(24);
    pLayout->addWidget(m_pLabelSoName);
    pLayout->addSpacing(1);

    //创建跟买数据列表
    m_yybSbView = new LhbYybSbView(this);
    pLayout->addWidget(m_yybSbView,1);
    pLayout->addSpacing(1);

    //创建历史标题
    QLabel* pLabel = new QLabel(this);
    pLabel->setFixedHeight(24);
    pLabel->setObjectName("labelTitle");
    pLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    pLabel->setText(QStringLiteral("历史操作"));
    pLayout->addWidget(pLabel);

    //创建操作历史列表
    m_yybHoView = new LhbYybHoView(this);
    pLayout->addWidget(m_yybHoView,4);
    connect(m_yybHoView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(m_yybHoView,SIGNAL(itemEntered(const QString &)),this,SIGNAL(signalSWitchDetailPage(const QString &)));

    this->setLayout(pLayout);
}

SecuritiesMarginTradingChildWnd::SecuritiesMarginTradingChildWnd(QWidget *parent, QString instName):
    DatacenterBaseWnd(parent,instName)
{
    m_reqId = -1;
}

void SecuritiesMarginTradingChildWnd::afterActive()
{
    StockDzjyDateListCallBack fun1 = std::bind(&SecuritiesMarginTradingChildWnd::onStockDzjyDateListCall, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId = HzInfoCenter::getInstance()->reqStockDzjyDateList(fun1);
}

void SecuritiesMarginTradingChildWnd::afterDeactive()
{
    if(m_reqId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_reqId);
        m_reqId =-1;
    }
    m_stkDzjyView->afterActive();
}

void SecuritiesMarginTradingChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(createTimeTableWnd());
    pLayout->addSpacing(1);
    pLayout->addWidget(createDzjyListWnd());
    this->setLayout(pLayout);
}

void SecuritiesMarginTradingChildWnd::slotsCalandarBarClicked()
{
    QString dateStr = mCalandar->getCurrentDate();
    if(dateStr.isEmpty()==false)
    {
        QDateTime time = QDateTime::fromString(dateStr,"yyyyMMdd");
        if(time.isValid())
        {
            m_currTime = time.toTime_t();
            proceTimeChange();
        }
    }
}

QWidget *SecuritiesMarginTradingChildWnd::createTimeTableWnd()
{
    //日历
    mCalandar = new CalandarBarMaster(true);
    mCalandar->setObjectName("CalendarBkClr");
    mCalandar->setFixedHeight(30);
    connect(mCalandar, &CalandarBarMaster::signalCalandarBarClicked, this, &SecuritiesMarginTradingChildWnd::slotsCalandarBarClicked);
    return mCalandar;
}

QWidget *SecuritiesMarginTradingChildWnd::createDzjyListWnd()
{
    m_stkDzjyView  = new StockDzjyView(this);
    connect(m_stkDzjyView,SIGNAL(itemSelected(const QString &)),this,SIGNAL(signalStockObjChanged(const QString &)));
    connect(m_stkDzjyView,SIGNAL(itemEntered(const QString&)),this,SIGNAL(signalSWitchDetailPage(const QString &)));
    return m_stkDzjyView;
}

void SecuritiesMarginTradingChildWnd::proceTimeChange()
{
    qDebug() << "m_currTime" << m_currTime;
    m_stkDzjyView->setTime(m_currTime);
    m_stkDzjyView->afterActive();
}

void SecuritiesMarginTradingChildWnd::onStockDzjyDateListCall(const InfoRet &ret, const StockDzjyDateList &data)
{
    if(0 != ret.code)
        return;
    if(!data.isEmpty())
        m_stockDzjyDateList = data;

    if(!m_stockDzjyDateList.isEmpty())
    {
        QVector<quint64> times;
        for(int i = 0;i < m_stockDzjyDateList.size();i++)
        {
            //            times.push_back(m_stockDzjyDateList[i]);
            times.prepend(m_stockDzjyDateList[i]);
            qDebug() << QDateTime::fromTime_t(m_stockDzjyDateList[i]).toString("yyyy-MM-dd");
        }
        m_currTime = m_stockDzjyDateList[0];
        mCalandar->setValidTimes(times);
        //proceTimeChange();
    }
}

BlockTradeChildWnd::BlockTradeChildWnd(QWidget *parent, QString instName):
    NormalChildWnd(parent,instName),
    m_reqId(-1)
{

}

void BlockTradeChildWnd::afterActive()
{
    StockRzrqDateListCallBack fun1 = std::bind(&BlockTradeChildWnd::onStockRzrqDateListCall, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId = HzInfoCenter::getInstance()->reqStockRzrqDateList(fun1);
}

void BlockTradeChildWnd::afterDeactive()
{
    if(m_reqId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_reqId);
        m_reqId =-1;
    }

    if(m_listReqId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_listReqId);
        m_listReqId =-1;
    }

    m_stockRzrqNetInputView->afterDeactive();
    m_stockRzrqNetOutputView->afterDeactive();

    if(m_pstackWnd->currentIndex() == 0)
        m_stkRzrqview->afterDeactive();
    else
        m_bkRzrqView->afterDeactive();
}

qint64 BlockTradeChildWnd::getTime()
{
    return 0;
}

void BlockTradeChildWnd::slotsSwitchRzrqDetail(const QString &id)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(STOCK_ID,id);
    msg.emplace(TIME_NUMBER,QString::number(m_currTime));
    msg.emplace(CHILD_WND_ID,QString::number(BlockTradeDetailWnd));                  //可选
    msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
    sendMessage(ROOT_FRAME,msg,true);
}

void BlockTradeChildWnd::slotsSwitchRzrqBkStockList(const QString &bkCode)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(STOCK_ID,bkCode);
    msg.emplace(TIME_NUMBER,QString::number(m_currTime));
    msg.emplace(CHILD_WND_ID,QString::number(BlockTradeBkAndStocksWnd));                  //可选
    msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
    sendMessage(ROOT_FRAME,msg,true);
}

void BlockTradeChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab =0;
    //创建个股融券买入榜单
    m_stkRzrqview = new StockRzrqView(this);
    connect(m_stkRzrqview,&StockRzrqView::itemEntered,this,&BlockTradeChildWnd::slotsSwitchRzrqDetail);
    list.push_back(NormalTabStatckItem(0,QStringLiteral("个股融资买入榜"),m_stkRzrqview,m_stkRzrqview));

    //创建融券融券板块榜单
    m_bkRzrqView = new BlockRzrqView(this);
    connect(m_bkRzrqView,&StockRzrqView::itemEntered,this,&BlockTradeChildWnd::slotsSwitchRzrqBkStockList);
    list.push_back(NormalTabStatckItem(1,QStringLiteral("概念板块"),m_bkRzrqView,m_bkRzrqView));
}

BaseWidget *BlockTradeChildWnd::createTopWnd()
{
    QWidget* pWnd = nullptr;
    QGridLayout* pLayout = new QGridLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setVerticalSpacing(1);
    pLayout->setHorizontalSpacing(1);

    //融资融券曲线
    pWnd = createCalandarBar();
    pWnd->setFixedHeight(30);
    pLayout->addWidget(pWnd,0,0,1,1);
    pWnd = createRzrqHistoryChartWnd();
    pLayout->addWidget(pWnd,1,0,1,1);

    //净买入额前十
    QLabel* pLabel = new QLabel;
    pLabel->setObjectName("textLabel");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFixedHeight(30);
    pLabel->setText(QStringLiteral("融资净买入额前10"));
    pLayout->addWidget(pLabel,0,1,1,1);
    pLayout->addWidget(createTheNetBuyingList(),1,1,1,1);

    //净卖出额前10
    pLabel = new QLabel;
    pLabel->setObjectName("textLabel");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFixedHeight(30);
    pLabel->setText(QStringLiteral("融资净卖出额前10"));
    pLayout->addWidget(pLabel,0,2,1,1);
    pLayout->addWidget(createTheNetSellingList(),1,2,1,1);

    pLayout->setColumnStretch(0,2);
    pLayout->setColumnStretch(1,1);
    pLayout->setColumnStretch(2,1);

    BaseWidget* topWnd = new BaseWidget;
    topWnd->setFixedHeight(270);
    topWnd->setLayout(pLayout);
    return topWnd;
}

QWidget *BlockTradeChildWnd::createCalandarBar()
{
    mCalandarBtn = new CustomCalandarBtnMarster(false,true);
    mCalandarBtn->setObjectName("CustomCalandarBtnMarster");
    connect(mCalandarBtn, &CustomCalandarBtnMarster::signalCalandarBarClicked, [=](quint64 time){
        QString timeStr = QDateTime::fromTime_t(time).toString("yyyy-MM-dd");
        mCalandarBtn->setStrText(timeStr);
        m_currTime = time;
        proceTimeChange();
    });

    QWidget * pWnd = new QWidget();
    pWnd->setObjectName("calanTitle");
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(mCalandarBtn);
    pLayout->addStretch();
    pWnd->setLayout(pLayout);
    return pWnd;
}

BaseWidget *BlockTradeChildWnd::createRzrqHistoryChartWnd()
{
    m_smtCurveWnd = new SmtCurveGraphCanvas(this);
    m_smtCurveWnd->setGridNum(5,5);
    m_smtCurveWnd->createWg();
    QVBoxLayout* pLayout  = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_smtTitle = new SmtLsGraphTitle(this);
    m_smtTitle->setObjectName("marketRzrqTitle");
    m_smtTitle->setFixedHeight(48);
    pLayout->addWidget(m_smtTitle);
    pLayout->addWidget(m_smtCurveWnd);

    BaseWidget* pWnd = new BaseWidget(this);
    pWnd->setObjectName("marketRzrq");
    pWnd->setLayout(pLayout);
    return pWnd;
}

QWidget *BlockTradeChildWnd::createTheNetBuyingList()
{
    m_stockRzrqNetInputView = new StockRzrqRankTableView(StockRzrqRankTableView::Rzrq_NetInput);
    connect(m_stockRzrqNetInputView,SIGNAL(itemEntered(const QString&)),this,SLOT(slotsSwitchRzrqDetail(const QString &)));
    return m_stockRzrqNetInputView;
}

QWidget* BlockTradeChildWnd::createTheNetSellingList()
{
    m_stockRzrqNetOutputView = new StockRzrqRankTableView(StockRzrqRankTableView::Rzrq_NetOutput);
    connect(m_stockRzrqNetOutputView,SIGNAL(itemEntered(const QString&)),this,SLOT(slotsSwitchRzrqDetail(const QString &)));
    return m_stockRzrqNetOutputView;
}

void BlockTradeChildWnd::reqStockDatas()
{
    // InfoReqId reqStockMarketRzrqList(INT64 nTime,StockMarketRzrqListCallBack& fun);

    StockMarketRzrqListCallBack fun = std::bind(&BlockTradeChildWnd::onStocDatasCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_listReqId = HzInfoCenter::getInstance()->reqStockMarketRzrqList(m_currTime,fun);
}

void BlockTradeChildWnd::onStocDatasCallBack(const InfoRet &ret, const StockMarketRzrqList &datas)
{
    m_datas = datas;
    m_smtCurveWnd->setDatas(m_datas);
    m_smtCurveWnd->afterActive();
    SmtTiletDataItem item;
    if(!m_datas.dataList.isEmpty())
    {
        item.lsye = m_datas.dataList[0].rzrqye;
        item.rzye = m_datas.dataList[0].rzye;
        item.rqye = m_datas.dataList[0].rqye;
        if(m_datas.dataList.size() > 1)
        {
            item.preLsye =  m_datas.dataList[1].rzrqye;
            item.preRzye = m_datas.dataList[1].rzye;
            item.preRqye = m_datas.dataList[1].rqye;
        }
    }
    m_smtTitle->updateData(item);
}

void BlockTradeChildWnd::onStockRzrqDateListCall(const InfoRet &ret, const StockRzrqDateList &data)
{
    if(0 != ret.code)
        return;
    if(!data.isEmpty())
        m_stockDateList = data;

    if(!m_stockDateList.isEmpty())
    {
        QVector<quint64> times;
        for(int i = 0;i < m_stockDateList.size();i++)
        {
            times.prepend(m_stockDateList[i]);
            qDebug() << QDateTime::fromTime_t(m_stockDateList[i]).toString("yyyy-MM-dd");
        }
        m_currTime = m_stockDateList[0];
        mCalandarBtn->addValidTimes(times);
        mCalandarBtn->setStrText(QDateTime::fromTime_t(m_currTime).toString("yyyy-MM-dd"));
        //proceTimeChange();
    }
}

void BlockTradeChildWnd::proceTimeChange()
{
    qDebug()<<"838============proceTimeChange";
    m_stockRzrqNetInputView->setTime(m_currTime);
    m_stockRzrqNetInputView->afterActive();

    m_stockRzrqNetOutputView->setTime(m_currTime);
    m_stockRzrqNetOutputView->afterActive();

    m_stkRzrqview->setTime(m_currTime);
    m_bkRzrqView->setTime(m_currTime);
    reqStockDatas();
    if(m_pstackWnd->currentIndex() == 0)
        m_stkRzrqview->afterActive();
    else
        m_bkRzrqView->afterActive();
}


BlockTradeDetailChildWnd::BlockTradeDetailChildWnd(QWidget *parent, QString instName)
    :DatacenterBaseWnd(parent,instName)
{

}

void BlockTradeDetailChildWnd::afterActive()
{
    if(m_topWnd)
        m_topWnd->afterActive();
}

void BlockTradeDetailChildWnd::afterDeactive()
{
    if(m_topWnd)
        m_topWnd->afterDeactive();
}

void BlockTradeDetailChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_topWnd = new BlockTradeTopWnd;
    m_topWnd->setFixedHeight(299);
    connect(m_topWnd,SIGNAL(signalDataChanged()),this,SLOT(slotsDataListChanged()));
    pLayout->addWidget(m_topWnd);
    pLayout->addWidget(createStockDataList());
    this->setLayout(pLayout);
}

void BlockTradeDetailChildWnd::setCurrTime(qint64 currTime)
{
    m_topWnd->setCurrShowTime(currTime);
}

void BlockTradeDetailChildWnd::slotsDataListChanged()
{
    qDebug()<<"slotsDataListChanged :"<<m_topWnd->getDatas().dataList.size();
    m_stkRzrqDetailView->setDataList(m_topWnd->getDatas().dataList);
}

void BlockTradeDetailChildWnd::OnStockCodeChange()
{
    m_topWnd->setStockCode(m_stockCode);
}

QWidget *BlockTradeDetailChildWnd::createStockDataList()
{
    m_stkRzrqDetailView = new StockRzrqDetailView(this);
    return m_stkRzrqDetailView;
}


BlockTradeBkAndStocksChildWnd::BlockTradeBkAndStocksChildWnd(QWidget *parent, QString instName)
    :DatacenterBaseWnd(parent,instName),
      m_childCount(0)
{

}

void BlockTradeBkAndStocksChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(createBkList());
    pLayout->addSpacing(1);
    pLayout->addWidget(createBottomBar());
    pLayout->addSpacing(1);
    pLayout->addWidget(createChildList());
    this->setLayout(pLayout);
}

void BlockTradeBkAndStocksChildWnd::setCurrTime(qint64 currTime)
{
    m_currTime = currTime;

    m_rzrqBlockView->setTime(m_currTime);
    m_rzrqBkStockView->setTime(m_currTime);
    //m_rzrqBlockView->afterActive();
}

void BlockTradeBkAndStocksChildWnd::afterActive()
{
    //只激活板块列表
    m_rzrqBlockView->afterActive();
}

void BlockTradeBkAndStocksChildWnd::afterDeactive()
{
    //板块列表和成分股列表同时取消
    m_rzrqBlockView->afterDeactive();
    m_rzrqBkStockView->afterDeactive();
}

void BlockTradeBkAndStocksChildWnd::slotsHideBottomWnd(bool bHide)
{
    if(bHide && m_rzrqBkStockView->isVisible())
    {
        m_rzrqBkStockView->afterDeactive();
        m_rzrqBkStockView->hide();
    }else if(bHide == false && m_rzrqBkStockView->isHidden())
    {
        m_rzrqBkStockView->show();
        m_rzrqBkStockView->afterActive();
    }
}

void BlockTradeBkAndStocksChildWnd::slotsCategoryWndItemSelected(const QString &stockCode)
{
    //标题文本处理
    m_bkname = Httpdata::instance()->getCodeNameByCodeFromBk(stockCode);

    //m_childListDesc->update();
    //通知右侧切换合约
    emit signalStockObjChanged(stockCode);
    //通知底部切换成分股 成分股列表取消激活-激活成分股列表
    m_rzrqBkStockView->setStockCode(stockCode);
    m_rzrqBkStockView->afterActive();
}

QWidget* BlockTradeBkAndStocksChildWnd::createBkList()
{
    m_rzrqBlockView = new BlockRzrqView(this);
    connect(m_rzrqBlockView,&BlockRzrqView::itemSelected,this,&BlockTradeBkAndStocksChildWnd::slotsCategoryWndItemSelected);
    connect(m_rzrqBlockView,&BlockRzrqView::itemEntered,this,&BlockTradeBkAndStocksChildWnd::signalSWitchDetailPage);

    return m_rzrqBlockView;
}

QWidget* BlockTradeBkAndStocksChildWnd::createChildList()
{
    m_rzrqBkStockView = new BlockStockRzrqView(this);
    connect(m_rzrqBkStockView,&BlockRzrqView::itemSelected,this,&BlockTradeBkAndStocksChildWnd::signalStockObjChanged);
    connect(m_rzrqBkStockView,&BlockRzrqView::itemEntered,this,&BlockTradeBkAndStocksChildWnd::signalSWitchDetailPage);
    connect(m_rzrqBkStockView,&BlockStockRzrqView::bkStockCountChanged,this,&BlockTradeBkAndStocksChildWnd::bkChilCountChanged);

    m_rzrqBkStockView->setFixedHeight(212);
    return m_rzrqBkStockView;
}

QWidget *BlockTradeBkAndStocksChildWnd::createBottomBar()
{
    //左隐藏和显示按钮
    QToolButton* pButton = new QToolButton(this);
    pButton->setCheckable(true);
    pButton->setObjectName("bottomExpandButton");
    connect(pButton,SIGNAL(clicked(bool)),this,SLOT(slotsHideBottomWnd(bool)));

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);

    pButton->setFixedWidth(26);
    pLayout->addWidget(pButton);
    HZLine* pLine = new HZLine(Qt::Vertical);
    pLine->setFixedSize(1,26);
    pLayout->addWidget(pLine);

    m_childListDesc = new QLabel;
    m_childListDesc->setObjectName("titleText");
    m_childListDesc->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_childListDesc->setText(Default_Item_String);
    pLayout->addWidget(m_childListDesc);
    QWidget* pWnd =  new QWidget;
    pWnd->setObjectName("BottomWnd");
    pWnd->setFixedHeight(25);
    pWnd->setLayout(pLayout);
    return pWnd;
}


void BlockTradeBkAndStocksChildWnd::bkChilCountChanged(int count)
{
    QString ret = Default_Item_String;
    m_childCount = count;
    if(m_bkname.isEmpty() == false && m_childCount != -1)
    {
        ret = m_bkname + QStringLiteral("板块个股共有") + QString::number(m_childCount) + QStringLiteral("只");
    }

    m_childListDesc->setText(ret);
}

NewStockCalendarChildWnd::NewStockCalendarChildWnd(QWidget *parent, QString instName)
    :NormalChildWnd(parent,instName)
{

}

void NewStockCalendarChildWnd::afterActive()
{
    NewStockCalandarCallBack fun1 = std::bind(&NewStockCalendarChildWnd::onStockDateListCall, this, std::placeholders::_1, std::placeholders::_2);
    m_reqDateListId = HzInfoCenter::getInstance()->reqNewStockCalandars(fun1);
    int id =  m_tabSub->currentButton();
    auto it = m_listTabMap.find(id);
    if(it != m_listTabMap.end() && it.value().pObj)
    {
        it.value().pObj->afterActive();
    }
}

void NewStockCalendarChildWnd::afterDeactive()
{
    NormalChildWnd::afterDeactive();
    if(m_reqDateListId != -1)
    {
        HzInfoCenter::getInstance()->cancle(m_reqDateListId);
        m_reqDateListId = -1;
    }
}

void NewStockCalendarChildWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(createCalandarBar());
    createTopWndEx();

    m_xgrlXgsg->setFixedHeight(200);
    pLayout->addWidget(m_xgrlXgsg);

    pLayout->addSpacing(1);
    pLayout->addWidget(createTab());
    pLayout->addSpacing(1);
    pLayout->addWidget(createBottomWnd());
    this->setLayout(pLayout);
}

void NewStockCalendarChildWnd::getChildWndItems(QVector<NormalTabStatckItem> &list, int &currTab)
{
    currTab =0;
    XgrlTableView *xgygView = new XgrlTableView(Xgrl_Xgyg);
    //connect(xgygView,&XgrlTableView::itemEntered,this,&NewStockCalendarChildWnd::signalSWitchDetailPage);
    list.push_back(NormalTabStatckItem(0,QStringLiteral("新股预告"),xgygView,xgygView));

    //今日申购窗体
    XgrlJrsgWidget *jrsgWidget = new XgrlJrsgWidget(this);
    //connect(jrsgWidget,&XgrlJrsgWidget::signalSWitchDetailPage,this,&NewStockCalendarChildWnd::signalSWitchDetailPage);
    list.push_back(NormalTabStatckItem(1,QStringLiteral("新股申购"),jrsgWidget,jrsgWidget));

    XgrlTableView *xgssbxView = new XgrlTableView(Xgrl_Xgssbx);
    connect(xgssbxView,&XgrlTableView::itemEntered,this,&NewStockCalendarChildWnd::signalSWitchDetailPage);
    list.push_back(NormalTabStatckItem(2,QStringLiteral("新股上市表现"),xgssbxView,xgssbxView));
}

void NewStockCalendarChildWnd::createTopWndEx()
{
    m_xgrlXgsg = new XgrlTableView(Xgrl_Xgsg);
    //connect(m_xgrlXgsg,&XgrlTableView::itemEntered,this,&NewStockCalendarChildWnd::signalSWitchDetailPage);
}

QWidget *NewStockCalendarChildWnd::createCalandarBar()
{
    //日历
    mCalandar = new CalandarBarMaster(true);
    mCalandar->setObjectName("CalendarBkClr");
    mCalandar->setFixedHeight(30);
    connect(mCalandar, &CalandarBarMaster::signalCalandarBarClicked, this, &NewStockCalendarChildWnd::slotsCalandarBarClicked);
    return mCalandar;
}

void NewStockCalendarChildWnd::proceTimeChange()
{
    m_xgrlXgsg->setTime(m_currTime);
    m_xgrlXgsg->afterActive();
}

void NewStockCalendarChildWnd::slotsCalandarBarClicked()
{
    QString dateStr = mCalandar->getCurrentDate();
    if(dateStr.isEmpty()==false)
    {
        QDateTime time = QDateTime::fromString(dateStr,"yyyyMMdd");
        if(time.isValid())
        {
            m_currTime = time.toTime_t();
            proceTimeChange();
        }
    }
}

void NewStockCalendarChildWnd::onStockDateListCall(const InfoRet ret, const NewStockCalandars &data)
{
    if(0 != ret.code)
        return;
    if(!data.isEmpty())
    {
        m_dataList = data;
    }

    if(!m_dataList.isEmpty())
    {
        QVector<quint64> times;
        for(int i = 0;i < m_dataList.size();i++)
        {
            times.push_back(m_dataList[i].time);
        }
        m_currTime = m_dataList[0].time;
        mCalandar->setValidTimes(times);
        proceTimeChange();
    }
}

///XgrlJrsgWidget
XgrlJrsgWidget::XgrlJrsgWidget(QWidget *parent  ,QString instName )
    :BaseWidget(parent,instName)
{
    createChildWndItems();
}

void XgrlJrsgWidget::afterActive()
{
    for(int i=0;i<m_NormalItems.size();i++)
        m_NormalItems[i].pObj->afterActive();
}

void XgrlJrsgWidget::afterDeactive()
{
    for(int i=0;i<m_NormalItems.size();i++)
        m_NormalItems[i].pObj->afterDeactive();
}

void XgrlJrsgWidget::createChildWndItems()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);

    mainLayout->addWidget(createTab(QStringLiteral("今日申购")));
    XgrlTableView *jrsgView = new XgrlTableView(Xgrl_Jrsg);
    connect(jrsgView,&XgrlTableView::itemEntered,this,&XgrlJrsgWidget::signalSWitchDetailPage);
    m_NormalItems.push_back(NormalTabStatckItem(0,QStringLiteral("今日申购"),jrsgView,jrsgView));
    mainLayout->addWidget(jrsgView);

    mainLayout->addWidget(createTab(QStringLiteral("今日上市")));
    XgrlTableView *jrssView = new XgrlTableView(Xgrl_Jrss);
    connect(jrssView,&XgrlTableView::itemEntered,this,&XgrlJrsgWidget::signalSWitchDetailPage);
    m_NormalItems.push_back(NormalTabStatckItem(1,QStringLiteral("今日上市"),jrssView,jrssView));
    mainLayout->addWidget(jrssView);

    mainLayout->addWidget(createTab(QStringLiteral("已发行待上市")));
    XgrlTableView *yfxdssView = new XgrlTableView(Xgrl_Yfxdss);
    connect(yfxdssView,&XgrlTableView::itemEntered,this,&XgrlJrsgWidget::signalSWitchDetailPage);
    m_NormalItems.push_back(NormalTabStatckItem(2,QStringLiteral("已发行待上市"),yfxdssView,yfxdssView));
    mainLayout->addWidget(yfxdssView);

    this->setLayout(mainLayout);
}

CSubTab *XgrlJrsgWidget::createTab(const QString& title)
{
    CSubTab *tab = new CSubTab();
    tab->addButton(title,0);
    tab->getButton(0)->setCheckable(false);
    tab->setFixedHeight(26);
    tab->setStretchLastType(CSubTab::Empty_Strecth);
    return tab;
}
