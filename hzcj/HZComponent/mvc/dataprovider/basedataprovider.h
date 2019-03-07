#ifndef BASEDATAPROVIDER_H
#define BASEDATAPROVIDER_H

#include "databasemodel.h"
#include "datamodeldef.h"
#include "dataproviderdef.h"


//数据页面数据管理基类
class BaseDataProvider : public QObject
{
    Q_OBJECT
public:
    BaseDataProvider(AbstractDataBaseModel *model,QObject *parent = NULL);

    virtual void actived(bool active = true);

    virtual void currentRangeChanged(int ,int ){}
    virtual void setSortIndicator(SortField ,bool ){}
    virtual void setShowCount(int showCount){m_showCount = showCount;}
    QVector<QString> getZXStocks();
protected slots:
    void    slotSubTimeOut();
protected:
    void    startSub();
    virtual void  cancelSub();

    //请求和订阅数据
    virtual void reqData(){}
    virtual void subData(){}

protected:
    AbstractDataBaseModel *m_model;
    SubReqInfo    m_subReqInfo;
    bool        m_actived;
    bool        m_isInitialed;
    int           m_showCount = 0;
private:
    QTimer  m_timer;
};

//集合竞价provider,拉取排行
class BiddingDataProvider : public BaseDataProvider
{
public:
    BiddingDataProvider(BiddingItemModel *model,QObject *parent = Q_NULLPTR);

    void currentRangeChanged(int start,int count);
    void setSortIndicator(SortField sortField,bool desc);

protected:
    void  cancelSub();

private:
    void reqData();
    void subData();

    void OnBatchBiddingArrived(ReqId, const HZData::BiddingInfo &, ErrCode);

    BiddingItemModel              *m_dataModel;
    HZData::ReqBatchBidding  m_reqBatchBidInfo;

    ReqId   m_reqId = INVALID_REQID;
    int         m_totalCount = 0;
};

//集合竞价provider,拉取optional
class BiddingOpotionalProvider: public BaseDataProvider
{
    Q_OBJECT
public:
    BiddingOpotionalProvider(BiddingItemModel *model,QObject *parent = Q_NULLPTR);

    void actived(bool active = true);

signals:
    void rowCountChanged(int count);

protected slots:
    void slotStkCountChanged(int count);

protected:
    void subOptionalBidding();
    void OnOptionalBiddingArrived(ReqId,const HZData::BiddingInfo &, ErrCode);

    void subBidding(const std::list<std::string> &stockCodes);

private:
    int m_rowCount = 0;
    BiddingItemModel   *m_dataModel;
};


//涨停监测数据拉取
class RiseFallMoniProvider : public BaseDataProvider
{
public:
    RiseFallMoniProvider(RFMonitorItemModel *model,QObject *parent = Q_NULLPTR);

    void setRFMonitorType(HZData::RFMonitorType type);
    void currentRangeChanged(int start,int count);
    void setSortIndicator(SortField sortField,bool desc);

protected:
    void cancelSub();

private:
    void reqData();
    void subData();
    void OnBatchRfmArrived(ReqId, const HZData::RiseFallMonitorInfo &, ErrCode);

    RFMonitorItemModel               *m_dataModel;
    HZData::ReqBatchRfmInfo       m_reqBatchRfmInfo;
    ReqId                                       m_reqId = INVALID_REQID;
    int                                             m_totalCount;
};


//资金异动排行数据拉取
class FunAbnormalProvider : public BaseDataProvider
{
public:
    FunAbnormalProvider(FundAbnormalItemModel *model,QObject *parent = Q_NULLPTR);

    void setFunAbnormalType(HZData::FundAbnormalType type);
    void currentRangeChanged(int start,int count);
    void setSortIndicator(SortField sortField,bool desc);
protected:
    void cancelSub();

private:
    void reqData();
    void subData();
    void OnBatchFalArrived(ReqId, const HZData::FundAbnormalInfo &, ErrCode);

    FundAbnormalItemModel                  *m_dataModel;
    HZData::ReqBatchFundAbnormal      m_reqBatchFalInfo;
    ReqId                                                m_reqId = INVALID_REQID;
    int m_totalCount = 0;
};

//资金异动自选数据拉取
class FamlOptionalProvider : public BaseDataProvider
{
    Q_OBJECT
public:
    FamlOptionalProvider(FundAbnormalItemModel *model,QObject *parent = Q_NULLPTR);

    void setFunAbnormalType(HZData::FundAbnormalType type);
    void actived(bool active = true);
    void currentRangeChanged(int  ,int  ){}
    void setSortIndicator(SortField  ,bool ){}

protected slots:
    void slotStkCountChanged(int );

protected:
    void cancelSub();

private:
    void subFundAbnormal();
    void OnOptionalFalArrived(ReqId, const HZData::FundAbnormalInfo &, ErrCode);

    FundAbnormalItemModel                  *m_dataModel;
    HZData::ReqBatchFundAbnormal      m_reqBatchFalInfo;
    ReqId    m_reqId = INVALID_REQID;
};

//按页去拉取数据基类
class BasePageProvider : public BaseDataProvider
{
public:
    //页请求类型
    enum PageReqType
    {
        PRTOnce   = 0, //一次请求完成
        PRTAllPage= 1   //全部页数据请求
    };
    BasePageProvider(PageReqType type,AbstractDataBaseModel *model,QObject *parent = NULL);

    void setPageReqType(const PageReqType &type);

    void actived(bool active = true);
    void currentRangeChanged(int  ,int );

    void setTime(UINT64 time_t){m_time = time_t;}
protected:
    void cancelSub();
    void reqNextPage(); //请求下一页接口

    UINT64             m_time;
    int                    m_curPage;
    InfoReqId         m_reqId;
    int                    m_minExceCount;  //最小余留
    QPair<int,int>  m_curRange;
    PageReqType m_prtType;
};

//龙虎榜-个股榜单
class LhbStockProvider : public BasePageProvider
{
public:
    LhbStockProvider(LhbStockItemModel *model,QObject *parent = Q_NULLPTR);
    void setStockLhbType(StockLhbType  type);

private:
    void reqData();
    void stockLhbDatasArrived(const InfoRet&,const StockLhbDatas&);

    StockLhbType           m_stkLhbType;
    LhbStockItemModel *m_dataModel;
};



//龙虎榜-营业部当日榜
class LhbYybDrbProvider : public BasePageProvider
{
public:
    LhbYybDrbProvider(LhbYybDrbStockItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void onLhbYybDrbArrived(const InfoRet&,const StockYybDrbList&,int);

private:
    LhbYybDrbStockItemModel *m_dataModel;
};

//龙虎榜-营业部实力榜
class LhbYybSlbProvider :public BasePageProvider
{
public:
    LhbYybSlbProvider(LhbYybSlbStockItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void onLhbYybSlbArrived(const InfoRet&,const StockYybSlbList&,int);

private:
    LhbYybSlbStockItemModel *m_dataModel;
};


//龙虎榜-营业部活跃榜
class LhbYybHybProvider :public BasePageProvider
{
public:
    LhbYybHybProvider(LhbYybHybStockItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void onLhbYybHybArrived(const InfoRet&,const StockYybHybList&,int);

private:
    LhbYybHybStockItemModel *m_dataModel;
};


//大宗交易
class StockDzjyProvider : public BasePageProvider
{
public:
    StockDzjyProvider(StockDzjyItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void stkDzjyArrived(const InfoRet&,const StockDzjyList&);

private:
    StockDzjyItemModel *m_dataModel;
};


//融资融券-个股净买入卖出排行
class StockRzrqRankProvider : public BasePageProvider
{
public:
    //融资融券类型:1)前10排行和全部排行
    StockRzrqRankProvider(PageReqType type,StockRzrqRankItemModel *model,QObject *parent= Q_NULLPTR);

    void setSort(int sort){m_sort = sort;}

protected:
    void reqData();
    void stockRzrqTopListArrived(const InfoRet&,const StockRzrqTopList&,int);

private:
    StockRzrqRankItemModel *m_dataModel;
    int          m_sort;
};


//融资融券-个股详情
class StockRzrqDetailProvider : public BasePageProvider
{
public:
    StockRzrqDetailProvider(StockRzrqDetailItemModel *model,QObject *parent = Q_NULLPTR);

    void setStockCode(const QString &stockCode);

protected:
    void reqData();
    void stockRzrqDetailArrived(const InfoRet& ret,const StockRzrqDetailList& stkRzrqDetailList);

private:
    StockRzrqDetailItemModel *m_dataModel;
    QString m_stockCode;
};

//融资融券-板块排行
class  BlockRzrqProvider : public BasePageProvider
{
public:
    BlockRzrqProvider(BlockRzrqItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void blockRzrqArrived(const InfoRet&,const StockBoardRzrqTopList&);

    void reqStockDyna(const std::list<std::string> &);
    void onStockDynaArrived(ReqId  , char *data, int num, ErrCode );

private:
    BlockRzrqItemModel *m_dataModel;
};

//融资融券-板块成分股
class BlockStockRzrqProvider : public BasePageProvider
{
    Q_OBJECT
public:
    BlockStockRzrqProvider(BlockStockRzrqItemModel *model,QObject *parent = Q_NULLPTR);

    void setBlockCode(const QString& blockCode);

signals:
    void bkStockCountChanged(int count);

protected:
    void reqData();
    void blockStockRzrqArrived(const InfoRet&,const StockBoardRzrqList&,int);

private:
    BlockStockRzrqItemModel *m_dataModel;
    QString m_blockCode;
};



//新股日历-全部数据请求
class  XgrlStockProvider : public BasePageProvider
{
public:
    XgrlStockProvider(XgrlType type,XgrlStockItemModel *model,QObject *parent = Q_NULLPTR);

protected:
    void reqData();
    void xgrlNewStockArrived(const InfoRet&,const QVector<StockXgrlItem>&);

    void optionalDynaArrived(ReqId  , char *data, int num, ErrCode );

private:
    XgrlStockItemModel *m_dataModel;
    XgrlType                    m_xgrlType;
};

#endif // BASEDATAPROVIDER_H
