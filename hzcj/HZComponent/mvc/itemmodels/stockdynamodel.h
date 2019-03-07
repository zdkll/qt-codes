/********************************************************************
created: 2018/5/21
author: dk.zhang
purpose:  动态行情列表相关的Model数据模型
*********************************************************************/
#ifndef STOCKDYNAMODEL_H
#define STOCKDYNAMODEL_H

#include <unordered_map>
#include "baseitemmodel.h"

#include "sorter.h"

class AbstractBaseStockModel : public AbstractBaseModel
{
public:
    AbstractBaseStockModel(const QString &modelName,QObject *parent= Q_NULLPTR)
        :AbstractBaseModel(modelName,parent),m_sorter(new DynaSorter),m_needSort(false){}

    virtual ~AbstractBaseStockModel();

    //HZData::StockCode
    virtual void initData(const HZData::StockCode *stockCodes, int num) =0;
    virtual void updateData(const char *data,int start,int num,bool first = true) = 0;

    //缩略列表定时拉取排行使用
    virtual void initData(const char *data,int num){}
    virtual void updateData(const char *data,int num){}
    //移除某个
    virtual void removeStock(const QString& ){}
    virtual void clearData() = 0;

    int    stkRow(const std::string &stockCode);

    void setSortIndicator(int sortField,bool desc);

    void setNeedSort(bool sort){m_needSort = sort;}

protected:
    //当前股票map
    std::map<std::string,int>                  m_stkMap;

    DynaSorter                                      *m_sorter;
    bool                                                  m_needSort;  //是否需要排序
};

//===================新版Model 根据数据格式定义===============
//HZData::SLStock
class SLStockItemModel : public AbstractBaseStockModel
{
public:
    SLStockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR);

    //用股票码初始化数据
    void initData(const HZData::StockCode *stockCodes, int num);

    //更新当前快照
    void updateData(const char *data,int start,int num,bool first);

    //缩略图使用的接口
    void initData(const char *data,int num);
    void updateData(const char *data,int num);

    void removeStock(const QString& stockCode);
    void clearData();
    int  rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;
    QVariant  data(const QModelIndex &index, int role) const;
private:
    QVector<HZData::SLStock>     m_slStocks;
};

//HZData::Stock:个股行情(包括指数和板块成分股)列表，需要手动排序
class StockItemModel : public AbstractBaseStockModel
{
public:
    StockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR);

    //用股票码初始化数据
    void initData(const HZData::StockCode *stockCodes, int num);

    //更新当前快照，手动排序
    void updateData(const char *data,int start,int num,bool first);

    void clearData();

    int  rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;

    QVariant  data(const QModelIndex &index, int role) const;

protected:
    QVector<HZData::Stock>     m_stocks;
};

//自选，最近浏览等列表使用,sortModel自动排序
class OptionStockItemModel : public StockItemModel
{
public:
    OptionStockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR)
        :StockItemModel(modelName,parent){}
    //用股票码初始化数据
    void initData(const HZData::StockCode *stockCodes, int num);

    //更新当前快照
    void updateData(const char *data,int start,int num,bool first);

    void removeStock(const QString& stockCode);

};

//HZData::Block
class BlockItemModel : public AbstractBaseStockModel
{
public:
    BlockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR);

    //用股票码初始化数据
    void initData(const HZData::StockCode *stockCodes, int num);

    //更新当前快照
    void updateData(const char *data,int start,int num,bool first);

    void clearData();

    int  rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;

    QVariant  data(const QModelIndex &index, int role) const;

protected:
    QVector<HZData::Block>     m_blocks;
};

//异动板块
class AblBlockItemModel : public BlockItemModel
{
public:
    AblBlockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR)
        :BlockItemModel(modelName,parent){}

    void initData(const QVector<HZData::Block>& blocks);
    void updateData(const QVector<HZData::Block>& blocks);
};

//HZData::Index
class IndexItemModel : public AbstractBaseStockModel
{
public:
    IndexItemModel(const QString &modelName,QObject *parent= Q_NULLPTR);

    //用股票码初始化数据
    void initData(const HZData::StockCode *stockCodes, int num);

    //更新当前快照
    void updateData(const char *data,int start,int num,bool first);

    void clearData();

    int  rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;

    QVariant  data(const QModelIndex &index, int role) const;

protected:
    QVector<HZData::Index>     m_indexs;
};


//重要指数
class OptionIndexItemModel : public IndexItemModel
{
public:
    OptionIndexItemModel::OptionIndexItemModel(const QString &modelName,QObject *parent)
        :IndexItemModel(modelName,parent) {}

    //更新当前快照
    void updateData(const char *data,int start,int num,bool first);
};



//BKAndStock
class BKAndStockItemModel : public StockItemModel
{
public:
    BKAndStockItemModel(const QString &modelName,QObject *parent= Q_NULLPTR);

    void updateData(const char *data,int start,int num,bool first);

    void updateData(int index,const HZData::Stock &stock);
};






#endif // STOCKDYNAMODEL_H
