#include "pricetickview.h"

#include <QHeaderView>

#include "datacontroller.h"
#include "handdataprovider.h"
#include "pricetickdelegates.h"
#include "pricetickmodel.h"

PriceTickView::PriceTickView(QWidget *parent )
    :HandTableView(parent)
{
    m_model = new PriceTickModel("PriceTick",this);
    m_priceTickProvider = new PriceTickProvider(m_model);

    this->initTableView(m_model,m_priceTickProvider);

    this->setShowGrid(true);
    this->horizontalHeader()->hide();
}

void PriceTickView::setColStretchFactor(int idx,float factor)
{
    if(idx<0 || idx >= m_columnInfos.size()) return;
    m_columnInfos[idx].width = factor;
}

void PriceTickView::setStockCode(const QString &stockCode)
{
    m_stockCode = stockCode;
    m_priceTickProvider->setStockCode(stockCode);
}

void PriceTickView::currentRangeChanged(int start, int count)
{
    m_priceTickProvider->currentRangeChange(start,count);
}

void PriceTickView::initItemDelegates()
{
    this->setItemDelegateForColumn(0,new PriceColDelegate(this));
    this->setItemDelegateForColumn(1,new VolumeColDelegate(this));
}

