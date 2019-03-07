#ifndef QUOTEPACKET_H
#define QUOTEPACKET_H

#include <QHash>
#include <unordered_map>

#include  "Any.hpp"
#include "datamodeldef.h"
#include "protobufs.h"

//数据和包转换
namespace QuotePacket
{
//QList<QuoteProto::PublishItem>

//股票位掩码
extern QHash<HZData::StockFieldMask,UINT64>                m_stockFieldMasks;
//资金位掩码
extern QHash<HZData::MoneyFieldMask,UINT64> m_moneyFieldMasks;

UINT64 stockMaskVal(HZData::StockFieldMask mask);
UINT64 moneyMaskVal(HZData::MoneyFieldMask mask);

//行情数据保存到对应结构体=====
void readSLStock(const HZData::AllStock &allStock,HZData::SLStock &slStock);
void readStock(const HZData::AllStock &allStock,HZData::Stock &stock);
void readBlock(const HZData::AllStock &allStock,HZData::Block &block);
void readIndex(const HZData::AllStock &allStock,HZData::Index &index);

//直接从包读取
void readQuoteStockCode(const ::QuoteProto::StockMsg &msg,HZData::StockCode &stockCode);
void readQuoteSLStock(const ::QuoteProto::StockMsg &msg,HZData::SLStock &slStock);
void readQuoteStock(const ::QuoteProto::StockMsg &msg,HZData::Stock &stock);
void readQuoteBlock(const ::QuoteProto::StockMsg &msg,HZData::Block &block);
void readQuoteIndex(const ::QuoteProto::StockMsg &msg,HZData::Index &index);

//资金数据
void readStockMoney(const HZData::StockMoney &allStockMoney, HZData::StockMoney &stockMoney);
void readMoneyNetput(const HZData::StockMoney &allStockMoney, HZData::MoneyNetput &moneyNetput);

//解析包
//买卖五档
void readMmp(HZData::StockMmp *buy,HZData::StockMmp *sell,const QuoteProto::StockMmp);
void readKline(HZData::HZKline &kline,const QuoteProto::KLine &kline_info);
void readBidKline(HZData::HZKline &kline,const QuoteProto::BidClientMsg &bidClientMsg);
void readTick(HZData::HZTick &tick,const ::QuoteProto::Zhubi& zhubi);

void readAllStock(HZData::AllStock &stock,const QuoteProto::StockMsg &response);

//读取全部资金数据
void readStockMoney(HZData::StockMoney &allStockMoney, const QuoteProto::MoneyMsg &moneyMsg);
//读取某日资金
void readStockMoneyField(HZData::MoneyField &moneyFeild, const QuoteProto::MoneyFeild &moneyFeild1);
//读取资金单
void readOrdField(HZData::OrdField &ordFeild, const QuoteProto::OrdFeild &ordFeild1);

//读取涨跌分布
void readRiseFallInfo(HZData::StockRiseFallInfo &riseFallInfo,const QuoteProto::StockRiseFallInfo &riseFall);
//读取板块异动
void readBlockAbnormal(HZData::BlockAbnormal &blockAbnormal,const QuoteProto::BlockFundAbnormalItem &item);
void readBlockAbnormalItem(HZData::BlockAbnormalItem &item,const QuoteProto::StockMsg &stkMsg,bool isBlock);
//读取短线异动
void readShortMotion(HZData::ShortMotion &shotMotion,const ::QuoteProto::ShortMotionMsg &mesg);
//读取个股集合竞价
void  readStockBidding(HZData::StockBid &stockBid,const QuoteProto::BidClientMsg& bidClientMsg);
//读取板块集合竞价
void  readBlockBidding(HZData::BlockBid &blockBid,const QuoteProto::BidBlockMsg& bidBlockMsg);

//读取资金异动
void readFundAbnormal(HZData::FundAbnormal &fundAbnormal, const QuoteProto::FundAbnormalMsg&msg);
//读取涨停监测统计信息
void readRfmNumInfo(HZData::RiseFallMonitorNumInfo &,const QuoteProto::RiseFallMonitorNumMsg& );
//读取涨停监测数据
void readRFMonitorInfo(HZData::RiseFallData &riseFallData,const QuoteProto::RateMsg& );


//fucntions-----
std::string subString(const std::string &str,const std::string &split,int count = 3);

}



#endif // QUOTEPACKET_H
