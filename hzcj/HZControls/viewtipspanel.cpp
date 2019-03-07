#include "viewtipspanel.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>

ViewTipsPanel::ViewTipsPanel(QWidget *parent,QString instName)
    :BaseWidget(parent,instName)
{
    m_pButtonGroup = new QButtonGroup(this) ;
    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotsButtonClicked(int)));
}

void ViewTipsPanel::setTipItems(const QVector<TipsItem> &list,int width,int hight)
{
    m_itemList = list;
    m_nWidth =  width;
    m_nHight = hight;
}

void ViewTipsPanel::setNormalWnd(QWidget *pWnd,int type)
{
    m_typeIndexMap[type] = pWnd;
    m_pWnd= pWnd;
}

void ViewTipsPanel::createWg()
{
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setSpacing(0);
    QStackedWidget* pStackWnd = new QStackedWidget();
    for(int i=0;i < m_itemList.size();i++)
    {
        QVBoxLayout* hLayout =  new QVBoxLayout();
        hLayout->setMargin(0);
        hLayout->setSpacing(0);
        hLayout->addStretch(1);

        QPushButton *btn = new QPushButton();
        btn->setFixedSize(m_nWidth,m_nHight);
        btn->setObjectName(m_itemList[i].btObjName);
        QLabel  *label  = new QLabel(m_itemList[i].descText);
        label->setObjectName(m_itemList[i].descLabelName);
        m_pButtonGroup->addButton(btn,m_itemList[i].type);
        hLayout->addWidget(btn,0,Qt::AlignCenter);
        hLayout->addSpacing(16);
        hLayout->addWidget(label,0,Qt::AlignHCenter);
        hLayout->addStretch(1);
        QWidget* pwnd = new QWidget(this);
        pwnd->setLayout(hLayout);

        pStackWnd->addWidget(pwnd);
        m_typeIndexMap[m_itemList[i].type] = pwnd;
        if(m_currType == -1)
            m_currType == m_itemList[i].type;
    }
    if(m_pWnd)
        pStackWnd->addWidget(m_pWnd);
    pLayout->addWidget(pStackWnd);
    m_statck = pStackWnd;
}

void ViewTipsPanel::switchType(int type)
{
    auto it = m_typeIndexMap.find(type);
    if(it != m_typeIndexMap.end())
    {
         m_statck->setCurrentWidget(it.value());
         m_currType = type;
    }
}

int ViewTipsPanel::getCurrType()
{
    return m_currType;
}

QWidget *ViewTipsPanel::getStackWnd()
{
    return m_statck;
}

void ViewTipsPanel::slotsButtonClicked(int type)
{
    emit clickItem(type);
}
