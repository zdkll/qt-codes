#include "quotepanelwnd.h"
#include <QHBoxLayout>
#include "httpdata.h"
#include "stockdetailquotepanelwnd.h"
#include "optionalstockquotewidget.h"
#include "indexhandicapwidget.h"
#include "indexdetailhandicapwidget.h"
#include "platedetailhandicapwidget.h"
#include "normalindexlhandicapwidget.h"

QuotePanelWnd::QuotePanelWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_childWndMgr(new HzChildWndMgr<QuotePanelWnd>(this,new QStackedWidget(this)))
{

}

QuotePanelWnd::~QuotePanelWnd()
{
    delete m_childWndMgr;
    m_childWndMgr = nullptr;
}

void QuotePanelWnd::createWg()
{
    //布局
    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(m_childWndMgr->getCurrStackWnd());
    this->setLayout(pLayout);

//    if(m_defaultType != QuotepanelTypeUnknown)
//    {
//        auto it = m_typeIndexMap.find(m_defaultType);
//        if(it != m_typeIndexMap.end())
//        {
//            setCurrWidget(it.value());
//        }
//    }
}

void QuotePanelWnd::setDefaultType(QuoteRightPanelType defaultType)
{
    m_defaultType = defaultType;
}

void QuotePanelWnd::afterActive()
{
    QuoteRightPanelType type =  convertTypeByObjCode(m_stockCode);
    if(type != QuotepanelTypeUnknown)
    {
        m_childWndMgr->setCurrWndByPageType(type,true);
    }
}

void QuotePanelWnd::afterDeactive()
{
    ChildWndItem item;
    m_childWndMgr->currChildWnd(item);
    item.pObj->afterDeactive();
}

void QuotePanelWnd::hideWnd(bool bHide)
{
    if(this->isHidden() && bHide == false)
    {
        this->show();
        this->afterActive();
    }else if(this->isVisible() && bHide)
    {
        this->hide();
        this->afterDeactive();
    }
}

void QuotePanelWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
    afterActive();
}

void QuotePanelWnd::OnStockCodeChange()
{
    m_childWndMgr->setCurrStock(m_stockCode,true);
}

ChildWndItem QuotePanelWnd::createStockDetailPanel()
{
    ChildWndItem item;
    StockDetailQuotePanelWnd* stockpanel = new StockDetailQuotePanelWnd;
    stockpanel->createWg();
    item.pObj = stockpanel;
    item.pWnd = stockpanel;
    return item;
}

ChildWndItem QuotePanelWnd::createPlateDetailPanel()
{
    ChildWndItem item;
    PlateDetailHandicapWidget* platedetail = new PlateDetailHandicapWidget;
    item.pObj = platedetail;
    item.pWnd = platedetail;
    return item;
}

ChildWndItem QuotePanelWnd::createIndexDetailPanel()
{
    ChildWndItem item;
    IndexDetailHandicapWidget* indexDetail = new IndexDetailHandicapWidget;
    item.pObj = indexDetail;
    item.pWnd = indexDetail;
    return item;
}

ChildWndItem QuotePanelWnd::createStockListPanel()
{
    ChildWndItem item;
    OptionalStockQuoteWidget* optionStock = new OptionalStockQuoteWidget;
    optionStock->createWg();
    item.pObj = optionStock;
    item.pWnd = optionStock;
    return item;
}

ChildWndItem QuotePanelWnd::createIndexListPanel()
{
    ChildWndItem item;
    IndexHandicapWidget* indexhandicap = new IndexHandicapWidget;
    item.pObj = indexhandicap;
    item.pWnd = indexhandicap;
    return item;
}

ChildWndItem QuotePanelWnd::createNonImpIndexListPanel()
{
    ChildWndItem item;
    NormalIndexHandicapWidget* indexhandicap = new NormalIndexHandicapWidget;
    item.pObj = indexhandicap;
    item.pWnd = indexhandicap;
    return item;
}

QuoteDetailPanelWnd::QuoteDetailPanelWnd(QWidget *parent, QString instName):
    QuotePanelWnd(parent,instName)
{
    m_impStockMap = Httpdata::instance()->getImpIndexStockMap();
}

void QuoteDetailPanelWnd::createWg()
{
//    //创建股票详情盘口
//    StockDetailQuotePanelWnd* stockpanel = new StockDetailQuotePanelWnd;
//    stockpanel->createWg();
//    addWidget(stockpanel,stockpanel);
//    m_typeIndexMap[stock_detail_right_panel] = 0;

//    //创建板块详情盘口
//    PlateDetailHandicapWidget* platedetail = new PlateDetailHandicapWidget;
//    addWidget(platedetail,platedetail);
//    m_typeIndexMap[block_detail_right_panel] = 1;

//    //创建指数详情盘口
//    IndexDetailHandicapWidget* indexDetail = new IndexDetailHandicapWidget;
//    addWidget(indexDetail,indexDetail);
//    m_typeIndexMap[index_detail_right_panel] = 2;


    m_childWndMgr->registChildCreater(&QuotePanelWnd::createStockDetailPanel,stock_detail_right_panel,true);
    m_childWndMgr->registChildCreater(&QuotePanelWnd::createPlateDetailPanel,block_detail_right_panel);
    m_childWndMgr->registChildCreater(&QuotePanelWnd::createIndexDetailPanel,index_detail_right_panel);
    m_childWndMgr->registChildCreater(&QuotePanelWnd::createNonImpIndexListPanel,index_nomal_detail_right_panel);
    QuotePanelWnd::createWg();
}

void QuoteDetailPanelWnd::showTickWnd()
{

}

QuoteRightPanelType QuoteDetailPanelWnd::convertTypeByObjCode(const QString &objCode)
{
    if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::MarketAll))
    {
        return stock_detail_right_panel;
    }else if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::IndexInner))
    {
        if(m_impStockMap.contains(objCode))
            return index_detail_right_panel;
        return index_nomal_detail_right_panel;
    }else if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::BlockWhole))
    {
        return block_detail_right_panel;
    }
    return stock_detail_right_panel;
}

QuoteListPanelWnd::QuoteListPanelWnd(QWidget *parent, QString instName):
    QuotePanelWnd(parent,instName)
{

}

void QuoteListPanelWnd::createWg()
{ 
//    //股票列表右侧盘口
//   OptionalStockQuoteWidget* optionStock = new OptionalStockQuoteWidget;
//   optionStock->createWg();
//   addWidget(optionStock,optionStock);
//   m_typeIndexMap[stock_list_right_panel] = 0;

//    //指数列表右侧盘口
//   IndexHandicapWidget* indexhandicap = new IndexHandicapWidget;
//   addWidget(indexhandicap,indexhandicap);
//   m_typeIndexMap[index_list_right_panel] = 1;


   m_childWndMgr->registChildCreater(&QuotePanelWnd::createStockListPanel,stock_list_right_panel,true);
   m_childWndMgr->registChildCreater(&QuotePanelWnd::createIndexListPanel,index_list_right_panel);

   QuotePanelWnd::createWg();
}

QuoteRightPanelType QuoteListPanelWnd::convertTypeByObjCode(const QString &objCode)
{
    if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::MarketAll))
    {
        return stock_list_right_panel;
    }else if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::IndexInner) ||
             Httpdata::instance()->isHasObjFromFilter(objCode,HZData::BlockWhole))
    {
        return index_list_right_panel;
    }/*else if(Httpdata::instance()->isHasObjFromFilter(objCode,HZData::BlockWhole))
    {
        return block_list_right_panel;
    }*/
    return stock_list_right_panel;
}
