/********************************************************************
created:
author: dk.zhang
purpose:分价表Model
*********************************************************************/
#ifndef PRICETICKMODEL_H
#define PRICETICKMODEL_H

#include "baseitemmodel.h"

class PriceTickModel : public AbstractBaseModel
{
public:
    PriceTickModel(const QString &modelName,QObject *parent = 0);

    void initData(const HZData::PriceTickInfo& priceTickInfo,int index);

    void updateData(const HZData::PriceTickInfo& priceTickInfo,int index);

    void clearData();

    int  rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

private:
    HZData::PriceTickInfo              m_priceTickInfo;
};



#endif // PRICETICKMODEL_H
