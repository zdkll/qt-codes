#ifndef FUNDDEF_H
#define FUNDDEF_H

#include "datamodelpublic.h"

//资金
namespace HZData
{
//资金报单
typedef struct Str_OrdField
{
    INT64 money_input   = INVALID_INT64;	//资金流入
    INT64 money_output = INVALID_INT64;   //资金流出
    INT64 money_netput = INVALID_INT64;   //资金净流入 (资金流入 - 资金流出)
    double money_rate     = INVALID_DOUBLE;   //资金净占比 (资金净流入/总成交额) 注:非当日资金没有此字段
}OrdField;

//资金明细-某一天
typedef struct Str_MoneyField
{
    UINT32   trading_day = INVALID_UINT32;   //交易日  注:非当日资金没有此字段
    double    last_price = INVALID_DOUBLE;    //最新价
    double    pre_close_price = INVALID_DOUBLE;//昨收价 注:非当日资金没有此字段
    double    up_down_price = INVALID_DOUBLE; //涨跌
    double    rate = INVALID_DOUBLE;          //涨跌幅
    UINT64  auction_volume = INVALID_UINT64;//集合竞价成交量 注:非当日资金没有此字段
    OrdField leader_money  ;//主力资金
    OrdField active_money ;//主动资金
    OrdField maxord_money ;//超大单资金
    OrdField bigord_money  ;//大单资金
    OrdField midord_money ;//中单资金
    OrdField minord_money ;//小单资金
}MoneyField;


//3 5 10 日全部资金流向
//typedef struct Str_AllMoneyMsg
//{
//    std::string stock_code  ;     //股票代码
//    std::string stock_name ;     //股票名称
//    MoneyField money_one_day ;  //当日资金

//    INT64  one_money_netput = INVALID_INT64;  //当日主力净流入
//    INT64  three_money_netput = INVALID_INT64;//3日主力净流入
//    INT64  five_money_netput  = INVALID_INT64;  //5日主力净流入
//    INT64  ten_money_netput  = INVALID_INT64;   //10日主力净流入
//}AllStockMoney;

//资金数据
typedef struct Str_MoneyMsg
{
    std::string stock_code  ;     //股票代码
    std::string stock_name ;     //股票名称
    MoneyField money_one_day ;  //当日资金

    INT64  one_money_netput = INVALID_INT64;  //当日主力净流入
    INT64  three_money_netput = INVALID_INT64;//3日主力净流入
    INT64  five_money_netput  = INVALID_INT64;  //5日主力净流入
    INT64  ten_money_netput  = INVALID_INT64;   //10日主力净流入
}StockMoney;

//当日 3 5 10 日主力净流入
struct MoneyNetput
{
    std::string stock_code;     //股票代码
    std::string stock_name;     //股票名称

    INT64  one_money_netput;  //当日主力净流入
    INT64  three_money_netput;//3日主力净流入
    INT64  five_money_netput;  //5日主力净流入
    INT64  ten_money_netput;   //10日主力净流入
};

//资金数据结构类型分为两类：当日资金数据;全部净流入
enum MoneyFieldMask
{
    MoneyStockCode_Mask = 0,//股票码，拉取全部股票排行时候使用
    MoneyField_Mask    = 1, //资金数据，当日资金和所有日净流入
    MoneyNetput_Mask = 2,    //全部净流入
};

//主力增减仓
struct IncrDecPostion
{
    UINT32  date;  //日期
    INT64    money; //资金，正数增仓，负数减仓
};

//30日主力增减仓
typedef QVector<IncrDecPostion>  IncrDecPostionInfo;


//主力净流入明细
struct LeaderNetputData
{
    UINT32    update_time;//成交时间
    UINT64    volume;   //成交量
    double     turnover;   //成交额
    double     aver_price;//成交均价
    UINT32    buy_sell =  1;       //买卖方向 买(2)卖(1)
};


//主力净流入明细
typedef QVector<LeaderNetputData> FundLeaderNetputInfo;


//==================  请求=====================================
//资金批量请求
struct ReqBatchFundInfo
{
    INT32 	filter = 0;	//过滤条件，以市场， 板块， 指数等
    std::string	filter_detail ;					//过滤条件的详细信息， eg：板块的某一种类型详细过滤
    SortField	 sort_field ; //根据用户所选择的字段进行排序，大体也就5-6个属性
    bool    	is_desc;					    // true 表示降序
    INT32	index = 0;		  	// 初始index位置
    INT32	count_range = 0;	//请求获取股票信息的条数
    MoneyFieldMask mask = MoneyField_Mask; //默认全部资金字段（注意只是需求所需）
};

//资金自选请求
struct ReqOptionalFundInfo
{
    std::list<std::string>	stock_code;		//指定股票代码（带市场的统一代码）
    MoneyFieldMask mask = MoneyField_Mask;
};

//请求主力增减仓
typedef std::string ReqLeaderIncDecPostionInfo;

//请求主力净流入
typedef std::string ReqFundLeaderNetputInfo;
}

#endif // FUNDDEF_H
