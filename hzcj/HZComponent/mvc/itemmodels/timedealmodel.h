/********************************************************************
created:2018/5/25
author: dk.zhang
purpose: 分时成交，逐笔数据model
*********************************************************************/
#ifndef TIMEDEALMODEL_H
#define TIMEDEALMODEL_H

#include  <QAbstractTableModel>
#include  "datamodeldef.h"
#include  "mvcpublicdef.h"

class TimeDealModel  : public QAbstractTableModel
{
public:
    TimeDealModel(QObject *parent = Q_NULLPTR);
    ~TimeDealModel(){}

    //设置数据,当前总的逐笔数据
    void setData(const QVector<HZData::HZTick> &ticks);
    //清除数据
    void clearData();
    //行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //表头数据
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant  data(const QModelIndex &index, int role) const;

private:
    QStringList  m_horizLabels;

private:
    QVector<HZData::HZTick>  m_ticks;
};

#endif // TIMEDEALMODEL_H
