/********************************************************************
created:
author: dk.zhang
purpose:行情数据管理基类
*********************************************************************/
#ifndef BASESTOCKDYNAPROVIDER_H
#define BASESTOCKDYNAPROVIDER_H

#include "datamodeldef.h"
#include "stockdynamodel.h"
#include "dataproviderdef.h"

//行情列表数据管理基类,需要定时拉取排行
class BaseStockDynaProvider  : public QObject
{
    Q_OBJECT
public:
    BaseStockDynaProvider(AbstractBaseStockModel *model,QObject *parent = 0);
    virtual ~BaseStockDynaProvider();

    //对应列表激活状态true 为激活，false为非激活
    virtual void actived(const bool &active = true);
    //设置字段类型，注意：字段类型对应返回的结构体必须和Model 表示的结构体一致
    virtual void setStockFieldMask(HZData::StockFieldMask mask);
    //设置股票数
    virtual void setStockCount(int ){}
    virtual void deleteStock(const QString&){}
    //获取当前股票码表
    virtual const QVector<HZData::StockCode> &stockTable(){ return m_stockTable;}

    //设置filterFlag和filterDetail
    virtual  void setFilterFlag(const int &filterFlag);
    virtual  void setFilterDetail(const QString &filterDetial);
    virtual  void setFilterInfo(const int &filterFlag,const QString &filterDetial);

    //当前显示行范围改变
    virtual void currentRangeChanged(int start , int count);
    //当前排序改变
    void sortIndicatorChanged(const SortField &sortField,const bool &desc);

signals:
    void signalModelReset();

protected  slots:
    void slotStartSubBatchDyna();

protected:
    void startSub();//开始订阅
    void cancelSub();
    void cancelReqBatch();//取消行情排行请求
    void cancelBatchSub();//取消当前行情排行订阅
    void cancelOptSub(); //取消订阅

    //拉取排行,一般是active后第一次拉取
    void reqBatchStockData();
    virtual void subBatchStockData(); //订阅排行-定时拉取
    virtual void subOptionalData(const HZData::StockCode *stockCodes,int num);//订阅快照
    virtual void onStockBatchDataArrived(ReqId reqId,const char * data,int num,ErrCode errCode);
    virtual void onStockOptionalDataArrived(ReqId reqId,char *data,int num,ErrCode errCode);

protected:
    AbstractBaseStockModel *m_model;
    //排行请求(或订阅)信息
    HZData::ReqBatchDynaInfo           m_reqBatchInfo;
    //订阅信息
    SubReqInfo                      m_subBatchInfo;//排行订阅
    SubReqInfo                      m_subOptInfo;//快照订阅
    ReqId                               m_reqBatchId = INVALID_REQID;

    QVector<HZData::StockCode>       m_stockTable;  //股票码表

    //当前View是否激活
    bool              m_actived;
    //当前model 是否初始化，初始化完成定时拉取排行只更新数据
    bool             m_isInitialed;

private:
    QTimer        m_subTimer;
};

#endif // BASESTOCKDYNAPROVIDER_H
