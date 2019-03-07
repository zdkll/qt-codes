/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QAbstractTableModel 首页热门资讯列表模型
*********************************************************************/
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#define UrlIdRole Qt::UserRole+1
#define INTERVAL 60*1000*2

#include <QAbstractItemModel>
#include <QMap>
#include "hzinfocenter.h"


class InfoWidget;
class TableModel : public QAbstractTableModel
{
public:
    explicit TableModel(QObject *parent = nullptr);


    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


    void clearData();
    void setRecords(const HostNews &news);
    HostNews getRecords() const;

private:
    void init();
//    void getHotInfo();

//    void onhotNewsArrived(const InfoRet& errCode,HostNews& vec,int nRet);

private:
    HostNews m_hostVec;
    friend class InfoWidget;

//    HostNewsCallBack m_callback;

};

#endif // TABLEMODEL_H
