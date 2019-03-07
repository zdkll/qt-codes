#include "biddingprovider.h"
#include "biddingcanvas.h"
BiddingProvider::BiddingProvider(BiddingCanvas* pWnd,QObject *parent)
    :QObject(parent),
     m_pBiddingCanvase(pWnd),
     m_reqId(-1)
{

}

void BiddingProvider::subData(const QString &stock)
{
    if(m_reqId != -1)
        DataController::instance()->cancelSub(m_reqId);
    HZData::ReqBidKlineInfo req = stock.toStdString();
    auto fun = std::bind(&BiddingProvider::onDataCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_reqId = DataController::instance()->subBidKline(req,fun);
}

void BiddingProvider::cancle()
{
    DataController::instance()->cancelSub(m_reqId);
}

void BiddingProvider::onDataCallBack(ReqId id, DataKLine *data, ErrCode err)
{
    BiddingData retData;
    if(data == nullptr || data->m_data.isEmpty() || err != 0)
    {
        m_pBiddingCanvase->onDataRefresh(&retData);
        return;
    }
    BiddingItem item;
    for(int i=0;i < data->m_data.size();i++)
    {
        item.bid_rate = data->m_data[i].open_price;
        item.last_price = data->m_data[i].close_price;
        item.pre_close_price = data->m_data[i].pre_close_price;
        item.bid_price = data->m_data[i].avg_price;
        if(data->m_data[i].volume != INVALID_INT64)
            item.volume = data->m_data[i].volume;
        item.upsert_time = data->m_data[i].upsert_time;
        retData.data.push_back(item);
        if(item.bid_price == 0)
        {
            item.bid_price = item.pre_close_price;
        }
        if(retData.maxValue < item.bid_price && item.bid_price != 0)
            retData.maxValue = item.bid_price;
        if(retData.minValue > item.bid_price && item.bid_price != 0)
            retData.minValue = item.bid_price;
        if(retData.volMaxVal < item.volume)
            retData.volMaxVal = item.volume;
//        if(retData.volMinVal > item.volume)
//            retData.volMinVal = item.volume;
    }
    if(item.pre_close_price != DBL_MAX)
    {
        double v1 = abs(item.pre_close_price-retData.minValue);
        double v2 = abs(item.pre_close_price-retData.maxValue);
        v1 = v1>v2?v1:v2;
        double rate = v1/item.pre_close_price;
        if(v1  < 0.07)
            v1 = 0.07 ;
        else if(rate < 0.01) //小于1%
        {
            v1 = item.pre_close_price*0.01;
        }
        retData.maxValue = item.pre_close_price + v1;
        retData.minValue = item.pre_close_price - v1;
    }
    m_pBiddingCanvase->onDataRefresh(&retData);
}

