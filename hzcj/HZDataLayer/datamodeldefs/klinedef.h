#ifndef KLINEDEF_H
#define KLINEDEF_H

#include "datamodelpublic.h"


//===================协议返回==============================
namespace HZData
{
//K线数据
typedef struct tagKlineInfo
{
    UINT32	trading_day = INVALID_UINT32;		 //交易日
    UINT32	update_time	= INVALID_UINT32;		    //时间
    UINT32 upsert_time  = INVALID_UINT32;         // upsert时间
    double	high_price		= INVALID_DOUBLE;		//最高
    double	open_price	= INVALID_DOUBLE;		//开盘价
    double	low_price	= INVALID_DOUBLE;		   //最低
    double	close_price	 = INVALID_DOUBLE;		//收盘价

    UINT64 volume		= INVALID_UINT64;		//成交量
    double	turnover = INVALID_DOUBLE;		//成交额
    double	pre_close_price  = INVALID_DOUBLE;		// 昨收价
    double  avg_price = INVALID_DOUBLE;// 均价 成交额/成交量

    bool operator==(const tagKlineInfo& other)
    {
        if(other.trading_day != trading_day)
            return false;
        if(other.update_time != update_time)
            return false;
        if(other.upsert_time != upsert_time)
            return false;
        if(other.high_price != high_price)
            return false;
        if(other.open_price != open_price)
            return false;
        if(other.low_price != low_price)
            return false;
        if(other.close_price != close_price)
            return false;
        if(other.volume != volume)
            return false;
        if(other.turnover != turnover)
            return false;
        if(other.pre_close_price != pre_close_price)
            return false;
        if(other.avg_price != avg_price)
            return false;
        return true;
    }
}HZKline;


//K线信息(包括分时)
struct KlineInfo
{
    INT32       kline_type = INVALID_INT32;
    std::string stock_code;
   QVector<HZData::HZKline>    klines;
};

//==================  请求=====================================
//K线数据请求-获取K线数据
struct ReqKlineInfo
{
    std::string  stock_name;//股票代码（纯数字）

    INT32   kline_type;//

    INT32   start_time;//指定起始点位置
    INT32   count;   //表明请求拉取的根数
    bool    is_desc;//true 表示降序(倒拉)
};

//分时数据请求
struct ReqTimeKlineInfo
{
    std::string    stock_name;//股票代码sh600000
    INT32          kline_type = KLINE_PERIOD_MIN;
    INT32          start_time = INVALID_INT32;		//指定起始点位置 -1 当天 -2...
};

}

#endif // KLINEDEF_H
