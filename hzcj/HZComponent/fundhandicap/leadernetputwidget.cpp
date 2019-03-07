#include "leadernetputwidget.h"

#include <QBoxLayout>
#include "introductionbutton.h"
#include "slfundleadernetputview.h"

LeaderNetputWidget::LeaderNetputWidget(QWidget *parent ,QString instName)
    :BaseFundWidget(parent,instName)
{
    createWg();
}
void LeaderNetputWidget::afterActive()
{
    m_view->afterActive();
}

void LeaderNetputWidget::afterDeactive()
{
    m_view->afterDeactive();
}

void LeaderNetputWidget::stockChanged(const QString &stockCode)
{
    m_view->setStockCode(stockCode);
}

void LeaderNetputWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);

    m_introductionBtn = new IntroductionButton(QStringLiteral("主力资金净流入（元）"),this);
    m_introductionBtn->setFixedHeight(26);
    mainLayout->addWidget(m_introductionBtn);

    //主力净流入列表
    m_view = new SLFundLeaderNetputView(this);
    m_view->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding);

    mainLayout->addWidget(m_view);

    this->setLayout(mainLayout);
}



