#include "quotelistinnerwnd.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QStackedWidget>

#include "hszhrightwnd.h"
#include "hszhleftwnd.h"

#include "fundview.h"
#include "stockdynaview.h"
#include "optionstockview.h"

QuoteListInnerWnd::QuoteListInnerWnd(QWidget *parent, QString instName):
    QuotePageBaseInnerWnd(parent,instName),
    m_pMainChildWndMgr(new HzChildWndMgr<QuoteListInnerWnd>(this,new QStackedWidget(this)))
{

}

void QuoteListInnerWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
//    pLayout->addWidget(getStackWnd());

//    //添加基础行情
//    QuoteListInnerWndItem item = createBaseQuoteList();
//    addWidget(item.pWnd,item.pObj);
//    m_typeIndexMap[QuoteLisBtBaseQuoteListType] = 0;

//    //添加资金流向
//    item = createFundFlowList();
//    addWidget(item.pWnd,item.pObj);
//    m_typeIndexMap[QuoteLisBtFundFlowerFlowType] =1;

////    //添加DDE决策
////    item = createDDEList();
////    addWidget(item.pWnd,item.pObj);
////    m_typeIndexMap[QuoteLisBtDDEType] =2;


    m_pMainChildWndMgr->registChildCreater(&QuoteListInnerWnd::createBaseQuoteListEx,QuoteLisBtBaseQuoteListType,true);
    m_pMainChildWndMgr->registChildCreater(&QuoteListInnerWnd::createFundFlowListEx,QuoteLisBtFundFlowerFlowType,false);
    m_pMainChildWndMgr->setCurrWndByPageType(QuoteLisBtBaseQuoteListType,false);
    //m_pMainChildWndMgr->registChildCreater(&QuoteListInnerWnd::createDDEListEx,QuoteLisBtDDEType,false);
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd());
    this->setLayout(pLayout);
}

void QuoteListInnerWnd::switchChildWnd(int id, bool bactive)
{
//    auto it = m_typeIndexMap.find(id);
//    if(it != m_typeIndexMap.end())
//    {
//        clearCurrWidget();
//        setCurrWidget(it.value(),bactive);
//    }
    if(m_pMainChildWndMgr->isRegistedType(id))
    {
        m_pMainChildWndMgr->setCurrWndByPageType(id,bactive);
    }
}

void QuoteListInnerWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
    qDebug() << "QuoteListInnerWnd::afterDeactive" << "currId: " << m_pMainChildWndMgr->getCurrType();
}

void QuoteListInnerWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
    qDebug() << "QuoteListInnerWnd::afterActive" << "currId: " << m_pMainChildWndMgr->getCurrType();
}

int QuoteListInnerWnd::currType()
{
    return m_pMainChildWndMgr->getCurrType();
}

void QuoteListInnerWnd::slotsSwtichChildWnd(int type)
{
    if(this->isVisible())
    {
        qDebug() << "QuoteListInnerWnd::slotsSwtichChildWnd type: " << type;
        if(m_pMainChildWndMgr->isRegistedType(type))
        {
            m_pMainChildWndMgr->setCurrWndByPageType(type,true);
        }
    }
}

QuoteListInnerWndItem QuoteListInnerWnd::createBaseQuoteList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}

QuoteListInnerWndItem QuoteListInnerWnd::createFundFlowList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}

QuoteListInnerWndItem QuoteListInnerWnd::createDDEList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}

ChildWndItem QuoteListInnerWnd::createBaseQuoteListEx()
{
    QuoteListInnerWndItem retItem = createBaseQuoteList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

ChildWndItem QuoteListInnerWnd::createFundFlowListEx()
{
    QuoteListInnerWndItem retItem = createFundFlowList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

ChildWndItem QuoteListInnerWnd::createDDEListEx()
{
    QuoteListInnerWndItem retItem = createDDEList();
    ChildWndItem item;
    item.pObj = retItem.pObj;
    item.pWnd = retItem.pWnd;
    return item;
}

HszhInnerWnd::HszhInnerWnd(QWidget *parent, QString instName):
    QuotePageBaseInnerWnd(parent,instName)
{

}

void HszhInnerWnd::afterActive()
{
    m_rightWnd->afterActive();
    m_leftWnd->afterActive();
}

void HszhInnerWnd::afterDeactive()
{
    m_rightWnd->afterDeactive();
    m_leftWnd->afterDeactive();
}

void HszhInnerWnd::setStockCode(const QString &stock)
{

}

void HszhInnerWnd::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(1);
    mainLayout->setSpacing(1);

    //左侧
    m_leftWnd = new HszhLeftWnd;
    mainLayout->addWidget(m_leftWnd,1);

    //右侧
    m_rightWnd = new  HszhRightWnd(this);
    mainLayout->addWidget(m_rightWnd,1);

    this->setLayout(mainLayout);
}





StockListWnd::StockListWnd(QWidget *parent, QString instName):
    QuoteListInnerWnd(parent,instName)
{

}

void StockListWnd::setFilterFlag(const int &filterFlag)
{
    m_filterFlag = filterFlag;
    if(m_pBaseView)
        m_pBaseView->setFilterFlag(filterFlag);
    if(m_fundView)
        m_fundView->setFilterFlag(filterFlag);
}

void StockListWnd::setFilterDetail(const QString &filter_detail)
{
    m_filter_detail = filter_detail;
    if(m_pBaseView)
        m_pBaseView->setFilterDetail(filter_detail);
    if(m_fundView)
        m_fundView->setFilterDetail(filter_detail);
}

QuoteListInnerWndItem StockListWnd::createBaseQuoteList()
{
    QuoteListInnerWndItem item;
    m_pBaseView = new StockDynaView(StockDynaView::StockDyna);
    connect(m_pBaseView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_pBaseView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    m_pBaseView->setFilterFlag(m_filterFlag);
    m_pBaseView->setFilterDetail(m_filter_detail);
    item.pObj = m_pBaseView;
    item.pWnd = m_pBaseView;
    return item;
}

QuoteListInnerWndItem StockListWnd::createFundFlowList()
{
    QuoteListInnerWndItem item;

    //个股资金
    m_fundView = new FundFlowView(FundFlowView::StockMoney);
    connect(m_fundView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_fundView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    m_fundView->setFilterFlag(m_filterFlag);
    m_fundView->setFilterDetail(m_filter_detail);
    item.pWnd = m_fundView;
    item.pObj = m_fundView;
    return item;
}

QuoteListInnerWndItem StockListWnd::createDDEList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}

///ImpIndexListWnd ----
ImpIndexListWnd::ImpIndexListWnd(QWidget *parent, QString instName)
    :QuoteListInnerWnd(parent,instName)
{

}

QuoteListInnerWndItem ImpIndexListWnd::createBaseQuoteList()
{
    QuoteListInnerWndItem item;
    //个股资金
    m_pBaseView = new ImpIndexDynaView(OverallView,this);
    connect(m_pBaseView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_pBaseView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    item.pWnd = m_pBaseView;
    item.pObj = m_pBaseView;
    return item;
}

QuoteListInnerWndItem ImpIndexListWnd::createFundFlowList()
{
    QuoteListInnerWndItem item;
    m_fundView = new OptionFundFlowView(OptionFundFlowView::OFT_ImpIndex,this);
    connect(m_fundView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_fundView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    item.pWnd = m_fundView;
    item.pObj = m_fundView;
    return item;
}

QuoteListInnerWndItem ImpIndexListWnd::createDDEList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}

///IndexListWnd-----
IndexListWnd::IndexListWnd(QWidget *parent, QString instName):
    QuoteListInnerWnd(parent,instName)
{

}

void IndexListWnd::setFilterFlag(const int &filterFlag)
{
    if(m_pBaseView)
        m_pBaseView->setFilterFlag(filterFlag);
}

void IndexListWnd::setFilterDetail(const QString &filter_detail)
{
    if(m_pBaseView)
         m_pBaseView->setFilterDetail(filter_detail);
}

QuoteListInnerWndItem IndexListWnd::createBaseQuoteList()
{
    QuoteListInnerWndItem item;
    //个股资金
    m_pBaseView = new StockDynaView(StockDynaView::IndexDyna);
    connect(m_pBaseView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_pBaseView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));
    m_pBaseView->setFilterFlag(m_filterFlag);
    item.pWnd = m_pBaseView;
    item.pObj = m_pBaseView;
    return item;
}


QuoteListInnerWndItem IndexListWnd::createFundFlowList()
{
    QuoteListInnerWndItem item;
    m_fundView = new OptionFundFlowView(OptionFundFlowView::OFT_ImpIndex,this);

    connect(m_fundView,SIGNAL(itemSelected(const QString &)),
            this,SIGNAL(signalsClickObjItem(const QString&)));
    connect(m_fundView,SIGNAL(itemEntered(const QString &)),
            this,SIGNAL(signalsDbClickObjItem(const QString&)));

    item.pWnd = m_fundView;
    item.pObj = m_fundView;
    return item;
}

QuoteListInnerWndItem IndexListWnd::createDDEList()
{
    QuoteListInnerWndItem item;
    BaseWidget* pWnd = new BaseWidget;
    item.pWnd = pWnd;
    item.pObj = pWnd;
    return item;
}
