#include "statusbar.h"

#include <QGridLayout>

#include <QPushButton>

#include "hzline.h"
#include "micro.h"

StatusBar::StatusBar(QWidget *parent,QString instName)
    :BaseWidget(parent,instName),
     m_pMainLayout(new QGridLayout(this))
{
    createWg();
    this->setFixedHeight(41);
}

QWidget *StatusBar::widgetAtPosition(const int &row,const int &column)
{
    QLayoutItem *layoutItem = m_pMainLayout->itemAtPosition(row,column);
    if(layoutItem)
        return layoutItem->widget();

    return 0;
}

void StatusBar::createWg()
{
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(0);

    QWidget *zeroRowCol0 = new QWidget;

    zeroRowCol0->setObjectName("zeroRowCol0");

    m_pMainLayout->addWidget(zeroRowCol0, 0, 0, 1, 4);

    QWidget *FirstRowCol0 = new QWidget;

    FirstRowCol0->setFixedWidth(28);

    FirstRowCol0->setObjectName("FirstRowCol0");

    m_pMainLayout->addWidget(FirstRowCol0, 1, 0, 1, 1);

    QWidget *FirstRowCol1 = new QWidget;

    FirstRowCol1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    FirstRowCol1->setObjectName("FirstRowCol1");

    m_pMainLayout->addWidget(FirstRowCol1, 1, 1, 1, 1);

    QWidget *FirstRowCol2 = new QWidget;

    FirstRowCol2->setFixedWidth(200);

    FirstRowCol2->setObjectName("FirstRowCol2");

    m_pMainLayout->addWidget(FirstRowCol2, 1, 2, 1, 1, Qt::AlignRight);

    QWidget *FirstRowCol3 = new QWidget;

    FirstRowCol3->setFixedWidth(90);

    FirstRowCol3->setObjectName("FirstRowCol3");

    m_pMainLayout->addWidget(FirstRowCol3, 1, 3, 1, 1, Qt::AlignRight);

}




