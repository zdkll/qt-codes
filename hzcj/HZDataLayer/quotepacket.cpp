#include "quotepacket.h"

#include <QDebug>
#include <QDateTime>
#include <string>
#include <unordered_map>

//AllStock
//StockCode
//SLStock  5 9 15 17 25 36 39 47 51
//Stock      2 5 6 7 9 10 11 12 13 15 17 20 21 22 25 36 37 39 40 41 42  44 47 49 50 51
//Block      3 5 9 13 15 17 21 22 25 28 29 30 36 39 38 41 45 47 48 49 50
//Index      5 9 10 11 12 17

//行情位类型掩码
#define  ALLSTOCK_MASK_VALUE      0xFFFFFFFFFFFFFFFF
#define  STOCKCODE_MASK_VALUE  0x00
#define  SLSTOCK_MASK_VALUE        0x0000404801014110
#define  STOCK_MASK_VALUE            0x00074BD801295F72
#define  BLOCK_MASK_VALUE            0x0003D1C839315114
#define  INDEX_MASK_VALUE             0x0000000000010F10

//StockMoney    3 4   5   6  7 8 9 10 11 12 13 59 23 37 51
//LeaderNetput  9 23 37 51

//资金位类型掩码
#define  MONEYSTOCKCODE_MASK_VALUE      0x00
#define  MONEY_MASK_VALUE                           0x404001000401FFC
#define  MONEY_NETPUT_VALUE                        0x0004001000400100

using namespace HZData;

//行情位掩码
QHash<HZData::StockFieldMask,quint64>  QuotePacket::m_stockFieldMasks = \
{
    {HZData::AllStock_Mask,ALLSTOCK_MASK_VALUE},
    {HZData::StockCode_Mask,STOCKCODE_MASK_VALUE},
    {HZData::SLStock_Mask,SLSTOCK_MASK_VALUE},
    {HZData::Stock_Mask,STOCK_MASK_VALUE},
    {HZData::Block_Mask,BLOCK_MASK_VALUE},
    {HZData::Index_Mask,INDEX_MASK_VALUE}
};

QHash<HZData::MoneyFieldMask,UINT64> QuotePacket::m_moneyFieldMasks = \
{
    {HZData::MoneyStockCode_Mask,MONEYSTOCKCODE_MASK_VALUE},
    {HZData::MoneyField_Mask,MONEY_MASK_VALUE},
    {HZData::MoneyNetput_Mask,MONEY_NETPUT_VALUE}
};

UINT64 QuotePacket::stockMaskVal(HZData::StockFieldMask mask)
{
    return m_stockFieldMasks.value(mask,0x00);
}

UINT64 QuotePacket::moneyMaskVal(HZData::MoneyFieldMask mask)
{
    return m_moneyFieldMasks.value(mask,0x00);
}

void QuotePacket::readSLStock(const HZData::AllStock &allStock,HZData::SLStock &slStock)
{
    slStock.stock_code = allStock.stock_code;
    slStock.stock_name= allStock.stock_name;
    slStock.last_price     = allStock.last_price;
    slStock.rate             = allStock.rate;
    slStock.pre_close_price = allStock.pre_close_price;

    slStock.up_down_price  =allStock.up_down_price;
    slStock.turnover_rate     =allStock.turnover_rate;
    slStock.turnover             =allStock.turnover;

    slStock.speed_rate        = allStock.speed_rate;
    slStock.five_rate            = allStock.five_rate;
    slStock.year_rate           = allStock.year_rate;
}

void QuotePacket::readStock(const HZData::AllStock &allStock,HZData::Stock &stock)
{
    stock.stock_code = allStock.stock_code;
    stock.stock_name= allStock.stock_name;
    stock.last_price     = allStock.last_price;
    stock.up_down_price=allStock.up_down_price;
    stock.rate             = allStock.rate;

    stock.volume = allStock.volume;
    //stock.HZData::leader_money_netput =
    stock.speed_rate = allStock.speed_rate;
    stock.turnover_rate = allStock.turnover_rate;
    stock.turnover = allStock.turnover;
    stock.block_name = allStock.block_name;

    stock.highest_price = allStock.highest_price;
    stock.lowest_price = allStock.lowest_price;
    stock.pre_close_price = allStock.pre_close_price;
    stock.open_price = allStock.open_price;
    stock.buy_price = allStock.stk_buy[0].price;
    stock.sell_price = allStock.stk_sell[0].price;

    stock.sa = allStock.sa;
    stock.ratio = allStock.ratio;
    stock.committee = allStock.committee;

    stock.pbv = allStock.pbv;
    stock.ped = allStock.ped;

    stock.cir_stock = allStock.cir_stock;
    stock.cir_value = allStock.cir_value;
    stock.tot_stock = allStock.tot_stock;
    stock.tot_value = allStock.tot_value;

    stock.three_rate = allStock.three_rate;
    stock.five_rate = allStock.five_rate;
    stock.three_tuv_rate = allStock.three_tuv_rate;
    stock.five_tuv_rate = allStock.five_tuv_rate;
    stock.year_rate = allStock.year_rate;
}

void QuotePacket::readBlock(const HZData::AllStock &allStock,HZData::Block &block)
{
    block.stock_code = allStock.stock_code;
    block.stock_name = allStock.stock_name;

    block.rate = allStock.rate;
    block.speed_rate = allStock.speed_rate;

    block.risest_stock = allStock.rise_stock;
    block.risest_code  = allStock.rise_code;
    block.risest_stock_rate = allStock.rise_stock_rate;

    block.rise_day_num = allStock.rise_day_num;
    block.rise = allStock.rise;
    block.fall  = allStock.fall;
    block.flat = allStock.flat;

    block.three_rate = allStock.three_rate;
    block.five_rate = allStock.five_rate;
    block.year_rate = allStock.year_rate;

    block.volume = allStock.volume;
    block.turnover = allStock.turnover;
    block.cir_value = allStock.cir_value;
    block.tot_value = allStock.tot_value;

    block.limit_up = allStock.limit_up;
    block.update_time = allStock.update_time;
}

void QuotePacket::readIndex(const HZData::AllStock &allStock,HZData::Index &index)
{
    index.stock_code = allStock.stock_code;
    index.stock_name = allStock.stock_name;

    index.rate = allStock.rate;
    index.last_price = allStock.last_price;
    index.up_down_price = allStock.up_down_price;

    index.turnover = allStock.turnover;
    index.highest_price = allStock.highest_price;
    index.lowest_price = allStock.lowest_price;
    index.pre_close_price = allStock.pre_close_price;
    index.open_price = allStock.open_price;
}

void QuotePacket::readQuoteStockCode(const ::QuoteProto::StockMsg &msg,HZData::StockCode &stockCode)
{
    stockCode.stock_code  = msg.stock_code();
    stockCode.stock_name = msg.stock_name();
}

void QuotePacket::readQuoteSLStock(const ::QuoteProto::StockMsg &msg,HZData::SLStock &slStock)
{
    slStock.stock_code  = msg.stock_code();
    slStock.stock_name = msg.stock_name();

    QuoteProto::StockBase stk =  std::move(msg.stk_base());

    bool clear_sig   = stk.clear_sig(); //是否清盘
    int trade_status = stk.trade_status();//交易状态
    if(stk.has_pre_close_price())
        slStock.pre_close_price = stk.pre_close_price()/1000.f;
    //停牌或者清盘不赋值字段
    if(clear_sig || trade_status == DealStatusP) return;
    if(stk.has_last_pice() && stk.last_pice()>0){
        //qDebug()<<QString::fromStdString( slStock.stock_name)<<"last price:"<<stk.last_pice()<<"pre_close_price:"<<stk.pre_close_price();
        slStock.last_price = stk.last_pice()/1000.f;
        slStock.up_down_price = slStock.last_price - slStock.pre_close_price;
        slStock.rate  = slStock.up_down_price/slStock.pre_close_price*100;
    }
    slStock.turnover_rate = stk.turnover_rate()*100;
    slStock.turnover = stk.turnover()/1000.f;
    slStock.five_rate = stk.five_rate();
    slStock.speed_rate = stk.speed_rate()*100;
}

void QuotePacket::readQuoteStock(const ::QuoteProto::StockMsg &msg,HZData::Stock &stock)
{
    stock.stock_code = msg.stock_code();
    stock.stock_name = msg.stock_name();

    stock.tot_value = msg.stk_base().tot_value();
}

void QuotePacket::readQuoteBlock(const ::QuoteProto::StockMsg &msg,HZData::Block &block)
{
    block.stock_code  = msg.stock_code();
    block.stock_name = msg.stock_name();
    //领涨股
    if(msg.has_risest_stock()){
        block.risest_stock =  msg.risest_stock().stock_name();
        block.risest_code  = msg.risest_stock().stock_code();
        if(msg.risest_stock().last_pice()>0){
            float rlast_price =  msg.risest_stock().last_pice()/1000.f;
            float rpre_close_price = msg.risest_stock().pre_close_price()/1000.f;
            block.risest_stock_rate = (rlast_price - rpre_close_price)/rpre_close_price*100;
        }
    }
    QuoteProto::StockBase stk =  std::move(msg.stk_base());

    block.rise_day_num = stk.rise_day_num();
    block.rise = stk.rise();
    block.flat = stk.flat();
    block.fall = stk.fall();

    if(stk.has_three_rate())
        block.three_rate = stk.three_rate()*100;
    if(stk.has_five_rate())
        block.five_rate = stk.five_rate()*100;
    if(stk.has_pre_close_price()){
        block.pre_close_price = stk.pre_close_price()/1000.f;
    }
    if(stk.last_pice() > 0){
        block.last_price = stk.last_pice()/1000.f;
        if(stk.has_rate())
            block.rate = stk.rate()*100;
        else
            block.rate = (block.last_price -block.pre_close_price)/block.pre_close_price * 100;
    }

    block.cir_value = stk.cir_value();
    if(stk.has_tot_stock())
        block.tot_value = stk.tot_value();


    //停牌或者清盘不赋值字段-板块不清盘
    //if(clear_sig || trade_status == DealStatusP) return;
    block.speed_rate = stk.speed_rate()*100;
    block.turnover = stk.turnover()/1000.f;

    block.turnover_rate = stk.turnover_rate()*100;
    if(stk.has_three_tuv_rate())
        block.three_tuv_rate = stk.three_tuv_rate()*100;

    block.volume = stk.volume();
    block.limit_up = stk.limit_up();
    block.update_time = stk.update_time();
}

void QuotePacket::readQuoteIndex(const ::QuoteProto::StockMsg &msg,HZData::Index &index)
{

}


void QuotePacket::readStockMoney(const HZData::StockMoney &allStockMoney, HZData::StockMoney &stockMoney)
{
    stockMoney.stock_code = allStockMoney.stock_code;
    stockMoney.stock_name = allStockMoney.stock_name;
    stockMoney.money_one_day = allStockMoney.money_one_day;
}

void QuotePacket::readMoneyNetput(const HZData::StockMoney &allStockMoney, HZData::MoneyNetput &moneyNetput)
{
    moneyNetput.stock_code = allStockMoney.stock_code;
    moneyNetput.stock_name = allStockMoney.stock_name;
    moneyNetput.one_money_netput = allStockMoney.one_money_netput;
    moneyNetput.three_money_netput = allStockMoney.three_money_netput;
    moneyNetput.five_money_netput = allStockMoney.five_money_netput;
    moneyNetput.ten_money_netput = allStockMoney.ten_money_netput;
}

void QuotePacket::readMmp(HZData::StockMmp *buy,HZData::StockMmp *sell,const QuoteProto::StockMmp stockMmp)
{
    buy[0].price =stockMmp.buy_price1()/1000.f;
    buy[0].volume =stockMmp.buy_volume1();

    buy[1].price =stockMmp.buy_price2()/1000.f;
    buy[1].volume =stockMmp.buy_volume2();

    buy[2].price =stockMmp.buy_price3()/1000.f;
    buy[2].volume =stockMmp.buy_volume3();

    buy[3].price =stockMmp.buy_price4()/1000.f;
    buy[3].volume =stockMmp.buy_volume4();

    buy[4].price =stockMmp.buy_price5()/1000.f;
    buy[4].volume =stockMmp.buy_volume5();

    //
    sell[0].price =stockMmp.sell_price1()/1000.f;
    sell[0].volume =stockMmp.sell_volume1();

    sell[1].price =stockMmp.sell_price2()/1000.f;
    sell[1].volume =stockMmp.sell_volume2();

    sell[2].price =stockMmp.sell_price3()/1000.f;
    sell[2].volume =stockMmp.sell_volume3();

    sell[3].price =stockMmp.sell_price4()/1000.f;
    sell[3].volume =stockMmp.sell_volume4();

    sell[4].price =stockMmp.sell_price5()/1000.f;
    sell[4].volume =stockMmp.sell_volume5();
}

void QuotePacket::readKline(HZData::HZKline &kline,const QuoteProto::KLine &kline_info)
{
    kline.trading_day = kline_info.trading_day();
    kline.update_time = kline_info.update_time();

    kline.high_price = kline_info.high_price()/1000.f;
    kline.upsert_time= kline_info.upsert_time();
    kline.open_price = kline_info.open_price()/1000.f;
    kline.low_price = kline_info.low_price()/1000.f;
    kline.close_price = kline_info.close_price()/1000.f;

    kline.volume = kline_info.volume()/100;
    kline.turnover = kline_info.turnover()/1000.f;
    kline.pre_close_price = kline_info.pre_close_price()/1000.f;
    kline.avg_price = kline_info.avg_price()/1000.f;
}

void QuotePacket::readBidKline(HZData::HZKline &kline,const QuoteProto::BidClientMsg &bidClientMsg)
{
    if(bidClientMsg.has_base_info())
        kline.avg_price = bidClientMsg.base_info().bidding_price()/1000.f;
    kline.open_price = bidClientMsg.rate(); //暂时收盘价代替涨幅
    kline.close_price        =  bidClientMsg.last_pice()/1000.f;//现价
    kline.pre_close_price = bidClientMsg.pre_close_price()/1000.f;
    kline.upsert_time       = bidClientMsg.upsert_time();

    //BidMsg
    QuoteProto::BidMsg bmsg = bidClientMsg.base_info();
    kline.update_time = bmsg.update_time();
    if(bmsg.has_bidding_volume1())
        kline.volume = bmsg.bidding_volume1();
    if(bmsg.has_bidding_turnover())
        kline.turnover       = bmsg.bidding_turnover()/1000.f;
}

void QuotePacket::readTick(HZData::HZTick &tick,const ::QuoteProto::Zhubi& zhubi)
{
    tick.trading_day = zhubi.trading_day();
    tick.price = zhubi.price()/1000.f;
    tick.volume = zhubi.volume();
    tick.access_way = zhubi.access_way();
    tick.turnover = zhubi.turnover()/1000.f;
    tick.update_time = zhubi.update_time();
}

void QuotePacket::readAllStock(HZData::AllStock &stock,const QuoteProto::StockMsg &stkMsg)
{
    stock.stock_code  = stkMsg.stock_code();
    stock.stock_name = stkMsg.stock_name();
    if(stkMsg.has_is_block())
        stock.is_block = stkMsg.is_block();

    //领涨股====================
    if(stkMsg.has_risest_stock()){
        QuoteProto::RisestStockInfo riseStockInfo = stkMsg.risest_stock();
        stock.rise_stock = riseStockInfo.stock_name();
        stock.rise_code = riseStockInfo.stock_code();
        if(riseStockInfo.last_pice()>0){
            float last_price = riseStockInfo.last_pice()/1000.f;
            float pre_close_price = riseStockInfo.pre_close_price()/1000.f;
            stock.rise_stock_rate = (last_price - pre_close_price)/pre_close_price;
        }
    }
    //基础行情
    QuoteProto::StockBase stk =  stkMsg.stk_base();
    stock.clear_sig = stk.clear_sig();
    stock.trade_status = stk.trade_status();
    //清盘部分字段===============
    if(stk.has_block_name())
        stock.block_name = stk.block_name();
    if(stk.has_pre_close_price())
        stock.pre_close_price = stk.pre_close_price()/1000.f;
    if(stk.has_cir_stock())
        stock.cir_stock = stk.cir_stock();
    if(stk.has_tot_stock())
        stock.tot_stock = stk.tot_stock();
    if(stk.has_pbv())
        stock.pbv        = stk.pbv();
    if(stk.has_pes())
        stock.pes = stk.pes();
    if(stk.has_ped())
        stock.ped = stk.ped();
    if(stk.has_ro())
        stock.ro = stk.ro();
    if(stk.has_roe())
        stock.roe = stk.roe();
    if(stk.has_profit())
        stock.profit = stk.profit();

    if(stk.clear_sig() || stk.trade_status() == DealStatusP){
        stock.cir_value = stk.cir_stock()*stock.pre_close_price;
        stock.tot_value =  stk.tot_stock()*stock.pre_close_price;
        return;
    }

    //读取买卖五档===================
    if(stkMsg.has_stk_mmp()){
        readMmp(stock.stk_buy,stock.stk_sell, stkMsg.stk_mmp());
    }

    //现价-涨跌-涨跌幅*100
    if(stk.has_last_pice() && stk.last_pice()>0){
        stock.last_price = stk.last_pice()/1000.f;
        stock.up_down_price = stock.last_price - stock.pre_close_price;
        stock.rate  = stock.up_down_price/stock.pre_close_price*100;

        stock.cir_value = stk.cir_stock()*stock.last_price;
        stock.tot_value   =  stk.tot_stock()*stock.last_price;
    }
    if(stk.has_committee())
        stock.committee = stk.committee();
    if(stk.has_ratio())
        stock.ratio  = stk.ratio();
    if(stk.has_trading_day())
        stock.trading_day   = stk.trading_day();
    if(stk.has_open_price())
        stock.open_price        = stk.open_price()/1000.f;
    if(stk.has_highest_price())
        stock.highest_price    = stk.highest_price()/1000.f;
    if(stk.has_lowest_price())
        stock.lowest_price      = stk.lowest_price()/1000.f;
    //振幅*100=(最高价-最低价)/昨收价
    if(stk.has_highest_price() && stk.has_lowest_price())
        stock.sa = (stock.highest_price - stock.lowest_price)/stock.pre_close_price*100;
    if(stk.has_volume())
        stock.volume = stk.volume();
    if(stk.has_cur_volume())
        stock.cur_volume = stk.cur_volume();
    if(stk.has_turnover_rate())
        stock.turnover_rate = stk.turnover_rate()*100;
    if(stk.has_turnover())
        stock.turnover   = stk.turnover()/1000.f;

    if(stk.has_avg_price())
        stock.avg_price = stk.avg_price()/1000.f;
    if(stk.has_update_time())
        stock.update_time = stk.update_time();
    if(stk.has_np_volume())
        stock.np_volume = stk.np_volume();
    if(stk.has_wp_volume())
        stock.wp_volume = stk.wp_volume();
    if(stk.has_np_volume() && stk.has_wp_volume())
        stock.nwp_rate = double(stock.np_volume)/double(stock.wp_volume);
    if(stk.has_avg_stock())
        stock.avg_stock  = stk.avg_stock();
    if(stk.has_rise())
        stock.rise = stk.rise();
    if(stk.has_fall())
        stock.fall = stk.fall();
    if(stk.has_flat())
        stock.flat = stk.flat();
    if(stk.has_upper_limit_rice())
        stock.upper_limit_rice = stk.upper_limit_rice()/1000.f;
    if(stk.has_down_limit_price())
        stock.down_limit_price = stk.down_limit_price()/1000.f;
    if(stk.has_buy_sell())
        stock.buy_sell = stk.buy_sell();

    //涨速*100
    if(stk.has_speed_rate())
        stock.speed_rate = stk.speed_rate()*100;
    //3日涨幅*100
    if(stk.has_three_rate())
        stock.three_rate = stk.three_rate()*100;
    if(stk.has_five_rate())
        stock.five_rate = stk.five_rate()*100;
    if(stk.has_three_tuv_rate())
        stock.three_tuv_rate = stk.three_tuv_rate()*100;
    if(stk.has_five_tuv_rate())
        stock.five_tuv_rate = stk.five_tuv_rate()*100;
    if(stk.has_rise_day_num())
        stock.rise_day_num = stk.rise_day_num();
    if(stk.has_limit_up())
        stock.limit_up = stk.limit_up();
    //    if(stk.has_year_rate()){
    //        stock.year_rate = stk.year_rate()*100;
    //    }
}

void QuotePacket::readStockMoney(HZData::StockMoney &allStockMoney, const QuoteProto::MoneyMsg &moneyMsg)
{
    allStockMoney.stock_code  = moneyMsg.stock_code();
    allStockMoney.stock_name = moneyMsg.stock_name();
    //读取当日资金
    if(moneyMsg.has_money_one_day())
        QuotePacket::readStockMoneyField(allStockMoney.money_one_day, moneyMsg.money_one_day());

    //读取净流入...
    if(moneyMsg.has_money_one_day()){
        if(moneyMsg.money_one_day().has_leader_money())//{
            allStockMoney.one_money_netput = moneyMsg.money_one_day().leader_money().money_netput()/1000;
    }

    if(moneyMsg.has_money_three_day())
        if(moneyMsg.money_three_day().has_leader_money())
            allStockMoney.three_money_netput = moneyMsg.money_three_day().leader_money().money_netput()/1000;

    if(moneyMsg.has_money_five_day())
        if(moneyMsg.money_five_day().has_leader_money())
            allStockMoney.five_money_netput = moneyMsg.money_five_day().leader_money().money_netput()/1000;

    if(moneyMsg.has_money_ten_day())
        if(moneyMsg.money_ten_day().has_leader_money())
            allStockMoney.ten_money_netput = moneyMsg.money_ten_day().leader_money().money_netput()/1000;
}

void QuotePacket::readStockMoneyField(HZData::MoneyField &moneyFeild, const QuoteProto::MoneyFeild &moneyFeild_in)
{
    if(moneyFeild_in.has_trading_day())
        moneyFeild.trading_day = moneyFeild_in.trading_day();
    if(moneyFeild_in.has_last_price())
        moneyFeild.last_price = moneyFeild_in.last_price()/1000.f;

    if(moneyFeild.last_price==0) return;

    if(moneyFeild_in.has_rate())
        moneyFeild.rate = moneyFeild_in.rate()*100;//涨幅*100
    if(moneyFeild_in.has_auction_volume())
        moneyFeild.auction_volume = moneyFeild_in.auction_volume();
    if(moneyFeild_in.has_up_down_price())
        moneyFeild.up_down_price =  moneyFeild_in.up_down_price()/1000.f;

    //读取资金单
    if(moneyFeild_in.has_leader_money())
        readOrdField(moneyFeild.leader_money,moneyFeild_in.leader_money());
    if(moneyFeild_in.has_active_money())
        readOrdField(moneyFeild.active_money,moneyFeild_in.active_money());
    if(moneyFeild_in.has_maxord_money())
        readOrdField(moneyFeild.maxord_money,moneyFeild_in.maxord_money());
    if(moneyFeild_in.has_bigord_money())
        readOrdField(moneyFeild.bigord_money,moneyFeild_in.bigord_money());
    if(moneyFeild_in.has_midord_money())
        readOrdField(moneyFeild.midord_money,moneyFeild_in.midord_money());
    if(moneyFeild_in.has_minord_money())
        readOrdField(moneyFeild.minord_money,moneyFeild_in.minord_money());
}

void QuotePacket::readOrdField(HZData::OrdField &ordFeild, const QuoteProto::OrdFeild &ordFeild_in)
{
    ordFeild.money_input   = ordFeild_in.money_input()/1000;
    ordFeild.money_netput = ordFeild_in.money_netput()/1000;
    ordFeild.money_output = ordFeild_in.money_output()/1000;
    ordFeild.money_rate     = ordFeild_in.money_rate()*100;
}

void QuotePacket::readRiseFallInfo(HZData::StockRiseFallInfo &riseFallInfo,const QuoteProto::StockRiseFallInfo &info)
{
    riseFallInfo.block_name = info.block_name();
    riseFallInfo.block_code  = info.block_code();
    riseFallInfo.greater_seven = info.greater_seven();
    riseFallInfo.up_five_seven = info.up_five_seven();
    riseFallInfo.up_two_five = info.up_two_five();
    riseFallInfo.up_zero_two = info.up_zero_two();
    riseFallInfo.down_zero_two = info.down_zero_two();
    riseFallInfo.down_two_five = info.down_two_five();
    riseFallInfo.down_five_seven = info.down_five_seven();
    riseFallInfo.less_seven = info.less_seven();
    riseFallInfo.limit_up_num = info.limit_up_num();
    riseFallInfo.up_num = info.up_num();
    riseFallInfo.limit_down_num = info.limit_down_num();
    riseFallInfo.down_num = info.down_num();
    riseFallInfo.flat_num = info.flat_num();
    riseFallInfo.stop_num = info.stop_num();
}

void QuotePacket::readBlockAbnormal(HZData::BlockAbnormal &blockAbnormal,const QuoteProto::BlockFundAbnormalItem &item)
{
    int size = item.stk_size() +1;
    blockAbnormal.resize(size) ;

    readBlockAbnormalItem(blockAbnormal[0],  item.block_info(),true);
    for(int i=0;i<item.stk_size();i++)
        readBlockAbnormalItem(blockAbnormal[i+1],  item.stk(i),false);
}

void QuotePacket::readBlockAbnormalItem(HZData::BlockAbnormalItem &item,const QuoteProto::StockMsg &stkMsg,bool isBlock)
{
    QuoteProto::StockBase stk =  stkMsg.stk_base();
    item.stock_code  = stkMsg.stock_code();
    item.stock_name = stkMsg.stock_name();
    item.update_time = stk.update_time();
    if(isBlock)
    {
        item.rate = stk.rate();
    }
    else if(stk.last_pice() > 0){
        float last_price         = stk.last_pice()/1000.f;
        float pre_close_price= stk.pre_close_price()/1000.f;
        item.up_down_price = last_price -pre_close_price;
        item.rate = item.up_down_price/pre_close_price;
    }
}

void QuotePacket::readShortMotion(HZData::ShortMotion &shotMotion,const ::QuoteProto::ShortMotionMsg&  msg)
{
    shotMotion.stock_code = msg.stock_code();
    shotMotion.stock_name = msg.stock_name();
    shotMotion.short_type = (HZData::ShortMotionType)msg.short_type();
    shotMotion.index         = msg.index();
    shotMotion.update_time_sec = msg.update_time_sec();

    if(msg.has_detail_vol())
        shotMotion.detail_vol = msg.detail_vol();
    if(msg.has_rate())
        shotMotion.rate = msg.rate();
    if(msg.has_last_price())
        shotMotion.last_price = msg.last_price()/1000.f;
    if(msg.has_buy_volume())
        shotMotion.buy_volume = msg.buy_volume();
    if(msg.has_buy_volume1())
        shotMotion.buy_volume1 = msg.buy_volume1();
    if(msg.has_sell_volume())
        shotMotion.sell_volume = msg.sell_volume();
    if(msg.has_sell_volume1())
        shotMotion.sell_volume1 = msg.sell_volume1();
}

void QuotePacket::readStockBidding(HZData::StockBid &stockBid,const QuoteProto::BidClientMsg &bidClientMsg)
{
    QuoteProto::BidMsg bidMsg = bidClientMsg.base_info();

    stockBid.stock_code = bidMsg.stock_code();
    stockBid.stock_name= bidMsg.stock_name();
    stockBid.trading_day = bidMsg.trading_day();
    stockBid.plate = bidMsg.plate();
    if(bidMsg.has_trade_status())
        stockBid.trade_status = bidMsg.trade_status();
    if(bidMsg.has_yesterday_rate())
        stockBid.yesterday_rate = bidMsg.yesterday_rate()*100;

    //根据现价清盘
    if(!bidClientMsg.has_last_pice() || bidClientMsg.last_pice() == 0) return;

    stockBid.last_price = bidClientMsg.last_pice()/1000.f;
    stockBid.rate = bidClientMsg.rate()*100;
    stockBid.pre_close_price = bidClientMsg.pre_close_price()/1000.f;

    if(bidMsg.has_bidding_price())
        stockBid.bidding_price = bidMsg.bidding_price()/1000.f;
    if(bidMsg.has_bidding_rise())
        stockBid.bidding_rise = bidMsg.bidding_rise()*100;

    stockBid.upsert_time = bidClientMsg.upsert_time();

    if(bidMsg.has_main_funds())
        stockBid.main_funds = bidMsg.main_funds();
    if(bidMsg.has_bidding_quantity_than())
        stockBid.bidding_quantity_than = bidMsg.bidding_quantity_than();
    if(bidMsg.has_bidding_volume())
        stockBid.bidding_volume = bidMsg.bidding_volume();
    if(bidMsg.has_bidding_turnover())
        stockBid.bidding_turnover = bidMsg.bidding_turnover()/1000.f;
    if(bidMsg.has_bidding_volume1())
        stockBid.bidding_volume1 = bidMsg.bidding_volume1();
    if(bidMsg.has_current_value())
        stockBid.current_value = bidMsg.current_value()/1000.f;
    if(bidMsg.has_update_time())
        stockBid.update_time = bidMsg.update_time();
}

void QuotePacket::readBlockBidding(HZData::BlockBid &blockBid,const QuoteProto::BidBlockMsg& bidBlockMsg)
{
    blockBid.block_code = bidBlockMsg.block_code();
    blockBid.block_name= bidBlockMsg.block_name();
    //qDebug()<<<<QString::fromStdString(bidBlockMsg.block_name());
    if(bidBlockMsg.has_rate())
        blockBid.rate = bidBlockMsg.rate()*100;

    blockBid.update_time_sec = bidBlockMsg.update_time_sec();
}

void QuotePacket::readFundAbnormal(HZData::FundAbnormal &fundAbnormal,const QuoteProto::FundAbnormalMsg&msg)
{
    fundAbnormal.stock_code = msg.stock_code();
    fundAbnormal.stock_name = msg.stock_name();
    fundAbnormal.stock_type = msg.stock_code();
    if(msg.has_block_name())
        fundAbnormal.block_name = msg.block_name();
    if(msg.has_last_pice() && msg.last_pice()>0){
        fundAbnormal.last_price = msg.last_pice()/1000.f;
        fundAbnormal.rate = msg.rate()*100;
        if(msg.has_thirty_rate())
            fundAbnormal.thirty_rate = msg.thirty_rate()*100;
        if(msg.has_active_netput())
            fundAbnormal.active_netput = msg.active_netput()/1000;

        if(msg.has_thirty_active_netput())
            fundAbnormal.thirty_active_netput = msg.thirty_active_netput()/1000;
        if(msg.has_leader_netput())
            fundAbnormal.leader_netput = msg.leader_netput()/1000;

        if(msg.has_avtive_rate())
            fundAbnormal.avtive_rate = msg.avtive_rate()*100;
        if(msg.has_leader_rate())
            fundAbnormal.leader_rate = msg.leader_rate()*100;
        if(msg.has_turnover_rate())
            fundAbnormal.turnover_rate = msg.turnover_rate()*100;
        if(msg.has_ratio())
            fundAbnormal.ratio = msg.ratio();
        if(msg.has_three_avtive_netput())
            fundAbnormal.three_avtive_netput = msg.three_avtive_netput()/1000;
        if(msg.has_three_rate()){
            fundAbnormal.three_rate = msg.three_rate()*100;
        }
        if(msg.has_five_avtive_netput())
            fundAbnormal.five_avtive_netput = msg.five_avtive_netput()/1000;
        if(msg.has_five_rate()){
            fundAbnormal.five_rate = msg.five_rate()*100;
        }
    }
    if(msg.has_cir_value())
        fundAbnormal.cir_value = msg.cir_value();
    if(msg.has_ped())
        fundAbnormal.ped = msg.ped();
}

void QuotePacket::readRfmNumInfo(HZData::RiseFallMonitorNumInfo &rfmNumInfo,const QuoteProto::RiseFallMonitorNumMsg& msg )
{
    rfmNumInfo.real_time_num = msg.real_time_num();

    //qDebug()<<"readRfmNumInfo:"<<msg.real_time_num();
    rfmNumInfo.natural_num = msg.natural_num();
    rfmNumInfo.continuous_num = msg.continuous_num();
    rfmNumInfo.aword_plate = msg.aword_plate();
    rfmNumInfo.sealing_plate_rate = msg.sealing_plate_rate();
    rfmNumInfo.fried_plate = msg.fried_plate();
    rfmNumInfo.yeaterday_limit_rate = msg.yeaterday_limit_rate();
    rfmNumInfo.yeaterday_high_open_rate = msg.yeaterday_high_open_rate();
    rfmNumInfo.yeaterday_profit_rate = msg.yeaterday_profit_rate()/1000.f;
    rfmNumInfo.yeaterday_continuous_limit_rate = msg.yeaterday_continuous_limit_rate();
    rfmNumInfo.yeaterday_continuous_high_open_rate = msg.yeaterday_continuous_high_open_rate();
    rfmNumInfo.yeaterday_continuous_profit_rate = msg.yeaterday_continuous_profit_rate()/1000.f;
    rfmNumInfo.update_time_sec = msg.update_time_sec();
    rfmNumInfo.trading_day = msg.trading_day();
}

void QuotePacket::readRFMonitorInfo(HZData::RiseFallData &riseFallData,const QuoteProto::RateMsg& msg)
{
    riseFallData.message_type = msg.message_type();
    riseFallData.stock_code = msg.security_code();
    riseFallData.stock_type = msg.security_type();
    riseFallData.stock_name = msg.security_name();
    riseFallData.message_type = msg.message_type();
    //    riseFallData.last_price  = msg
    riseFallData.rise = msg.rise()*100;
    riseFallData.plate  = subString(msg.plate(),",",3);  //最多取前三个

    riseFallData.up_limit_rate_time = msg.up_limit_rate_time()+59;//此字段特殊处理，往后一分钟
    //封板时间 15::00 之后的显示15:00
    QDateTime dateTime = QDateTime::fromTime_t(msg.up_limit_rate_time());
    if(dateTime.time()>QTime(15,0,0)){
        dateTime.setTime(QTime(15,0,0));
        riseFallData.up_limit_rate_time = dateTime.toTime_t();
    }

    riseFallData.continuous_up_limit_rate = (msg.continuous_up_limit_rate() == "NULL")?Default_Item_String:msg.continuous_up_limit_rate();
    riseFallData.continuous_lower_limit_rate = (msg.continuous_lower_limit_rate() == "NULL")?Default_Item_String:msg.continuous_lower_limit_rate();

    riseFallData.up_limit_volume = msg.up_limit_volume();
    riseFallData.lower_limit_volume = msg.lower_limit_volume();
    riseFallData.lower_limit_rate_time = msg.lower_limit_rate_time();
    riseFallData.fried_time = msg.fried_time();
    riseFallData.fried_num = msg.fried_num();
    riseFallData.main_funds = msg.main_funds()/1000;
    //qDebug()<<"rfm main funds:"<<msg.main_funds();
    riseFallData.to_up_limit_time = msg.to_up_limit_time();
    riseFallData.to_lower_limit_time = msg.to_lower_limit_time();

    riseFallData.history_continuous = (msg.history_continuous() == "NULL")?Default_Item_String:msg.history_continuous();

    riseFallData.update_time_sec = msg.update_time_sec();
    riseFallData.tradingday = msg.tradingday();
    riseFallData.last_price = msg.last_price()/1000.f;
}


std::string QuotePacket::subString(const std::string &str,const std::string &split,int count)
{
    std::string str_in = str,str_out;
    int cnt = 0;
    int index = std::string::npos;
    while(cnt<count && !str_in.empty())
    {
        if((index = str_in.find_first_of(split)) != std::string::npos)
        {
            str_out += str_in.substr(0,index)+",";
            str_in     = str_in.substr(index+1);
            cnt++;
        }else{
            str_out += str_in;
            return str_out;
        }
    }
    str_out.pop_back();
    return str_out;
}

