/********************************************************************
created:
author: dk.zhang
purpose:资金主力净流入
*********************************************************************/
#ifndef FUNDLEADERNETPUTMODEL_H
#define FUNDLEADERNETPUTMODEL_H

#include "baseitemmodel.h"
#include "datamodeldef.h"
#include "itemmodeldef.h"
#include "mvcpublicdef.h"

class FundLeaderNetputModel : public AbstractBaseModel
{
public:
    FundLeaderNetputModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<HZData::HZTick> &data);

    void clearData();

    int rowCount(const QModelIndex &parent= QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<HZData::HZTick>  m_ticks;

};

#endif // FUNDLEADERNETPUTMODEL_H
