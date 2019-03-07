/********************************************************************
created:2018/8/5
author: dk.zhang
purpose: 资金流相关的model
*********************************************************************/

#ifndef FUNDMODEL_H
#define FUNDMODEL_H

#include "baseitemmodel.h"
#include "datamodeldef.h"

// @brief 资金流表头model，用于TableView，实现自定义表头合并
class FundHeaderItemModel : public QAbstractTableModel
{
public:
    FundHeaderItemModel(QObject *parent = 0);

    //设置头部几列，不参与合并的列数
    void setHeaderColCount(const int &count){m_headerColCount = count;}
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

private:
    QVector<ColumnInfo> m_columnInfos;
    QStringList                   m_priHeaderLbls;
    int                                 m_headerColCount;
    SortIndicator                m_sortIndicator;
};


// @brief 资金流表格model 基类 -直接拉取排行刷新-不需要要本地排序
class FundBaseModel : public  AbstractBaseModel
{
public:
    FundBaseModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :AbstractBaseModel(modelName,parent){}

    //初始化数据
    virtual void initData(const QVector<HZData::StockCode > &stockCodes) = 0;

    virtual void initData(const char * ,int  ,int  ){}

    virtual void removeStock(const QString& ){}
    virtual void clearData() = 0;
    //更新数据
    virtual void updateData(const char *data,int start,int num) = 0;

    virtual  QVariant  colIdxData(const int & , const ColumnIdx & ) const{return QVariant();}

protected:

};

// @brief 缩略资金流表格model-只显示净流入
class SLFundModel : public  FundBaseModel
{
public:
    SLFundModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :FundBaseModel(modelName,parent){}

    virtual void initData(const QVector<HZData::StockCode > & ){}

    void initData(const char *data,int start,int num);

    void  clearData();
    //更新数据
    virtual void updateData(const char *data,int start,int num);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //实现父类的data函数
    QVariant  data(const QModelIndex &index, int role) const;
    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;

private:
    QVector<HZData::MoneyNetput>  m_moneyNetput;
};

//@brief 资金流表格model
class FundModel : public  FundBaseModel
{
public:
    FundModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :FundBaseModel(modelName,parent){}

    virtual void initData(const QVector<HZData::StockCode > &stockCodes);

    void  initData(const char *data,int start,int num);
    //更新数据
    virtual void updateData(const char *data,int start,int num);

    void removeStock(const QString& stockCode);
    void clearData();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //实现父类的data函数
    QVariant  data(const QModelIndex &index, int role) const;

    QVariant  colIdxData(const int &row, const ColumnIdx &inColumn) const;

protected:
    QVector<HZData::StockMoney>  m_stockMoney;
};




//板块及其成分股同时显示Model
class BkStockFundModel : public FundModel
{
public:
    BkStockFundModel(const QString &modelName,QObject *parent = Q_NULLPTR)
        :FundModel(modelName,parent){}

    //更新包括第一行板块数据和当前显示成分股数据
    void updateData(const char *data,int start,int num);

    void  clearData();
};

#endif // FUNDMODEL_H
