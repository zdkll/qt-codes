#include "stockdetailbasewnd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

StockDetailBaseWnd::StockDetailBaseWnd(QWidget *parent, QString instName)
    :BaseStackWidget(parent,instName)
{

}

void StockDetailBaseWnd::hideWnd(bool bHide)
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

void StockDetailBaseWnd::slotsObjChange(const QString &obj)
{

}

StockBaseDetailInfoWnd::StockBaseDetailInfoWnd(QWidget *parent, QString instName):
    StockDetailBaseWnd(parent,instName)
{

}

void StockBaseDetailInfoWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    QWidget* pWnd = createBody();
    pLayout->addWidget(pWnd);
    this->setLayout(pLayout);
}

int StockBaseDetailInfoWnd::getCurrTabId()
{
    return m_currTabId;
}

void StockBaseDetailInfoWnd::slotsSwitchWnd(int id)
{
    m_currTabId =id;
}

void StockBaseDetailInfoWnd::slotsChildStockListDbClick(const QString &stockCode)
{
    setStockCode(stockCode);
    emit signalObjChange(stockCode);
}

StockBaseDetailListWnd::StockBaseDetailListWnd(QWidget *parent, QString instName)
    :StockDetailBaseWnd(parent,instName),
      m_table(nullptr)
{

}

void StockBaseDetailListWnd::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    QWidget* pWnd = createTitle();
    pWnd->setFixedHeight(26);
    pLayout->addWidget(pWnd);
    pLayout->setSpacing(1);
    pWnd = createBody();
    pLayout->addWidget(pWnd);
    this->setLayout(pLayout);
}

QWidget *StockBaseDetailListWnd::createTitle()
{
    //tab
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_table = createSubAndItems();
    m_table->setObjectName("sletTab");
    connect(m_table,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchWnd(int)));
    pLayout->addWidget(m_table);
    QWidget* pWnd = new QWidget;
    pWnd->setLayout(pLayout);
    pWnd->setObjectName("titleWnd");
    return pWnd;
}
