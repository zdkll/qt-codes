#ifndef DATACTRLDEF_H
#define DATACTRLDEF_H

#include <functional>
#include "datamodeldef.h"
#include "datakline.h"

//查询请求Mask
typedef  std::function<int (HZData::ServiceType,ReqId)>     QueryMask;

//ResponseReciever 接收包回调函数，指向公共对外的类DataController 的接口===========
typedef  std::function<void (ReqId, char *data,int num)>      OnRecvBatchStock;

//optional 请求
typedef  std::function<void (ReqId,const  QVector<HZData::AllStock>& )>  OnRecvOptionalStock;
//K线，分时
typedef  std::function<void (ReqId,const HZData::KlineInfo &)>               OnRecvKLine;
//分时为1分钟K线
typedef  OnRecvKLine                                                              OnRecvTimeKline;
//集合竞价
typedef  OnRecvKLine                                                              OnRecvBidKline;

//逐笔
typedef  std::function<void (ReqId, HZData::Zhubi)>                               OnRecvTick;

typedef std::function<void (ReqId, QVector<HZData::CodeInfo>)>          OnRecvCodeInfo;

//资金,排行和快照
typedef std::function<void (ReqId, const QVector<HZData::StockMoney>&) >     OnRecvStockMoney;


//用户登录踢掉
typedef std::function<void (const HZData::UserLoginKickInfo &)>     OnRecvUserKick;

typedef std::function<void (const HZData::TradeDayMsg &)>           OnRecvTradeDay;


//涨跌分布
typedef std::function<void (ReqId,HZData::StockRiseFallInfo)>         OnRecvRiseFallInfo;

//板块异动
typedef std::function<void (ReqId,HZData::BlockAbnormalInfo)>  OnRecvBlockAbnormalInfo;

//资金异动
typedef std::function<void (ReqId,const HZData::FundAbnormalInfo &)>  OnRecvFundAbnormalInfo;

//板块集合竞价
typedef std::function<void (ReqId,const HZData::BiddingBlockInfo &)>  OnRecvBiddingBlock;

//个股集合竞价
typedef std::function<void (ReqId,const HZData::BiddingInfo &)>  OnRecvBidding;

//短线异动
typedef std::function<void (ReqId,const HZData::ShortMotionInfo&)>           OnRecvShortMotionInfo;

//涨停监测
typedef std::function<void (ReqId,const HZData::RiseFallMonitorNumInfo&)>        OnRecvRfmNumInfo;

typedef std::function<void (ReqId,const HZData::RiseFallMonitorInfo&)>                OnRecvRFMonitorInfo;


//分价表
typedef std::function<void (ReqId,const HZData::PriceTickInfo&)>  OnRecvPriceTickInfo;

//30日主力增减仓
typedef std::function<void (ReqId,const HZData::IncrDecPostionInfo&)>  OnRecvIncDecPosInfo;

//指标
typedef std::function<void (ReqId,const HZData::ZhibiaoInfo&)>  OnRecvZhibiaoInfo;

//盘中异动
typedef std::function<void (ReqId,const HZData::MidAbnormalInfo&)>  OnRecvMidAbnormalInfo;

typedef std::function<void (ReqId,const QVector<HZData::Block>&)>  OnRecvAbnormalBlockInfo;


//网络层ready 信号
typedef std::function<void(bool)> NetIoReadCallBack;
//授时
typedef std::function<void (ReqId, uint )>         OnRecvTimeArrived;


//接收消息回调函数
struct OnRecvFunction
{
    //向DataController 查询Mask
    QueryMask getMask;

    //原返回行情接口，包括排行和快照
    OnRecvBatchStock       recvBatchStock;
    OnRecvOptionalStock   recvOptionalStock;

    OnRecvStockMoney      recvStockMoney;

    OnRecvKLine                recvKLine;
    OnRecvTimeKline          recvTimeKline;
    OnRecvBidKline            recvBidKline;

    OnRecvTick                   recvTick;

    OnRecvCodeInfo           recvCodeInfo;
    OnRecvRiseFallInfo       recvRiseFallInfo;

    OnRecvBlockAbnormalInfo  recvBlockAbnormalInfo;  //板块异动
    OnRecvFundAbnormalInfo  recvFundAbnormalInfo;  //资金异动

    OnRecvBiddingBlock           recvBiddingInfoBlock;//板块集合竞价
    OnRecvBidding                    recvBiddingInfo;//个股集合竞价

    OnRecvShortMotionInfo       recvShortMotionInfo;  //短线异动

    OnRecvRfmNumInfo    recvRfmNumInfo;//涨停统计
    OnRecvRFMonitorInfo  recvRFMonitorInfo;//涨停监测

    OnRecvTimeArrived     recvTime;//授时

    OnRecvPriceTickInfo           recvPriceTickInfo; //分价
    OnRecvIncDecPosInfo        recvIncDecPosInfo;//30日主力增加仓

    OnRecvZhibiaoInfo           recvZhibiaoInfo;//指标
    OnRecvMidAbnormalInfo  recvMidAbnormalInfo;
    //异动热门板块
    OnRecvAbnormalBlockInfo   recvAbnormalBlockInfo;

    //推动消息
    OnRecvUserKick            recvUserKick;
    OnRecvTradeDay           recvTradeDay;
};
//==========================================================

//DataController 返回请求或订阅的回调函数,对外使用  ==================
//合约行情-批量
typedef std::function<void (ReqId  ,const QVector<HZData::AllStock>&, ErrCode) >      OnStockDataArrived;

//KLine
typedef std::function<void (ReqId  ,DataKLine *, ErrCode) >  OnKLineArrived;
//分时
typedef OnKLineArrived                                                         OnTimeKLineArrived;
//集合竞价
typedef OnKLineArrived                                                         OnBidKLineArrived;
//逐笔
typedef std::function<void (ReqId  ,const  HZData::Zhubi&, ErrCode )>           OnTickArrived;
//资金
typedef std::function<void (ReqId , char *data,int num, ErrCode )>   OnMoneyArrived;

//涨跌分布
typedef std::function<void (ReqId , const HZData::StockRiseFallInfo&, ErrCode )>   onRiseFallInfoArrived;

//板块异动
typedef std::function<void (ReqId , const HZData::BlockAbnormalInfo&, ErrCode )>   OnBlockAbnormalInfoArrived;

//资金异动
typedef std::function<void (ReqId , const HZData::FundAbnormalInfo&, ErrCode )>   OnFundAbnormalInfoArrived;

//板块集合竞价
typedef std::function<void (ReqId,const HZData::BiddingBlockInfo &, ErrCode)>  OnBiddingBlockArrived;

//个股集合竞价
typedef std::function<void (ReqId,const HZData::BiddingInfo &, ErrCode)>   OnBiddingArrived;


//个股短线异动
typedef std::function<void (ReqId , const HZData::ShortMotionInfo&, ErrCode )>   OnShortMotionInfoArrived;

//涨停监测统计
typedef std::function<void (ReqId , const HZData::RiseFallMonitorNumInfo&, ErrCode )>   OnRfmNumInfoArrived;

//涨停监测数据
typedef std::function<void (ReqId , const HZData::RiseFallMonitorInfo&, ErrCode )>   OnRFMonitorInfoArrived;

//分价表
typedef std::function<void (ReqId , const HZData::PriceTickInfo&, ErrCode )>   OnPriceTickInfoArrived;

//30日主力增减仓
typedef std::function<void (ReqId , const HZData::IncrDecPostionInfo&, ErrCode )>   OnIncrDecPostionInfoArrived;

typedef std::function<void (ReqId , const HZData::ZhibiaoInfo&, ErrCode )>   OnZhibiaoInfoArrived;

//搜索
typedef std::function<void (ReqId  , QVector<HZData::CodeInfo>, ErrCode )>                    OnCodeInfoArrived;


//行情返回接口，新版
typedef std::function<void (ReqId  ,const QVector<HZData::StockCode >&, ErrCode) >      OnStockCodeArrived;

typedef std::function<void (ReqId  , char *data, int num, ErrCode )>             OnStockDynaArrived;

//盘中异动
typedef std::function<void (ReqId,const HZData::MidAbnormalInfo&,ErrCode)>  OnMidAbnormalArrived;

//异动热门板块
typedef std::function<void (ReqId , const QVector<HZData::Block>& , ErrCode )>  OnAbnormalBlockArrived;

//授时
typedef std::function<void (ReqId, int ,ErrCode)>         OnTimeArrived;
typedef OnRecvTradeDay                                           OnTradeDayArrived;

//======================================================================

//动态行情信息-批量
struct RequestBatchStock
{
    RequestBatchStock(){}
    RequestBatchStock(const HZData::ReqBatchDynaInfo &req,OnStockDynaArrived fun)
        :reqInfo(req),stockDynaArrived(fun){}
    HZData::ReqBatchDynaInfo    reqInfo ;
    OnStockDynaArrived stockDynaArrived;
};


//动态行情信息
struct RequestOptionalStock
{
    RequestOptionalStock(){}
    RequestOptionalStock(const HZData::ReqOptionalDynaInfo &req,OnStockDynaArrived fun)
        :reqInfo(req),stockDataArrived(fun){}

    HZData::ReqOptionalDynaInfo    reqInfo ;
    OnStockDynaArrived     stockDataArrived;
};



//逐笔交易
struct RequestTick
{
    RequestTick(const HZData::ReqTickInfo &req,OnTickArrived fun)
        :reqInfo(req),tickArrived(fun){}
    HZData::ReqTickInfo     reqInfo ;
    OnTickArrived tickArrived;
};

//K线
struct RequestKline
{
    RequestKline(){}
    RequestKline(const HZData::ReqKlineInfo &req,OnKLineArrived fun)
        :reqInfo(req),klineArrived(fun){}
    HZData::ReqKlineInfo      reqInfo ;
    OnKLineArrived klineArrived;
};

//分时
struct RequestTimeKline
{
    RequestTimeKline(){}
    RequestTimeKline(const HZData::ReqTimeKlineInfo &req,OnTimeKLineArrived fun)
        :reqInfo(req),timeKlineArrived(fun){}
    HZData::ReqTimeKlineInfo             reqInfo ;
    OnTimeKLineArrived         timeKlineArrived;
};

//集合竞价
struct RequestBidKline
{
    RequestBidKline(const HZData::ReqBidKlineInfo &req,OnBidKLineArrived fun)
        :reqInfo(req),bidKlineArrived(fun){}
    HZData::ReqBidKlineInfo     reqInfo ;
    OnBidKLineArrived               bidKlineArrived;
};

//当日资金排行
struct RequestBatchMoney
{
    RequestBatchMoney(){}
    RequestBatchMoney(const HZData::ReqBatchFundInfo &req,OnMoneyArrived fun)
        :reqInfo(req),moneyArrived(fun){}
    HZData::ReqBatchFundInfo     reqInfo ;
    OnMoneyArrived           moneyArrived;
};

//资金快照
struct RequestOptionalMoney
{
    RequestOptionalMoney(){}
    RequestOptionalMoney(const HZData::ReqOptionalFundInfo &req,OnMoneyArrived fun)
        :reqInfo(req),moneyArrived(fun){}
    HZData::ReqOptionalFundInfo     reqInfo ;
    OnMoneyArrived              moneyArrived;
};

//涨跌分布
struct RequestRiseFallInfo
{
    RequestRiseFallInfo(const HZData::ReqStockRiseFallInfo &req,onRiseFallInfoArrived fun)
        :reqInfo(req),riseFallArrived(fun){}
    HZData::ReqStockRiseFallInfo     reqInfo ;
    onRiseFallInfoArrived     riseFallArrived;
};

//板块异动
struct RequestBlockAbnormalInfo
{
    RequestBlockAbnormalInfo(const HZData::ReqBlockAbnormalInfo &req,OnBlockAbnormalInfoArrived fun)
        :reqInfo(req),blockAbnormalArrived(fun){}
    HZData::ReqBlockAbnormalInfo                reqInfo;
    OnBlockAbnormalInfoArrived       blockAbnormalArrived;
};

//请求资金异动排行
struct RequestBatchFundAbnormal
{
    RequestBatchFundAbnormal(const HZData::ReqBatchFundAbnormal &req,OnFundAbnormalInfoArrived fun)
        :reqInfo(req),fundAbnormalInfoArrived(fun){}
    HZData::ReqBatchFundAbnormal     reqInfo ;
    OnFundAbnormalInfoArrived             fundAbnormalInfoArrived;
};

//资金异动快照
struct RequestOptionalFundAbnormal
{
    RequestOptionalFundAbnormal(const HZData::ReqOptionalFundAbnormal &req,OnFundAbnormalInfoArrived fun)
        :reqInfo(req),fundAbnormalInfoArrived(fun){}
    HZData::ReqOptionalFundAbnormal     reqInfo ;
    OnFundAbnormalInfoArrived                      fundAbnormalInfoArrived;
};

//集合竞价信息-板块
struct RequestBatchBidBlock
{
    RequestBatchBidBlock(const HZData::ReqBatchBiddingBlock &req,OnBiddingBlockArrived fun)
        :reqInfo(req),biddingBlockArrived(fun){}
    HZData::ReqBatchBiddingBlock           reqInfo ;
    OnBiddingBlockArrived                         biddingBlockArrived;
};
struct RequestOptionalBidBlock
{
    RequestOptionalBidBlock(const HZData::ReqOptionalBiddingBlock &req,OnBiddingBlockArrived fun)
        :reqInfo(req),biddingBlockArrived(fun){}
    HZData::ReqOptionalBiddingBlock       reqInfo ;
    OnBiddingBlockArrived                         biddingBlockArrived;
};

//集合竞价信息-个股
struct RequestBatchBidding
{
    RequestBatchBidding(const HZData::ReqBatchBidding &req,OnBiddingArrived fun)
        :reqInfo(req),biddingArrived(fun){}
    HZData::ReqBatchBidding     reqInfo ;
    OnBiddingArrived                   biddingArrived;
};
struct RequestOptionalBidding
{
    RequestOptionalBidding(const HZData::ReqOptionalBidding &req,OnBiddingArrived fun)
        :reqInfo(req),biddingArrived(fun){}
    HZData::ReqOptionalBidding     reqInfo ;
    OnBiddingArrived                       biddingArrived;
};


//个股短线异动
struct RequestShortMotionInfo
{
    RequestShortMotionInfo(const HZData::ReqShortMotionInfo &req,OnShortMotionInfoArrived fun)
        :reqInfo(req),shotMotionArrived(fun){}
    HZData::ReqShortMotionInfo          reqInfo ;
    OnShortMotionInfoArrived               shotMotionArrived;
};

//涨停监测统计
struct RequestRfmNumInfo
{
    RequestRfmNumInfo(OnRfmNumInfoArrived fun)
        :rfmNumInfoArrived(fun){}
    OnRfmNumInfoArrived                     rfmNumInfoArrived;
};


//涨停监测数据
struct RequestRFMonitorInfo
{
    RequestRFMonitorInfo(const HZData::ReqBatchRfmInfo &req,OnRFMonitorInfoArrived fun)
        :reqInfo(req),rfmonitorInfoArrived(fun){}
    HZData::ReqBatchRfmInfo          reqInfo ;
    OnRFMonitorInfoArrived              rfmonitorInfoArrived;
};

//分价表
struct RequestPriceTickInfo
{
    RequestPriceTickInfo(const HZData::ReqPriceTickInfo &req,OnPriceTickInfoArrived fun)
        :reqInfo(req),priceTickArrived(fun){}
    HZData::ReqPriceTickInfo                reqInfo ;
    OnPriceTickInfoArrived       priceTickArrived;
};

//30日主力增加仓
struct RequestIncrDecPositionInfo
{
    RequestIncrDecPositionInfo(const HZData::ReqLeaderIncDecPostionInfo &req,OnIncrDecPostionInfoArrived fun)
        :reqInfo(req),incrDecArrived(fun){}
    HZData::ReqLeaderIncDecPostionInfo                reqInfo ;
    OnIncrDecPostionInfoArrived                 incrDecArrived;
};

//指标
struct RequestZhibiaoInfo
{
    RequestZhibiaoInfo(const HZData::ReqZhibiaoInfo &req,OnZhibiaoInfoArrived fun)
        :reqInfo(req),zhibiaoInfoArrived(fun){}
    HZData::ReqZhibiaoInfo          reqInfo ;
    OnZhibiaoInfoArrived               zhibiaoInfoArrived;
};

//search code 股票搜索
struct RequestCodeInfo
{
    RequestCodeInfo(const HZData::ReqQueryCode &req,OnCodeInfoArrived fun)
        :reqInfo(req),codeInfoArrived(fun){}
    HZData::ReqQueryCode     reqInfo ;
    OnCodeInfoArrived         codeInfoArrived;
};

//盘中异动
struct RequestMidAbnormalInfo
{
    RequestMidAbnormalInfo(const HZData::ReqMidAbnormalInfo &req,OnMidAbnormalArrived fun)
        :reqInfo(req),midAbnormalArrived(fun){}
    HZData::ReqMidAbnormalInfo   reqInfo ;
    OnMidAbnormalArrived             midAbnormalArrived;
};

//异动热门板块
struct RequestAbnormalBlockInfo
{
    RequestAbnormalBlockInfo(const HZData::ReqAbnormalBlockInfo &req,OnAbnormalBlockArrived fun)
        :reqInfo(req),ablHotBlockArrived(fun){}
    HZData::ReqAbnormalBlockInfo   reqInfo ;
    OnAbnormalBlockArrived             ablHotBlockArrived;
};


#endif // DATACTRLDEF_H
