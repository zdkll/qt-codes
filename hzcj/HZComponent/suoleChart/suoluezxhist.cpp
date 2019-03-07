#include "suoluezxhist.h"

#include <QStackedWidget>
#include "optionstockview.h"
#include "pubmsgdef.h"

SuolueZxHist::SuolueZxHist(SuolueInfo info,QWidget *parent)
    :SuolueBase(info,parent)
{

}
void SuolueZxHist::afterActive()
{
    if(m_stackedWidget->currentIndex()  == 0)
        m_zxTableView->afterActive();
    else
        m_histTableView->afterActive();
}

void SuolueZxHist::afterDeactive()
{
    if(m_stackedWidget->currentIndex()  == 0)
        m_zxTableView->afterDeactive();
    else
        m_histTableView->afterDeactive();
}

//创建内容区域窗口
QWidget* SuolueZxHist::createContentWnd()
{
    m_stackedWidget = new QStackedWidget(this);
    m_zxTableView   = new ZXStockDynaView(SLView,this);
    m_histTableView = new HistStockDynaView(SLView,this);
    m_stackedWidget->addWidget(m_zxTableView);
    m_stackedWidget->addWidget(m_histTableView);

    connect(m_zxTableView->tableView(),&OptionDynaView::itemSelected,this,&SuolueZxHist::currentStockChanged);
    connect(m_histTableView,&OptionDynaView::itemSelected,this,&SuolueZxHist::currentStockChanged);

    connect(m_zxTableView->tableView(),SIGNAL(itemEntered(QString)),this,SLOT(slotItemSeleced(const QString&)));
    connect(m_histTableView,SIGNAL(itemEntered(QString)),this,SLOT(slotItemSeleced(const QString&)));
    return m_stackedWidget;
}

//导航点击以后触发
void SuolueZxHist::onClickItem(const SuolueItem& item)
{
    int currentIdx =  m_stackedWidget->currentIndex();
    if(item.id != currentIdx){
        if(currentIdx == 0)
            m_zxTableView->afterDeactive();
        else
            m_histTableView->afterDeactive();
        m_stackedWidget->setCurrentIndex(item.id);
        if(item.id == 0)
            m_zxTableView->afterActive();
        else
            m_histTableView->afterActive();
    }
}

void SuolueZxHist::onFocusOut()
{
     int currentIdx =  m_stackedWidget->currentIndex();
     if(currentIdx == 0)
         m_zxTableView->tableView()->onFocusOut();
     else
         m_histTableView->onFocusOut();
}

SuolueTitleBase *SuolueZxHist::createTitleWnd()
{
    SuolueTitleBase * pWnd = new SuolueTitleBase(m_slInfo);
    pWnd->setObjectName("optionalList");
    return pWnd;
}

void SuolueZxHist::slotItemSeleced(const QString &stockCode)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,stockCode);
    sendMessage(ROOT_FRAME,params,true);
}
