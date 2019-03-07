#include "optionmainwnd.h"
#include <QVBoxLayout>
#include <QStackedWidget>

#include "optionmaininnerwnd.h"
#include "subtab.h"
#include "basepage.h"
#include "pubmsgdef.h"
#include "selfstocksettingdialog.h"

OptionMainWnd::OptionMainWnd(QWidget *parent, QString instName)
    :BaseWidget(parent,instName),
     m_bottomSub(new CSubTab),
     m_topSub(new CSubTab),
     m_pMainChildWndMgr(new HzChildWndMgr<OptionMainWnd>(this,new QStackedWidget(this)))
{
}

OptionMainWnd::~OptionMainWnd()
{
    delete m_pMainChildWndMgr;
    m_pMainChildWndMgr = nullptr;
}

void OptionMainWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void OptionMainWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void OptionMainWnd::createWg()
{
    QVBoxLayout * pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addWidget(createTopTitle());
    pLayout->setSpacing(1);
    pLayout->addWidget(createMidWnd());
    pLayout->setSpacing(1);
    pLayout->addWidget(createBottomWnd());
    pLayout->setSpacing(1);
    this->setLayout(pLayout);
}

void OptionMainWnd::setRightPnaleWnd(BaseWidget *pPanel)
{
    m_rightPanel = pPanel;
}

void OptionMainWnd::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    int thirdChildWndId = param.thirdChildWndId;
    int id = OptionListWndType;
    if(param.childWndId != -1)
        id = param.childWndId;
    if(id != m_topSub->currentButton())
    {
        m_topSub->setCurrentButton(id);
    }
    if(m_pMainChildWndMgr->isRegistedType(id))
    {
        m_pMainChildWndMgr->setCurrStock(m_rightPanel->getStockCode(),true);
        m_pMainChildWndMgr->setCurrWndByPageType(id,false);//先切换到当前页面
        ChildWndItem item;
        m_pMainChildWndMgr->currChildWnd(item);
        OptionMainInnerWnd* pWnd  = (OptionMainInnerWnd*)item.pWnd;
        //隐藏显示底部导航
        bool bShow = pWnd->bShowBtChildWnd();
        if(bShow && thirdChildWndId == -1)//默认则赋值
            thirdChildWndId = m_bottomSub->currentButton();
       if(bShow == false && m_bottomSub->isVisible())
           m_bottomSub->hide();
       else if(bShow)
       {
           if(m_bottomSub->isHidden())
           {
              m_bottomSub->show();
           }
           if(thirdChildWndId == -1)
               thirdChildWndId = m_bottomSub->currentButton();
           if(thirdChildWndId != pWnd->currType())
           {
               pWnd->switchChildWnd(thirdChildWndId,false);
           }
           if(thirdChildWndId != m_bottomSub->currentButton())
               m_bottomSub->setCurrentButton(thirdChildWndId);
       }
       if(bCurrPage)//激活当前页面
           m_pMainChildWndMgr->setCurrWndByPageType(id,true);
       //隐藏显示右侧盘口
       bShow = pWnd->bShowRightPanelWnd();
       if(bShow && m_rightPanel->isHidden())
       {
           m_rightPanel->show();
       }else if(bShow == false  && m_rightPanel->isVisible())
       {
           m_rightPanel->afterDeactive();
           m_rightPanel->hide();
       }
    }
}

void OptionMainWnd::slotsSwtichWnd(int id)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(CHILD_WND_ID,QString::number(id));
    params.emplace(HZ_PAGE_ID,QString::number(option_stock_page));
    sendMessage(ROOT_FRAME,params,true);
}

void OptionMainWnd::onSetBtnClicked()
{
    SelfStockSettingDialog dlg;
    dlg.exec();
}

QWidget *OptionMainWnd::createTopTitle()
{
    m_topSub->setObjectName("topTab");
    m_topSub->addButton(QStringLiteral("自选股"),OptionListWndType);
    m_topSub->addButton(QStringLiteral("最近浏览"),OptionRecentBrowseWndType);
    m_topSub->addButton(QStringLiteral("自选股资讯"),OptionInfoWndType);
    m_topSub->addButton(QStringLiteral("智能盯盘"),OptionSmartMonitorWndType);
    m_topSub->addButton(QStringLiteral("自选股周报"),OptionWeeklyNewspaperWndType);
    m_topSub->setCurrentButton(OptionListWndType);
    m_topSub->setStretchLastType(CSubTab::Empty_Strecth);

    QPushButton *setBtn = new QPushButton(this);
    setBtn->setObjectName("setting");
    connect(setBtn, &QPushButton::clicked, this, &OptionMainWnd::onSetBtnClicked);
    connect(m_topSub,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwtichWnd(int)));
    QWidget* pWnd = new QWidget;
//    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_topSub);
    layout->addStretch();
    layout->addWidget(setBtn);
    layout->addSpacing(12);
    pWnd->setLayout(layout);
    pWnd->setObjectName("topTitle");
    pWnd->setFixedHeight(24);
    return pWnd;
}

QWidget *OptionMainWnd::createMidWnd()
{
     m_pMainChildWndMgr->registChildCreater(&OptionMainWnd::createOptionWnd,OptionListWndType,true);
     m_pMainChildWndMgr->registChildCreater(&OptionMainWnd::createRecentBrowseWnd,OptionRecentBrowseWndType,false);
     m_pMainChildWndMgr->registChildCreater(&OptionMainWnd::createOptionInfoWnd,OptionInfoWndType,false);
     m_pMainChildWndMgr->registChildCreater(&OptionMainWnd::createOptionSmartMonitorWnd,OptionSmartMonitorWndType,false);
     m_pMainChildWndMgr->registChildCreater(&OptionMainWnd::createOptionWeeklyNewspaperWnd,OptionWeeklyNewspaperWndType,false);
     return m_pMainChildWndMgr->getCurrStackWnd();

     //    QWidget* pStackWnd = getStackWnd();
//    QVector<OptionMainInnerWnd*> wndList;
//    OptionMainInnerWnd* pWnd = nullptr;
//    //创建自选股
//    pWnd = new OptionListWnd;
//    wndList.push_back(pWnd);
//    //创建最近浏览
//    pWnd = new OptionRecentBrowseWnd;
//    wndList.push_back(pWnd);
//    //创建自选股资讯
//    pWnd = new OptionInfoWnd;
//    wndList.push_back(pWnd);
//    //创建智能盯盘
//    pWnd = new OptionSmartMonitorWnd;
//    wndList.push_back(pWnd);
////    //创建自选股周报
////    pWnd = new OptionWeeklyNewspaperWnd;
////    wndList.push_back(pWnd);
//    for(int i=0;i < wndList.size();i++)
//    {
//       pWnd = wndList[i];
//       connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
//       connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
//       connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
//       pWnd->createWg();
//       addWidget(pWnd,pWnd);
//    }
//    return pStackWnd;
}

QWidget *OptionMainWnd::createBottomWnd()
{
    m_bottomSub->setPacing(0);
    m_bottomSub->setObjectName("sletTab");
    m_bottomSub->addButton(QStringLiteral("基础行情"),BtBaseQuoteListType);
    m_bottomSub->addLine(1,20);
    m_bottomSub->addButton(QStringLiteral("资金流向"),BtFundFlowerFlowType);
    m_bottomSub->addLine(1,20);
//    m_bottomSub->addButton(QStringLiteral("DDE决策"),BtDDEType);
//    m_bottomSub->addLine(1,20);
    m_bottomSub->setCurrentButton(BtBaseQuoteListType);
    m_bottomSub->setStretchLastType(CSubTab::Button_Strecth);
    m_bottomSub->setFixedHeight(20);
    return m_bottomSub;
}

ChildWndItem OptionMainWnd::createOptionWnd()
{
    OptionMainInnerWnd* pWnd = new OptionListWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem OptionMainWnd::createRecentBrowseWnd()
{
    OptionMainInnerWnd* pWnd = new OptionRecentBrowseWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem OptionMainWnd::createOptionInfoWnd()
{
    OptionMainInnerWnd* pWnd = new OptionInfoWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem OptionMainWnd::createOptionSmartMonitorWnd()
{
    OptionMainInnerWnd* pWnd = new OptionSmartMonitorWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}

ChildWndItem OptionMainWnd::createOptionWeeklyNewspaperWnd()
{
    OptionMainInnerWnd *pWnd = new OptionWeeklyNewspaperWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    pWnd->createWg();
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    return item;
}
