#include "optionmaininnerwnd.h"
#include <QGridLayout>
#include <QLabel>

#include "subtab.h"
#include "optionstockview.h"
#include "hzinfonews.h"
#include "infodataprovidor.h"
#include "fundview.h"
#include "navilistview.h"
#include "weeklys.h"

OptionMainInnerWnd::OptionMainInnerWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName)
{
}

OptionListInnerWnd::OptionListInnerWnd(QWidget *parent, QString instName):
    OptionMainInnerWnd(parent,instName),
    m_pMainChildWndMgr(new HzChildWndMgr<OptionListInnerWnd>(this,new QStackedWidget(this)))
{

}

void OptionListInnerWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);


    //    pLayout->addWidget(getStackWnd());

    //    //添加基础行情
    //    ListInnerWndItem item = createBaseQuoteList();
    //    addWidget(item.pWnd,item.pObj);
    //    m_typeIndexMap[BtBaseQuoteListType] = 0;

    //    //添加资金流向
    //    item = createFundFlowList();
    //    addWidget(item.pWnd,item.pObj);
    //    m_typeIndexMap[BtFundFlowerFlowType] =1;

    ////    //添加DDE决策
    ////    item = createDDEList();
    ////    addWidget(item.pWnd,item.pObj);
    ////    m_typeIndexMap[BtDDEType] =2;
    //    this->setLayout(pLayout);

    m_pMainChildWndMgr->registChildCreater(&OptionListInnerWnd::createBaseQuoteListEx,BtBaseQuoteListType,true);
    m_pMainChildWndMgr->registChildCreater(&OptionListInnerWnd::createFundFlowListEx,BtFundFlowerFlowType,false);
    m_pMainChildWndMgr->setCurrWndByPageType(BtBaseQuoteListType,false);
    //m_pMainChildWndMgr->registChildCreater(&OptionListInnerWnd::createDDEListEx,BtDDEType,false);
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd());
    this->setLayout(pLayout);
}

void OptionListInnerWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void OptionListInnerWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

int OptionListInnerWnd::currType()
{
    return m_pMainChildWndMgr->getCurrType();
}

void OptionListInnerWnd::switchChildWnd(int type, bool bActive)
{
    m_pMainChildWndMgr->setCurrWndByPageType(type,bActive);
}

void OptionListInnerWnd::slotsSwtichChildWnd(int type)
{
    if(this->isVisible())
    {
        if(m_pMainChildWndMgr->isRegistedType(type))
        {
            m_pMainChildWndMgr->setCurrWndByPageType(type,true);
        }
    }
}

ListInnerWndItem OptionListInnerWnd::createBaseQuoteList()
{
    return ListInnerWndItem();
}

ListInnerWndItem OptionListInnerWnd::createFundFlowList()
{
    return ListInnerWndItem();
}

ListInnerWndItem OptionListInnerWnd::createDDEList()
{
    return ListInnerWndItem();
}

ChildWndItem OptionListInnerWnd::createBaseQuoteListEx()
{
    ListInnerWndItem retItem = createBaseQuoteList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

ChildWndItem OptionListInnerWnd::createFundFlowListEx()
{
    ListInnerWndItem retItem = createFundFlowList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

ChildWndItem OptionListInnerWnd::createDDEListEx()
{
    ListInnerWndItem retItem = createDDEList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

OptionListWnd::OptionListWnd(QWidget *parent, QString instName):
    OptionListInnerWnd(parent,instName)
{

}

ListInnerWndItem OptionListWnd::createBaseQuoteList()
{
    ListInnerWndItem item;
    ZXStockDynaView* view = new ZXStockDynaView(OverallView,this);
    connect(view,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(view,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));

    item.pWnd = view;
    item.pObj = view;
    return item;
}

ListInnerWndItem OptionListWnd::createFundFlowList()
{
    ListInnerWndItem item;
    OptionFundFlowView *view = new OptionFundFlowView(OptionFundFlowView::OFT_ZXStock);

    connect(view,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(view,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));

    item.pWnd = view;
    item.pObj = view;
    return item;
}



ListInnerWndItem OptionListWnd::createDDEList()
{
    ListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    item.pWnd->setObjectName("ddelist");
    return item;
}

OptionRecentBrowseWnd::OptionRecentBrowseWnd(QWidget *parent, QString instName):
    OptionListInnerWnd(parent,instName)
{

}

ListInnerWndItem OptionRecentBrowseWnd::createBaseQuoteList()
{
    HistStockDynaView* view = new HistStockDynaView(OverallView,this);
    connect(view,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(view,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    ListInnerWndItem item;

    item.pWnd = view;
    item.pObj = view;

    return item;
}

ListInnerWndItem OptionRecentBrowseWnd::createFundFlowList()
{
    ListInnerWndItem item;
    OptionFundFlowView *view = new OptionFundFlowView(OptionFundFlowView::OFT_HistStock);

    connect(view,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(view,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));

    item.pWnd = view;
    item.pObj = view;
    return item;
}

ListInnerWndItem OptionRecentBrowseWnd::createDDEList()
{
    ListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    item.pWnd->setObjectName("ddelist");
    return item;
}

///
/// \brief OptionInfoWnd::OptionInfoWnd
/// \param parent
/// \param instName
///
OptionInfoWnd::OptionInfoWnd(QWidget *parent, QString instName):
    OptionMainInnerWnd(parent,instName)
  ,mZxView(new NaviZXListView)
{
    mZxView->setFixedWidth(226);

    InfoNewsProvidor *mInfoNewsData = new InfoNewsProvidor;
    mNewWnd = new NewsMarster( mInfoNewsData,NewsMarster::ZXG_NEWS);
    mNewWnd->setObjectName("InfoWnd");
    mZxView->setListViewAutoSelected(true);
    connect(mZxView,&NaviZXListView::selectedItem, mNewWnd, &NewsMarster::slotZXStockClicked);
    connect(mZxView, &NaviZXListView::signalRowCountChanged, [=]{
        qDebug() << "----------count changed------------";
        mNewWnd->afterActive();
    });

    QLabel *titleLabel = new QLabel(QStringLiteral("自选"));
    titleLabel->setObjectName("CustomTitle");
    titleLabel->setFixedHeight(28);
    titleLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(0);
    vl->addWidget(titleLabel);
    vl->addWidget(mZxView);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->setContentsMargins(0,0,0,0);
    hl->setSpacing(1);
    hl->addLayout(vl);
    hl->addWidget(mNewWnd);
    setLayout(hl);
}

void OptionInfoWnd::createWg()
{
    //    QVBoxLayout* pLayout = new QVBoxLayout();
    //    pLayout->setMargin(0);
    //    pLayout->setSpacing(0);
    //    pLayout->addWidget(getStackWnd());
    //    this->setLayout(pLayout);
}

void OptionInfoWnd::afterActive()
{
    if(mZxView)
        mZxView->afterActive();

    if(mNewWnd)
        mNewWnd->afterActive();
}

void OptionInfoWnd::afterDeactive()
{
    if(mZxView)
        mZxView->afterDeactive();

    if(mNewWnd)
        mNewWnd->afterDeactive();
}

OptionSmartMonitorWnd::OptionSmartMonitorWnd(QWidget *parent, QString instName):
    OptionMainInnerWnd(parent,instName),
    m_pMainChildWndMgr(new HzChildWndMgr<OptionSmartMonitorWnd>(this,new QStackedWidget(this)))
{
    setStyleSheet("QPushButton{border:0px solid transparent;}");
}

void OptionSmartMonitorWnd::createWg()
{
    QGridLayout* pGridLayout = new QGridLayout;
    pGridLayout->setMargin(0);
    pGridLayout->setHorizontalSpacing(1);
    pGridLayout->setVerticalSpacing(1);
    //自选股标题
    QLabel* pLabel = new QLabel;
    pLabel->setObjectName("optionLabel");
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setText(QStringLiteral("自选股"));
    pLabel->setFixedSize(210,26);
    pGridLayout->addWidget(pLabel,0,0);

    //右侧标题
    m_topSub = new CSubTab;
    m_topSub->setObjectName("rightSletTab");
    m_topSub->setFixedHeight(26);
    m_topSub->addButton(QStringLiteral("公司大事"),0);
    m_topSub->addLine(1,12);
    m_topSub->addButton(QStringLiteral("盘中异动"),1);

    m_topSub->addLine(1,12);
    m_topSub->setCurrentButton(0);
    m_topSub->setStretchLastType(CSubTab::StretchLastType::Empty_Strecth);
    connect(m_topSub,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwtichWnd(int)));
    pGridLayout->addWidget(m_topSub,0,1);

    //添加自选列表
    QWidget* pWnd = createLeftOptionListWnd().pWnd;
    pWnd->setFixedWidth(210);
    pGridLayout->addWidget(pWnd,1,0);

    m_pMainChildWndMgr->registChildCreater(&OptionSmartMonitorWnd::createCompanyEventWndEx,CompanyEventWndId,true);
    m_pMainChildWndMgr->registChildCreater(&OptionSmartMonitorWnd::createAbnormalChangesWndEx,AbnormalChangesWndId,false);
    m_pMainChildWndMgr->setCurrWndByPageType(CompanyEventWndId,false);
    //添加盘中异动，公司大事窗口
    pGridLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd(),1,1);

    this->setLayout(pGridLayout);
    //update custom stocks
    connect(gInfoStyle, &InfoStyle::signalUpdateCustomStockCache, [=]{
        afterActive();
    });
}

void OptionSmartMonitorWnd::afterActive()
{
    if(mZxView)
    {
        mZxView->afterActive();
    }
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
    OptionMainInnerWnd::afterActive();
}

void OptionSmartMonitorWnd::afterDeactive()
{
    if(mZxView)
        mZxView->afterDeactive();
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void OptionSmartMonitorWnd::slotsSwtichWnd(int id)
{
    if(m_pMainChildWndMgr->isRegistedType(id))
    {
        m_pMainChildWndMgr->setCurrWndByPageType(id,true);
    }
}

void OptionSmartMonitorWnd::OnStockCodeChange()
{
    if(mZxView)
        mZxView->setStockCode(m_stockCode);
}

ListInnerWndItem OptionSmartMonitorWnd::createLeftOptionListWnd()
{
    /*
        void signalsClickObjItem(const QString& obj);
        void signalsDbClickObjItem(const QString& obj);
        实现右侧列表时，实现这个俩个信号的抛出
    */

#if 0
    ListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    item.pWnd->setObjectName("LeftOptionListWnd");AA
            return item;
#else //新增自选股列表
    ListInnerWndItem item;
    mZxView = new NaviZXListView;
    mZxView->setListViewAutoSelected(true);
    connect(mZxView,SIGNAL(selectedItem(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));

    item.pWnd = mZxView;
    item.pObj = mZxView;
    item.pWnd->setObjectName("LeftOptionListWnd");
    return item;
#endif
}

ChildWndItem OptionSmartMonitorWnd::createAbnormalChangesWndEx()
{
    ChildWndItem item;
    //    BaseWidget* pWnd = new BaseWidget;
    mSmartPanel = new DrawMaster(SmartPlate_Page);
    item.pWnd = mSmartPanel;
    item.pObj = mSmartPanel;
    item.pWnd->setObjectName("AbnormalChangesWnd");
    connect(mZxView,&NaviZXListView::selectedItem, mSmartPanel, &DrawMaster::slotZXStockClicked);
    connect(mSmartPanel, &DrawMaster::signalStockCodeClicked, [=](const QString &code){//跳转详情页
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(CHILD_WND_ID, QString::number(stock_detail_page));
        params.emplace(STOCK_ID,code);
        sendMessage(ROOT_FRAME,params);
    });
    return item;
}

ChildWndItem OptionSmartMonitorWnd::createCompanyEventWndEx()
{
    ChildWndItem item;
    mCompanyEvent = new DrawMaster(CompanyEvent_Page);
    item.pWnd = mCompanyEvent;
    item.pObj = mCompanyEvent;
    item.pWnd->setObjectName("CompanyEventWnd");
    connect(mZxView,&NaviZXListView::selectedItem, mCompanyEvent, &DrawMaster::slotZXStockClicked);
    return item;
}

OptionWeeklyNewspaperWnd::OptionWeeklyNewspaperWnd(QWidget *parent, QString instName):
    OptionMainInnerWnd(parent,instName)
{

}

void OptionWeeklyNewspaperWnd::createWg()
{
    //创建自选股周报
    m_weeklys = new weeklys;
    connect(m_weeklys,SIGNAL(signalRowClicked(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    //connect(m_weeklys,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));


//  m_weeklys->setObjectName("weeklyNewspaperWnd");
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(m_weeklys);
    this->setLayout(pLayout);
}

void OptionWeeklyNewspaperWnd::afterActive()
{
    m_weeklys->afterActive();
}

void OptionWeeklyNewspaperWnd::afterDeactive()
{
    m_weeklys->afterDeactive();
}

void OptionWeeklyNewspaperWnd::OnStockCodeChange()
{
//    m_weeklys->setStock(m_stockCode);
}
