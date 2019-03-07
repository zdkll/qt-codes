/********************************************************************
created:
author: dk.zhang
purpose:数据页面model类
*********************************************************************/
#ifndef ABSTRACTDATABASEMODEL_H
#define ABSTRACTDATABASEMODEL_H

#include "baseitemmodel.h"
#include "hzinfocenter.h"

//数据页面model基类
class AbstractDataBaseModel : public AbstractBaseModel
{
public:
    AbstractDataBaseModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    virtual void clearData() = 0;
};


//集合竞价Model
class BiddingItemModel : public AbstractDataBaseModel
{
public:
    BiddingItemModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :AbstractDataBaseModel(modelName,parent),m_totalCount(0){}

    void initData(const HZData::BiddingInfo &biddingInfo);

    void updateData(const HZData::BiddingInfo &biddingInfo,int start);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const{return m_totalCount;}

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

protected:
    int                                            m_totalCount;
    QVector<HZData::StockBid>   m_stockBids;
};




//涨停监测model
class RFMonitorItemModel : public AbstractDataBaseModel
{
public:
    RFMonitorItemModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :AbstractDataBaseModel(modelName,parent),m_totalCount(0){}

    void initData(const HZData::RiseFallMonitorInfo &rfmInfo);

    void updateData(const HZData::RiseFallMonitorInfo &rfmInfo,int start);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const{return m_totalCount;}

    QVariant data(const QModelIndex &index, int role) const;
    QVariant colIdxData(const int &, const ColumnIdx &) const;
private:
    int                                                 m_totalCount;
    QVector<HZData::RiseFallData>   m_riseFallData;
};

//资金异动model
class FundAbnormalItemModel : public AbstractDataBaseModel
{
public:
    FundAbnormalItemModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :AbstractDataBaseModel(modelName,parent),m_totalCount(0){}

    void initData(const HZData::FundAbnormalInfo &falInfo);

    void updateData(const HZData::FundAbnormalInfo &falInfo,int start);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const{return m_totalCount;}

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    int                                                     m_totalCount;
    QVector<HZData::FundAbnormal>     m_fundAbnormal;
};


//龙虎榜-个股榜单
class LhbStockItemModel : public AbstractDataBaseModel
{
public:
    LhbStockItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const StockLhbDatas &stockLhbDatas);

    int rowCount(const QModelIndex & = QModelIndex()) const{return m_totalCount;}

    void clearData();

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    int m_totalCount;
    QVector<StockLhbTypeItem> m_stkLhbItem;
};


//龙虎榜-营业部当日榜
class LhbYybDrbStockItemModel : public AbstractDataBaseModel
{
public:
    LhbYybDrbStockItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const StockYybDrbList &stkYybDrbList);

    void appendData(const StockYybDrbList &stkYybDrbList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    int m_totalCount;
    QVector<StockYybDrbItem> m_stkLhbItem;
};

//实力榜
class LhbYybSlbStockItemModel : public AbstractDataBaseModel
{
public:
    LhbYybSlbStockItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const StockYybSlbList &stkYybSlbList);

    void appendData(const StockYybSlbList &stkYybSlbList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    int m_totalCount;
    QVector<StockYybSlbItem> m_stkLhbItem;
};

//活跃榜
class LhbYybHybStockItemModel : public AbstractDataBaseModel
{
public:
    LhbYybHybStockItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const StockYybHybList &stkYybHybList);

    void appendData(const StockYybHybList &stkYybHybList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    int m_totalCount;
    QVector<StockYybHybItem> m_stkLhbItem;
};

//龙虎榜-营业部详细-上榜次数
class LhbYybSbItemModel : public AbstractDataBaseModel
{
public:
    LhbYybSbItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockYybDetailStatistics> &stkYybSbList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockYybDetailStatistics> m_stkLhbItem;
    QStringList  m_histSbList;
};

//龙虎榜-营业部详细-历史操作
class LhbYybHoItemModel : public AbstractDataBaseModel
{
public:
    LhbYybHoItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockYybDetaiHistory> &stkYybHoList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockYybDetaiHistory> m_stkLhbItem;
};

//大宗交易
class StockDzjyItemModel : public AbstractDataBaseModel
{
public:
    StockDzjyItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const  QVector<StockDzjyItem> &stkDzjyList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockDzjyItem> m_stkDzjyItem;
};



//融资融券净买入和卖出列表
class  StockRzrqRankItemModel : public AbstractDataBaseModel
{
public:
    StockRzrqRankItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockRzrqTopItem> &stkRzrqTopList);

    void appendData(const QVector<StockRzrqTopItem> &stkRzrqTopList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockRzrqTopItem> m_stkRzrqRanktem;
};

//融资融券 - 个股详情
class StockRzrqDetailItemModel : public AbstractDataBaseModel
{
public:
    StockRzrqDetailItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockRzrqDetailItem> &stkRzrqDetailList);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockRzrqDetailItem> m_stkRzrqDetailItem;
};


//融资融券 - 概念板块
class BlockRzrqItemModel : public AbstractDataBaseModel
{
public:
    BlockRzrqItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockBoardRzrqTopItem> &bkStockRzrqList);

    void setStockDyna(HZData::Stock *blocks,int num);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockBoardRzrqTopItem> m_bkStockRzrqItem;
};


//融资融券 - 板块成分股 = 融资融券个股Model
typedef  StockRzrqRankItemModel BlockStockRzrqItemModel;


//新股日历-共同的model
class XgrlStockItemModel : public AbstractDataBaseModel
{
public:
    XgrlStockItemModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    void initData(const QVector<StockXgrlItem> &stockXgrlItems);

    void updateDynaData(char *data, int num);

    void clearData();

    int rowCount(const QModelIndex & = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant colIdxData(const int &, const ColumnIdx &) const;

private:
    QVector<StockXgrlItem> m_stockXgrlItems;
};


#endif // ABSTRACTDATABASEMODEL_H
