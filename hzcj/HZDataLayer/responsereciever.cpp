#include "responsereciever.h"

#include <QDebug>
#include <QDateTime>

#include "protobufs.h"
#include "datasimulator.h"
#include "quotepacket.h"

using namespace HZData;
//接收回复------------------------------------
void ResponseReciever::setRecvCbFunction(OnRecvFunction recvFunc)
{
    m_recvFunc  = recvFunc;
    DataSimulator::instance()->setRecvCbFunction(recvFunc);
}

void  ResponseReciever::onMessageArrived(const HZPack &pack)
{
    //静态码表
    ReqId     reqId       =  pack.head.unSeq;
    quint16   cmdType = pack.head.unCmd;

    // qDebug()<<"recv msg:"<<pack.head.unSeq <<cmdType << "reqId" << reqId;
    switch (cmdType) {
    case  Cmd_Publish : //订阅返回
    {
        QuoteProto::PublishResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        //订阅第一次数据返回
        //订阅行情返回
        if(response.quote_arr_size()> 0){
            int size = response.quote_arr_size();
            QVector<HZData::AllStock > stock(size);
            for(int i =0;i<size;i++){
                QuotePacket::readAllStock(stock[i],response.quote_arr(i));
            }
            m_recvFunc.recvOptionalStock(reqId,stock);
        }
        //订阅资金返回
        if(response.money_arr_size() >0){}
    }
        break;
    case  Cmd_BatchStock://行情排行
        readBatchDyna(reqId,pack.strMsg);
        break;
    case  Cmd_OptionalStock://动态行情
    {
        QuoteProto::StockOptionalResponse response;
        response.ParseFromString(pack.strMsg);

        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size = response.stk_size();
        QVector<HZData::AllStock > stocks(size);
        for(int i =0;i<size;i++){
            QuotePacket::readAllStock(stocks[i],response.stk(i));
        }
        m_recvFunc.recvOptionalStock(reqId,stocks);//订阅返回订阅快照
    }
        break;
    case  Cmd_Zhubi://逐笔
    {
        QuoteProto::GetZhubiResponse response;
        response.ParseFromString(pack.strMsg);

        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size           = response.zhubi_info_size();
        HZData::Zhubi zhubi;
        zhubi.totalCount = response.total_count();
        QVector<HZData::HZTick > ticks(size);
        for(int i =0;i<size;i++)
            QuotePacket::readTick(ticks[i],response.zhubi_info(i));

        zhubi.ticks = std::move(ticks);
        m_recvFunc.recvTick(reqId,zhubi);
    }
        break;
    case  Cmd_Kline://K线
    {
        QuoteProto::GetKLineResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::KlineInfo klineInfo;
        klineInfo.stock_code = response.stock_code();
        klineInfo.kline_type  = response.kline_type();
        int size = response.kline_info_size();
        QVector<HZData::HZKline > klines(size);
        for(int i =0;i<size;i++){
            QuotePacket::readKline(klines[i],response.kline_info(i));
        }
        klineInfo.klines = std::move(klines);
        //K线
        m_recvFunc.recvKLine(reqId,klineInfo);
    }
        break;
    case  Cmd_TimeKline://分时
    {
        QuoteProto::GetTimeKLineResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::KlineInfo klineInfo;
        klineInfo.stock_code = response.stock_code();
        klineInfo.kline_type  = response.kline_type();
        int size = response.kline_info_size();
        QVector<HZData::HZKline > klines(size);
        for(int i =0;i<size;i++)
            QuotePacket::readKline(klines[i],response.kline_info(i));

        klineInfo.klines = std::move(klines);
        //分时
        m_recvFunc.recvTimeKline(reqId,klineInfo);
    }
        break;
    case  Cmd_BiddingKline://集合竞价
        readBiddingKline(reqId,pack.strMsg);
        break;
    case Cmd_BatchMoney://资金排行
    {
        QuoteProto::BatchGetMoneyResponse  response;
        response.ParseFromString(pack.strMsg);

        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();
        int size = response.stk_size();
        QVector<HZData::StockMoney> stkMoneys(size);
        for(int i =0;i<size;i++)
            QuotePacket::readStockMoney(stkMoneys[i], response.stk(i));

        m_recvFunc.recvStockMoney(reqId,stkMoneys);
    }
        break;
    case  Cmd_OptionalMoney://资金快照
    {
        QuoteProto::MoneyOptionalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size = response.stk().size();
        QVector<HZData::StockMoney> stkMoneys(size);
        for(int i =0;i<size;i++)
            QuotePacket::readStockMoney(stkMoneys[i], response.stk(i));
        //当日资金
        m_recvFunc.recvStockMoney(reqId,stkMoneys);
    }
        break;

    case Cmd_GetFenjia://分价
        readPriceTick(reqId,pack.strMsg);
        break;
    case Cmd_GetLeaderIncDecPos:
        readLeaderIncDecPos(reqId,pack.strMsg); //主力增减仓
        break;
    case  Cmd_Query_Code://搜索代码
    {
        QuoteProto::QueryCodeResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size = response.codes_table_size();
        QVector<HZData::CodeInfo> codes(size);
        for(int i =0;i<size;i++){
            codes[i].type  = response.codes_table(i).type();
            codes[i].code = response.codes_table(i).code();
            codes[i].name = response.codes_table(i).name();
        }
        m_recvFunc.recvCodeInfo(reqId,codes);
    }
        break;
    case  Cmd_Kick_User://用户剔掉
    {
        QuoteProto::UserLoginKickInfo response;
        response.ParseFromString(pack.strMsg);

        HZData::UserLoginKickInfo userKickInfo;

        userKickInfo.uid = response.uid();
        userKickInfo.device_type = (DeviceType)response.device_type();
        userKickInfo.kick_desc    = response.kick_desc();
        m_recvFunc.recvUserKick(userKickInfo);
    }
        break;
    case  Cmd_RiseFallInfo://涨跌分布
    {
        QuoteProto::GetStockRiseFallInfoResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::StockRiseFallInfo riseFallInfo;
        QuotePacket::readRiseFallInfo(riseFallInfo,response.info());

        m_recvFunc.recvRiseFallInfo(reqId,riseFallInfo);
    }
        break;
    case Cmd_BlockAbnormal: //板块异动
    {
        QuoteProto::BlockMoneyAbnoramlResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size = response.blocks_size();

        HZData::BlockAbnormalInfo  info;
        info.blockAbnormals.resize(size);
        info.totalCout = size;
        for(int i=0;i<size;i++)
            QuotePacket::readBlockAbnormal(info.blockAbnormals[i],response.blocks(i));

        m_recvFunc.recvBlockAbnormalInfo(reqId,info);
    }
        break;

    case Cmd_BatchFundAbnormal://资金异动排行
    {
        QuoteProto::BatchGetMoneyAbnormalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int num =  response.stk_size();
        HZData::FundAbnormalInfo fundAbmlInfo;
        fundAbmlInfo.totalCount = response.total_cnt();
        QVector<HZData::FundAbnormal> fundAbnormals(num);
        for(int i=0;i<num;i++)
            QuotePacket::readFundAbnormal(fundAbnormals[i],response.stk(i));
        fundAbmlInfo.fundAbmls = std::move(fundAbnormals);
        m_recvFunc.recvFundAbnormalInfo(reqId, fundAbmlInfo);
    }
        break;
    case Cmd_OptionalFundAbnormal://资金异动快照
    {
        QuoteProto::MoneyAbnormalOptionalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int num =  response.stk_size();
        HZData::FundAbnormalInfo fundAbmlInfo;
        fundAbmlInfo.totalCount = num;
        QVector<HZData::FundAbnormal> fundAbnormals(num);
        for(int i=0;i<num;i++)
            QuotePacket::readFundAbnormal(fundAbnormals[i],response.stk(i));
        fundAbmlInfo.fundAbmls = std::move(fundAbnormals);
        m_recvFunc.recvFundAbnormalInfo(reqId, fundAbmlInfo);
    }
        break;

    case Cmd_BatchBidding: //个股集合竞价
    {
        QuoteProto::BatchGetBiddingResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::BiddingInfo biddingInfo;
        biddingInfo.total_cnt = response.total_cnt();
        int num = response.stk_size();
        biddingInfo.stk.resize(num);
        for(int i=0;i<num;i++)
            QuotePacket::readStockBidding(biddingInfo.stk[i],response.stk(i));
        m_recvFunc.recvBiddingInfo(reqId,biddingInfo);
    }
        break;
    case Cmd_OptionalBidding://集合竞价
    {
        QuoteProto::BiddingOptionalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::BiddingInfo biddingInfo;
        int num = response.stk_size();
        biddingInfo.stk.resize(num);
        biddingInfo.total_cnt = num;
        for(int i=0;i<num;i++)
            QuotePacket::readStockBidding(biddingInfo.stk[i],response.stk(i));
        m_recvFunc.recvBiddingInfo(reqId,biddingInfo);
    }
        break;
    case Cmd_BatchBiddingBlock://集合竞价
    {
        QuoteProto::BatchGetBiddingBlockResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::BiddingBlockInfo biddingInfo;
        biddingInfo.total_cnt = response.total_cnt();
        int num = response.stk_size();
        biddingInfo.stk.resize(num);
        for(int i=0;i<num;i++)
            QuotePacket::readBlockBidding(biddingInfo.stk[i],response.stk(i));
        m_recvFunc.recvBiddingInfoBlock(reqId,biddingInfo);
    }
        break;
    case Cmd_OptionalBiddingBlock://集合竞价-板块
    {
        QuoteProto::BiddingBlockOptionalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::BiddingBlockInfo biddingInfo;
        int num = response.stk_size();
        biddingInfo.stk.resize(num);
        for(int i=0;i<num;i++)
            QuotePacket::readBlockBidding(biddingInfo.stk[i],response.stk(i));
        m_recvFunc.recvBiddingInfoBlock(reqId,biddingInfo);
    }
        break;
    case Cmd_ShortMotion: //短线异动
    {
        QuoteProto::BatchShortMotionResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        int size = response.stk_size();
        HZData::ShortMotionInfo  info;
        info.shortMotions.resize(size);
        info.total_cnt = response.total_cnt();
        for(int i=0;i<size;i++)
            QuotePacket::readShortMotion(info.shortMotions[i],response.stk(i));
        m_recvFunc.recvShortMotionInfo(reqId,info);
    }
        break;
    case Cmd_RFMNumInfo://涨停统计
    {
        QuoteProto::RFMonitorNumInfoOptionalResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::RiseFallMonitorNumInfo info;
        QuotePacket::readRfmNumInfo(info,response.msg());

        m_recvFunc.recvRfmNumInfo(reqId,info);
    }
        break;
    case Cmd_RFMonitorInfo://涨停监测
    {
        QuoteProto::BatchRFMonitorResponse  response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        HZData::RiseFallMonitorInfo info;
        info.totalCount = response.total_cnt();
        const int num = response.stk_size();
        info.rfmData.resize(num);
        for(int i=0;i<num;i++)
            QuotePacket::readRFMonitorInfo(info.rfmData[i],response.stk(i));

        m_recvFunc.recvRFMonitorInfo(reqId,info);
    }
        break;
    case Cmd_Zhibiao_Hjjj:
    case Cmd_Zhibiao_Dcxg:
    case Cmd_Zhibiao_Byzs:
    case Cmd_Zhibiao_Bdww:
    {
        readZhibiao(reqId,pack.strMsg);
    }
        break;
    case Cmd_ReqTime:
    {
        QuoteProto::GetTimeResponse response;
        response.ParseFromString(pack.strMsg);
        m_ret.err_code = response.ret().err_code();
        m_ret.err_msg  = response.ret().err_msg();

        m_recvFunc.recvTime(reqId,response.cur_time());
    }
        break;
    case Cmd_Trade_Day:
    {
        QuoteProto::BroadCastTradeDayMsg  response;
        response.ParseFromString(pack.strMsg);

        HZData::TradeDayMsg tradeDayMsg;
        tradeDayMsg.trade_day = response.trade_day();
        tradeDayMsg.trade_type = response.trade_type();
        m_recvFunc.recvTradeDay(tradeDayMsg);
    }
        break;
    case  Cmd_Mid_Abnormal:
    {
        readMidAbnormal(reqId,pack.strMsg);
    }
        break;
    case  Cmd_Abnormal_Block:
        readAbnormalBlock(reqId,pack.strMsg);
        break;
    default:
        break;
    }
}


void ResponseReciever::readBatchDyna(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::BatchGetStockResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    int mask = m_recvFunc.getMask(Req_Batch_Dyna,reqId);
    if(mask<0)  return;

    //根据返回股票码
    const int num = response.stk_size();
    switch (mask) {
    case HZData::StockCode_Mask:
    {
        HZData::StockCode *stockCode = new HZData::StockCode[num];
        for(int i=0;i<num;i++)
            QuotePacket::readQuoteStockCode(response.stk(i),stockCode[i]);
        m_recvFunc.recvBatchStock(reqId, (char*)stockCode,num);
        delete[] stockCode;
        break;
    }
    case HZData::SLStock_Mask:
    {
        HZData::SLStock *slStock = new HZData::SLStock[num];
        for(int i=0;i<num;i++)
            QuotePacket::readQuoteSLStock(response.stk(i),slStock[i]);
        m_recvFunc.recvBatchStock(reqId, (char*)slStock,num);
        delete[] slStock;
        break;
    }
    case HZData::Stock_Mask:
    {
        HZData::Stock *stock = new HZData::Stock[num];
        for(int i=0;i<num;i++)
            QuotePacket::readQuoteStock(response.stk(i),stock[i]);
        m_recvFunc.recvBatchStock(reqId, (char*)stock,num);
        delete[] stock;
    }
        break;
    case HZData::Block_Mask:
    {
        HZData::Block *block = new HZData::Block[num];
        for(int i=0;i<num;i++)
            QuotePacket::readQuoteBlock(response.stk(i),block[i]);
        m_recvFunc.recvBatchStock(reqId, (char*)block,num);
        delete[] block;
        break;
    }
        break;
    case HZData::Index_Mask:
        break;
    default:
        break;
    }
}


void ResponseReciever::readPriceTick(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::GetFenJiaResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    HZData::PriceTickInfo priceTickInfo;

    priceTickInfo.totalCount = response.total_count();
    priceTickInfo.volume = response.volume();
    priceTickInfo.max_volume = response.max_volume();
    priceTickInfo.pre_close_price = response.pre_close_price();

    int size = response.fenjia_info_size();

    priceTickInfo.priceTicks.resize(size);
    for(int i=0;i<size;i++){
        priceTickInfo.priceTicks[i].price = response.fenjia_info(i).price()/1000.f;
        priceTickInfo.priceTicks[i].buy_volume= response.fenjia_info(i).buy_volume();
        priceTickInfo.priceTicks[i].sell_volume = response.fenjia_info(i).sell_volume();
        priceTickInfo.priceTicks[i].volume = priceTickInfo.priceTicks[i].buy_volume + priceTickInfo.priceTicks[i].sell_volume;
    }
    m_recvFunc.recvPriceTickInfo(reqId,priceTickInfo);
}

void ResponseReciever::readLeaderIncDecPos(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::MoneyFieldLineResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    int size = response.stk_size();
    HZData::IncrDecPostionInfo  incrDecPosInfo(size);
    for(int i=0;i<size;i++){
        incrDecPosInfo[i].date =  response.stk(i).trading_day();
        incrDecPosInfo[i].money =  response.stk(i).leader_money().money_netput()/1000;
    }

    m_recvFunc.recvIncDecPosInfo(reqId,incrDecPosInfo);
}

void ResponseReciever::readBiddingKline(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::GetBiddingQuoteKlineResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    HZData::KlineInfo klineInfo;
    klineInfo.stock_code = response.stock_code();

    int size = response.stk_size();
    QVector<HZData::HZKline > klines(size);

    //截取时间
    QDateTime start,end;
    QDate date  =QDate::currentDate();
    if(size>0)
        date = QDateTime::fromTime_t(response.stk(0).upsert_time()).date();
    start.setDate(date);
    start.setTime(QTime(9,15,3));
    end.setDate(date);
    end.setTime(QTime(9,25,3));
    uint start_t = start.toTime_t();
    uint end_t  = end.toTime_t();

    int num = 0;
    for(int i =0;i<size;i++){
        long long time = response.stk(i).upsert_time();
        if(start_t <= response.stk(i).upsert_time() && response.stk(i).upsert_time() <= end_t){
            QuotePacket::readBidKline(klines[num],response.stk(i));
            num ++;
        }
    }
    klines.resize(num);
    klineInfo.klines = std::move(klines);
    //集合竞价分时K线
    m_recvFunc.recvBidKline(reqId,klineInfo);
}

void ResponseReciever::readZhibiao(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::GetZhibiaoResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    HZData::ZhibiaoInfo zhibiaoInfo;
    zhibiaoInfo.ztype = (ZhibiaoType)response.ztype();

    zhibiaoInfo.dotInfo.resize(response.dot_info_size());
    for(int i=0;i<response.dot_info_size();i++){
        zhibiaoInfo.dotInfo[i].stype = response.dot_info(i).stype();
        zhibiaoInfo.dotInfo[i].upserttime_time = response.dot_info(i).upserttime_time();
    }
    zhibiaoInfo.colInfo.resize(response.col_info_size());
    for(int i=0;i<response.col_info_size();i++){
        zhibiaoInfo.colInfo[i].color_type = response.col_info(i).color_type();
        zhibiaoInfo.colInfo[i].upserttime_time = response.col_info(i).upserttime_time();
        zhibiaoInfo.colInfo[i].high = response.col_info(i).high();
        zhibiaoInfo.colInfo[i].low = response.col_info(i).low();
    }
    m_recvFunc.recvZhibiaoInfo(reqId,zhibiaoInfo);
}

void ResponseReciever::readMidAbnormal(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::MidAbnormalResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    int size = response.ab_infos_size();

    HZData::MidAbnormalInfo  midAbnormalInfo;
    midAbnormalInfo.totalCount  =response.total_count();
    midAbnormalInfo.midAbnormals.resize(size);
    for(int i=0;i<size;i++){
        midAbnormalInfo.midAbnormals[i].abnormal_type = (HZData::MidAbnormalType)response.ab_infos(i).abnormal_type();
        midAbnormalInfo.midAbnormals[i].stock_code = response.ab_infos(i).stock_code();
        midAbnormalInfo.midAbnormals[i].stock_name = response.ab_infos(i).stock_name();
        midAbnormalInfo.midAbnormals[i].update_time = response.ab_infos(i).update_time();
        midAbnormalInfo.midAbnormals[i].rate = response.ab_infos(i).rate();
        midAbnormalInfo.midAbnormals[i].last_price = float(response.ab_infos(i).last_price())/1000.f;
        midAbnormalInfo.midAbnormals[i].turnover = response.ab_infos(i).turnover()/1000.f;
        midAbnormalInfo.midAbnormals[i].volume = response.ab_infos(i).volume();
        midAbnormalInfo.midAbnormals[i].real_time_rate = response.ab_infos(i).real_time_rate();
    }

    m_recvFunc.recvMidAbnormalInfo(reqId,midAbnormalInfo);
}

void ResponseReciever::readAbnormalBlock(ReqId reqId,const std::string& strMsg)
{
    QuoteProto::BlockMoneyAbnormalPCResponse response;
    response.ParseFromString(strMsg);
    m_ret.err_code = response.ret().err_code();
    m_ret.err_msg  = response.ret().err_msg();

    int size = response.blocks_size();

    QVector<HZData::Block> blocks(size);
    for(int i=0;i<size;i++){
        QuotePacket::readQuoteBlock(response.blocks(i),blocks[i]);
    }
    m_recvFunc.recvAbnormalBlockInfo(reqId,blocks);
}
