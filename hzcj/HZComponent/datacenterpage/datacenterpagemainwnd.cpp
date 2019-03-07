#include "datacenterpagemainwnd.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "subtab.h"
#include "basepage.h"
#include "datacenterpagepubdef.h"
#include "callmarkethandicapwidget.h"
#include "lhlisthandicapwidget.h"
#include "quotepanelwnd.h"
#include "pubmsgdef.h"
#include "platehandicapwidget.h"
#include "hzauthenopt.h"
#include "hzpixlayer.h"
#include "similarklinemainwnd.h"

///////////////右侧盘口类//////////////////
//股票右侧
#define StockrightPanel 10
//集合竞价右侧
#define CallAuctionrightPanel 11
//创建龙虎榜右侧
#define LimitUpMonitorrightPanel 12


DataCenterPageMainWnd::DataCenterPageMainWnd(QWidget *parent, QString instName)
    :BaseWidget(parent,instName),
     m_salesOfficeDetailWnd(nullptr),
     m_blockTradeChildWnd(nullptr),
     m_pMainChildWndMgr(new HzChildWndMgr<DataCenterPageMainWnd>(this,new QStackedWidget(this)))
{
    createWg();
}

DataCenterPageMainWnd::~DataCenterPageMainWnd()
{
    delete m_pMainChildWndMgr;
    m_pMainChildWndMgr = nullptr;
}

void DataCenterPageMainWnd::afterDeactive()
{
    ChildWndItem item;
    auto it =  m_rightPanelMap.find(m_pMainChildWndMgr->currChildWnd(item));
    if(it != m_rightPanelMap.end() && m_pRightStackWnd->isVisible())
    {
        BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
        rightPanel->afterDeactive();
    }
    item.pObj->afterDeactive();
}

void DataCenterPageMainWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void DataCenterPageMainWnd::createWg()
{
    m_pRightStackWnd = new QStackedWidget(this);

    //股票右侧盘口
    QuoteListPanelWnd* panel = new QuoteListPanelWnd(this);
    panel->createWg();
    m_pRightStackWnd->addWidget(panel);
    //创建涨停监测右侧盘口
    m_rightPanelMap[LimitUpMonitorWnd] = RightPanel{0,350};
    //资金异动
    m_rightPanelMap[FundAbnormalWnd] = RightPanel{0,350};
    //大宗交易
    m_rightPanelMap[SecuritiesMarginTradingWnd] = RightPanel{0,350};
    //融资融券详情页面
    m_rightPanelMap[BlockTradeDetailWnd] = RightPanel{0,350};

    //创建集合竞价右侧盘口
    BaseWidget* rightPanel = new BaseWidget(this);
    m_pRightStackWnd->addWidget(new CallMarketHandicapWidget);
    m_rightPanelMap[CallAuctionWnd] = RightPanel{1,350};

    //创建龙虎榜
    m_pRightStackWnd->addWidget(new LhListHandicapWidget);
    m_rightPanelMap[WinnersListWnd] = RightPanel{2,708};
    m_rightPanelMap[WinnersListSalesOfficeDetailWnd] = RightPanel{2,708};

    //融资融券板块成分股
    m_pRightStackWnd->addWidget(new PlateHandicapWidget);
    m_rightPanelMap[BlockTradeBkAndStocksWnd] = RightPanel{3,350};

    m_pRightStackWnd->setCurrentIndex(m_rightPanelMap[CallAuctionWnd].index);

    QHBoxLayout* pLayout  = new QHBoxLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setStretch(0,1);

    QWidget* pLeft = new QWidget();
    QVBoxLayout* pLeftLayout  = new QVBoxLayout();
    pLeftLayout->setMargin(0);
    pLeftLayout->setSpacing(0);
    pLeftLayout->addWidget(createSubTab());
    pLeftLayout->addSpacing(1);
    pLeftLayout->addWidget(createMidWnd());
    pLeft->setLayout(pLeftLayout);

    pLayout->addWidget(pLeft);
    pLayout->setSpacing(1);
    m_pRightStackWnd->setMaximumWidth(350);
    pLayout->addWidget(m_pRightStackWnd);
    this->setLayout(pLayout);
}

void DataCenterPageMainWnd::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    if(!m_pLastCmdParam)
        m_pLastCmdParam = new DetailPageParam;
    *m_pLastCmdParam = param;
    int id = CallAuctionWnd;
    if(param.childWndId != -1)
    {
        id = param.childWndId;
    }
    //权限判断
    id = getPageFromPrmission(id);
    auto tabIter =  m_typeTabIdMap.find(id);
    if(tabIter != m_typeTabIdMap.end() && m_topSub->currentButton() != tabIter.value())
    {
        m_topSub->setCurrentButton(tabIter.value());

    }
    if(m_pMainChildWndMgr->isRegistedType(id))
    {
        ChildWndItem item;
        int currPage = m_pMainChildWndMgr->currChildWnd(item);
        if(bCurrPage)
        {
            item.pObj->afterDeactive();
            auto it =  m_rightPanelMap.find(currPage);
            if(it != m_rightPanelMap.end() && m_pRightStackWnd->isVisible())
            {
                BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
                rightPanel->afterDeactive();
            }
            item = m_pMainChildWndMgr->setCurrWndByPageType(id,false);

            if(WinnersListSalesOfficeDetailWnd == id && param.winnersListId.isEmpty() == false)
            {
                if(m_salesOfficeDetailWnd)
                    m_salesOfficeDetailWnd->setSalesOfficeId(param.winnersListId);
            }
            if(BlockTradeDetailWnd == id || BlockTradeBkAndStocksWnd == id)
            {
                if(param.timeNumber != -1)
                {
                    if(m_blockTradeDetailChildWnd)
                        m_blockTradeDetailChildWnd->setCurrTime(param.timeNumber);
                    if(m_blockTradeBkAndStockChildWnd)
                            m_blockTradeBkAndStockChildWnd->setCurrTime(param.timeNumber);
                }
            }

            if(!param.stock.isEmpty())
                item.pObj->setStockCode(param.stock);
            item.pObj->afterActive();
            it =  m_rightPanelMap.find(id);
            if(it != m_rightPanelMap.end())
            {
                m_pRightStackWnd->show();
                m_pRightStackWnd->setCurrentIndex(it.value().index);
                m_pRightStackWnd->setMaximumWidth(it.value().width);
                if(!param.stock.isEmpty())
                {
                    BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
                    rightPanel->setStockCode(param.stock);
                    if(BlockTradeDetailWnd == id)
                        rightPanel->afterActive();

                }
            }else
            {
                BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
                rightPanel->afterDeactive();
                m_pRightStackWnd->hide();
            }
        }else
        {
            item = m_pMainChildWndMgr->setCurrWndByPageType(id,false);
            if(WinnersListSalesOfficeDetailWnd == id && param.winnersListId.isEmpty() == false)
            {
                if(m_salesOfficeDetailWnd)
                    m_salesOfficeDetailWnd->setSalesOfficeId(param.winnersListId);
            }
            if(BlockTradeDetailWnd == id || BlockTradeBkAndStocksWnd == id)
            {
                if(param.timeNumber != -1)
                {
                    if(m_blockTradeDetailChildWnd)
                        m_blockTradeDetailChildWnd->setCurrTime(param.timeNumber);
                    if(m_blockTradeBkAndStockChildWnd)
                            m_blockTradeBkAndStockChildWnd->setCurrTime(param.timeNumber);
                }
            }
            if(!param.stock.isEmpty())
                item.pObj->setStockCode(param.stock);
            auto it =  m_rightPanelMap.find(id);
            if(it != m_rightPanelMap.end())
            {
                //->setFixedWidth(it.value().width);
                m_pRightStackWnd->show();
                m_pRightStackWnd->setCurrentIndex(it.value().index);
                m_pRightStackWnd->setMaximumWidth(it.value().width);
                if(!param.stock.isEmpty())
                {
                    BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
                    rightPanel->setStockCode(param.stock);
                    if(BlockTradeDetailWnd == id)
                        rightPanel->afterActive();
                }
            }else
            {
                m_pRightStackWnd->hide();
            }
        }
    }
}

void DataCenterPageMainWnd::onNetIoReady(bool ready)
{
    if(ready)
    {
        recvParam(*m_pLastCmdParam,true);
    }
}

void DataCenterPageMainWnd::slotsSwtichWnd(int id)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(CHILD_WND_ID,QString::number(id));
    params.emplace(HZ_PAGE_ID,QString::number(data_center_page));
    sendMessage(ROOT_FRAME,params,true);
}

void DataCenterPageMainWnd::slotsStockObjChanged(const QString &stock)
{
    qDebug() << "DataCenterPageMainWnd::slotsStockObjChanged" << stock;
    ChildWndItem item;
    int index = m_pMainChildWndMgr->currChildWnd(item);
    auto it =  m_rightPanelMap.find(index);
    if(it != m_rightPanelMap.end() && m_pRightStackWnd->isVisible())
    {
        if(index == WinnersListWnd)//设定下当前时间
        {
            LhListHandicapWidget* panel = (LhListHandicapWidget*)m_pRightStackWnd->currentWidget();
            if(panel)
                panel->setDateTime(m_winnerListWnd->getTime());
        }
        BaseWidget* rightPanel  = (BaseWidget*)m_pRightStackWnd->currentWidget();
        rightPanel->setStockCode(stock);
        rightPanel->afterActive();
    }

}

void DataCenterPageMainWnd::slotsSWitchDetailPage(const QString &stock)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stock);
    sendMessage(ROOT_FRAME,params,true);
}

int DataCenterPageMainWnd::getPageFromPrmission(int page)
{
    int id  = page;
    if(id == CallAuctionWnd && HZAuthenOpt::instance()->hasPermission(HZAuthen::up_jhjj) == false)
    {
        //如果过没有权限
        id = CallAuctionNoPermissionWnd;
    }else if(LimitUpMonitorWnd == id && HZAuthenOpt::instance()->hasPermission(HZAuthen::up_ztjc) == false)
    {
        id = LimitUpMonitorChildNoPermissionWnd;
    }else if(SimilarKlinWnd == id && HZAuthenOpt::instance()->hasPermission(HZAuthen::up_xskx) == false)
    {
        id = SimilarKlinWndoPermissionWnd;
    }
    return id;
}

QWidget* DataCenterPageMainWnd::createSubTab()
{
    m_topSub = new CSubTab;
    m_topSub->setObjectName("topTab");
    m_topSub->addButton(QStringLiteral("集合竞价"),CallAuctionWnd);
    m_topSub->addButton(QStringLiteral("相似K线"),SimilarKlinWnd);
    m_topSub->addButton(QStringLiteral("涨停监测"),LimitUpMonitorWnd);
    m_topSub->addButton(QStringLiteral("龙虎榜单"),WinnersListWnd);
    m_topSub->addButton(QStringLiteral("资金异动"),FundAbnormalWnd);
    m_topSub->addButton(QStringLiteral("新股日历"),NewStockCalendarWnd);
    m_topSub->addButton(QStringLiteral("融资融券"),BlockTradeWnd);
    m_topSub->addButton(QStringLiteral("大宗交易"),SecuritiesMarginTradingWnd);
    m_topSub->setCurrentButton(CallAuctionWnd);
    m_topSub->setStretchLastType(CSubTab::Empty_Strecth);
    connect(m_topSub,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwtichWnd(int)));
    QWidget* pWnd = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_topSub);
    pWnd->setLayout(layout);
    pWnd->setObjectName("topTitle");
    pWnd->setFixedHeight(24);
    return pWnd;
}

QWidget *DataCenterPageMainWnd::createMidWnd()
{
    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createCallAuctionWnd,CallAuctionWnd,true);
    m_typeTabIdMap[CallAuctionWnd] = CallAuctionWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createCallAuctionNoPermissionWnd,CallAuctionNoPermissionWnd);
    m_typeTabIdMap[CallAuctionNoPermissionWnd] = CallAuctionWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createSimilarKlineWnd,SimilarKlinWnd,true);
    m_typeTabIdMap[SimilarKlinWnd] = SimilarKlinWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createSimilarKlineNoPermissionWnd,SimilarKlinWndoPermissionWnd);
    m_typeTabIdMap[SimilarKlinWndoPermissionWnd] = SimilarKlinWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createLimitUpMonitorWnd,LimitUpMonitorWnd);
    m_typeTabIdMap[LimitUpMonitorWnd] = LimitUpMonitorWnd;


    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createLimitUpMonitorNoPermissionWnd,LimitUpMonitorChildNoPermissionWnd);
    m_typeTabIdMap[LimitUpMonitorChildNoPermissionWnd] = LimitUpMonitorWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createWinnersListWndWnd,WinnersListWnd);
    m_typeTabIdMap[WinnersListWnd] = WinnersListWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createFundAbnormalWnd,FundAbnormalWnd);
    m_typeTabIdMap[FundAbnormalWnd] = FundAbnormalWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createNewStockCalendarWnd,NewStockCalendarWnd);
    m_typeTabIdMap[NewStockCalendarWnd] = NewStockCalendarWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createBlockTradeWnd,BlockTradeWnd);
    m_typeTabIdMap[BlockTradeWnd] = BlockTradeWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createSecuritiesMarginTradingWnd,SecuritiesMarginTradingWnd);
    m_typeTabIdMap[SecuritiesMarginTradingWnd] = SecuritiesMarginTradingWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createWinnersListSalesOfficeDetailWnd,WinnersListSalesOfficeDetailWnd);
    m_typeTabIdMap[WinnersListSalesOfficeDetailWnd] = WinnersListWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createBlockTradeDetailWnd,BlockTradeDetailWnd);
    m_typeTabIdMap[BlockTradeDetailWnd] = BlockTradeWnd;

    m_pMainChildWndMgr->registChildCreater(&DataCenterPageMainWnd::createBlockTradeBkAndStocksWnd,BlockTradeBkAndStocksWnd);
    m_typeTabIdMap[BlockTradeBkAndStocksWnd] = BlockTradeWnd;

    return m_pMainChildWndMgr->getCurrStackWnd();
}

ChildWndItem DataCenterPageMainWnd::createCallAuctionWnd()
{
    CallAuctionChildWnd * pWnd = new CallAuctionChildWnd;
    connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createSimilarKlineWnd()
{
    SimilarKlineMainWnd * pWnd = new SimilarKlineMainWnd;
    //connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createCallAuctionNoPermissionWnd()
{
    HZPixLayer * pWnd = new HZPixLayer(":/skin/default/img/permissionUi/permission_jhjj_bk.jpg",
                                       ":/skin/default/img/permissionUi/permission_jhjj_content.png",this);
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createLimitUpMonitorNoPermissionWnd()
{
    HZPixLayer * pWnd = new HZPixLayer(":/skin/default/img/permissionUi/permission_ztjc_bl.jpg",
                                       ":/skin/default/img/permissionUi/permission_ztjc_content.png",this);
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createSimilarKlineNoPermissionWnd()
{
    HZPixLayer * pWnd = new HZPixLayer(":/skin/default/img/permissionUi/permission_xskx_bl.jpg",
                                       ":/skin/default/img/permissionUi/permission_xskx_content.png",this);
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createLimitUpMonitorWnd()
{
    LimitUpMonitorChildWnd * pWnd = new LimitUpMonitorChildWnd;
    connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createWinnersListWndWnd()
{
    m_winnerListWnd = new WinnersListChildWnd;
    connect(m_winnerListWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(m_winnerListWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    m_winnerListWnd->createWg();
    ChildWndItem item;
    item.pObj = m_winnerListWnd;
    item.pWnd = m_winnerListWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createFundAbnormalWnd()
{
    FundAbnormalChildWnd * pWnd = new FundAbnormalChildWnd;
    connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createNewStockCalendarWnd()
{
    NewStockCalendarChildWnd * pWnd = new NewStockCalendarChildWnd;
    //connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createBlockTradeWnd()
{
    m_blockTradeChildWnd = new BlockTradeChildWnd;
    connect(m_blockTradeChildWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(m_blockTradeChildWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    m_blockTradeChildWnd->createWg();
    ChildWndItem item;
    item.pObj = m_blockTradeChildWnd;
    item.pWnd = m_blockTradeChildWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createSecuritiesMarginTradingWnd()
{
    SecuritiesMarginTradingChildWnd * pWnd = new SecuritiesMarginTradingChildWnd;
    connect(pWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(pWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;

}

ChildWndItem DataCenterPageMainWnd::createWinnersListSalesOfficeDetailWnd()
{
    m_salesOfficeDetailWnd = new WinnersListSalesOfficeChildWnd;
    connect(m_salesOfficeDetailWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(m_salesOfficeDetailWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    m_salesOfficeDetailWnd->createWg();
    ChildWndItem item;
    item.pObj = m_salesOfficeDetailWnd;
    item.pWnd = m_salesOfficeDetailWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createBlockTradeDetailWnd()
{
    m_blockTradeDetailChildWnd = new BlockTradeDetailChildWnd;
    m_blockTradeDetailChildWnd->createWg();
    ChildWndItem item;
    item.pObj = m_blockTradeDetailChildWnd;
    item.pWnd = m_blockTradeDetailChildWnd;
    return item;
}

ChildWndItem DataCenterPageMainWnd::createBlockTradeBkAndStocksWnd()
{
    m_blockTradeBkAndStockChildWnd = new BlockTradeBkAndStocksChildWnd;
    connect(m_blockTradeBkAndStockChildWnd,SIGNAL(signalStockObjChanged(const QString &)),this,SLOT(slotsStockObjChanged(const QString &)));
    connect(m_blockTradeBkAndStockChildWnd,SIGNAL(signalSWitchDetailPage(const QString&)),this,SLOT(slotsSWitchDetailPage(const QString &)));
    m_blockTradeBkAndStockChildWnd->createWg();
    ChildWndItem item;
    item.pObj = m_blockTradeBkAndStockChildWnd;
    item.pWnd = m_blockTradeBkAndStockChildWnd;
    return item;
}

