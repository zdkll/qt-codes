#include "requestsender.h"

#include <QDebug>

#include "protobufs.h"
#include "datasimulator.h"
#include "quotepacket.h"
#include "packctrl.h"


using namespace HZData;

RequestSender::RequestSender(PackCtrl *pkctrl)
    :m_sendReady(false), m_packCtrl(pkctrl)
{
    //设置UID
    m_pack.head.unUid =  -1;

    //设置版本号
    m_pack.head.unVer   = Client_UnVersion;
    m_pack.head.unType = Client_UnType;
}

RequestSender::~RequestSender()
{
    if(m_packCtrl)
        m_packCtrl->disConnect();
}


void RequestSender::setUid(quint64 unUid)
{
    m_pack.head.unUid =  unUid;
}

void RequestSender::setSendReady(bool ready)
{
    m_sendReady = ready;
    qDebug()<<"send ready:"<<ready;

    //断开连接清空所有订阅
    if(!ready)
        m_subDynaRcrds.clear();
}

void RequestSender::sendReqBatchDyna(const  ReqId &reqId,const HZData::ReqBatchDynaInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd  = Cmd_BatchStock;
    m_pack.head.unSeq   = reqId;

    QuoteProto::BatchGetStockRequest  request;
    request.set_filter(QuoteProto::QuoteFilterFlag(req.filter));
    request.set_filter_detail(req.filter_detail);

    request.set_sort_field(QuoteProto::SortType(req.sort_field));
    request.set_is_desc(req.is_desc);
    request.set_index(req.index);
    request.set_count_range(req.count_ange);

    //拉取字段
    request.mutable_msg_mask()->set_mask(QuotePacket::stockMaskVal(req.mask));

    string  str;
    request.SerializeToString(&str);


    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqOptionalDyna(const  ReqId &reqId,const HZData::ReqOptionalDynaInfo &req)
{
    if(!m_sendReady)
        return;
    //   qDebug()<<"sendReqOptionalDyna 78============:"<<reqId;
    //1 组包
    m_pack.head.unCmd  = Cmd_OptionalStock;
    m_pack.head.unSeq = reqId;
    QuoteProto::StockOptionalRequest  request;
    foreach(string code,req.stock_code)
        request.add_stock_code(code);

    //设置使用掩码拉取
    request.mutable_msg_mask()->set_mask(QuotePacket::stockMaskVal(req.mask));

    string  str;
    request.SerializeToString(&str);

    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}


void RequestSender::sendReqBatchMoney(const  ReqId &reqId,const HZData::ReqBatchFundInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BatchMoney;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchGetMoneyRequest request;
    request.set_filter(QuoteProto::QuoteFilterFlag(req.filter));
    request.set_filter_detail(req.filter_detail);
    request.set_sort_field(QuoteProto::SortType(req.sort_field));
    request.set_is_desc(req.is_desc);
    request.set_index(req.index);
    request.set_count_range(req.count_range);

    //qDebug()<<"send batch money:"<<req.mask<<QString::number(QuotePacket::moneyMaskVal(req.mask),16);
    request.mutable_msg_mask()->set_mask(QuotePacket::moneyMaskVal(req.mask));

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqOptionalMoney(const  ReqId &reqId,const HZData::ReqOptionalFundInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_OptionalMoney;
    m_pack.head.unSeq  = reqId;

    QuoteProto::MoneyOptionalRequest request;
    foreach(string code,req.stock_code)
        request.add_stock_code(code);

    request.mutable_msg_mask()->set_mask(QuotePacket::moneyMaskVal(req.mask));

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}


void RequestSender::sendReqTick(const  ReqId &reqId,const HZData::ReqTickInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_Zhubi;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetZhubiRequest request;
    request.set_stock_code(req.stock_code);
    request.set_is_desc(req.is_desc);
    request.set_update_time(req.update_time);
    request.set_count(req.count);
    request.set_zb_type(QuoteProto::emZhubiType(req.zb_type));

    string  str;
    request.SerializeToString(&str);

    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqKline(const  ReqId &reqId,const HZData::ReqKlineInfo &req)
{
    if(!m_sendReady)
        return;
    //1 组包
    m_pack.head.unCmd = Cmd_Kline;
    m_pack.head.unSeq = reqId;
    QuoteProto::GetKLineRequest request;
    request.set_stock_name(req.stock_name);
    request.set_count(req.count);
    request.set_is_desc(req.is_desc);
    request.set_kline_type(req.kline_type);
    request.set_start_time(req.start_time);

    string  str;
    request.SerializeToString(&str);

    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqTimeKline(const  ReqId &reqId,const HZData::ReqTimeKlineInfo &req)
{
    // qDebug()<<"send req time divide"<<reqId;
    // DataSimulater::instance()->reqTimeDivide(reqId,req);
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_TimeKline;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetTimeKLineRequest request;
    request.set_stock_name(req.stock_name);
    request.set_start_time(req.start_time);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqBidKline(const  ReqId &reqId,const HZData::ReqBidKlineInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BiddingKline;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetBiddingQuoteKlineRequest request;
    request.set_stock_code(req);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}


void RequestSender::sendReqQueryCode(const ReqId &reqId,const HZData::ReqQueryCode &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_Query_Code;
    m_pack.head.unSeq = reqId;

    QuoteProto::QueryCodeRequest request;
    request.set_type(QuoteProto::SearchType(req.search_type));
    request.set_search_word(req.search_word);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqStockRiseFallInfo(ReqId reqId,const HZData::ReqStockRiseFallInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_RiseFallInfo;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetStockRiseFallInfoRequest request;
    //目前只有一种
    request.set_stock_code(req);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqBlockAbnormalInfo(ReqId reqId,const HZData::ReqBlockAbnormalInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BlockAbnormal;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BlockMoneyAbnoramlRequest request;
    //目前只有一种
    request.set_block_code(req.block_code);
    request.set_update_time(req.update_time);
    request.set_count_range(req.count_range);
    request.set_is_desc(req.is_desc);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqBatchFundAbnormal(ReqId reqId,const HZData::ReqBatchFundAbnormal &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BatchFundAbnormal;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchGetMoneyAbnormalRequest request;

    request.set_type(QuoteProto::FundAbnormalType(req.type));

    request.set_sort_field(QuoteProto::FASortType(req.sort_field));
    request.set_is_desc(req.is_desc);
    request.set_index(req.index);
    request.set_count_range(req.count_range);
    request.mutable_msg_mask()->set_mask(req.mask);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqOptionalFundAbnormal(ReqId reqId,const HZData::ReqOptionalFundAbnormal &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_OptionalFundAbnormal;
    m_pack.head.unSeq  = reqId;

    QuoteProto::MoneyOptionalRequest request;
    for(std::string stockcode:req.stock_code)
        request.add_stock_code(stockcode);

    //    request.mutable_msg_mask()->set_mask(req.mask);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqBatchBiddingBlock(ReqId reqId,const HZData::ReqBatchBiddingBlock &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BatchBiddingBlock;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchGetBiddingBlockRequest request;
    request.set_sort_field(QuoteProto::BiddingBlockSortType(req.sort_field));
    request.set_is_desc(req.is_desc);
    request.set_index(req.index);
    request.set_count_range(req.count_range);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqOptionalBiddingBlock(ReqId reqId,const HZData::ReqOptionalBiddingBlock &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_OptionalBiddingBlock;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BiddingBlockOptionalRequest request;
    for(std::string stockCode:req)
        request.add_stock_code(stockCode);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}
//个股集合竞价
void RequestSender::sendReqBatchBidding(ReqId reqId,const HZData::ReqBatchBidding &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_BatchBidding;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchGetBiddingRequest  request;

    request.set_sort_field(QuoteProto::BiddingSortType(req.sort_field));
    request.set_is_desc(req.is_desc);
    request.set_index(req.index);
    request.set_count_range(req.count_range);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqOptionalBidding(ReqId reqId,const HZData::ReqOptionalBidding &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_OptionalBidding;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BiddingOptionalRequest request;
    for(std::string stockCode:req)
        request.add_stock_code(stockCode);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}


void RequestSender::sendReqBatchRFMonitor(ReqId reqId,const HZData::ReqBatchRfmInfo& req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_RFMonitorInfo;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchRFMonitorRequest request;
    request.set_m_type(QuoteProto::RFMonitorType(req.m_type));
    request.set_sort_type(QuoteProto::RiseFallMSortType(req.sort_type));
    request.set_index(req.index);
    request.set_is_desc(req.is_desc);
    request.set_count_range(req.count_range);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqRfmNumInfo(ReqId reqId)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_RFMNumInfo;
    m_pack.head.unSeq  = reqId;

    QuoteProto::RFMonitorNumInfoOptionalRequest request;

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqShortMotionInfo(ReqId reqId,const HZData::ReqShortMotionInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_ShortMotion;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BatchShortMotionRequest request;

    request.set_short_type(QuoteProto::ShortMotionClientType(req.type));
    request.set_index(req.index);
    request.set_is_desc(req.is_desc);
    request.set_count_range(req.count_range);

    string  str;
    request.SerializeToString(&str);
    m_pack.strMsg = str;

    //2 发送
    m_packCtrl->sendData(m_pack);
}



void RequestSender::sendReqPriceTickInfo(ReqId reqId,const HZData::ReqPriceTickInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_GetFenjia;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetFenJiaRequest request;
    request.set_stock_code(req.stock_code);
    request.set_index(req.index);
    request.set_count(req.count);

    request.SerializeToString(&m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqIncrDecPosInfo(ReqId reqId,const HZData::ReqLeaderIncDecPostionInfo &req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_GetLeaderIncDecPos;
    m_pack.head.unSeq  = reqId;

    QuoteProto::MoneyFieldLineRequest request;
    request.set_stock_code(req);

    request.SerializeToString(&m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqAbnormalBlockInfo(ReqId reqId, const HZData::ReqAbnormalBlockInfo& req)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_Abnormal_Block;
    m_pack.head.unSeq  = reqId;

    QuoteProto::BlockMoneyAbnormalPCRequest request;
    //设置参数
    request.set_filter(QuoteProto::QuoteFilterFlag(req.filter));
    request.SerializeToString(&m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendZhibiaoInfo(ReqId reqId,const HZData::ReqZhibiaoInfo &req)
{
    if(!m_sendReady)
        return;
    //1 组包
    m_pack.head.unCmd = req.cmdType;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetZhibiaoRequest request;
    request.set_stock_code(req.stock_code);
    request.set_ztype((QuoteProto::ZhibiaoType)req.ztype);
    request.set_start_time(req.start_time);
    request.set_end_time(req.end_time);

    request.SerializeToString(&m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqMidAbnormalInfo(ReqId reqId,const HZData::ReqMidAbnormalInfo &req)
{
    if(!m_sendReady)
        return;
    QuoteProto::MidAbnormalRequest request;
    //1 组包
    m_pack.head.unCmd = Cmd_Mid_Abnormal;
    m_pack.head.unSeq  = reqId;

    request.set_type(req.type);
    request.set_stock_code(req.stock_code);
    request.set_index(req.index);
    request.set_count(req.count);
    request.set_direction(req.direction);
    request.SerializeToString(&m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::sendReqTime(ReqId reqId)
{
    if(!m_sendReady)
        return;

    //1 组包
    m_pack.head.unCmd = Cmd_ReqTime;
    m_pack.head.unSeq  = reqId;

    QuoteProto::GetTimeRequest request;

    request.SerializeToString(& m_pack.strMsg);

    //2 发送
    m_packCtrl->sendData(m_pack);
}

void RequestSender::subStockDyna(ReqId reqId,const HZData::ReqOptionalDynaInfo &req,int state)
{
    if(!m_sendReady)
        return;
    //1 组包
    m_pack.head.unCmd = Cmd_Publish;
    m_pack.head.unSeq  = reqId;

    QuoteProto::PublishRequest request;
    QuoteProto::PublishItem      *item;
    //添加订阅计数+1
    if(state == 1){
        foreach(string code,req.stock_code){
            //查找是否存在订阅列表
            QString  stock_code = QString::fromStdString(code);
            item = request.add_stocks();
            item->set_stock_code(code);
            item->set_ptype(QuoteProto::PublishType_stock_block_quote);

            auto it =  m_subDynaRcrds.find(stock_code);
            if(it  == m_subDynaRcrds.end())
                m_subDynaRcrds[stock_code]  = 0;
            m_subDynaRcrds[stock_code]++;
        }
    }else{
        //取消订阅,计数取消为0 的取消订阅
        foreach(string code,req.stock_code){
            //查找是否存在订阅列表
            QString  stock_code = QString::fromStdString(code);
            auto it =  m_subDynaRcrds.find(stock_code);
            if(it  != m_subDynaRcrds.end()){
                m_subDynaRcrds[stock_code]--;
                //计数为零，取消
                if( m_subDynaRcrds[stock_code] == 0){
                    m_subDynaRcrds.remove(stock_code);

                    item = request.add_stocks();
                    item->set_stock_code(code);
                    item->set_ptype(QuoteProto::PublishType_stock_block_quote);
                }
            }
        }
    }
    if(request.stocks_size()<1)
        return;

    if(state == 1){
        qDebug()<<"sub items:"<<request.stocks_size()<<"=================";
        //        for(int i=0;i<request.stocks_size();i++)
        //            qDebug()<<QString::fromStdString(request.stocks(i).stock_code());
    }
    else{
        qDebug()<<"cancel sub items:"<<request.stocks_size()<<"================";
        //        for(int i=0;i<request.stocks_size();i++)
        //            qDebug()<<QString::fromStdString(request.stocks(i).stock_code());
    }
    request.set_state(state);
    string  str;
    request.SerializeToString(&str);

    m_pack.strMsg = str;
    m_packCtrl->sendData(m_pack);
}


void RequestSender::subKline(ReqId reqId,const std::string &stock_code, INT32  klineType,int state)
{
    m_pack.head.unCmd = Cmd_Publish;
    m_pack.head.unSeq  = reqId;

    QuoteProto::PublishRequest request;
    QuoteProto::PublishItem      *item;

    item = request.add_stocks();
    item->set_stock_code(stock_code);
    item->set_kline_type(klineType);
    item->set_ptype(QuoteProto::PublishType_stock_block_kline);

    request.set_state(state);
    string  str;
    request.SerializeToString(&str);

    m_pack.strMsg = str;
    m_packCtrl->sendData(m_pack);
}

