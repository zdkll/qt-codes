#include "scrollwidget.h"

#include <QScrollArea>
#include <QBoxLayout>

ScrollWidget::ScrollWidget(QWidget *parent ,QString instName)
    :BaseWidget(parent,instName),m_scrollArea(new QScrollArea(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(m_scrollArea);

    m_scrollArea->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    m_scrollArea->setWidgetResizable(true);

    this->setLayout(mainLayout);
}

void ScrollWidget::setWidget(QWidget *widget)
{
    m_scrollArea->setWidget(widget);
}

