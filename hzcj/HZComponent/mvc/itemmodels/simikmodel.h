#ifndef SIMIKMODEL_H
#define SIMIKMODEL_H

#include "baseitemmodel.h"
#include "datamodeldef.h"
struct SmkStock
{
    QString stockCode;
    QString stockName;
    double  fiveRate;
    double price;
    double rate;
};

struct SimiKData
{
    SmkStock selectedStk;
    double similar;
    SmkStock similarStk;
    bool   isExpaned =false;
};

class SimiKHeaderItemModel : public QAbstractTableModel
{
public:
    SimiKHeaderItemModel(QObject *parent = 0);
    //设置列信息
    void setColumnInfos(const QVector<ColumnInfo> &columnInfos);
    //返回列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //数据
    QVariant data(const QModelIndex &index, int role) const;
    //设置排行字段和排行顺序
    void setSortIndicator(const int &sortIndex,const Qt::SortOrder &sortOrder);

    void setStkColumnCount(int count);
private:
    QVector<ColumnInfo>  m_columnInfos;
    QStringList                  m_priLabels;
    SortIndicator                m_sortIndicator;
    int                               m_stkColumnCount;
};



class SimiKModel : public AbstractBaseModel
{
public:
    SimiKModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :AbstractBaseModel(modelName,parent){}

    void setData(const QVector<SimiKData>& simkDatas);

    void toggledItemExpaned(int row,bool expanded = false);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<SimiKData>  m_simikDatas;
};

#endif // SIMIKMODEL_H
