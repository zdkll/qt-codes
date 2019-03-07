#include "blocklistmainwnd.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include "bklistbottomnavigator.h"
#include "pubmsgdef.h"
#include "subtab.h"
#include "hzline.h"
#include "fundview.h"
#include "stockdynaview.h"
#include "optionstockview.h"
#include "pubmsgdef.h"
#include "httpdata.h"
#include "datamodeldef.h"
#include "platehandicapwidget.h"
#include "basepage.h"

BlockListMainWnd::BlockListMainWnd(QWidget *parent, QString instName)
    :BaseWidget(parent,instName),
      m_pMainChildWndMgr(new HzChildWndMgr<BlockListMainWnd>(this,new QStackedWidget(this)))
{

}

void BlockListMainWnd::slotDbClickItem(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}

void BlockListMainWnd::slotBkCategoryNavChange(int filterFlag)
{

    if(m_filterFlag != filterFlag)
    {
        m_pCategoryWnd->setFilterFlag(filterFlag);
        m_bRecodFlag = true;
        m_filterFlag = filterFlag;
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(CHILD_WND_ID,QString::number(filterFlag));
        params.emplace(HZ_PAGE_ID,QString::number(block_list_page));
        sendMessage(ROOT_FRAME,params,true);
    }else
    {
        //设置列表filter
        m_pMainChildWndMgr->setCurrWndByPageType(BkListCategoryType,false);
        m_pCategoryWnd->setFilterFlag(filterFlag);
        m_pMainChildWndMgr->setCurrWndByPageType(BkListCategoryType,true); //激活当前页面
    }
}

void BlockListMainWnd::slotBkNavChange(int filterFlag, const QString &bkCode)
{
    m_filterStockMap[filterFlag] = bkCode;
    if(filterFlag != m_filterFlag || m_bRecodFlag == true)
    {
        m_bRecodFlag = false;
        m_filterFlag = filterFlag;
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(CHILD_WND_ID,QString::number(filterFlag));
        params.emplace(STOCK_ID,bkCode);
        params.emplace(HZ_PAGE_ID,QString::number(block_list_page));
        sendMessage(ROOT_FRAME,params,true);
    }else
    {
        //设置列表filter 和板块ID
        m_pMainChildWndMgr->setCurrWndByPageType(BkListChildType,false); //设置当前页面
        m_pChildWnd->setFilterInfo(filterFlag,bkCode); //设置参数
        m_pMainChildWndMgr->setCurrWndByPageType(BkListChildType,true); //激活当前页面
    }
}

void BlockListMainWnd::recvParam(const DetailPageParam &param, bool bCurrPage)
{
    m_filterFlag = param.childWndId;
    if(m_filterFlag == -1)
        m_filterFlag = 103;
    if(param.stock.isEmpty()) //通过判断是否有股票代码，来区分是板块，还是成分股页面
    {
        m_bkListNaibar->setFilterFlag(m_filterFlag,false);
        m_pCategoryWnd->setFilterFlag(m_filterFlag);
        //设置列表filter
        m_pMainChildWndMgr->setCurrWndByPageType(BkListCategoryType,bCurrPage);
    }else
    {
        m_bkListNaibar->setFilterFlag(m_filterFlag,true);
        m_stockCode = param.stock;
        if(m_filterStockMap.contains(m_filterFlag))
            m_stockCode =m_filterStockMap[m_filterFlag];
        if(bCurrPage)
        {
            //设置列表filter 和板块ID
            m_pMainChildWndMgr->setCurrWndByPageType(BkListChildType,false);
            m_pChildWnd->setFilterInfo(m_filterFlag,m_stockCode);
            m_pMainChildWndMgr->setCurrWndByPageType(BkListChildType,true);
        }
    }
}

void BlockListMainWnd::createWg()
{
    m_bkListNaibar = new BkListBottomNavigator(this);
    connect(m_bkListNaibar,SIGNAL(signalBkCategoryNavChange(int)),this,SLOT(slotBkCategoryNavChange(int)));
    connect(m_bkListNaibar,SIGNAL(signalBkNavChange(int,const QString&)),this,SLOT(slotBkNavChange(int,const QString&)));

    QGridLayout* pLayout = new QGridLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setVerticalSpacing(1);
    pLayout->setHorizontalSpacing(1);

    //标题
    m_bkListNaibar->setFixedHeight(22);
    pLayout->addWidget(m_bkListNaibar,0,0,1,1);

    //创建分类子导航
    m_pMainChildWndMgr->registChildCreater(&BlockListMainWnd::createBlockListCategoryWnd,BkListCategoryType,true);
    m_pMainChildWndMgr->registChildCreater(&BlockListMainWnd::createBlockListWnd,BkListChildType,false);
    //减少后面页面切换复杂度，这里先暂时创建出两个页面
    m_pMainChildWndMgr->setCurrWndByPageType(BkListChildType,false);
    m_pMainChildWndMgr->setCurrWndByPageType(BkListCategoryType,false);
    //添加堆栈窗口 管理页面切换
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd(),1,0,1,1);

    //添加右侧窗口
    QWidget* pWnd = createRightPanelWnd();
    pWnd->setFixedWidth(350);
    pLayout->addWidget(pWnd,0,1,2,1);
    this->setLayout(pLayout);
}

void BlockListMainWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
    m_rightHandicap->afterActive();
}

void BlockListMainWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
    m_rightHandicap->afterDeactive();
}

QWidget *BlockListMainWnd::createRightPanelWnd()
{
    //    QWidget* pWnd = new QWidget;
    //    pWnd->setObjectName("panel");

    m_rightHandicap = new PlateHandicapWidget;
    return m_rightHandicap;
}

ChildWndItem BlockListMainWnd::createBlockListCategoryWnd()
{
    ChildWndItem item;
    //分类窗口
    m_pCategoryWnd = new BlockListCategoryWnd(this);
    m_pCategoryWnd->setFilterFlag(HZData::BlockWhole);
    connect(m_pCategoryWnd, &BlockListCategoryWnd::signalListCategoryCodeChange, this, &BlockListMainWnd::onBkCodeChanged);
    connect(m_pCategoryWnd, &BlockListCategoryWnd::signalStockCodeChange, this, &BlockListMainWnd::onStockCodeChanged);
    item.pObj = m_pCategoryWnd;
    item.pWnd = m_pCategoryWnd;
    return item;
}

ChildWndItem BlockListMainWnd::createBlockListWnd()
{
    ChildWndItem item;
    //子板块窗口
    m_pChildWnd = new BlockListWnd(this);
    connect(m_pChildWnd, &BlockListWnd::signalStockCodeChange, this, &BlockListMainWnd::onStockCodeChanged);
    item.pObj = m_pChildWnd;
    item.pWnd = m_pChildWnd;
    return item;
}

void BlockListMainWnd::onBkCodeChanged(const QString &bkCode)
{
    m_rightHandicap->setStockCode(bkCode);
    m_rightHandicap->afterActive();
}

void BlockListMainWnd::onStockCodeChanged(const QString &bkCode)
{
    m_rightHandicap->setStockCode(bkCode);
    m_rightHandicap->afterActive();
}


BlockListCategoryWnd::BlockListCategoryWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_pMainChildWndMgr(new HzChildWndMgr<BlockListCategoryWnd>(this,new QStackedWidget(this)))
{
    m_filterFlag = -1;
    createWg();
}

void BlockListCategoryWnd::setFilterFlag(const int &filterFlag)
{
    m_filterFlag = filterFlag;
    if(m_pBaseView)
        m_pBaseView->setFilterFlag(filterFlag);
    if(m_fundView)
        m_fundView->setFilterFlag(filterFlag);
    if(m_ablBlockDynaView)
        m_ablBlockDynaView->setFilterFlag(filterFlag);
    if(m_bkStockDynaView)
        m_bkStockDynaView->setFilterFlag(filterFlag);
}

void BlockListCategoryWnd::afterDeactive()
{
    m_bkStockDynaView->afterDeactive();
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void BlockListCategoryWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void BlockListCategoryWnd::slotsTabCurrentBtnChanged(int nId)
{
    if(this->isVisible())
    {
        if(m_pMainChildWndMgr->isRegistedType(nId))
        {
            m_pMainChildWndMgr->setCurrWndByPageType(nId,true);
        }
    }
}

void BlockListCategoryWnd::slotsHideBottomWnd(bool bHide)
{
    m_bkStockDynaView->setHidden(bHide);
}

void BlockListCategoryWnd::slotsCategoryWndItemSelected(const QString &stockCode)
{
    //标题文本处理
    m_childListDesc->setText(foramtTitleTextFromBkCode(m_filterFlag,stockCode));
    m_childListDesc->update();
    //通知右侧切换合约
    emit signalListCategoryCodeChange(stockCode);
    //通知底部切换成分股
    if(m_bkStockDynaView->isVisible())
    {
        m_bkStockDynaView->afterDeactive();
        m_bkStockDynaView->setFilterInfo(m_filterFlag,stockCode);
        m_bkStockDynaView->afterActive();
    }
}

void BlockListCategoryWnd::slotsCategoryWndEntered(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}

void BlockListCategoryWnd::slotsChildWndItemSelected(const QString &stockCode)
{
    //通知右侧切换成分股详情
    emit signalStockCodeChange(stockCode);
}

void BlockListCategoryWnd::slotsChildWndEntered(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}

void BlockListCategoryWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    //添加中间基础行情和资金页面
    m_pMainChildWndMgr->registChildCreater(&BlockListCategoryWnd::createBaseQuoteList,0,true);
    m_pMainChildWndMgr->registChildCreater(&BlockListCategoryWnd::createFundFlowList,1,false);
    m_pMainChildWndMgr->registChildCreater(&BlockListCategoryWnd::createAbnormalBkList,2,false);

    m_pMainChildWndMgr->setCurrWndByPageType(0,false);
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd());
    pLayout->setSpacing(1);

    CSubTab* subTab = new CSubTab();
    subTab->setPacing(0);
    subTab->setObjectName("roottab");
    subTab->setCSubTabType(CSubTab::Root_Tab);
    connect(subTab,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsTabCurrentBtnChanged(int)));
    subTab->addButton(QStringLiteral("基础行情"),0);
    subTab->addLine(1,24);
    subTab->addButton(QStringLiteral("资金流向"),1);
    subTab->addLine(1,24);
    subTab->addButton(QStringLiteral("异动板块"),2);
    subTab->addLine(1,24);
    subTab->setFixedHeight(24);
    subTab->setStretchLastType(CSubTab::Empty_Strecth);
    subTab->setCurrentButton(0);
    pLayout->addWidget(subTab);
    pLayout->setSpacing(1);

    QWidget* pBtNav = createBottomBar();
    pBtNav->setFixedHeight(24);
    pLayout->addWidget(pBtNav);
    pLayout->setSpacing(1);

    QWidget* pChildWnd = createChildWnd();
    pChildWnd->setFixedHeight(212);
    pLayout->addWidget(pChildWnd);
    this->setLayout(pLayout);
}

ChildWndItem BlockListCategoryWnd::createBaseQuoteList()
{
    ChildWndItem item;
    m_pBaseView = new StockDynaView(StockDynaView::BlockDyna);
    connect(m_pBaseView,SIGNAL(itemSelected(const QString &)),this,SLOT(slotsCategoryWndItemSelected(const QString &)));
    connect(m_pBaseView,SIGNAL(itemEntered(const QString&)),this,SLOT(slotsCategoryWndEntered(const QString &)));
    item.pObj = m_pBaseView;
    item.pWnd = m_pBaseView;
    m_pBaseView->setFilterFlag(m_filterFlag);
    return item;
}

ChildWndItem BlockListCategoryWnd::createFundFlowList()
{
    ChildWndItem item;
    //个股资金
    m_fundView = new FundFlowView(FundFlowView::BKMoney);
    connect(m_fundView,SIGNAL(itemSelected(const QString &)),this,SLOT(slotsCategoryWndItemSelected(const QString &)));
    connect(m_fundView,SIGNAL(itemEntered(const QString&)),this,SLOT(slotsCategoryWndEntered(const QString &)));
    item.pWnd = m_fundView;
    item.pObj = m_fundView;
    m_fundView->setFilterFlag(m_filterFlag);
    return item;
}

ChildWndItem BlockListCategoryWnd::createAbnormalBkList()
{
    ChildWndItem item;
    //异动板块列表
    m_ablBlockDynaView = new AblBlockDynaView(this);
    connect(m_ablBlockDynaView,SIGNAL(itemSelected(const QString &)), this,SLOT(slotsCategoryWndItemSelected(const QString&)));
    connect(m_ablBlockDynaView,SIGNAL(itemEntered(const QString &)),this,SLOT(slotsCategoryWndEntered(const QString&)));
    item.pWnd = m_ablBlockDynaView;
    item.pObj = m_ablBlockDynaView;
    m_ablBlockDynaView->setFilterFlag(m_filterFlag);
    return item;
}

QWidget *BlockListCategoryWnd::createChildWnd()
{
    m_bkStockDynaView = new  StockDynaView(StockDynaView::BKStockDyna,this);
    connect(m_bkStockDynaView,SIGNAL(itemSelected(const QString &)),this,SLOT(slotsChildWndItemSelected(const QString &)));
    connect(m_bkStockDynaView,SIGNAL(itemEntered(const QString&)),this,SLOT(slotsChildWndEntered(const QString &)));
    return m_bkStockDynaView;

}

QWidget *BlockListCategoryWnd::createBottomBar()
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
    //pLayout->setSpacing(15);

    m_childListDesc = new QLabel;
    m_childListDesc->setObjectName("titleText");
    m_childListDesc->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_childListDesc->setText(Default_Item_String);
    pLayout->addWidget(m_childListDesc);
    QWidget* pWnd =  new QWidget;
    pWnd->setLayout(pLayout);
    return pWnd;
}

QString BlockListCategoryWnd::foramtTitleTextFromBkCode(int filter, const QString &code)
{
    QString ret = Default_Item_String;
    if(!code.isEmpty())
    {
        int count = Httpdata::instance()->getCountByFiter(filter,code);
        QString name = Httpdata::instance()->getCodeNameByCodeFromBk(code);
        if(name.isEmpty() == false && count != -1)
        {
            ret = name + QStringLiteral("板块个股共有") + QString::number(count) + QStringLiteral("只");
        }
    }
    return ret;
}

BlockListWnd::BlockListWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_pMainChildWndMgr(new HzChildWndMgr<BlockListWnd>(this,new QStackedWidget(this)))

{
    createWg();
}

void BlockListWnd::setFilterInfo(const int &filterFlag, const QString &filter_detail)
{
    m_filterFlag = filterFlag;
    m_filter_detail = filter_detail;
    m_childListDesc->setText(foramtTitleTextFromBkCode(filterFlag,filter_detail));
    m_childListDesc->update();
    if( m_fundView)
        m_fundView->setFilterInfo(filterFlag,filter_detail);
    if(m_pBaseView)
        m_pBaseView->setFilterInfo(filterFlag,filter_detail);
}

void BlockListWnd::afterDeactive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void BlockListWnd::afterActive()
{
    ChildWndItem item;
    m_pMainChildWndMgr->currChildWnd(item);
    item.pObj->afterActive();
}

void BlockListWnd::slotsBlockListWndItemSelected(const QString &stockCode)
{
    //通知右侧
    emit signalStockCodeChange(stockCode);
}

void BlockListWnd::slotsBlockListWndEntered(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}

void BlockListWnd::slotsTabCurrentBtnChanged(int nId)
{
    if(this->isVisible())
    {
        if(m_pMainChildWndMgr->isRegistedType(nId))
        {
            m_pMainChildWndMgr->setCurrWndByPageType(nId,true);
        }
    }
}

void BlockListWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    m_childListDesc = new QLabel;
    m_childListDesc->setFixedHeight(24);
    m_childListDesc->setObjectName("titleText");
    m_childListDesc->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_childListDesc->setText(Default_Item_String);
    pLayout->addWidget(m_childListDesc);
    pLayout->setSpacing(1);

    //添加中间基础行情和资金页面
    m_pMainChildWndMgr->registChildCreater(&BlockListWnd::createBaseQuoteList,0,true);
    m_pMainChildWndMgr->registChildCreater(&BlockListWnd::createFundFlowList,1,false);
    m_pMainChildWndMgr->setCurrWndByPageType(0,false);
    pLayout->addWidget(m_pMainChildWndMgr->getCurrStackWnd());
    pLayout->setSpacing(1);

    CSubTab* subTab = new CSubTab();
    subTab->setPacing(0);
    subTab->setObjectName("roottab");
    subTab->setCSubTabType(CSubTab::Root_Tab);
    connect(subTab,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsTabCurrentBtnChanged(int)));
    subTab->addButton(QStringLiteral("基础行情"),0);
    subTab->addLine(1,24);
    subTab->addButton(QStringLiteral("资金流向"),1);
    subTab->addLine(1,24);
    subTab->setFixedHeight(24);
    subTab->setStretchLastType(CSubTab::Empty_Strecth);
    subTab->setCurrentButton(0);
    pLayout->addWidget(subTab);
    this->setLayout(pLayout);
}

ChildWndItem BlockListWnd::createBaseQuoteList()
{
    ChildWndItem item;
    m_pBaseView = new  BKAndStockView(this);
    connect(m_pBaseView,SIGNAL(itemSelected(const QString &)),
            this,SLOT(slotsBlockListWndItemSelected(const QString&)));
    connect(m_pBaseView,SIGNAL(itemEntered(const QString &)),
            this,SLOT(slotsBlockListWndEntered(const QString&)));
    item.pWnd = m_pBaseView;
    item.pObj = m_pBaseView;
    m_pBaseView->setFilterInfo(m_filterFlag,m_filter_detail);
    return item;
}

ChildWndItem BlockListWnd::createFundFlowList()
{
    ChildWndItem item;
    //个股资金
    m_fundView = new BKStockFundFlowView(this);
    connect(m_fundView,SIGNAL(itemSelected(const QString &)),
            this,SLOT(slotsBlockListWndItemSelected(const QString&)));
    connect(m_fundView,SIGNAL(itemEntered(const QString &)),
            this,SLOT(slotsBlockListWndEntered(const QString&)));
    item.pWnd = m_fundView;
    item.pObj = m_fundView;
    m_fundView->setFilterInfo(m_filterFlag,m_filter_detail);
    return item;
}

QString BlockListWnd::foramtTitleTextFromBkCode(int filter,const QString &code)
{
    QString ret = Default_Item_String;
    if(!code.isEmpty())
    {
        int count = Httpdata::instance()->getCountByFiter(filter,code);
        QString name = Httpdata::instance()->getCodeNameByCodeFromBk(code);
        if(name.isEmpty() == false && count != -1)
        {
            ret = name + QStringLiteral("板块个股共有") + QString::number(count) + QStringLiteral("只");
        }
    }
    return ret;
}
