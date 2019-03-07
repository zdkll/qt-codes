#ifndef BIDDINGPROVIDER_H
#define BIDDINGPROVIDER_H
/********************************************************************
created:
author: min.cheng
purpose: 集合竞价数据处理类
*********************************************************************/
#include "datacontroller.h"
#include <QObject>
struct BiddingItem
{
    double bid_rate = INVALID_DOUBLE;//集合竞价涨幅
    double bid_price = INVALID_DOUBLE; //集合竞价价格
    double last_price = INVALID_DOUBLE;          //现价
    double pre_close_price = INVALID_DOUBLE;    //昨收价格
    long long upsert_time = INVALID_INT64;        //插入时间
    double volume = INVALID_DOUBLE;   //成交量
};
struct BiddingData
{
    double maxValue=DBL_MIN;
    double minValue=DBL_MAX;
    double volMaxVal = DBL_MIN;
    double volMinVal = 0;
    QVector<BiddingItem> data;
};

class BiddingCanvas;
class BiddingProvider:public QObject
{
    Q_OBJECT
public:
    BiddingProvider(BiddingCanvas* pWnd,QObject *parent=Q_NULLPTR);

    void subData(const QString& stock);

    void cancle();

    void onDataCallBack(ReqId id ,DataKLine * data,ErrCode err);
 private:
    BiddingCanvas* m_pBiddingCanvase;
    ReqId m_reqId;
};

#endif // BIDDINGPROVIDER_H
