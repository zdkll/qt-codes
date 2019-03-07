#include "stockdetaillistwnd.h"
#include <QVBoxLayout>

#include <QPainter>
#include "subtab.h"
#include  "navilistview.h"
#include "localstocksource.h"
#include "fundchangewidget.h"

void MyBaseWidget::paintEvent(QPaintEvent *e)
{
    BaseWidget::paintEvent(e);
    QPainter pt(this);
    pt.setBrush(Qt::red);
    pt.drawRect(0,0,120,120);
}

StockDetailListWnd::StockDetailListWnd(QWidget *parent, QString instName)
    :StockBaseDetailListWnd(parent,instName)
{

}

void StockDetailListWnd::slotsSwitchWnd(int id)
{
    clearCurrWidget();
    setCurrWidget(id);
}

void StockDetailListWnd::slotsSwitchUpDownStock(bool isUp, int step)
{
    if(m_table->currentButton() == OptionTab)
        m_naviZXListView->switchToUpDownStock(isUp,step);
    else if(m_table->currentButton() == RecentlyBrowsingTab)
        m_naviHistListView->switchToUpDownStock(isUp,step);
    else if(m_table->currentButton() == FundChnge)
        m_fundChangeView->switchToUpDownStock(isUp,step);
}

void StockDetailListWnd::OnStockCodeChange()
{
    m_naviZXListView->setStockCode(m_stockCode);
    m_naviHistListView->setStockCode(m_stockCode);
}

CSubTab *StockDetailListWnd::createSubAndItems()
{
    //tab
    CSubTab* table = new CSubTab();
    table->setObjectName("sletTab");
    table->addButton(QStringLiteral("自选"),OptionTab);
    table->addButton(QStringLiteral("浏览"),RecentlyBrowsingTab);
    table->addButton(QStringLiteral("资金异动"),FundChnge);
    table->setCurrentButton(OptionTab);
    return table;
}

void StockDetailListWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
    if(m_table->currentButton() != RecentlyBrowsingTab)
    {
        m_table->setCurrentButton(RecentlyBrowsingTab);
        clearCurrWidget();
        setCurrWidget(RecentlyBrowsingTab,false);
    }
}

QWidget *StockDetailListWnd::createBody()
{
    m_naviZXListView = new NaviZXListView(this);
    addWidget(m_naviZXListView,m_naviZXListView);

    qDebug()<<"after constructor  m_naviZXListView";

    m_naviHistListView = new NaviZXHistListView(HistStock);
    addWidget(m_naviHistListView,m_naviHistListView);

    //异动资金
    m_fundChangeView = new FundChangeWidget(this);
    addWidget(m_fundChangeView,m_fundChangeView);


    connect(m_fundChangeView,&FundChangeWidget::clickRow,this,&StockDetailListWnd::signalObjChange);
    connect(m_naviZXListView,&NaviZXListView::selectedItem,this,&StockDetailListWnd::signalObjChange);
    connect(m_naviHistListView,&NaviZXHistListView::selectedItem,this,&StockDetailListWnd::signalObjChange);
    return getStackWnd();
}

