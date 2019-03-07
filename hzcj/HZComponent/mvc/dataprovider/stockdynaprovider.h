/********************************************************************
created:2018/06
author: dk.zhang
purpose:行情数据拉取订阅管理器
*********************************************************************/
#ifndef STOCKDYNAPROVIDER_H
#define STOCKDYNAPROVIDER_H

#include "basestockdynaprovider.h"

//SL 缩略图个股数据拉取，定时拉取排行，不需要手动排序-返回数据SLStock
class SLStockDynaProvider  : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    SLStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent){m_reqBatchInfo.mask = HZData::SLStock_Mask;}

    void actived(const bool &active = true) Q_DECL_OVERRIDE;

public:
    //设置filterFlag和filterDetail
    virtual  void setFilterFlag(const int &filterFlag);
    virtual  void setFilterDetail(const QString &filterDetial);
    virtual  void setFilterInfo(const int &filterFlag,const QString &filterDetial);

    //重写以下函数不做处理
    void currentRangeChanged(int  , int  ){}

    void sortIndicatorChanged(const SortField &,const bool & );

    //设置缩略拉取排行的数目
    void setStockCount(int  count) Q_DECL_OVERRIDE;

protected:
    virtual void subBatchStockData() Q_DECL_OVERRIDE;

    virtual void onStockBatchArrived(ReqId,const char *data,int num,ErrCode errCode);

    virtual void subOptionalData(){}
};


///定时拉取排行管理类，目前行情用在板块列表下
class BatchDynaProvider : public BaseStockDynaProvider
{
public:
    BatchDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);

    void actived(const bool &active = true) Q_DECL_OVERRIDE;

    void setStockFieldMask(HZData::StockFieldMask mask);

protected:
    virtual void onStockBatchDataArrived(ReqId reqId, const char * data,int num,ErrCode errCode) Q_DECL_OVERRIDE;
};


///一般排行列表数据管理，和基类行为一致
class  StockDynaProvider  : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    StockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent){}
};


//板块成分股-作为板块下成分股列表
class  BKStockDynaProvider  : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    BKStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent){}

    void  actived(const bool &active = true);

protected:
    void onStockBatchDataArrived(ReqId reqId,const char *data,int num,ErrCode errCode);
};

//指数成分股
class IndexStockDynaProvider : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    IndexStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent){}

    void  actived(const bool &active = true);

protected:
    void onStockBatchDataArrived(ReqId reqId,const char *data,int num,ErrCode errCode);
};


//某个板块及其成分股，单独显示
class  BKAndStockDynaProvider : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    BKAndStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent){}

    void  actived(const bool &active = true);

public slots:
    void currentRangeChanged(int start , int count);

protected:
    void onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode errCode);
};


//异动板块数据请求/定10秒拉取一次
class AblBlockDynaProvider : public BaseStockDynaProvider
{
    Q_OBJECT
public:
    AblBlockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0)
        :BaseStockDynaProvider(model,parent)
        ,m_filter(HZData::BlockWhole),m_timerId(-1),m_reqId(INVALID_REQID){}

    void actived(const bool &active = true);

    virtual void setFilterFlag(const int &filterFlag);

public slots:
    void currentRangeChanged(int  , int ){}

protected:
    void timerEvent(QTimerEvent *e);
    void subAblBlockInfo();

    void onAblBlockInfoArrived(ReqId , const QVector<HZData::Block>& , ErrCode );
    void cancelSub();

private:
    void requestAblBlockInfo();

    int  m_filter;
    int  m_timerId;
    ReqId m_reqId;
};


#endif // STOCKDYNAPROVIDER_H
