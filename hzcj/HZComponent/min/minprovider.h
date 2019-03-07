#ifndef MINPROVIDER_H
#define MINPROVIDER_H
/********************************************************************
created:
author: min.cheng
purpose: 提供分时数据请求
*********************************************************************/
#include <map>
#include "klinedataprovider.h"
#include "datamodeldef.h"
#include "chartdef.h"
#include "datacontroller.h"

struct MinProviderReqInfo
{
    int id = INT_MAX;
    HZData::ReqTimeKlineInfo info;
};

class MinProvider : public DataProvider
{
    Q_OBJECT
public:
    MinProvider(BaseChart* pchart);

    ~MinProvider();

    //查询分时数据
    virtual int query(const std::string& req);

    //取消上次查询
    virtual void cancle(int id);

    //获取缓存数据
    int getData(DataKLine& vKlineData,QVector<int>& dayIndexList,double& max,double& min,int nDay,bool bAvg=true);

signals:

public slots:

private:
    //K线请求和订阅回调
    void OnKline(ReqId id ,DataKLine* data,ErrCode code);
    //交易日切换
    void OnTradeChanged(const HZData::TradeDayMsg & data);
    //初始数据
    void reset();
private:
    //key 第几天  value K线数据
    std::map<int,DataKLine> m_daysMap;
    //key reqId value 请求数据
    QMap<int,HZData::ReqTimeKlineInfo> m_reqIdMap; //多日循环请求的map
    //定义信息
    MinProviderReqInfo m_subInfo;
    //请求信息
    MinProviderReqInfo m_reqInfo;
    //标记发送过错误信息  用于在服务器出错的情况下，避免重复通知界面发送错，
    bool m_bLastSendedError;
    //当前订阅的第一次请求
    bool m_bSubFirstPrush;
};

#endif // MINPROVIDER_H
