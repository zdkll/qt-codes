#include "slfundleadernetputview.h"

#include <QDebug>
#include <QHeaderView>

#include "datacontroller.h"
#include "slleadernetputdelegates.h"
#include "handdataprovider.h"

SLFundLeaderNetputView::SLFundLeaderNetputView(QWidget *parent)
    :HandTableView(parent)
{
    FundLeaderNetputModel *model = new FundLeaderNetputModel("LeaderNetput",this);

    m_lnpProvider = new LeaderNetputProvider(model);
    this->initTableView(model,m_lnpProvider);

    this->horizontalHeader()->show();
}

void SLFundLeaderNetputView::setStockCode(const QString &stock)
{
    m_stockCode = stock;
    m_lnpProvider->setStockCode(stock);
}

void SLFundLeaderNetputView::initItemDelegates()
{
    this->setItemDelegateForColumn(0,new SLLeaderNetputTimeDelegate(this));
    this->setItemDelegateForColumn(1,new SLLeaderNetputTurnoverDelegate(this));
    this->setItemDelegateForColumn(2,new SLLeaderNetputVolumeDelegate(this));
    this->setItemDelegateForColumn(3,new SLLeaderNetputAverPriceDelegate(this));
}




