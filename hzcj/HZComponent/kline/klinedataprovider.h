#ifndef KLINEDATAPROVIDER_H
#define KLINEDATAPROVIDER_H
/********************************************************************
created: 2018-4-17
author: min.cheng
purpose: 数据请求封装 内部是直接调用数据层
         处理K线请求：
         kline?obj=SH600000&period=1&count=300&dire=-1
         kline?obj=SH600000&period=1&count=300&dire=-1
         码表请求：
         /market/static?market=SH&obj=SH600000
*********************************************************************/
#include <qlist.h>
#include <map>

#include "dataprovider.h"
#include "datamodeldef.h"
#include "chartdef.h"
#include "datacontroller.h"
#include "chartmodle.h"
#include "hzinfocenter.h"

struct ReqKlineInfoEx:HZData::ReqKlineInfo
{
    int rspCount = 0;
    bool endPoint = false;
};

class KlineDataProvider : public DataProvider
{
    Q_OBJECT
public:
    KlineDataProvider(BaseChart* pchart);

    ~KlineDataProvider();

    //K线数据查询
    int query(const std::string& req);

    //取消查詢
    virtual void cancleEx();

    //获取数据
    int getData(int offset,int count,DataKLine& vKlineData,double& max,double& min,int& maxIndex,int& minIndex);

    //固定获取指定数据
    void getFlexCountDataBySub(DataKLine& vKlineData,double& max,double& min,int& maxIndex,int& minIndex);

    //获取缓存数据量
    int getCacheSize();
signals:

public slots:

protected:
    //清理请求
    void resetId();
    //K线请求和订阅回调
    void OnKline(ReqId id ,DataKLine* data,ErrCode code);
    //处理K线请求逻辑
    int procesKlineReq(ReqKlineInfoEx info,const QString& key);
    //请求剩余数量的K线
    int reqNextKline(DataKLine *data);
    //判断当前方向数据是否请求到头了
    int dataEndPoint(bool dirc,const std::string& obj,int period);
    //复权回调
    void OnDirCallBack(const InfoRet& ret,const StockDrList& data);
 private:
    //请求订阅Id储存
    std::map<int,HZData::ReqKlineInfo> reqMap_;
    //当前请求内容记录
    ReqKlineInfoEx  reqKlineInfo_;
    //当前订阅内容
    ReqKlineInfoEx subKlineInfo_;
    //标记发送过错误信息  用于在服务器出错的情况下，避免重复通知界面发送错，
    bool m_bLastSendedError;
    //当前订阅的第一次请求
    bool m_bSubFirstPrush;
    //复权类型
    RestorationType m_reType;
    //复权数据
    StockDrList m_drData;
    //复权请求ID
    int m_rdReqId;
};































#endif // KLINEDATAPROVIDER_H
