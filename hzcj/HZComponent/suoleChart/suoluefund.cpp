#include "suoluefund.h"
#include "pubmsgdef.h"
SuolueFund::SuolueFund(SuolueInfo info,QWidget *parent)
    :SuolueBase(info,parent)
{

}
void SuolueFund::afterActive()
{
    m_fundTableView->afterActive();
}

void SuolueFund::afterDeactive()
{
    m_fundTableView->afterDeactive();
}

void SuolueFund::onClickItem(const SuolueItem& item)
{
    bool desc = item.id == 0?false:true;
    m_fundTableView->setSortOrder(desc);
}

QWidget *SuolueFund::createContentWnd()
{
    m_fundTableView = new SLFundNetputView(this);
    m_fundTableView->setFilterFlag(HZData::MarketAll);
    connect(m_fundTableView,&SLFundNetputView::itemSelected,this,&SuolueFund::currentStockChanged);
    connect(m_fundTableView,SIGNAL(itemEntered(QString)),this,SLOT(slotItemSeleced(const QString&)));
    return m_fundTableView;
}

void SuolueFund::onFocusOut()
{
    m_fundTableView->onFocusOut();
}

SuolueTitleBase *SuolueFund::createTitleWnd()
{
   SuolueTitleBase * pWnd = new SuolueTitleBase(m_slInfo);
   pWnd->setObjectName("nobtBorderTitle");
   return pWnd;
}

void SuolueFund::slotItemSeleced(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}
