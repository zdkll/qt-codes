#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include  "datactrldef.h"
#include "./clientsocket/head.h"

#include  <QHash>
#include <unordered_map>

class PackCtrl;
// @brief 请求包发送类,负责将服务的请求信息结构封装成PACK包,通过 PackCtrl发送
class RequestSender
{
public:
    RequestSender(PackCtrl *pkctrl);
    ~RequestSender();

    //设置UID
    void setUid(quint64 unUid);

    //PackCtrl 连接后设置是否可发送状态
    void setSendReady(bool ready);

    //一下发送服务的请求的接，口根据请求信息封包发送
    void sendReqBatchDyna(const  ReqId &,const HZData::ReqBatchDynaInfo &req);
    void sendReqOptionalDyna(const  ReqId &,const HZData::ReqOptionalDynaInfo &req);
    void sendReqBatchMoney(const  ReqId &,const HZData::ReqBatchFundInfo &req);
    void sendReqOptionalMoney(const  ReqId &,const HZData::ReqOptionalFundInfo &req);

    void sendReqTick(const  ReqId &,const HZData::ReqTickInfo &req);
    void sendReqKline(const  ReqId &,const HZData::ReqKlineInfo &req);
    void sendReqTimeKline(const  ReqId &,const HZData::ReqTimeKlineInfo &req);
    void sendReqBidKline(const  ReqId &,const HZData::ReqBidKlineInfo &req);
    void sendReqQueryCode(const ReqId &,const HZData::ReqQueryCode &req);
    void sendReqStockRiseFallInfo(ReqId,const HZData::ReqStockRiseFallInfo &req);
    void sendReqBlockAbnormalInfo(ReqId,const HZData::ReqBlockAbnormalInfo &req);
    void sendReqBatchFundAbnormal(ReqId,const HZData::ReqBatchFundAbnormal &req);
    void sendReqOptionalFundAbnormal(ReqId,const HZData::ReqOptionalFundAbnormal &req);
    //板块集合竞价
    void sendReqBatchBiddingBlock(ReqId,const HZData::ReqBatchBiddingBlock &req);
    void sendReqOptionalBiddingBlock(ReqId,const HZData::ReqOptionalBiddingBlock &req);
    //个股集合竞价
    void sendReqBatchBidding(ReqId,const HZData::ReqBatchBidding &req);
    void sendReqOptionalBidding(ReqId,const HZData::ReqOptionalBidding &req);

    //涨停监测
    void sendReqBatchRFMonitor(ReqId,const HZData::ReqBatchRfmInfo& req) ;
    void sendReqRfmNumInfo(ReqId);

    void sendReqShortMotionInfo(ReqId,const HZData::ReqShortMotionInfo &req);
    void sendReqPriceTickInfo(ReqId,const HZData::ReqPriceTickInfo &req);
    void sendReqIncrDecPosInfo(ReqId,const HZData::ReqLeaderIncDecPostionInfo &req);

    void sendReqAbnormalBlockInfo(ReqId,const HZData::ReqAbnormalBlockInfo& req);

    void sendReqTime(ReqId);

    //指标
    void sendZhibiaoInfo(ReqId,const HZData::ReqZhibiaoInfo &req);
    //盘中异动
    void sendReqMidAbnormalInfo(ReqId,const HZData::ReqMidAbnormalInfo &req);

    //新版订阅 state = 1，订阅 ; 2 取消
    void subStockDyna(ReqId,const HZData::ReqOptionalDynaInfo &req,int state = 1);
    //订阅K线（包括分时）
    void subKline(ReqId ,const std::string &stock_code, INT32  klineType,int state = 1);

private:
    HZPack    m_pack;
    PackCtrl  *m_packCtrl;
    bool          m_sendReady;

    //订阅记录
    //行情订阅记录：股票-计数
    QHash<QString,int>  m_subDynaRcrds;
    //K线：股票_kline-type-计数
    QHash<QString,int>  m_subKlineRcrds;
};


#endif // REQUESTSENDER_H
