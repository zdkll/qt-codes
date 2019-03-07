#ifndef FUNDABNORMALDEF_H
#define FUNDABNORMALDEF_H

#include <string>
#include <list>
#include "datamodelpublic.h"

namespace  HZData
{
//资金异动排序
enum FASortType
{
    fa_rate = 1,		    //涨幅
    fa_thirty_rate = 2,    //30分钟涨幅
    fa_active_netput = 3,   //主动净买
    fa_thirty_active_netput = 4,   //30分钟主动净买
    fa_leader_netput = 5,   //主力净买
    fa_avtive_rate = 6,   //主动占比
    fa_leader_rate = 7,   //主力占比
    fa_turnover_rate = 8,		//换手率
    fa_ratio = 9,		//量比
    fa_cir_value = 10,       //流通市值
    fa_ped = 11,				//市盈率(动)
    fa_three_avtive_netput = 12, //3日主动净买
    fa_three_rate = 13, //3日涨幅
    fa_five_avtive_netput = 14, //5日主动净买
    fa_five_rate = 15, //5日涨幅
};

//资金异动类型
enum FundAbnormalType
{
    fat_abnormal = 1, //实时异动
    fat_first_increase_buy = 2, //首次增仓
    fat_low_rise = 3,//低涨幅
    fat_zxstock = 4 //自选异动，无效的Type值，仅本地使用
};

//个股资金异动
struct FundAbnormal
{
    std::string stock_code;     			//股票代码
    std::string stock_name;    				 //股票名称
    std::string stock_type;		         //股票类型
    std::string block_name;     			//股票所属板块名称
    double last_price = INVALID_DOUBLE;  //现价
    double pre_close_price = INVALID_DOUBLE;
    double rate = INVALID_DOUBLE;		    			//涨幅
    double thirty_rate = INVALID_DOUBLE;    			//30分钟涨幅
    INT64 active_netput = INVALID_INT64;   			//主动净买
    INT64 thirty_active_netput = INVALID_INT64;   	//30分钟主动净买
    INT64 leader_netput = INVALID_INT64;   			//主力净买
    double avtive_rate = INVALID_DOUBLE;   				//主动占比
    double leader_rate = INVALID_DOUBLE;   			//主力占比
    double	turnover_rate = INVALID_DOUBLE;				//换手率
    double	ratio = INVALID_DOUBLE;						//量比
    double cir_value = INVALID_DOUBLE;       			//流通市值
    double ped = INVALID_DOUBLE; 						//市盈率(动)
    INT64 three_avtive_netput = INVALID_INT64; 		//3日主动净买
    double three_rate = INVALID_DOUBLE; 				//3日涨幅
    INT64 five_avtive_netput = INVALID_INT64; 		//5日主动净买
    double five_rate = INVALID_DOUBLE; 				//5日涨幅
};

//资金异动排行请求
struct ReqBatchFundAbnormal
{
    FundAbnormalType type ; //个股异动类型
    SortField  sort_field  ;   //个股异动排序类型
    bool	is_desc ;				    // true 表示降序
    INT32	index ;		  			   // 初始index位置
    INT32	count_range ;		   //请求获取股票信息的条数
    UINT64 mask;  //按位操作请求哪些字分类，不填写就表示请求所有
};

//资金异动指定股票请求
struct ReqOptionalFundAbnormal
{
    std::list<std::string>	stock_code;		//指定股票代码（带市场的统一代码）
    UINT64   mask  ;		                //按位操作请求哪些字分类，不填写就表示请求所有
};

struct FundAbnormalInfo
{
   QVector<HZData::FundAbnormal> fundAbmls;
   int totalCount = 0;
};

}

#endif // FUNDABNORMALDEF_H
