#include "quotelistpagemainwnd.h"
#include <QVBoxLayout>
#include <QStackedWidget>

#include "quotelistinnerwnd.h"
#include "subtab.h"
#include "datamodeldef.h"
#include "basepage.h"
#include "pubmsgdef.h"
#include "quotepanelwnd.h"

QuoteListPageMainWnd::QuoteListPageMainWnd(QWidget *parent, QString instName)
    :BaseWidget(parent,instName),
     m_bottomSub(new CSubTab),
     m_topSub(new CSubTab),
     m_rightPwd(nullptr),
     m_pMainChildWndMgr(new HzChildWndMgr<QuoteListPageMainWnd>(this,new QStackedWidget(this)))
{
    createWg();
}

QuoteListPageMainWnd::~QuoteListPageMainWnd()
{
    delete m_pMainChildWndMgr;
     m_pMainChildWndMgr = nullptr;
}

void QuoteListPageMainWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void QuoteListPageMainWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void QuoteListPageMainWnd::createWg()
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

void QuoteListPageMainWnd::setRightWnd(QuoteListPanelWnd* rightWnd)
{
    m_rightPwd = rightWnd;
    m_rightPwd->hide();
}

void QuoteListPageMainWnd::recvParam(const DetailPageParam &param, bool bCurrPage)
{

    int id = HszhInnerWndType;
    if(param.childWndId != -1)
        id = param.childWndId;
    if(id != m_topSub->currentButton())
    {
        m_topSub->setCurrentButton(id);
    }
    if(m_pMainChildWndMgr->isRegistedType(id))
    {
        m_pMainChildWndMgr->setCurrWndByPageType(id,false);
        ChildWndItem item;
        m_pMainChildWndMgr->currChildWnd(item);
        QuotePageBaseInnerWnd* pWnd  = (QuotePageBaseInnerWnd*)item.pWnd;
        auto it =  m_typeFilterMap.find(id);//沪深综合页面
        if(it != m_typeFilterMap.end())
        {
            pWnd->setFilterFlag(it.value());
        }
        if(pWnd->bShowBtChildWnd())
        {
            m_bottomSub->show();
            hideRightPanel(false);
            if(param.thirdChildWndId != -1 && m_bottomSub->currentButton() != param.thirdChildWndId)
            {
                pWnd->switchChildWnd(param.thirdChildWndId,false);
                m_bottomSub->setCurrentButton(param.thirdChildWndId);
            }else if(pWnd->currType() != m_bottomSub->currentButton())
            {
                pWnd->switchChildWnd(m_bottomSub->currentButton(),false);
            }
        }
        else
        {
            m_bottomSub->hide();
            hideRightPanel(true);
        }
        m_pMainChildWndMgr->setCurrWndByPageType(id,bCurrPage);
    }
}

void QuoteListPageMainWnd::slotsSwtichWnd(int id)
{
    if(m_rightPwd && m_rightPwd->isVisible())
        m_rightPwd->afterDeactive();
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(CHILD_WND_ID,QString::number(id));
    params.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
    sendMessage(ROOT_FRAME,params,true);

//    int index = m_typeIndexMap[id];
//    QuotePageBaseInnerWnd* pWnd = (QuotePageBaseInnerWnd*)Widget(index);
//    clearCurrWidget();
//    if(pWnd)
//    {
//       auto it =  m_typeFilterMap.find(id);//沪深综合页面没有
//       if(it != m_typeFilterMap.end())
//       {
//           pWnd->setFilterFlag(it.value());
//       }
//       if(pWnd->bShowBtChildWnd())
//       {
//           m_bottomSub->show();
//           hideRightPanel(false);
//       }
//       else
//       {
//           m_bottomSub->hide();
//           hideRightPanel(true);
//       }
//    }
//    setCurrWidget(index);
}

QWidget *QuoteListPageMainWnd::createTopTitle()
{
    m_topSub->setObjectName("topTab");
    m_topSub->addButton(QStringLiteral("沪深综合"),HszhInnerWndType);
    m_topSub->addButton(QStringLiteral("沪深A股"),HsagListWndType);
    m_topSub->addButton(QStringLiteral("上证A股"),ShagListWndWndType);
    m_topSub->addButton(QStringLiteral("深证A股"),SzagListWndWndType);
    m_topSub->addButton(QStringLiteral("沪深风险警示"),HsfxListWndType);
    m_topSub->addButton(QStringLiteral("中小板"),ZxbkListWndType);
    m_topSub->addButton(QStringLiteral("创业板"),CybkListWndType);
    m_topSub->addButton(QStringLiteral("重要指数"),ZyzsListWndType);
    m_topSub->addButton(QStringLiteral("全部指数"),QbzsListWndType);
    m_topSub->setCurrentButton(HszhInnerWndType);
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

QWidget *QuoteListPageMainWnd::createMidWnd()
{
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createHszhInnerWnd,HszhInnerWndType,true);
     //特别注意，同一创建函数通过不同类型注册多次，内部创建时只会真正创建一个
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,HsagListWndType,false);
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,ShagListWndWndType,false);
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,SzagListWndWndType,false);
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,HsfxListWndType,false);
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,ZxbkListWndType,false);
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createQuoteListWnd,CybkListWndType,false);
     m_typeFilterMap[HsagListWndType] = HZData::MarketAll;
     m_typeFilterMap[ShagListWndWndType] = HZData::MarketSH;
     m_typeFilterMap[SzagListWndWndType] = HZData::MarketSZ;
     m_typeFilterMap[HsfxListWndType]  = HZData::MarketFX;
     m_typeFilterMap[ZxbkListWndType] = HZData::MarketZX;
     m_typeFilterMap[CybkListWndType] = HZData::MarketCY;

     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createImpIndexListWnd,ZyzsListWndType,false);
     m_typeFilterMap[ZyzsListWndType] = HZData::IndexSH;
     m_pMainChildWndMgr->registChildCreater(&QuoteListPageMainWnd::createIndexListWnd,QbzsListWndType,false);
     m_typeFilterMap[QbzsListWndType] = HZData::IndexInner;
     return m_pMainChildWndMgr->getCurrStackWnd();

//    QWidget* pStackWnd = getStackWnd();
//    QVector<QuotePageBaseInnerWnd*> wndList;
//    QuotePageBaseInnerWnd* pWnd = nullptr;

//    //沪深综合
//    pWnd = new HszhInnerWnd;
//    wndList.push_back(pWnd);
//    m_typeIndexMap[HszhInnerWndType] = 0;

//    //沪深A股 上证A股 深证A股 沪深风险警示 中小板 创业板
//    pWnd = new StockListWnd;
//    wndList.push_back(pWnd);
//    m_typeIndexMap[HsagListWndType] = 1;
//    m_typeIndexMap[ShagListWndWndType] = 1;
//    m_typeIndexMap[SzagListWndWndType] = 1;
//    m_typeIndexMap[HsfxListWndType] = 1;
//    m_typeIndexMap[ZxbkListWndType] = 1;
//    m_typeIndexMap[CybkListWndType] = 1;


//    m_typeFilterMap[HsagListWndType] = HZData::MarketAll;
//    m_typeFilterMap[ShagListWndWndType] = HZData::MarketSH;
//    m_typeFilterMap[SzagListWndWndType] = HZData::MarketSZ;
//    m_typeFilterMap[HsfxListWndType]  = HZData::MarketFX;
//    m_typeFilterMap[ZxbkListWndType] = HZData::MarketZX;
//    m_typeFilterMap[CybkListWndType] = HZData::MarketCY;

//    //重要指数
//    pWnd = new ImpIndexListWnd;
//    wndList.push_back(pWnd);
//    m_typeIndexMap[ZyzsListWndType] = 2;
//    m_typeFilterMap[ZyzsListWndType] = HZData::IndexSH;

//    //全部指数
//    pWnd = new IndexListWnd;
//    wndList.push_back(pWnd);
//    m_typeIndexMap[QbzsListWndType] = 3;
//    m_typeFilterMap[QbzsListWndType] = HZData::IndexInner;

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

QWidget *QuoteListPageMainWnd::createBottomWnd()
{
    m_bottomSub->setPacing(0);
    m_bottomSub->setObjectName("sletTab");
    m_bottomSub->addButton(QStringLiteral("基础行情"),QuoteLisBtBaseQuoteListType);
    m_bottomSub->addLine(1,20);
    m_bottomSub->addButton(QStringLiteral("资金流向"),QuoteLisBtFundFlowerFlowType);
    m_bottomSub->addLine(1,20);
//    m_bottomSub->addButton(QStringLiteral("DDE决策"),QuoteLisBtDDEType);
//    m_bottomSub->addLine(1,20);
    m_bottomSub->setCurrentButton(QuoteLisBtBaseQuoteListType);
    m_bottomSub->setStretchLastType(CSubTab::Button_Strecth);
    m_bottomSub->setFixedHeight(20);
    m_bottomSub->hide();
    return m_bottomSub;
}

void QuoteListPageMainWnd::hideRightPanel(bool bHide)
{
    if(m_rightPwd)
    {
        if(bHide && m_rightPwd->isVisible())
        {
            m_rightPwd->hide();

        }else if(bHide == false && m_rightPwd->isHidden())
        {
            m_rightPwd->show();
        }
    }
}

ChildWndItem QuoteListPageMainWnd::createHszhInnerWnd()
{
    QuotePageBaseInnerWnd* pWnd = new HszhInnerWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    pWnd->createWg();
    return item;
}

ChildWndItem QuoteListPageMainWnd::createQuoteListWnd()
{
    qDebug() << "QuoteListPageMainWnd::createQuoteListWnd()";
    QuotePageBaseInnerWnd* pWnd = new StockListWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    pWnd->createWg();
    return item;
}

ChildWndItem QuoteListPageMainWnd::createImpIndexListWnd()
{
    qDebug() << "QuoteListPageMainWnd::createImpIndexListWnd";
    QuotePageBaseInnerWnd* pWnd = new ImpIndexListWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    pWnd->createWg();
    return item;
}

ChildWndItem QuoteListPageMainWnd::createIndexListWnd()
{
    qDebug() << "QuoteListPageMainWnd::createIndexListWnd";
    QuotePageBaseInnerWnd* pWnd = new IndexListWnd;
    connect(pWnd,SIGNAL(signalsClickObjItem(const QString&)),this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(pWnd,SIGNAL(signalsDbClickObjItem(const QString&)),this,SIGNAL(signalsDbClickObjItem(const QString&)));
    connect(m_bottomSub,SIGNAL(currentButtonChanged(int)),pWnd,SLOT(slotsSwtichChildWnd(int)));
    ChildWndItem item;
    item.pObj = pWnd;
    item.pWnd = pWnd;
    pWnd->createWg();
    return item;
}
