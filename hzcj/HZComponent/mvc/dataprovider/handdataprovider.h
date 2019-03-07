#ifndef HANDDATAPROVIDER_H
#define HANDDATAPROVIDER_H

#include "datacontroller.h"

class FundLeaderNetputModel;
class PriceTickModel;
class HandDataProvider
{
public:
    HandDataProvider();

    virtual void actived(bool active = true);

    void  setStockCode(const QString &stockCode);

    //设置显示条目，对于固定显示的列表需要重新读取设置显示数据
    virtual void  setShowCount(const int &count);

    void currentRangeChange(int  ,int  ){}
protected:
    virtual void reqData() = 0;
    virtual void clearData()= 0;
    void cancelSub();

protected:
    bool      m_actived,m_initialed;
    QPair<int,int>   m_dataScope;
    ReqId   m_subReqId;
    int         m_showCount;
    QString m_stockCode;

};


//主力净流入数据提供
class LeaderNetputProvider : public HandDataProvider
{
public:
    LeaderNetputProvider(FundLeaderNetputModel *model);

protected:
    void reqData() Q_DECL_OVERRIDE;
    void clearData();
    void leaderNetputArrived(ReqId  ,const  HZData::Zhubi&, ErrCode );

    void subLeaderNetput();
    void subLeaderNetputArrived(ReqId  ,const  HZData::Zhubi&, ErrCode );

private:
    void updateData();
    void reqPrevData();

    HZData::Zhubi  m_zhubi;
    FundLeaderNetputModel *m_model;
};

//分价表
class PriceTickProvider : public HandDataProvider
{
public:
    PriceTickProvider(PriceTickModel *model);

    void setShowCount(const int &count) Q_DECL_OVERRIDE;
    void currentRangeChange(int start ,int count);

protected:
    void reqData() Q_DECL_OVERRIDE;
    void clearData();
    void priceTickInfoArrived(ReqId , const HZData::PriceTickInfo&, ErrCode );

private:
    PriceTickModel            *m_model;
    HZData::ReqPriceTickInfo m_reqPriceTickInfo;
    QPair<int,int>              m_curRange;
};

#endif // HANDDATAPROVIDER_H
