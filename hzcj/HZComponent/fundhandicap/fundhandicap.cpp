#include "fundhandicap.h"

#include <QBoxLayout>
#include <QScrollArea>

#include "fundflowwidget.h"
#include "leaderincdecwidget.h"
#include "leadernetputwidget.h"

FundHandicap::FundHandicap(QWidget *parent)
    :ScrollWidget(parent,"fundhandicap")
{
    createWg();
}

void FundHandicap::setStock(const QString& stock)
{
    m_fundFlowWidget->setStock(stock);
    m_incDecWidget->setStock(stock);
    m_leaderNetputWidget->setStock(stock);
}

void FundHandicap::afterActive()
{
    m_fundFlowWidget->afterActive();
    m_incDecWidget->afterActive();
    m_leaderNetputWidget->afterActive();
}

void FundHandicap::afterDeactive()
{
    m_fundFlowWidget->afterDeactive();
    m_incDecWidget->afterDeactive();
    m_leaderNetputWidget->afterDeactive();
}

void FundHandicap::setContentFixedWidth(int width)
{
    m_contentWg->setFixedWidth(width);
}

void FundHandicap::OnStockCodeChange()
{
    m_fundFlowWidget->setStock(m_stockCode);
    m_incDecWidget->setStock(m_stockCode);
    m_leaderNetputWidget->setStock(m_stockCode);
}

void FundHandicap::createWg()
{
    scrollArea()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_contentWg = new QWidget(this);
    m_contentWg->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);

    m_fundFlowWidget = new FundFlowWidget(this); //资金流向
    m_fundFlowWidget->setFixedHeight(229);

    m_incDecWidget = new LeaderIncDecWidget(this);//30日主力增减仓
    m_incDecWidget->setFixedHeight(186);

    m_leaderNetputWidget = new LeaderNetputWidget(this);//主力净流入
    m_leaderNetputWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding);

//    m_leaderNetputWidget->setFixedHeight(350);

    mainLayout->addWidget(m_fundFlowWidget);
    mainLayout->addWidget(m_incDecWidget);
    mainLayout->addWidget(m_leaderNetputWidget);
    connect(m_fundFlowWidget,&FundFlowWidget::recvLeaderNetput,m_incDecWidget,&LeaderIncDecWidget::recvLeaderNetput);
    m_contentWg->setLayout(mainLayout);

    m_contentWg->setObjectName("contentwidget");

    m_contentWg->setFixedWidth(348);

    this->setWidget(m_contentWg);
}
