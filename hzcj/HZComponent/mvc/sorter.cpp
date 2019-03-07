#include "sorter.h"

using namespace HZData;

BaseSorter::BaseSorter()
    :m_sortField(0),is_desc(true)
{

}

void BaseSorter::setSortIndicator(SortField sortField,bool desc)
{
    m_sortField = sortField;
    is_desc = desc;
}

///SLStock------------------------
bool DynaSorter::compareSlStock(const HZData::SLStock &slStock1,const HZData::SLStock &slStock2)
{
    switch (m_sortField) {
    case last_price:
        if(is_desc)
            return slStock1.last_price>slStock2.last_price;
        else
            return slStock1.last_price<slStock2.last_price;
        break;
    case rise_fall_price:
        if(is_desc)
            return slStock1.up_down_price>slStock2.up_down_price;
        else
            return slStock1.up_down_price<slStock2.up_down_price;
        break;
    case HZData::rise_fall_rate:
        if(is_desc)
            return slStock1.rate>slStock2.rate;
        else
            return slStock1.rate<slStock2.rate;
        break;
    case turnover:
        if(is_desc)
            return slStock1.rate>slStock2.turnover;
        else
            return slStock1.rate<slStock2.turnover;
        break;
    case turnover_rate:
        if(is_desc)
            return slStock1.rate>slStock2.turnover_rate;
        else
            return slStock1.rate<slStock2.turnover_rate;
        break;
    default:
        return true;
        break;
    }
}


///Stock---------------------------
bool DynaSorter::compareStock(const HZData::Stock &stock1,const HZData::Stock &stock2)
{
    double val1,val2;
    switch (m_sortField) {
    case rise_fall_rate:
        val1= stock1.rate==INVALID_DOUBLE?0:stock1.rate;
        val2 = stock2.rate==INVALID_DOUBLE?0:stock2.rate;
        break;
    case last_price:
        val1 = stock1.last_price==INVALID_DOUBLE?0:stock1.last_price;
        val2 = stock2.last_price==INVALID_DOUBLE?0:stock2.last_price;
        break;
    case rise_fall_price:
        val1 = stock1.up_down_price==INVALID_DOUBLE?0:stock1.up_down_price;
        val2 = stock2.up_down_price==INVALID_DOUBLE?0:stock2.up_down_price;
        break;
    case volume:
        val1 = stock1.volume==INVALID_UINT64?0:stock1.volume;
        val2 = stock2.volume==INVALID_UINT64?0:stock2.volume;
        break;
    case speed_rate:
        val1 = stock1.speed_rate==INVALID_DOUBLE?0:stock1.speed_rate;
        val2 = stock2.speed_rate==INVALID_DOUBLE?0:stock2.speed_rate;
        break;
    case turnover_rate:
        val1 = stock1.turnover_rate==INVALID_DOUBLE?0:stock1.turnover_rate;
        val2 = stock2.turnover_rate==INVALID_DOUBLE?0:stock2.turnover_rate;
        break;
    case turnover:
        val1 = stock1.turnover==INVALID_DOUBLE?0:stock1.turnover;
        val2 = stock2.turnover==INVALID_DOUBLE?0:stock2.turnover;
        break;
    case highest_price:
        val1 = stock1.highest_price==INVALID_DOUBLE?0:stock1.highest_price;
        val2 = stock2.highest_price==INVALID_DOUBLE?0:stock2.highest_price;
        break;
    case lowest_price:
        val1 = stock1.lowest_price==INVALID_DOUBLE?0:stock1.lowest_price;
        val2 = stock2.lowest_price==INVALID_DOUBLE?0:stock2.lowest_price;
        break;
    case pre_close_price:
        val1 = stock1.pre_close_price==INVALID_DOUBLE?0:stock1.pre_close_price;
        val2 = stock2.pre_close_price==INVALID_DOUBLE?0:stock2.pre_close_price;
        break;
    case open_price:
        val1 = stock1.open_price==INVALID_DOUBLE?0:stock1.open_price;
        val2 = stock2.open_price==INVALID_DOUBLE?0:stock2.open_price;
        break;
    case bid_price:
        val1 = stock1.buy_price==INVALID_DOUBLE?0:stock1.buy_price;
        val2 = stock2.buy_price==INVALID_DOUBLE?0:stock2.buy_price;
        break;
    case ask_price:
        val1= stock1.sell_price==INVALID_DOUBLE?0:stock1.sell_price;
        val2 = stock2.sell_price==INVALID_DOUBLE?0:stock2.sell_price;
    case sa:
        val1 = stock1.sa==INVALID_DOUBLE?0:stock1.sa;
        val2 = stock2.sa==INVALID_DOUBLE?0:stock2.sa;
        break;
    case ratio:
        val1 = stock1.ratio==INVALID_DOUBLE?0:stock1.ratio;
        val2 = stock2.ratio==INVALID_DOUBLE?0:stock2.ratio;
        break;
    case committee:
        val1 = stock1.committee==INVALID_DOUBLE?0:stock1.committee;
        val2 = stock2.committee==INVALID_DOUBLE?0:stock2.committee;
        break;
    case pbv:
        val1 = stock1.pbv==INVALID_DOUBLE?0:stock1.pbv;
        val2 = stock2.pbv==INVALID_DOUBLE?0:stock2.pbv;
        break;
    case ped:
        val1 = stock1.ped==INVALID_DOUBLE?0:stock1.ped;
        val2 = stock2.ped==INVALID_DOUBLE?0:stock2.ped;
        break;
    case cir_stock:
        val1 = stock1.cir_stock==INVALID_INT64?0:stock1.cir_stock;
        val2 = stock2.cir_stock==INVALID_INT64?0:stock2.cir_stock;
        break;
    case cir_value:
        val1 = stock1.cir_value==INVALID_DOUBLE?0:stock1.cir_value;
        val2 = stock2.cir_value==INVALID_DOUBLE?0:stock2.cir_value;
        break;
    case tot_stock:
        val1= stock1.tot_stock==INVALID_INT64?0:stock1.tot_stock;
        val2 = stock2.tot_stock==INVALID_INT64?0:stock2.tot_stock;
        break;
    case tot_value:
        val1 = stock1.tot_value==INVALID_DOUBLE?0:stock1.tot_value;
        val2 = stock2.tot_value==INVALID_DOUBLE?0:stock2.tot_value;
        break;
    case three_rate:
        val1 = stock1.three_rate==INVALID_DOUBLE?0:stock1.three_rate;
        val2 = stock2.three_rate==INVALID_DOUBLE?0:stock2.three_rate;
        break;
    case five_rate:
        val1 = stock1.five_rate==INVALID_DOUBLE?0:stock1.five_rate;
        val2 = stock2.five_rate==INVALID_DOUBLE?0:stock2.five_rate;
        break;
    case three_tuv_rate:
        val1 = stock1.three_tuv_rate==INVALID_DOUBLE?0:stock1.three_tuv_rate;
        val2 = stock2.three_tuv_rate==INVALID_DOUBLE?0:stock2.three_tuv_rate;
        break;
    case five_tuv_rate:
        val1 = stock1.five_tuv_rate==INVALID_DOUBLE?0:stock1.five_tuv_rate;
        val2 = stock2.five_tuv_rate==INVALID_DOUBLE?0:stock2.five_tuv_rate;
        break;
    default:
        //其他字段,不排
        return false;
        break;
    }
    if(is_desc)
        return val1>val2;
    else
        return val1<val2;
    return false;
}


///Block---------------------------
bool DynaSorter::compareBlock(const  HZData::Block &block1,const HZData::Block &block2)
{
    switch (m_sortField) {
    case HZData::block_rate:
        if(is_desc)
            return block1.rate>block2.rate;
        else
            return block1.rate<block2.rate;
        break;
    case block_speed_rate:
        if(is_desc)
            return block1.speed_rate>block2.speed_rate;
        else
            return block1.speed_rate<block2.speed_rate;
        break;
    case block_upper:
        if(is_desc)
            return block1.rise>block2.rise;
        else
            return block1.rise<block2.rise;
        break;
    case block_down:
        if(is_desc)
            return block1.fall>block2.fall;
        else
            return block1.fall<block2.fall;
        break;
    case block_three_rate:
        if(is_desc)
            return block1.three_rate>block2.three_rate;
        else
            return block1.three_rate<block2.three_rate;
        break;
    case block_five_rate:
        if(is_desc)
            return block1.five_rate>block2.five_rate;
        else
            return block1.five_rate<block2.five_rate;
        break;
    case block_volume:
        if(is_desc)
            return block1.volume>block2.volume;
        else
            return block1.volume<block2.volume;
        break;
    case block_turnover:
        if(is_desc)
            return block1.turnover>block2.turnover;
        else
            return block1.turnover<block2.turnover;
        break;
    case block_cir_value:
        if(is_desc)
            return block1.cir_value>block2.cir_value;
        else
            return block1.cir_value<block2.cir_value;
        break;
    case block_tot_value:
        if(is_desc)
            return block1.tot_value>block2.tot_value;
        else
            return block1.tot_value<block2.tot_value;
        break;
    default:
        return true;
        break;
    }
}


///Index---------------------------
bool DynaSorter::compareIndex(const HZData::Index &index1,const HZData::Index &index2)
{
    double val1,val2;
    switch (m_sortField) {
    case rise_fall_rate:
        val1 = index1.rate==INVALID_DOUBLE?0:index1.rate;
        val2 = index2.rate==INVALID_DOUBLE?0:index2.rate;
        break;
    case last_price:
        val1 = index1.last_price==INVALID_DOUBLE?0:index1.last_price;
        val2 = index2.last_price==INVALID_DOUBLE?0:index2.last_price;
        break;
    case rise_fall_price:
        val1 = index1.up_down_price==INVALID_DOUBLE?0:index1.up_down_price;
        val2 = index2.up_down_price==INVALID_DOUBLE?0:index2.up_down_price;
        break;
    case turnover:
        val1 = index1.turnover==INVALID_DOUBLE?0:index1.turnover;
        val2 = index2.turnover==INVALID_DOUBLE?0:index2.turnover;
        break;
    case highest_price:
        val1 = index1.highest_price==INVALID_DOUBLE?0:index1.highest_price;
        val2 = index2.highest_price==INVALID_DOUBLE?0:index2.highest_price;
        break;
    case lowest_price:
        val1 = index1.lowest_price==INVALID_DOUBLE?0:index1.lowest_price;
        val2 = index2.lowest_price==INVALID_DOUBLE?0:index2.lowest_price;
        break;
    case pre_close_price:
        val1 = index1.pre_close_price==INVALID_DOUBLE?0:index1.pre_close_price;
        val2 = index2.pre_close_price==INVALID_DOUBLE?0:index2.pre_close_price;
        break;
    case open_price:
        val1 = index1.open_price==INVALID_DOUBLE?0:index1.open_price;
        val2 = index2.open_price==INVALID_DOUBLE?0:index2.open_price;
        break;
    default:
        return false;
        break;
    }
    if(is_desc)
        return val1>val2;
    else
        return val1<val2;
    return false;
}

