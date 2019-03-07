#ifndef NAVISTOCKDYNAMODEL_H
#define NAVISTOCKDYNAMODEL_H

#include <QAbstractListModel>
#include "mvcpublicdef.h"
#include "itemmodeldef.h"
#include "datamodeldef.h"

//导航股票列表Model
class NaviStockDynaModel : public  QAbstractListModel
{
    Q_OBJECT
public:
    NaviStockDynaModel(QObject *parent = Q_NULLPTR)
        :QAbstractListModel(parent){}

    void initData(const char *data,int num);

    virtual void initData(const QVector<HZData::StockCode>& );

    virtual void updateData(const char *data,int start,int count,bool first = true);
    //行数
    int  rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

protected:
    QVector<HZData::SLStock>  m_slStocks;
    std::map<std::string,int>        m_stkMap;
};


class OptionNaviStockDynaModel : public  NaviStockDynaModel
{
public:
    OptionNaviStockDynaModel(QObject *parent = Q_NULLPTR)
        :NaviStockDynaModel(parent){}

    void initData(const QVector<HZData::StockCode>& );

    void updateData(const char *data,int start,int count,bool first = true);
};

#endif // NAVISTOCKDYNAMODEL_H
