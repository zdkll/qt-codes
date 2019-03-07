/********************************************************************
created:2018/08
author: dk.zhang
purpose:资金数据拉取、订阅管理类
*********************************************************************/
#ifndef FUNDPROVIDER_H
#define FUNDPROVIDER_H

#include "datamodeldef.h"
#include "itemmodels/fundmodel.h"
#include "dataproviderdef.h"

//brief 资金数据管理基类
class BaseFundProvider  : public QObject
{
    Q_OBJECT
public:
    BaseFundProvider(FundBaseModel *model,QObject *parent = 0);
    virtual ~BaseFundProvider();

    virtual void actived(const bool &active = true);

    void setMoneyFieldMask(HZData::MoneyFieldMask mask);

public  slots:
    void startSub();       //开始订阅
    void cancelSub();    //取消订阅
    void cancel();
    void cancelReq();
    //设置filterFlag 和 filterDetail
    virtual  void setFilterFlag(const int &filterFlag);
    virtual  void setFilterDetail(const QString &filterDetial);
    virtual  void setFilterInfo(const int &filterFlag,const QString &filterDetail);

    virtual void currentRangeChanged(int start , int count);
    virtual void sortIndicatorChanged(const SortField &sortField,const bool &desc);

protected:
    virtual void reqBatchMoneyData() ;
    virtual void subBatchMoneyData() ;

    virtual void onBatchMoneyArrived(ReqId reqId,const char *data,int num,ErrCode errCode);

protected:
    FundBaseModel    *m_model;
    bool                        m_initialed;
    bool                        m_actived;
    HZData::ReqBatchFundInfo m_reqBatchInfo;
    SubReqInfo           m_subReqInfo;
    ReqId                    m_reqBatchqId = INVALID_REQID;

    QVector<HZData::StockCode>   m_stockCodes;
private:
    QTimer        m_t;
    //资金流出排行字段,排序方向为反
    QList<SortField>  m_outSortField;
};

//个股\板块\指数  资金数据 等同于基类
class StockFundProvider : public BaseFundProvider
{
public:
    StockFundProvider(FundBaseModel *model,QObject *parent = 0)
        :BaseFundProvider(model,parent){}
};

//个股\板块  资金净流入列表数据
class SLStockFundProvider : public StockFundProvider
{
public:
    SLStockFundProvider(FundBaseModel *model,QObject *parent = 0);

    void  actived(const bool &active = true) Q_DECL_OVERRIDE;

    void setStockCount(int count);

    void setFilterFlag(const int &filterFlag);

    virtual void currentRangeChanged(int , int ){}
    void  sortIndicatorChanged(const SortField &sortField,const bool &desc);

protected  :
    void subBatchMoneyData();
    void onBatchMoneyArrived(ReqId  ,const char *data,int num,ErrCode errCode);
};

//选定股票资金
class OptionFundProvider : public StockFundProvider
{
public:
    OptionFundProvider(FundBaseModel *model, QObject *parent = 0);

    void  actived(const bool &active = true) Q_DECL_OVERRIDE;
    virtual void deleteStock(const QString &){}

    void setFilterFlag(const int & ){}
    void currentRangeChanged(int , int ){}
    void  sortIndicatorChanged(const SortField & ,const bool & ){}
protected:
    virtual QVector<QString> getStocks() =0;

protected:
    void startSubOptionalFund();
    void reqOptionalFund(const QVector<QString> &stock_codes);
    void subOptionalFund(const std::list<std::string> &);
    void onOptionalFundArrived(ReqId , char *data,int num, ErrCode);
};


//重要指数资金
class ImpIndexFundProvider : public OptionFundProvider
{
public:
    ImpIndexFundProvider(FundBaseModel *model, QObject *parent = 0);

protected:
    virtual QVector<QString> getStocks();

private:
    QVector<QString>  m_stocks;
};


//自选股资金
class ZXStockFundProvider : public OptionFundProvider
{
    Q_OBJECT
public:
    ZXStockFundProvider(FundBaseModel *model, QObject *parent = 0);

    void deleteStock(const QString &);

protected slots:
   void slotStkCountChanged(int count);

protected:
    virtual QVector<QString> getStocks();
      QString  m_currentDeletingStock;
};


//历史浏览资金
class HistStockFundProvider : public OptionFundProvider
{
public:
    HistStockFundProvider(FundBaseModel *model, QObject *parent = 0)
        :OptionFundProvider(model,parent){}

protected:
    virtual QVector<QString> getStocks();
};

#endif // FUNDPROVIDER_H
