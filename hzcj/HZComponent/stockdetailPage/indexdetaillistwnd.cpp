#include "indexdetaillistwnd.h"
#include <QLabel>
#include "navilistview.h"
#include "httpdata.h"

IndexDetailListWnd::IndexDetailListWnd(QWidget *parent, QString instName)
    :BlockDetaillistWnd(parent,instName)
{

}

void IndexDetailListWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
}

void IndexDetailListWnd::OnStockCodeChange()
{
    if(Httpdata::instance()->isHasObjFromFilter(m_stockCode,HZData::IndexInner))
    {
        m_filter = HZData::IndexInner;
        m_pView->setStockCode(m_stockCode);
        m_pView->setFilterFlag(m_filter);
        QString text = Httpdata::instance()->filterCodeToName(m_filter);
        m_pLabel->setText(text);
        m_pLabel->update();
    }
}

void IndexDetailListWnd::slotsButtonClicked(int id)
{
    qDebug()<<"IndexDetailListWnd id:"<<id;
    if(id == bk_detial_sort_up) //升序
    {
        m_pView->sortIndicatorChanged(HZData::rise_fall_rate,true);
    }else if(id == bk_detail_sort_down) //降序
    {
        m_pView->sortIndicatorChanged(HZData::rise_fall_rate,false);
    }
}

void IndexDetailListWnd::slotsSwitchUpDownStock(bool isUp, int step)
{
    m_pView->switchToUpDownStock(isUp,step);
}

NaviListView *IndexDetailListWnd::createView()
{
    return new NaviIndexListView(this);
}
