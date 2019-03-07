#include "suolueblock.h"
#include "pubmsgdef.h"
#include "sltableview.h"

SuolueBlock::SuolueBlock(SuolueInfo info,QWidget *parent)
    :SuolueBase(info,parent)
{

}
void SuolueBlock::afterActive()
{
    m_bkTableView->afterActive();
}
void SuolueBlock::afterDeactive()
{
    m_bkTableView->afterDeactive();
}

void SuolueBlock::onClickItem(const SuolueItem& item)
{
    if(item.id == 0){
        m_bkTableView->setFilterFlag(HZData::BlockIndustry);
    }else
        m_bkTableView->setFilterFlag(HZData::BlockConcept);
    m_bkTableView->afterActive();
}

QWidget* SuolueBlock::createContentWnd()
{
    m_bkTableView = new SLDynaTableView(SLDynaTableView::SLBlock,this);
    m_bkTableView->setFilterFlag(HZData::BlockIndustry);
    connect(m_bkTableView,&SLDynaTableView::itemSelected,this,&SuolueBlock::currentStockChanged);
    connect(m_bkTableView,SIGNAL(itemEntered(QString)),this,SLOT(slotItemSeleced(const QString&)));
    return  m_bkTableView;
}

SuolueTitleBase *SuolueBlock::createTitleWnd()
{
    SuolueTitleBase * pWnd = new SuolueTitleBase(m_slInfo);
    pWnd->setObjectName("nobtBorderTitle");
    return pWnd;
}

void SuolueBlock::onFocusOut()
{
    qDebug()<<"suolue block focus out";
    m_bkTableView->onFocusOut();
}

void SuolueBlock::slotItemSeleced(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}
