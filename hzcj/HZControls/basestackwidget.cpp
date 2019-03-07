#include "basestackwidget.h"
#include <QVBoxLayout>
BaseStackWidget::BaseStackWidget(QWidget *parent, QString instName)
    :BaseWidget(parent,instName),
     m_pstackWnd(nullptr)
{

}

void BaseStackWidget::afterActive()
{
    if(m_pstackWnd)
    {
        int index = m_pstackWnd->currentIndex();
        if(index != -1)
            m_ObjVec[index]->afterActive();
    }
}

void BaseStackWidget::afterDeactive()
{
    if(m_pstackWnd)
    {
        int index = m_pstackWnd->currentIndex();
        if(index != -1)
            m_ObjVec[index]->afterDeactive();
    }
}

int BaseStackWidget::count()
{
    return m_ObjVec.size();
}

QWidget *BaseStackWidget::currWidget()
{
    if(m_pstackWnd)
    {
        return m_pstackWnd->currentWidget();
    }
    return nullptr;
}

QWidget *BaseStackWidget::Widget(int index)
{
    if(m_pstackWnd)
    {
        return m_pstackWnd->widget(index);
    }
    return nullptr;
}

HZObject *BaseStackWidget::currHzObject()
{
    int id = m_pstackWnd->currentIndex();
    if(id == -1)
        return nullptr;
    return m_ObjVec[id];
}

void BaseStackWidget::addWidget(QWidget *pWnd, HZObject *obj)
{
    if(!m_pstackWnd)
        m_pstackWnd = new QStackedWidget();
    pWnd->setParent(m_pstackWnd);
    m_pstackWnd->addWidget(pWnd);
    m_ObjVec.push_back(obj);
}

void BaseStackWidget::clearCurrWidget()
{
    int id = m_pstackWnd->currentIndex();
    if(id == -1)
        return;
    m_ObjVec[id]->afterDeactive();
}

void BaseStackWidget::setCurrWidget(int index,bool bActive,QString stock)
{
    if(m_pstackWnd && index < m_ObjVec.size())
    {
//        int id = m_pstackWnd->currentIndex();
//        if(index == id)
//            return;
//        m_ObjVec[id]->afterDeactive();
        m_pstackWnd->setCurrentIndex(index);
        if(!stock.isEmpty())
            m_ObjVec[index]->setStockCode(stock);
        if(bActive)
            m_ObjVec[index]->afterActive();
    }
}

int BaseStackWidget::currWidgetIndex()
{
    if(m_pstackWnd)
    {
        return m_pstackWnd->currentIndex();
    }
    return -1;
}

QWidget *BaseStackWidget::getStackWnd()
{
    if(!m_pstackWnd)
        m_pstackWnd = new QStackedWidget;
    return m_pstackWnd;
}

void BaseStackWidget::layoutStackWnd()
{
    if(m_pstackWnd)
    {
        QVBoxLayout* pLayout = new QVBoxLayout;
        pLayout->setMargin(0);
        pLayout->setSpacing(0);
        pLayout->addWidget(m_pstackWnd);
        this->setLayout(pLayout);
    }
}

void BaseStackWidget::OnStockCodeChange()
{
    if(!m_ObjVec.isEmpty())
    {
        for(int i=0;i < m_ObjVec.size();i++)
        {
            m_ObjVec[i]->setStockCode(m_stockCode);
        }
    }
}
