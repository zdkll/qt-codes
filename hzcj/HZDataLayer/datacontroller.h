/********************************************************************
created:    2018/04/23
author: dk.zhang
purpose: 数据层对外公共接口类，数据服务请求和订阅类
*********************************************************************/
#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <unordered_map>
#include <QObject>
#include <QTimer>
#include <QVector>

#include "hzdatalayer_global.h"
#include "packetprocess.h"


class  HZDATALAYERSHARED_EXPORT DataController : public QObject
{
    Q_OBJECT
public:
    ~DataController();
    // @brief  获取单例
    static DataController *instance();

    // @brief  设置数据服务器@param[in] Ip地址  @param[in] port 端口号
    void setServer(const QString &ip,const int &port);

    //用户登录后，登录数据服务器
    bool logIn(quint64 uid,QString acessToken);

    //退出登录
    void logOut();

    // @brief  获取错误代码和错误信息
    const ErrCode &errCode() const {return m_PacketProcess->errCode();}
    const string  &errString() const{return m_PacketProcess->errString();}

    /**
     * @brief  设置和返回订阅周期
     * @param[in] 订阅周期 单位：S秒
     */
    void setSubPeriod(HZData::SubPeriod subPeriod);
    HZData::SubPeriod subPeriod() const {return m_subPeriod;}

    /******************************请求*************************************/
    /**
     * @brief  请求
     * @param[in] req 本地请求信息结构体
     * @param[in] fun  请求回调函数
     * @retval 请求ID
     */
    ReqId requestBatchDyna(const HZData::ReqBatchDynaInfo& req, OnStockDynaArrived fun);

    //  请求行情快照
    ReqId requestOptionalDyna(const HZData::ReqOptionalDynaInfo& req,OnStockDynaArrived fun);

    // 请求异动板块列表
    ReqId requestAbnormalBlock(const HZData::ReqAbnormalBlockInfo& req,OnAbnormalBlockArrived func);

    //请求K线
    ReqId requestKline(const HZData::ReqKlineInfo& req,OnKLineArrived fun);
    //请求分时数据
    ReqId requestTimeKline(const HZData::ReqTimeKlineInfo& req,OnTimeKLineArrived fun);

    //请求逐笔数据
    ReqId requestTick(const HZData::ReqTickInfo& req,OnTickArrived fun);

    //股票搜索
    ReqId requestCodeInfo(const HZData::ReqQueryCode& req,OnCodeInfoArrived fun);

    //请求当日资金排行
    ReqId requestBatchMoney(const HZData::ReqBatchFundInfo &req,OnMoneyArrived fun);
    //请求当日资金快照
    ReqId requestOptionalMoney(const HZData::ReqOptionalFundInfo &req,OnMoneyArrived fun);

    //请求板块异动
    ReqId requestBlockAbnormalInfo(const HZData::ReqBlockAbnormalInfo &req,OnBlockAbnormalInfoArrived fun);

    //请求资金异动排行
    ReqId requestBatchFundAbnormal(const HZData::ReqBatchFundAbnormal& req,OnFundAbnormalInfoArrived fun);
    ReqId requestOptionalFundAbnormal(const HZData::ReqOptionalFundAbnormal& req,OnFundAbnormalInfoArrived fun);

    //请求个股集合竞价
    ReqId requestBatchBidding(const HZData::ReqBatchBidding& req, OnBiddingArrived func);

    //请求资金异动
    ReqId requestOptionalBidding(const HZData::ReqOptionalBidding &req,OnBiddingArrived func);

    //请求短线异动
    ReqId requestShortMotionInfo(const HZData::ReqShortMotionInfo &req,OnShortMotionInfoArrived fun);

    //请求涨停监测
    ReqId requestRFMonitorInfo(const HZData::ReqBatchRfmInfo &req,OnRFMonitorInfoArrived fun);

    ReqId requestZhibiaoInfo(const HZData::ReqZhibiaoInfo &req,OnZhibiaoInfoArrived fun);

    //盘中异动
    ReqId requestMidAbnormalInfo(const HZData::ReqMidAbnormalInfo &req,OnMidAbnormalArrived fun);

    //授时接口
    ReqId requestTime(OnTimeArrived func);

    /******************************订阅*************************************/
    /**
     * @brief   订阅
     * @param[in] req 本地订阅信息结构体
     * @param[in] fun 订阅回调函数
     * @retval 订阅ID
     */
    //订阅动态行情-批量
    ReqId subBatchDyna(const HZData::ReqBatchDynaInfo&  ,OnStockDynaArrived fun);

    //订阅行情
    ReqId subOptionalDyna(const HZData::ReqOptionalDynaInfo& req,OnStockDynaArrived fun);

    //订阅K 线
    ReqId subKline(const HZData::ReqKlineInfo& req,OnKLineArrived fun);
    //订阅分时数据
    ReqId subTimeKline(const HZData::ReqTimeKlineInfo& req,OnTimeKLineArrived fun);
    //订阅集合竞价K线
    ReqId subBidKline(const HZData::ReqBidKlineInfo& req,OnBidKLineArrived fun);
    //订阅逐笔
    ReqId subTick(const HZData::ReqTickInfo& req,OnTickArrived fun);
    //订阅当日资金排行
    ReqId subBatchMoney(const HZData::ReqBatchFundInfo &req,OnMoneyArrived fun);
    //订阅当日资金快照
    ReqId subOptionalMoney(const HZData::ReqOptionalFundInfo &req,OnMoneyArrived fun);
    //订阅涨跌
    ReqId subRiseFallInfo(const HZData::ReqStockRiseFallInfo &req,onRiseFallInfoArrived fun);
    //订阅板块异动
    ReqId subBlockAbnormalInfo(const HZData::ReqBlockAbnormalInfo &req,OnBlockAbnormalInfoArrived fun);

    //订阅资金异动排行
    ReqId subBatchFundAbnormal(const HZData::ReqBatchFundAbnormal& req,OnFundAbnormalInfoArrived fun);

    //订阅资金异动快照
    ReqId subOptionalFundAbnormal(const HZData::ReqOptionalFundAbnormal &req,OnFundAbnormalInfoArrived fun);

    //订阅板块集合竞价排行
    ReqId subBatchBiddingBlock(const HZData::ReqBatchBiddingBlock &req,OnBiddingBlockArrived func);

    //订阅个股集合竞价排行
    ReqId subBatchBidding(const HZData::ReqBatchBidding &req,OnBiddingArrived func);
    ReqId subOptionalBidding(const HZData::ReqOptionalBidding &req,OnBiddingArrived func);

    //订阅短线异动
    ReqId subShortMotionInfo(const HZData::ReqShortMotionInfo &req,OnShortMotionInfoArrived fun);

    //订阅涨停统计
    ReqId subRfmNumInfo(OnRfmNumInfoArrived fun);

    //订阅涨停监测
    ReqId subRFMonitorInfo(const HZData::ReqBatchRfmInfo &req,OnRFMonitorInfoArrived fun);

    //订阅分价表
    ReqId subPriceTickInfo(const HZData::ReqPriceTickInfo &req,OnPriceTickInfoArrived fun);

    //订阅主力增减仓
    ReqId subIncDecPositionInfo(const HZData::ReqLeaderIncDecPostionInfo &req,OnIncrDecPostionInfoArrived func);

    //盘中异动
    ReqId subMidAbnormalInfo(const HZData::ReqMidAbnormalInfo &req,OnMidAbnormalArrived fun);

    //订阅用户互踢功能 @param[in] fun 回调函数
    //订阅用户登录踢出消息
    void subUserKickMsg(OnRecvUserKick fun){m_d->recvUserKick = fun;}

    //订阅交易日切换消息
    void subTradeDayMsg(OnRecvTradeDay fun);

    //新版订阅接口
    bool cancelSub(ReqId ReqId);

    //获取请求的Mask字段，给解包使用，根据不同的mask解包存为不同结构体
    int    getMask(HZData::ServiceType  type,ReqId reqId);

    bool isNetIoReady()const {return m_netIOReady;}
signals:
    void signalNetIoReady(bool ready);

protected:
    void timerEvent(QTimerEvent *e);
    void netioReady(bool ready);

private:
    DataController(QObject *parent = 0);

    void setRecvCallBack();

    /******************************接收回复数据包*************************************/
    //新的接口
    void recvBatchDyna(const  ReqId &, char *data, int num);

    void recvOptionalDyna(const  ReqId &,const QVector<HZData::AllStock> & );
    void recvBiddingInfo(ReqId reqId, const HZData::BiddingInfo &);
    void recvBiddingBlockInfo(ReqId reqId, const HZData::BiddingBlockInfo &);

    void recvTick(const  ReqId &,const HZData::Zhubi &zhubi);
    void recvKline(const  ReqId &,const HZData::KlineInfo &klineInfo);
    void recvTimeKline(const  ReqId &,const HZData::KlineInfo &klineInfo);
    void recvBidKline(const  ReqId &,const HZData::KlineInfo &klineInfo);
    void recvCodeInfo(const  ReqId &,const QVector<HZData::CodeInfo> &codes);
    void recvRiseFallInfo(const ReqId &,const HZData::StockRiseFallInfo &riseFallInfo);

    //资金
    void recvStockMoney(const ReqId &reqId, const QVector<HZData::StockMoney> &stockMoney);

    //板块异动
    void recvBlockAbnormalInfo(ReqId,const HZData::BlockAbnormalInfo &);

    //资金
    void recvFundAbnormalInfo(ReqId,const HZData::FundAbnormalInfo &);

    //个股短线异动
    void recvShortMotionInfo(ReqId,const HZData::ShortMotionInfo &);

    //涨停监测
    void recvRfmNumInfo(ReqId,const HZData::RiseFallMonitorNumInfo &);
    void recvRiseFallMonitorInfo(ReqId,const HZData::RiseFallMonitorInfo &);

    void recvPriceTickInfo(ReqId,const HZData::PriceTickInfo &);

    void recvIncDecPosInfo(ReqId,const HZData::IncrDecPostionInfo &);

    void recvZhibiaoInfo(ReqId,const HZData::ZhibiaoInfo&);

    void recvMidAbnormalInfo(ReqId,const HZData::MidAbnormalInfo& midAbnormalInfo);

    void recvAbnormalBlockInfo(ReqId,const QVector<HZData::Block>& ablHotBlocks);

    void recvUserKick(const HZData::UserLoginKickInfo &kickInfo);

    void recvTime(ReqId,uint timet);
    void recvTradeDay(const HZData::TradeDayMsg& msg);

    //数据订阅请求-发送
    void postSubRequestOneSec(){}  //1s周期请求
    void postSubRequestTwoSec();    //2s周期请求
    void postSubRequestThreeSec(); //3s周期请求

    //获取并保存reqId对应的服务类型
    ReqId            getReqId(HZData::ServiceType type);
    HZData::ServiceType  serviceType(ReqId reqId);

    void resolveReqOptStock(const  ReqId &reqId,const QVector<HZData::AllStock> &stocks);
    void resolveSubOptStock(const  ReqId &reqId,const QVector<HZData::AllStock> &stocks);

    void resloveBatchStockMoney(const ReqId reqId,const QVector<HZData::StockMoney> &stockMArroneys);
    void resloveOptStockMoney(const ReqId reqId,const QVector<HZData::StockMoney> &stockMArroneys);

private:
    struct PrivateData
    {
        void clearReqSubInfo();

        //请求和订阅信息
        QMap<ReqId, RequestBatchStock>                requestBatchStock;
        QMap<ReqId, RequestOptionalStock>            requestOptionalStock;
        QMap<ReqId, RequestTick>                           requestTick;
        QMap<ReqId, RequestKline>                          requestKline;
        QMap<ReqId, RequestTimeKline>                   requestTimeKline;
        QMap<ReqId, RequestBidKline>                     requestBidKline;
        QMap<ReqId, RequestCodeInfo>                    requestCodeInfo;

        //资金
        QMap<ReqId,RequestBatchMoney>                requestBatchMoney;
        QMap<ReqId,RequestOptionalMoney>            requestOptionalMoney;

        QMap<ReqId,RequestRiseFallInfo>                 requestRiseFallInfo;
        QMap<ReqId,RequestBlockAbnormalInfo>      requestBlockAbnormalInfo;

        //资金异动-排行和快照
        QMap<ReqId,RequestBatchFundAbnormal>      requestBatchFundAbnormal;
        QMap<ReqId,RequestOptionalFundAbnormal>  requestOptionalFundAbnormal;

        //板块集合竞价
        QMap<ReqId,RequestBatchBidBlock>               requestBatchBidBlock;
        QMap<ReqId,RequestOptionalBidBlock>           requestOptionalBidBlock;
        //个股集合竞价
        QMap<ReqId,RequestBatchBidding>                 requestBatchBidding;
        QMap<ReqId,RequestOptionalBidding>             requestOptionalBidding;

        //短线异动
        QMap<ReqId,RequestShortMotionInfo>           requestShortMotionInfo;
        //涨停监测
        QMap<ReqId,RequestRfmNumInfo>              requestRfmNumInfo;
        QMap<ReqId,RequestRFMonitorInfo>           requestRFMonitorInfo;

        QMap<ReqId,RequestPriceTickInfo>               requestPriceTickInfo;
        QMap<ReqId,RequestIncrDecPositionInfo>     requestIncDecPosInfo;

        QMap<ReqId,RequestZhibiaoInfo>                 requestZhibiaoInfo;

        QMap<ReqId,RequestMidAbnormalInfo>           requestMidAbnormalInfo;

        //异动板块
        QMap<ReqId,RequestAbnormalBlockInfo>        requestAbnormalBlockInfo;
        //授时
        QMap<ReqId,OnTimeArrived>                         requestTime;

        //请求Id对应的服务
        QHash<ReqId,HZData::ServiceType>    reqIdServices;

        //用户登录踢掉
        OnRecvUserKick                                   recvUserKick ;
        //交易日切换
        QVector<OnRecvTradeDay>                         recvTradeDay ;
    };
    struct TimerInfo
    {
        int timerId = -1;
        int count = 0;
    };

    TimerInfo                     m_timerInfo;      //本地订阅定时器
    HZData::SubPeriod                    m_subPeriod;    //订阅推送周期
    PrivateData                 *m_d;
    PacketProcessor         *m_PacketProcess;
    bool m_netIOReady = false;
    QHash<ReqType,int>  m_keysHash;
    ReqId m_reqId;
};

#define gHZDataController DataController::instance()

#endif // DATACONTROLLER_H
