#include "datacontroller.h"

#define Mesc_Per_Sec 1000

#include <list>
#include <hzlog.h>
#include "quotepacket.h"

using namespace HZData;

DataController::DataController(QObject *parent )
    :QObject(parent)
    ,m_subPeriod(OnceIn1Second)
    ,m_d(new PrivateData)
    ,m_reqId(MIN_REQID)
{
    NetIoReadCallBack fun = std::bind(&DataController::netioReady,this,std::placeholders::_1);
    m_PacketProcess =  new PacketProcessor(fun);

    m_timerInfo.timerId = this->startTimer(OnceIn1Second*Mesc_Per_Sec);

    setRecvCallBack();
}

void DataController::setRecvCallBack()
{
    OnRecvFunction   recvFuncs;
    recvFuncs.getMask = std::bind(&DataController::getMask,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvBatchStock = std::bind(&DataController::recvBatchDyna,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    recvFuncs.recvOptionalStock = std::bind(&DataController::recvOptionalDyna,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvStockMoney = std::bind(&DataController::recvStockMoney,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvTick = std::bind(&DataController::recvTick,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvKLine = std::bind(&DataController::recvKline,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvTimeKline = std::bind(&DataController::recvTimeKline,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvBidKline = std::bind(&DataController::recvBidKline,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvCodeInfo      = std::bind(&DataController::recvCodeInfo,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvUserKick = std::bind(&DataController::recvUserKick,this,std::placeholders::_1);
    recvFuncs.recvRiseFallInfo = std::bind(&DataController::recvRiseFallInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvBlockAbnormalInfo = std::bind(&DataController::recvBlockAbnormalInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvFundAbnormalInfo       = std::bind(&DataController::recvFundAbnormalInfo,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvBiddingInfo = std::bind(&DataController::recvBiddingInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvBiddingInfoBlock = std::bind(&DataController::recvBiddingBlockInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvShortMotionInfo = std::bind(&DataController::recvShortMotionInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvRfmNumInfo = std::bind(&DataController::recvRfmNumInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvRFMonitorInfo = std::bind(&DataController::recvRiseFallMonitorInfo,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvPriceTickInfo    = std::bind(&DataController::recvPriceTickInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvIncDecPosInfo  = std::bind(&DataController::recvIncDecPosInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvZhibiaoInfo  = std::bind(&DataController::recvZhibiaoInfo,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvMidAbnormalInfo = std::bind(&DataController::recvMidAbnormalInfo,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvAbnormalBlockInfo = std::bind(&DataController::recvAbnormalBlockInfo,this,std::placeholders::_1,std::placeholders::_2);

    recvFuncs.recvTime = std::bind(&DataController::recvTime,this,std::placeholders::_1,std::placeholders::_2);
    recvFuncs.recvTradeDay = std::bind(&DataController::recvTradeDay,this,std::placeholders::_1);

    m_PacketProcess->reciever()->setRecvCbFunction(recvFuncs);
}

DataController::~DataController()
{
    delete m_PacketProcess;
    delete m_d;

    if(m_timerInfo.timerId>0)
        this->killTimer(m_timerInfo.timerId);
}

DataController *DataController::instance()
{
    static  DataController  dataPusher;
    return  &dataPusher;
}

void DataController::setServer(const QString &ip,const int &port)
{
    m_PacketProcess->setServer(ip,port);
}

bool DataController::logIn(quint64 uid,QString acessToken)
{
    return m_PacketProcess->logIn(uid,acessToken);
}

void DataController::logOut()
{
    m_PacketProcess->logOut();
}

void DataController::setSubPeriod(SubPeriod subPeriod)
{
    if(m_timerInfo.timerId>0)
        this->killTimer(m_timerInfo.timerId);

    m_subPeriod = subPeriod;

    m_timerInfo.timerId = this->startTimer(m_subPeriod*Mesc_Per_Sec);
}


//Requst-------------------------------------------------------------------------------------------------------
ReqId DataController::requestTick(const HZData::ReqTickInfo& req,OnTickArrived fun)
{
    //创建Key--------
    int reqId =  getReqId(Req_TickInfo);

    m_d->requestTick.insert(reqId,RequestTick(req,fun));

    m_PacketProcess->sender()->sendReqTick(reqId,req);

    return reqId;
}

ReqId DataController::requestCodeInfo(const HZData::ReqQueryCode& req,OnCodeInfoArrived fun)
{
    ReqId reqId= getReqId(Req_SearchCode);

    m_d->requestCodeInfo.insert(reqId,RequestCodeInfo(req,fun));

    m_PacketProcess->sender()->sendReqQueryCode(reqId,req);

    return reqId;
}


//请求资金排行
ReqId DataController::requestBatchMoney(const HZData::ReqBatchFundInfo &req,OnMoneyArrived func)
{
    ReqId reqId= getReqId(Req_Batch_Money);

    m_d->requestBatchMoney.insert(reqId,RequestBatchMoney(req,func));

    m_PacketProcess->sender()->sendReqBatchMoney(reqId,req);

    return reqId;
}

//请求资金快照
ReqId DataController::requestOptionalMoney(const HZData::ReqOptionalFundInfo &req,OnMoneyArrived fun)
{
    ReqId reqId = getReqId(Req_Opt_Money);

    m_d->requestOptionalMoney.insert(reqId,RequestOptionalMoney(req, fun));

    m_PacketProcess->sender()->sendReqOptionalMoney(reqId,req);

    return reqId;
}

ReqId DataController::requestBlockAbnormalInfo(const HZData::ReqBlockAbnormalInfo &req,OnBlockAbnormalInfoArrived fun)
{
    //创建Key--------
    ReqId reqId = getReqId(Req_BlockAbnormal);

    //保留请求和回调
    m_d->requestBlockAbnormalInfo.insert(reqId,RequestBlockAbnormalInfo(req,fun));

    //发送请求
    m_PacketProcess->sender()->sendReqBlockAbnormalInfo(reqId,req);

    return reqId;
}

ReqId DataController::requestBatchFundAbnormal(const HZData::ReqBatchFundAbnormal& req,OnFundAbnormalInfoArrived fun)
{
    ReqId reqId = getReqId(Req_Batch_FundAbnormal);

    m_d->requestBatchFundAbnormal.insert(reqId,RequestBatchFundAbnormal(req,fun));

    m_PacketProcess->sender()->sendReqBatchFundAbnormal(reqId,req);

    return reqId;
}

// 请求行情快照
ReqId DataController::requestOptionalFundAbnormal(const HZData::ReqOptionalFundAbnormal& req,OnFundAbnormalInfoArrived fun)
{
    ReqId reqId = getReqId(Req_Opt_FundAbnormal);

    m_d->requestOptionalFundAbnormal.insert(reqId,RequestOptionalFundAbnormal(req,fun));

    m_PacketProcess->sender()->sendReqOptionalFundAbnormal(reqId,req);

    return reqId;
}

ReqId DataController::requestBatchBidding(const HZData::ReqBatchBidding& req, OnBiddingArrived func)
{
    ReqId reqId = getReqId(Req_Batch_Bidding);

    m_d->requestBatchBidding.insert(reqId,RequestBatchBidding(req,func));

    m_PacketProcess->sender()->sendReqBatchBidding(reqId,req);

    return reqId;
}

ReqId DataController::requestOptionalBidding(const HZData::ReqOptionalBidding &req,OnBiddingArrived func)
{
    ReqId reqId = getReqId(Req_Opt_Bidding);

    m_d->requestOptionalBidding.insert(reqId,RequestOptionalBidding(req,func));

    m_PacketProcess->sender()->sendReqOptionalBidding(reqId,req);

    return reqId;
}

ReqId DataController::requestShortMotionInfo(const HZData::ReqShortMotionInfo &req,OnShortMotionInfoArrived fun)
{
    ReqId reqId=  getReqId(Req_ShortMotion);

    m_d->requestShortMotionInfo.insert(reqId,RequestShortMotionInfo(req,fun));

    m_PacketProcess->sender()->sendReqShortMotionInfo(reqId,req);

    return reqId;
}

ReqId DataController::requestRFMonitorInfo(const HZData::ReqBatchRfmInfo &req,OnRFMonitorInfoArrived fun)
{
    ReqId reqId = getReqId(Req_RFMonitorInfo);

    m_d->requestRFMonitorInfo.insert(reqId,RequestRFMonitorInfo(req,fun));

    m_PacketProcess->sender()->sendReqBatchRFMonitor(reqId,req);

    return reqId;
}

ReqId DataController::requestZhibiaoInfo(const HZData::ReqZhibiaoInfo &req,OnZhibiaoInfoArrived fun)
{
    ReqId reqId = getReqId(Req_Zhibiao);

    m_d->requestZhibiaoInfo.insert(reqId,RequestZhibiaoInfo(req,fun));

    m_PacketProcess->sender()->sendZhibiaoInfo(reqId,req);

    return reqId;
}

ReqId DataController::requestMidAbnormalInfo(const HZData::ReqMidAbnormalInfo &req,OnMidAbnormalArrived fun)
{
    ReqId reqId = getReqId(Req_MidAbnormal);

    m_d->requestMidAbnormalInfo.insert(reqId,RequestMidAbnormalInfo(req,fun));

    m_PacketProcess->sender()->sendReqMidAbnormalInfo(reqId,req);

    return reqId;
}

ReqId DataController::requestTime(OnTimeArrived func)
{
    ReqId reqId = getReqId(Req_Time);

    m_d->requestTime.insert(reqId,func);

    m_PacketProcess->sender()->sendReqTime(reqId);

    return reqId;
}

ReqId DataController::requestBatchDyna(const HZData::ReqBatchDynaInfo& req, OnStockDynaArrived fun)
{
    ReqId reqId = getReqId(Req_Batch_Dyna);

    m_d->requestBatchStock.insert(reqId,RequestBatchStock(req,fun));

    m_PacketProcess->sender()->sendReqBatchDyna(reqId,req);

    return reqId;
}


ReqId DataController::requestOptionalDyna(const HZData::ReqOptionalDynaInfo& req,OnStockDynaArrived fun)
{
    ReqId reqId = getReqId(Req_Opt_Dyna);

    m_d->requestOptionalStock.insert(reqId,RequestOptionalStock(req,fun));

    m_PacketProcess->sender()->sendReqOptionalDyna(reqId,req);

    return reqId;
}

ReqId DataController::requestAbnormalBlock(const HZData::ReqAbnormalBlockInfo& req,OnAbnormalBlockArrived func)
{
    ReqId reqId = getReqId(Req_Abnormal_Block);

    m_d->requestAbnormalBlockInfo.insert(reqId,RequestAbnormalBlockInfo(req,func));

    m_PacketProcess->sender()->sendReqAbnormalBlockInfo(reqId,req);

    return reqId;
}

ReqId DataController::requestKline(const HZData::ReqKlineInfo& req,OnKLineArrived fun)
{
    //创建Key--------
    ReqId reqId= getReqId(Req_Kline);
    //保留请求和回调
    m_d->requestKline.insert(reqId,RequestKline(req,fun));
    //发送请求
    m_PacketProcess->sender()->sendReqKline(reqId,req);

    return reqId;
}

ReqId DataController::requestTimeKline(const HZData::ReqTimeKlineInfo& req,OnTimeKLineArrived fun)
{
    //创建Key--------
    ReqId reqId= getReqId(Req_TimeKline);
    //保留请求和回调
    m_d->requestTimeKline.insert(reqId,RequestTimeKline(req,fun));
    //发送请求
    m_PacketProcess->sender()->sendReqTimeKline(reqId,req);

    return reqId;
}


//subscription----------------------------------------------
//订阅动态行情-批量
ReqId DataController::subBatchDyna(const HZData::ReqBatchDynaInfo& req ,OnStockDynaArrived fun)
{
    ReqId reqId = getReqId(Sub_Batch_Dyna);

    m_d->requestBatchStock.insert(reqId,RequestBatchStock(req,fun));

    m_PacketProcess->sender()->sendReqBatchDyna(reqId,req);

    return reqId;
}


//订阅动态行情-
ReqId DataController::subOptionalDyna(const HZData::ReqOptionalDynaInfo& req,OnStockDynaArrived fun)
{
    //创建Key--------
    ReqId reqId = getReqId(Sub_Opt_Dyna);

    m_d->requestOptionalStock.insert(reqId,RequestOptionalStock(req,fun));

    m_PacketProcess->sender()->subStockDyna(reqId,req,1);

    return reqId;
}


//订阅逐笔
ReqId DataController::subTick(const HZData::ReqTickInfo& req,OnTickArrived fun)
{
    int reqId =  getReqId(Sub_TickInfo);

    m_d->requestTick.insert(reqId,RequestTick(req,fun));

    m_PacketProcess->sender()->sendReqTick(reqId,req);

    return reqId;
}

//订阅当日资金排行
ReqId DataController::subBatchMoney(const HZData::ReqBatchFundInfo &req,OnMoneyArrived fun)
{
    ReqId reqId = getReqId(Sub_Batch_Money);

    m_d->requestBatchMoney.insert(reqId,RequestBatchMoney(req, fun));

    m_PacketProcess->sender()->sendReqBatchMoney(reqId,req);

    return reqId;
}

//订阅当日资金快照
ReqId DataController::subOptionalMoney(const HZData::ReqOptionalFundInfo &req,OnMoneyArrived fun)
{
    ReqId reqId = getReqId(Sub_Opt_Money);

    m_d->requestOptionalMoney.insert(reqId,RequestOptionalMoney(req, fun));

    m_PacketProcess->sender()->sendReqOptionalMoney(reqId,req);

    return reqId;
}

ReqId DataController::subRiseFallInfo(const HZData::ReqStockRiseFallInfo &req,onRiseFallInfoArrived fun)
{
    //创建Key--------
    ReqId reqId = getReqId(Sub_Rise_Fall);

    m_d->requestRiseFallInfo.insert(reqId,RequestRiseFallInfo(req,fun));

    m_PacketProcess->sender()->sendReqStockRiseFallInfo(reqId,req);

    return reqId;
}

ReqId DataController::subBlockAbnormalInfo(const HZData::ReqBlockAbnormalInfo &req,OnBlockAbnormalInfoArrived fun)
{
    //创建Key--------
    ReqId reqId = getReqId(Sub_BlockAbnormal);

    m_d->requestBlockAbnormalInfo.insert(reqId,RequestBlockAbnormalInfo(req,fun));

    m_PacketProcess->sender()->sendReqBlockAbnormalInfo(reqId,req);

    return reqId;
}


//订阅资金异动排行
ReqId DataController::subBatchFundAbnormal(const HZData::ReqBatchFundAbnormal& req,OnFundAbnormalInfoArrived fun)
{
    ReqId reqId = getReqId(Sub_Batch_FundAbnormal);

    m_d->requestBatchFundAbnormal.insert(reqId,RequestBatchFundAbnormal(req,fun));

    m_PacketProcess->sender()->sendReqBatchFundAbnormal(reqId,req);

    return reqId;
}

//订阅资金异动快照
ReqId DataController::subOptionalFundAbnormal(const HZData::ReqOptionalFundAbnormal &req,OnFundAbnormalInfoArrived fun)
{
    ReqId reqId = getReqId(Sub_Opt_FundAbnormal);

    m_d->requestOptionalFundAbnormal.insert(reqId,RequestOptionalFundAbnormal(req,fun));

    m_PacketProcess->sender()->sendReqOptionalFundAbnormal(reqId,req);

    return reqId;
}

ReqId DataController::subBatchBiddingBlock(const HZData::ReqBatchBiddingBlock &req,OnBiddingBlockArrived func)
{
    ReqId reqId = getReqId(Sub_Batch_BiddingBlock);

    m_d->requestBatchBidBlock.insert(reqId,RequestBatchBidBlock(req,func));

    m_PacketProcess->sender()->sendReqBatchBiddingBlock(reqId,req);

    return reqId;
}

//订阅个股集合竞价排行
ReqId DataController::subBatchBidding(const HZData::ReqBatchBidding &req,OnBiddingArrived func)
{
    ReqId reqId = getReqId(Sub_Batch_Bidding);

    m_d->requestBatchBidding.insert(reqId,RequestBatchBidding(req,func));

    m_PacketProcess->sender()->sendReqBatchBidding(reqId,req);

    return reqId;
}

ReqId DataController::subOptionalBidding(const HZData::ReqOptionalBidding &req,OnBiddingArrived func)
{
    ReqId reqId = getReqId(Sub_Opt_Bidding);

    m_d->requestOptionalBidding.insert(reqId,RequestOptionalBidding(req,func));

    m_PacketProcess->sender()->sendReqOptionalBidding(reqId,req);

    return reqId;
}

ReqId DataController::subShortMotionInfo(const HZData::ReqShortMotionInfo &req,OnShortMotionInfoArrived fun)
{
    ReqId reqId=  getReqId(Sub_ShortMotion);

    m_d->requestShortMotionInfo.insert(reqId,RequestShortMotionInfo(req,fun));

    m_PacketProcess->sender()->sendReqShortMotionInfo(reqId,req);

    return reqId;
}

ReqId DataController::subRfmNumInfo(OnRfmNumInfoArrived fun)
{
    ReqId reqId = getReqId(Sub_RfmNumInfo);

    m_d->requestRfmNumInfo.insert(reqId,RequestRfmNumInfo(fun));

    m_PacketProcess->sender()->sendReqRfmNumInfo(reqId);

    return reqId;
}

//订阅涨停监测
ReqId DataController::subRFMonitorInfo(const HZData::ReqBatchRfmInfo &req,OnRFMonitorInfoArrived fun)
{
    ReqId reqId = getReqId(Sub_RFMonitorInfo);

    m_d->requestRFMonitorInfo.insert(reqId,RequestRFMonitorInfo(req,fun));

    m_PacketProcess->sender()->sendReqBatchRFMonitor(reqId,req);

    return reqId;
}


ReqId DataController::subPriceTickInfo(const HZData::ReqPriceTickInfo &req,OnPriceTickInfoArrived fun)
{
    ReqId reqId = getReqId(Sub_PriceTick);

    m_d->requestPriceTickInfo.insert(reqId,RequestPriceTickInfo(req,fun));

    m_PacketProcess->sender()->sendReqPriceTickInfo(reqId,req);

    return reqId;
}

ReqId DataController::subIncDecPositionInfo(const HZData::ReqLeaderIncDecPostionInfo &req,OnIncrDecPostionInfoArrived func)
{
    ReqId reqId= getReqId(Sub_IncrDec_Postion);

    m_d->requestIncDecPosInfo.insert(reqId,RequestIncrDecPositionInfo(req,func));

    m_PacketProcess->sender()->sendReqIncrDecPosInfo(reqId,req);

    return reqId;
}

ReqId DataController::subMidAbnormalInfo(const HZData::ReqMidAbnormalInfo &req,OnMidAbnormalArrived fun)
{
    ReqId reqId= getReqId(Sub_MidAbnormal);

    m_d->requestMidAbnormalInfo.insert(reqId,RequestMidAbnormalInfo(req,fun));

    m_PacketProcess->sender()->sendReqMidAbnormalInfo(reqId,req);

    return reqId;
}

void DataController::subTradeDayMsg(OnRecvTradeDay fun)
{
    m_d->recvTradeDay.push_back(fun);
}

//订阅K 线
ReqId DataController::subKline(const HZData::ReqKlineInfo& req,OnKLineArrived fun)
{
    int reqId =  getReqId(Sub_Kline);

    m_d->requestKline.insert(reqId,RequestKline(req,fun));

    m_PacketProcess->sender()->subKline(reqId,req.stock_name,req.kline_type);

    return reqId;
}

//订阅分时数据
ReqId DataController::subTimeKline(const HZData::ReqTimeKlineInfo& req,OnTimeKLineArrived fun)
{
    int reqId =  getReqId(Sub_TimeKline);

    m_d->requestTimeKline.insert(reqId,RequestTimeKline(req,fun));

    //1分钟K线
    m_PacketProcess->sender()->subKline(reqId,req.stock_name,req.kline_type);

    return reqId;
}

ReqId DataController::subBidKline(const HZData::ReqBidKlineInfo& req,OnBidKLineArrived fun)
{
    int reqId = getReqId(Sub_BidKline);

    m_d->requestBidKline.insert(reqId,RequestBidKline(req,fun));

    m_PacketProcess->sender()->sendReqBidKline(reqId,req);

    return reqId;
}

bool DataController::cancelSub(ReqId reqId)
{
    //判断是哪个服务类型
    HZData::ServiceType type = serviceType(reqId);
    if(type == None_Service)
        return false;
    //
    m_d->reqIdServices.remove(reqId);
    switch (type) {
    //本地订阅
    case Req_Batch_Dyna:
    case Sub_Batch_Dyna:
        m_d->requestBatchStock.remove(reqId);
        break;
        //服务器推送
    case Req_Opt_Dyna:
    case Sub_Opt_Dyna:
        m_PacketProcess->sender()->subStockDyna(reqId,m_d->requestOptionalStock.value(reqId).reqInfo,2);
        m_d->requestOptionalStock.remove(reqId);
        break;
        //资金
    case Req_Batch_Money:
    case Sub_Batch_Money:
        m_d->requestBatchMoney.remove(reqId);
        break;
    case Req_Opt_Money:
    case Sub_Opt_Money:
        m_d->requestOptionalMoney.remove(reqId);
        break;
        //k线分时，服务器取消
    case Req_Kline:
    case Sub_Kline:
    {
        HZData::ReqKlineInfo req = m_d->requestKline.value(reqId).reqInfo;
        m_PacketProcess->sender()->subKline(reqId,req.stock_name,req.kline_type,2);
        m_d->requestKline.remove(reqId);
    }
        break;
    case Req_TimeKline:
    case Sub_TimeKline:
    {
        HZData::ReqTimeKlineInfo req = m_d->requestTimeKline.value(reqId).reqInfo;
        m_PacketProcess->sender()->subKline(reqId,req.stock_name,req.kline_type,2);
        m_d->requestTimeKline.remove(reqId);
    }
        break;
        //集合竞价K线
    case Req_BidKline:
    case Sub_BidKline:
        m_d->requestBidKline.remove(reqId);
        break;
        //资金异动
    case Sub_Batch_FundAbnormal:
        m_d->requestBatchFundAbnormal.remove(reqId);
        break;
    case Sub_Opt_FundAbnormal:
        m_d->requestOptionalFundAbnormal.remove(reqId);
        break;
        //短线异动
    case Sub_ShortMotion:
        m_d->requestShortMotionInfo.remove(reqId);
        break;
        //集合竞价行情
    case Sub_Batch_Bidding:
        m_d->requestBatchBidding.remove(reqId);
        break;
    case Sub_Opt_Bidding:
        m_d->requestOptionalBidding.remove(reqId);
        break;
    case Sub_Batch_BiddingBlock:
        m_d->requestBatchBidBlock.remove(reqId);
        break;
    case Sub_Opt_BiddingBlock:
        m_d->requestOptionalBidBlock.remove(reqId);
        break;
        //涨停监测
    case Sub_RfmNumInfo:
        m_d->requestRfmNumInfo.remove(reqId);
        break;
    case Sub_RFMonitorInfo:
        m_d->requestRFMonitorInfo.remove(reqId);
        break;
    case Req_TickInfo:
    case Sub_TickInfo:
        m_d->requestTick.remove(reqId);
        break;
    case Sub_PriceTick:
    case Req_PriceTick:
        m_d->requestPriceTickInfo.remove(reqId);
        break;
    case Sub_IncrDec_Postion:
        m_d->requestIncDecPosInfo.remove(reqId);
        break;
    case Sub_Rise_Fall:
        m_d->requestRiseFallInfo.remove(reqId);
        break;
    case Sub_BlockAbnormal:
        m_d->requestBlockAbnormalInfo.remove(reqId);
        break;
    case Req_Zhibiao:
        m_d->requestZhibiaoInfo.remove(reqId);
        break;
    case Sub_MidAbnormal:
    case Req_MidAbnormal:
        m_d->requestMidAbnormalInfo.remove(reqId);
        break;
    case Req_Abnormal_Block:
        return m_d->requestAbnormalBlockInfo.remove(reqId);
        break;
    default:
        break;
    }
    return true;
}

int DataController::getMask(HZData::ServiceType  type,ReqId reqId)
{
    //分字段拉取协议需要
    switch (type) {
    case Sub_Batch_Dyna:
    case Req_Batch_Dyna:
        return m_d->requestBatchStock.value(reqId).reqInfo.mask;
        break;
    case Sub_Opt_Dyna:
    case Req_Opt_Dyna:
        return m_d->requestOptionalStock.value(reqId).reqInfo.mask;
        break;
    case Sub_Batch_Money:
    case Req_Batch_Money:
        return m_d->requestBatchMoney.value(reqId).reqInfo.mask;
        break;
    case Sub_Opt_Money:
    case Req_Opt_Money:
        return m_d->requestOptionalMoney.value(reqId).reqInfo.mask;
        break;
    default:
        break;
    }
    return -1;
}


void  DataController::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_timerInfo.timerId)
    {
        //1s
        m_timerInfo.count ++;
        //发送订阅请求
        postSubRequestOneSec();
        if(m_timerInfo.count%2 == 0)
            postSubRequestTwoSec();
        if(m_timerInfo.count%3 == 0)
            postSubRequestThreeSec();
    }

    QObject::timerEvent(e);
}

void DataController::netioReady(bool ready)
{
    m_netIOReady = ready;
    emit signalNetIoReady(ready);
    Log("DataLayerLOG","INFO","send ready = %d." ,ready);
    if(!ready){//网路断开需要将所有请求信息清除
        m_d->clearReqSubInfo();
    }
}


//private functions-----------------------------------------------
ReqId DataController::getReqId(HZData::ServiceType type)
{
    m_reqId++;
    if(m_reqId == MAX_REQID)
        m_reqId = MIN_REQID;

    m_d->reqIdServices.insert(m_reqId,type);

    return m_reqId;
}


void DataController::postSubRequestTwoSec()
{
    //资金快照
    auto  moit = m_d->requestOptionalMoney.begin();
    while(moit!=m_d->requestOptionalMoney.end()){
        if(serviceType(moit.key()) == Sub_Opt_Money){
            m_PacketProcess->sender()->sendReqOptionalMoney(moit.key(),moit.value().reqInfo);
        }
        moit ++;
    }
    //集合竞价K线
    auto bkit =  m_d->requestBidKline.begin();
    while(bkit != m_d->requestBidKline.end()){
        if(serviceType(bkit.key())  == Sub_BidKline)
            m_PacketProcess->sender()->sendReqBidKline(bkit.key(),bkit.value().reqInfo);
        bkit ++;
    }

    //个股集合竞价
    auto bbit =  m_d->requestBatchBidding.begin();
    while(bbit != m_d->requestBatchBidding.end()){
        if(serviceType(bbit.key())  == Sub_Batch_Bidding)
            m_PacketProcess->sender()->sendReqBatchBidding(bbit.key(),bbit.value().reqInfo);
        bbit ++;
    }
    auto obit =  m_d->requestOptionalBidding.begin();
    while(obit != m_d->requestOptionalBidding.end()){
        if(serviceType(obit.key())  == Sub_Opt_Bidding)
            m_PacketProcess->sender()->sendReqOptionalBidding(obit.key(),obit.value().reqInfo);
        obit ++;
    }
    //板块集合竞价-排行
    auto bbbit =  m_d->requestBatchBidBlock.begin();
    while(bbbit != m_d->requestBatchBidBlock.end()){
        if(serviceType(bbbit.key())  == Sub_Batch_BiddingBlock)
            m_PacketProcess->sender()->sendReqBatchBiddingBlock(bbbit.key(),bbbit.value().reqInfo);
        bbbit ++;
    }

    //资金异动
    auto fbit =  m_d->requestBatchFundAbnormal.begin();
    while(fbit != m_d->requestBatchFundAbnormal.end()){
        if(serviceType(fbit.key())  == Sub_Batch_FundAbnormal)
            m_PacketProcess->sender()->sendReqBatchFundAbnormal(fbit.key(),fbit.value().reqInfo);
        fbit ++;
    }
    auto foit =  m_d->requestOptionalFundAbnormal.begin();
    while(foit != m_d->requestOptionalFundAbnormal.end()){
        if(serviceType(foit.key())  == Sub_Opt_FundAbnormal)
            m_PacketProcess->sender()->sendReqOptionalFundAbnormal(foit.key(),foit.value().reqInfo);
        foit ++;
    }

    //短线异动
    auto  smit = m_d->requestShortMotionInfo.begin();
    while(smit!= m_d->requestShortMotionInfo.end()){
        if(serviceType(smit.key())  ==  Sub_ShortMotion)
            m_PacketProcess->sender()->sendReqShortMotionInfo(smit.key(),smit.value().reqInfo);
        smit ++;
    }

    //涨停监测
    auto  rnit = m_d->requestRfmNumInfo.begin();
    while(rnit!= m_d->requestRfmNumInfo.end()){
        if(serviceType(rnit.key())  ==  Sub_RfmNumInfo)
            m_PacketProcess->sender()->sendReqRfmNumInfo(rnit.key());
        rnit ++;
    }
    auto  rmit = m_d->requestRFMonitorInfo.begin();
    while(rmit!= m_d->requestRFMonitorInfo.end()){
        if(serviceType(rmit.key())  ==  Sub_RFMonitorInfo)
            m_PacketProcess->sender()->sendReqBatchRFMonitor(rmit.key(),rmit.value().reqInfo);
        rmit ++;
    }

    //逐笔交易
    QMap<ReqId,RequestTick>::const_iterator  tit =  m_d->requestTick.begin();
    while(tit !=m_d->requestTick.end()){
        if(serviceType(tit.key()) ==  Sub_TickInfo)
            m_PacketProcess->sender()->sendReqTick(tit.key(),tit.value().reqInfo);
        tit ++;
    }

    //分价表
    auto ptit =  m_d->requestPriceTickInfo.begin();
    while(ptit!=m_d->requestPriceTickInfo.end()){
        if(serviceType(ptit.key()) ==  Sub_PriceTick){
            m_PacketProcess->sender()->sendReqPriceTickInfo(ptit.key(),ptit.value().reqInfo);
        }
        ptit ++;
    }

    //30日主力增减仓
    auto  liit =  m_d->requestIncDecPosInfo.begin();
    while(liit!=m_d->requestIncDecPosInfo.end()){
        if(serviceType(liit.key()) == Sub_IncrDec_Postion){
            m_PacketProcess->sender()->sendReqIncrDecPosInfo(liit.key(),liit.value().reqInfo);
        }
        liit ++;
    }

    //涨跌分布
    auto  rfit =  m_d->requestRiseFallInfo.begin();
    while(rfit!=m_d->requestRiseFallInfo.end()){
        if(serviceType(rfit.key())  ==  Sub_Rise_Fall){
            m_PacketProcess->sender()->sendReqStockRiseFallInfo(rfit.key(),rfit.value().reqInfo);
        }
        rfit ++;
    }

    //板块异动
    auto  bait =  m_d->requestBlockAbnormalInfo.begin();
    while(bait!=m_d->requestBlockAbnormalInfo.end()){
        if(serviceType(bait.key()) == Sub_BlockAbnormal){
            m_PacketProcess->sender()->sendReqBlockAbnormalInfo(bait.key(),bait.value().reqInfo);
        }
        bait ++;
    }
    //盘中异动
    auto  mait =  m_d->requestMidAbnormalInfo.begin();
    while(mait!=m_d->requestMidAbnormalInfo.end()){
        if(serviceType(mait.key()) == Sub_MidAbnormal){
            m_PacketProcess->sender()->sendReqMidAbnormalInfo(mait.key(),mait.value().reqInfo);
        }
        mait ++;
    }
}

void DataController::postSubRequestThreeSec()
{
    //订阅行情排行
    auto bit = m_d->requestBatchStock.begin();
    while(bit != m_d->requestBatchStock.end()){
        if(serviceType(bit.key()) == Sub_Batch_Dyna){
            m_PacketProcess->sender()->sendReqBatchDyna(bit.key(),bit.value().reqInfo);
        }
        bit++;
    }

    //资金排行
    auto bmit = m_d->requestBatchMoney.begin();
    while(bmit != m_d->requestBatchMoney.end()){
        if(serviceType(bmit.key()) == Sub_Batch_Money){
            m_PacketProcess->sender()->sendReqBatchMoney(bmit.key(),bmit.value().reqInfo);
        }
        bmit++;
    }
}



//recieve 接收数据回复包--------------------------------------------
//行情排行返回
void DataController::recvBatchDyna(const  ReqId &reqId, char *data, int num)
{
    auto it = m_d->requestBatchStock.find(reqId);
    if (it != m_d->requestBatchStock.end())
    {
        it.value().stockDynaArrived(reqId,data, num,m_PacketProcess->errCode());

        if(serviceType(reqId) == Req_Batch_Dyna)
            m_d->requestBatchStock.remove(reqId);
    }
}

//行情快照返回
void DataController::recvOptionalDyna(const  ReqId &reqId,const QVector<HZData::AllStock> &stocks)
{
    if(reqId != 0){  //请求或者第一次订阅返回
        //        qDebug()<<"recv recvOptionalDyna:";
        resolveReqOptStock(reqId,stocks);
    }else{   //订阅返回
        //        qDebug()<<"recv sub recvOptionalDyna";
        resolveSubOptStock(reqId,stocks);
    }
}

void DataController::recvBiddingInfo(ReqId reqId, const HZData::BiddingInfo &biddingInfo)
{
    HZData::ServiceType type = serviceType(reqId);
    if(type == Sub_Batch_Bidding || type == Req_Batch_Bidding){
        auto it = m_d->requestBatchBidding.find(reqId);
        if(it != m_d->requestBatchBidding.end())
            it.value().biddingArrived(reqId,biddingInfo,m_PacketProcess->errCode());
        if(type == Req_Batch_Bidding)
            m_d->requestBatchBidding.remove(reqId);

    }else{
        auto it = m_d->requestOptionalBidding.find(reqId);
        if(it != m_d->requestOptionalBidding.end())
            it.value().biddingArrived(reqId,biddingInfo,m_PacketProcess->errCode());
        if(type == Req_Opt_Bidding)
            m_d->requestOptionalBidding.remove(reqId);
    }
}

void DataController::recvBiddingBlockInfo(ReqId reqId, const HZData::BiddingBlockInfo &biddingBlockInfo)
{
    HZData::ServiceType type = serviceType(reqId);
    if(type == Sub_Batch_BiddingBlock || type == Req_Batch_BiddingBlock){
        auto it = m_d->requestBatchBidBlock.find(reqId);
        if(it != m_d->requestBatchBidBlock.end())
            it.value().biddingBlockArrived(reqId,biddingBlockInfo,m_PacketProcess->errCode());
        if(type == Req_Batch_BiddingBlock)
            m_d->requestBatchBidBlock.remove(reqId);
    }else{
        auto it = m_d->requestOptionalBidBlock.find(reqId);
        if(it != m_d->requestOptionalBidBlock.end())
            it.value().biddingBlockArrived(reqId,biddingBlockInfo,m_PacketProcess->errCode());
        if(type == Req_Opt_BiddingBlock)
            m_d->requestOptionalBidBlock.remove(reqId);
    }
}

void DataController::recvTick(const  ReqId &reqId,const HZData::Zhubi &zhubi)
{
    auto it=  m_d->requestTick.find(reqId);
    if(it != m_d->requestTick.end()){
        it.value().tickArrived(reqId,zhubi,m_PacketProcess->errCode());

        if(serviceType(reqId) == Req_TickInfo)
            m_d->requestTick.remove(reqId);
    }
}

void DataController::recvKline(const  ReqId &reqId,const HZData::KlineInfo &klineInfo)
{
    DataKLine  *datKline = new DataKLine(klineInfo.klines);
    //请求
    if(reqId != 0)
    {
        auto it = m_d->requestKline.find(reqId);
        if(it != m_d->requestKline.end()){
            it.value().klineArrived(reqId,datKline,m_PacketProcess->errCode());
            m_d->requestKline.remove(reqId);
        }
    }else{//订阅
        //推送K线
        auto it = m_d->requestKline.begin();
        while(it != m_d->requestKline.end()){
            if(serviceType(it.key()) == Sub_Kline){
                HZData::ReqKlineInfo req = it.value().reqInfo;
                //股票和K线类型相同则推送
                if(req.stock_name == klineInfo.stock_code && req.kline_type == klineInfo.kline_type)
                    it.value().klineArrived(reqId,datKline,m_PacketProcess->errCode());
            }
            it++;
        }
        //推送分时
        auto it1 = m_d->requestTimeKline.begin();
        while(it1 != m_d->requestTimeKline.end()){
            if(serviceType(it1.key()) == Sub_TimeKline){
                HZData::ReqTimeKlineInfo req = it1.value().reqInfo;
                //股票和K线类型相同则推送
                if(req.stock_name == klineInfo.stock_code && req.kline_type == klineInfo.kline_type)
                    it1.value().timeKlineArrived(reqId,datKline,m_PacketProcess->errCode());
            }
            it1++;
        }
    }
    delete datKline;
}

void DataController::recvTimeKline(const  ReqId &reqId,const HZData::KlineInfo &klineInfo)
{
    DataKLine *datTimeDivide = new DataKLine(klineInfo.klines);
    auto it=  m_d->requestTimeKline.find(reqId);
    if(it != m_d->requestTimeKline.end()){
        it.value().timeKlineArrived(reqId,datTimeDivide,m_PacketProcess->errCode());
        m_d->requestTimeKline.remove(reqId);
    }
    delete datTimeDivide;
}

void DataController::recvBidKline(const  ReqId &reqId,const HZData::KlineInfo &klineInfo)
{
    DataKLine *datBidKline = new DataKLine(klineInfo.klines);
    auto it=  m_d->requestBidKline.find(reqId);
    if(it != m_d->requestBidKline.end()){
        it.value().bidKlineArrived(reqId,datBidKline,m_PacketProcess->errCode());

        if(serviceType(reqId) == Req_BidKline)
            m_d->requestBidKline.remove(reqId);
    }
    delete datBidKline;
}

void DataController::recvCodeInfo(const  ReqId &reqId,const QVector<HZData::CodeInfo> &codes)
{
    Log("DataLayerLOG","INFO","recieve search codeInfo reqId = %d,count=%d ." ,reqId,codes.size());
    auto it=  m_d->requestCodeInfo.find(reqId);
    if(it != m_d->requestCodeInfo.end()){
        it.value().codeInfoArrived(reqId,codes,m_PacketProcess->errCode());

        m_d->requestCodeInfo.remove(reqId);
    }
}

void DataController::recvRiseFallInfo(const ReqId &reqId,const HZData::StockRiseFallInfo &riseFallInfo)
{
    auto it=  m_d->requestRiseFallInfo.find(reqId);
    if(it != m_d->requestRiseFallInfo.end()){
        it.value().riseFallArrived(reqId,riseFallInfo,m_PacketProcess->errCode());

        //暂无请求接口
    }
}


void DataController::recvStockMoney(const ReqId &reqId, const QVector<HZData::StockMoney> &allStockMoney)
{
    HZData::ServiceType type = serviceType(reqId);
    //排行
    if(type == Req_Batch_Money || type == Sub_Batch_Money)
    {
        resloveBatchStockMoney(reqId,allStockMoney);

        if(type == Req_Batch_Money)
            m_d->requestBatchMoney.remove(reqId);
    }
    else{//快照
        resloveOptStockMoney(reqId,allStockMoney);

        if(type == Req_Opt_Money)
            m_d->requestOptionalMoney.remove(reqId);
    }
}

void DataController::recvBlockAbnormalInfo(ReqId reqId,const HZData::BlockAbnormalInfo &blockAbnormalInfo)
{
    auto it=  m_d->requestBlockAbnormalInfo.find(reqId);
    if(it != m_d->requestBlockAbnormalInfo.end()){
        it.value().blockAbnormalArrived(reqId,blockAbnormalInfo,m_PacketProcess->errCode());

        //请求
        if(HZData::ServiceType(it.key()) == Req_BlockAbnormal)
            m_d->requestBlockAbnormalInfo.remove(reqId);
    }
}

void DataController::recvFundAbnormalInfo(ReqId reqId,const HZData::FundAbnormalInfo &fundAbnormalInfo)
{
    HZData::ServiceType service = serviceType(reqId);

    //排行
    if(service == Sub_Batch_FundAbnormal || service == Req_Batch_FundAbnormal){
        auto it = m_d->requestBatchFundAbnormal.find(reqId);
        if(it != m_d->requestBatchFundAbnormal.end()){
            it.value().fundAbnormalInfoArrived(reqId,fundAbnormalInfo,m_PacketProcess->errCode());

            if(service == Req_Batch_FundAbnormal)
                m_d->requestBatchFundAbnormal.remove(reqId);
        }
    }else{
        auto it = m_d->requestOptionalFundAbnormal.find(reqId);
        if(it != m_d->requestOptionalFundAbnormal.end()){
            it.value().fundAbnormalInfoArrived(reqId,fundAbnormalInfo,m_PacketProcess->errCode());

            if(service == Req_Opt_FundAbnormal)
                m_d->requestOptionalFundAbnormal.remove(reqId);
        }
    }
}

void DataController::recvShortMotionInfo(ReqId reqId,const HZData::ShortMotionInfo &shortMotionInfo)
{
    auto it=  m_d->requestShortMotionInfo.find(reqId);
    if(it != m_d->requestShortMotionInfo.end()){
        it.value().shotMotionArrived(reqId,shortMotionInfo,m_PacketProcess->errCode());

        //请求
        if(serviceType(reqId)  == Req_ShortMotion)
            m_d->requestShortMotionInfo.remove(reqId);
    }
}


void DataController::recvRfmNumInfo(ReqId reqId,const HZData::RiseFallMonitorNumInfo &rfmNumInfo)
{
    auto it =m_d->requestRfmNumInfo.find(reqId);
    if(it != m_d->requestRfmNumInfo.end()){
        it.value().rfmNumInfoArrived(reqId,rfmNumInfo,m_PacketProcess->errCode());
        if(serviceType(it.key()) == Req_RfmNumInfo)
            m_d->requestRfmNumInfo.remove(reqId);
    }
}

void DataController::recvRiseFallMonitorInfo(ReqId reqId,const HZData::RiseFallMonitorInfo &rfmonitorInfo)
{
    auto it =m_d->requestRFMonitorInfo.find(reqId);
    if(it != m_d->requestRFMonitorInfo.end()){
        it.value().rfmonitorInfoArrived(reqId,rfmonitorInfo,m_PacketProcess->errCode());

        if(serviceType(it.key()) == Req_RFMonitorInfo)
            m_d->requestRFMonitorInfo.remove(reqId);
    }
}

void DataController::recvPriceTickInfo(ReqId reqId,const HZData::PriceTickInfo &priceTickInfo)
{
    auto it=  m_d->requestPriceTickInfo.find(reqId);
    if(it != m_d->requestPriceTickInfo.end()){
        it.value().priceTickArrived(reqId,priceTickInfo,m_PacketProcess->errCode());
        //暂无请求接口
    }
}

void DataController::recvIncDecPosInfo(ReqId reqId,const HZData::IncrDecPostionInfo &incrDecPosInfo)
{
    auto it = m_d->requestIncDecPosInfo.find(reqId);
    if(it != m_d->requestIncDecPosInfo.end()){
        it.value().incrDecArrived(reqId,incrDecPosInfo,m_PacketProcess->errCode());

        //暂无请求接口
    }
}

void DataController::recvZhibiaoInfo(ReqId reqId,const HZData::ZhibiaoInfo &zhibiaoInfo)
{
    auto it = m_d->requestZhibiaoInfo.find(reqId);
    if(it != m_d->requestZhibiaoInfo.end()){
        it.value().zhibiaoInfoArrived(reqId,zhibiaoInfo,m_PacketProcess->errCode());

        //暂无订阅接口
        m_d->requestZhibiaoInfo.remove(it.key());
    }
}

void DataController::recvMidAbnormalInfo(ReqId reqId,const HZData::MidAbnormalInfo& midAbnormalInfo)
{
    auto it = m_d->requestMidAbnormalInfo.find(reqId);
    if(it != m_d->requestMidAbnormalInfo.end()){
        it.value().midAbnormalArrived(reqId,midAbnormalInfo,m_PacketProcess->errCode());

        if(serviceType(it.key()) == Req_MidAbnormal)
            m_d->requestMidAbnormalInfo.remove(it.key());
    }
}

void DataController::recvAbnormalBlockInfo(ReqId reqId, const QVector<HZData::Block>& ablHotBlocks)
{
    auto it = m_d->requestAbnormalBlockInfo.find(reqId);
    if(it     != m_d->requestAbnormalBlockInfo.end()){
        it.value().ablHotBlockArrived(reqId,ablHotBlocks,m_PacketProcess->errCode());

        if(serviceType(it.key()) == Req_Abnormal_Block)
            m_d->requestAbnormalBlockInfo.remove(it.key());
    }
}

void DataController::recvUserKick(const HZData::UserLoginKickInfo &kickInfo)
{
    Log("DataLayerLOG","INFO","User kickout, uid= %ul." ,kickInfo.uid);
    if(m_d->recvUserKick)
        m_d->recvUserKick(kickInfo);
}

void DataController::recvTime(ReqId reqId,uint timet)
{
    auto it =  m_d->requestTime.find(reqId);
    if(it != m_d->requestTime.end()){
        //QDateTime local = QDateTime::fromTime_t(timet,Qt::OffsetFromUTC,8*3600).toLocalTime();
        it.value()(reqId,timet,m_PacketProcess->errCode());
        //只有请求
        m_d->requestTime.remove(reqId);
    }
}

void DataController::recvTradeDay(const HZData::TradeDayMsg& msg)
{
    Log("DataLayerLOG","INFO","Recieve TradeDay = %d." ,msg.trade_day);
    for(int i = 0;i < m_d->recvTradeDay.size();i++)
        m_d->recvTradeDay[i](msg);
}

HZData::ServiceType  DataController::serviceType(ReqId reqId)
{
    return m_d->reqIdServices.value(reqId,None_Service);
}

void DataController::resolveReqOptStock(const  ReqId &reqId,const QVector<HZData::AllStock> &allStocks)
{
    int num = allStocks.size();
    auto it =  m_d->requestOptionalStock.find(reqId);
    ErrCode   errCode = m_PacketProcess->errCode();
    if(it !=m_d->requestOptionalStock.end()){
        HZData::ReqOptionalDynaInfo req = it.value().reqInfo;
        switch (req.mask) {
        case HZData::AllStock_Mask:
        {
            HZData::AllStock  *stocks = new HZData::AllStock[num];
            for(int i=0;i<num;i++)
                stocks[i] = allStocks[i];
            it.value().stockDataArrived(reqId,(char*)stocks,num,errCode);
            delete[] stocks;
        }
            break;
        case HZData::SLStock_Mask:
        {
            HZData::SLStock *slStocks = new HZData::SLStock[num];
            for(int i=0;i<num;i++)
                QuotePacket::readSLStock(allStocks[i],slStocks[i]);
            it.value().stockDataArrived(reqId,(char*)slStocks,num,errCode);
            delete[] slStocks;
        }
            break;
        case HZData::Stock_Mask:
        {
            HZData::Stock *stocks = new HZData::Stock[num];
            for(int i=0;i<num;i++)
                QuotePacket::readStock(allStocks[i],stocks[i]);
            it.value().stockDataArrived(reqId,(char*)stocks,num,errCode);
            delete[] stocks;
        }
            break;
        case HZData::Block_Mask:
        {
            HZData::Block *blocks = new HZData::Block[num];
            for(int i=0;i<num;i++)
                QuotePacket::readBlock(allStocks[i],blocks[i]);
            it.value().stockDataArrived(reqId,(char*)blocks,num,errCode);
            delete[] blocks;
        }
            break;
        case HZData::Index_Mask:
        {
            HZData::Index *indexs = new HZData::Index[num];
            for(int i=0;i<num;i++)
                QuotePacket::readIndex(allStocks[i],indexs[i]);
            it.value().stockDataArrived(reqId,(char*)indexs,num,errCode);
            delete[] indexs;
        }
            break;
        default:
            break;
        }

        //如果是请求则移除请求信息
        if(serviceType(reqId) == Req_Opt_Dyna)
            m_d->requestOptionalStock.remove(reqId);
    }
}

void DataController::resolveSubOptStock(const  ReqId &reqId,const QVector<HZData::AllStock> &allStocks)
{
    //建立映射表
    std::unordered_map<std::string,int> stockmap;
    for(int i=0;i<allStocks.size();i++){
        stockmap[allStocks[i].stock_code] = i;
    }
    //遍历每个订阅，返回不同字段的结构体数据
    int num = 0;
    ErrCode errCode = m_PacketProcess->errCode();
    auto it =  m_d->requestOptionalStock.begin();
    while(it !=m_d->requestOptionalStock.end()){
        //查找订阅服务
        if(serviceType(it.key()) == Sub_Opt_Dyna)
        {
            HZData::ReqOptionalDynaInfo req =  it.value().reqInfo;
            QList<int> stock_list;
            //查找请求包含的股票
            foreach (std::string stock, req.stock_code) {
                auto sit = stockmap.find(stock);
                if(sit != stockmap.end())
                    stock_list.append(sit->second);
            }
            num = stock_list.size();

            if(num <1) {it++;continue;}

            switch (req.mask) {
            case HZData::AllStock_Mask:
            {
                HZData::AllStock  *stocks = new HZData::AllStock[num];
                for(int i=0;i<num;i++)
                    stocks[i] = allStocks[stock_list[i]];
                it.value().stockDataArrived(reqId,(char*)stocks,num,errCode);
                delete[] stocks;
            }
                break;
            case HZData::SLStock_Mask:
            {
                HZData::SLStock *slStocks = new HZData::SLStock[num];
                for(int i=0;i<num;i++)
                    QuotePacket::readSLStock(allStocks[stock_list[i]],slStocks[i]);
                it.value().stockDataArrived(reqId,(char*)slStocks,num,errCode);
                delete[] slStocks;
            }
                break;
            case HZData::Stock_Mask:
            {
                HZData::Stock *stocks = new HZData::Stock[num];
                for(int i=0;i<num;i++)
                    QuotePacket::readStock(allStocks[stock_list[i]],stocks[i]);
                it.value().stockDataArrived(reqId,(char*)stocks,num,errCode);
                delete[] stocks;
            }
                break;
            case HZData::Block_Mask:
            {
                HZData::Block *blocks = new HZData::Block[num];
                for(int i=0;i<num;i++)
                    QuotePacket::readBlock(allStocks[stock_list[i]],blocks[i]);
                it.value().stockDataArrived(reqId,(char*)blocks,num,errCode);
                delete[] blocks;
            }
                break;
            case HZData::Index_Mask:
            {
                HZData::Index *index = new HZData::Index[num];
                for(int i=0;i<num;i++)
                    QuotePacket::readIndex(allStocks[stock_list[i]],index[i]);
                it.value().stockDataArrived(reqId,(char*)index,num,errCode);
                delete[] index;
            }
                break;
            default:
                break;
            }
        }
        it++;
    }
}

void DataController::resloveBatchStockMoney(const ReqId reqId,const QVector<HZData::StockMoney> &allStockMoneys)
{
    auto it  = m_d->requestBatchMoney.find(reqId);
    if(it != m_d->requestBatchMoney.end())
    {
        const int num = allStockMoneys.size();
        HZData::MoneyFieldMask mask = it.value().reqInfo.mask;
        switch (mask) {
        case HZData::MoneyStockCode_Mask:
        {
            HZData::StockCode *stockCodes = new HZData::StockCode[num];
            for(int i=0;i<num;i++){
                stockCodes[i].stock_code =  allStockMoneys[i].stock_code;
                stockCodes[i].stock_name =  allStockMoneys[i].stock_name;
            }
            it.value().moneyArrived(reqId,(char*)stockCodes,num,m_PacketProcess->errCode());
            delete[] stockCodes;
        }
            break;
        case HZData::MoneyField_Mask:
            it.value().moneyArrived(reqId,(char*)allStockMoneys.data(),num,m_PacketProcess->errCode());
            break;
        case HZData::MoneyNetput_Mask:
        {
            HZData::MoneyNetput *moneyNetput = new  HZData::MoneyNetput[num];
            for(int i=0;i<num;i++)
                QuotePacket::readMoneyNetput(allStockMoneys[i],moneyNetput[i]);
            it.value().moneyArrived(reqId,(char*)moneyNetput,num,m_PacketProcess->errCode());
            delete[] moneyNetput;
        }
            break;
        default:
            break;
        }
    }
}

void DataController::resloveOptStockMoney(const ReqId reqId,const QVector<HZData::StockMoney> &allStockMoneys)
{
    auto it  = m_d->requestOptionalMoney.find(reqId);
    if(it != m_d->requestOptionalMoney.end())
    {
        const int num = allStockMoneys.size();
        HZData::MoneyFieldMask mask = it.value().reqInfo.mask;
        switch (mask) {
        case HZData::MoneyField_Mask:
        {
            it.value().moneyArrived(reqId,(char*)allStockMoneys.data(),num,m_PacketProcess->errCode());
            break;
        }
        case HZData::MoneyStockCode_Mask:
        {
            HZData::StockCode *stockCodes = new HZData::StockCode[num];
            for(int i=0;i<num;i++){
                stockCodes[i].stock_code  =  allStockMoneys[i].stock_code;
                stockCodes[i].stock_name =  allStockMoneys[i].stock_name;
            }
            it.value().moneyArrived(reqId,(char*)stockCodes,num,m_PacketProcess->errCode());
            delete[] stockCodes;
        }
            break;
        case HZData::MoneyNetput_Mask:
        {
            HZData::MoneyNetput *moneyNetput = new  HZData::MoneyNetput[num];
            for(int i=0;i<num;i++)
                QuotePacket::readMoneyNetput(allStockMoneys[i],moneyNetput[i]);
            it.value().moneyArrived(reqId,(char*)moneyNetput,num,m_PacketProcess->errCode());
            delete[] moneyNetput;
        }
            break;
        default:
            break;
        }
    }
}

///DataController::PrivateData----
void DataController::PrivateData::clearReqSubInfo()
{
    requestBatchStock.clear();
    requestOptionalStock.clear();

    requestTick.clear();
    requestKline.clear();
    requestTimeKline.clear();
    requestCodeInfo.clear();

    requestBatchMoney.clear();
    requestOptionalMoney.clear();

    requestRiseFallInfo.clear();

    requestBatchFundAbnormal.clear();
    requestOptionalFundAbnormal.clear();

    requestBlockAbnormalInfo.clear();
    requestShortMotionInfo.clear();
    requestRFMonitorInfo.clear();
    requestRfmNumInfo.clear();

    requestPriceTickInfo.clear();
    requestZhibiaoInfo.clear();

    requestIncDecPosInfo.clear();
    requestMidAbnormalInfo.clear();
    requestAbnormalBlockInfo.clear();
}

