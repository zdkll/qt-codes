#ifndef BIDDINGDEF_H
#define BIDDINGDEF_H

#include <string>
#include <list>
#include "datamodelpublic.h"

//集合竞价相关
namespace HZData
{
//请求集合竞价K线- 返回K线---
typedef std::string  ReqBidKlineInfo;

//个股集合竞价排行字段
enum BiddingSortType
{
    bid_fund = 1,	//根据资金排序
    bid_quantity_than = 2,	//根据量比排序
    bid_bidding_rise = 3,		//根据竞价涨幅排序
    bid_yesterday_rate = 4 	//根据昨涨幅排序
};

//板块集合竞价排行字段
enum BiddingBlockSortType
{
    bid_block_rate = 1,
};

//个股集合竞价
struct StockBid
{
    std::string stock_code;		    //股票代码
    std::string stock_type;		    //股票类型
    std::string stock_name;		    //股票名称
    UINT32 trading_day = INVALID_UINT32;		    //交易日
    double bidding_rise = INVALID_DOUBLE;       //竞价涨幅
    INT64  main_funds = INVALID_INT64;		 //主力资金
    double bidding_quantity_than = INVALID_DOUBLE;//竞价量比
    std::string plate;			        //板块
    UINT64 bidding_volume = INVALID_UINT64;		//竞价成交量
    double bidding_turnover = INVALID_DOUBLE;   //竞价成交额
    double bidding_price = INVALID_DOUBLE;			//买一价
    double bidding_volume1 = INVALID_DOUBLE;	//买一量
    double current_value = INVALID_DOUBLE;		    //流通值
    double yesterday_rate = INVALID_DOUBLE;		 //昨涨幅
    UINT32 trade_status = INVALID_UINT32;		    //交易状态
    UINT64 update_time = INVALID_UINT64;		    //成交时间

    double last_price = INVALID_DOUBLE;          //现价
    double rate = INVALID_DOUBLE;				//涨幅
    double pre_close_price = INVALID_DOUBLE;    //昨收价格
    UINT64 upsert_time = INVALID_UINT64;        //插入时间
};

//板块集合竞价
struct BlockBid
{
    std::string block_code;		        //板块代码
    std::string block_name;		        //板块名称
    double rate = INVALID_DOUBLE;                   //竞价涨幅
    UINT64   update_time_sec = INVALID_UINT64;		//成交时间
};

//批量请求板块集合竞价排行
struct ReqBatchBiddingBlock
{
    SortField	sort_field  ; //BiddingBlockSortType 根据用户所选择的字段进行排序，大体也就5-6个属性
    bool	is_desc;					// true 表示降序
    INT32	index;		  			// 初始index位置
    INT32	count_range;			//请求获取股票信息的条数
};

//集合竞价排行请求的响应
struct  BiddingBlockInfo
{
    QVector<BlockBid>	    stk;	//范围方式批量拉取股票基础信息
    INT32         		total_cnt= 0;  //当前排行的所有数量
};

//发送请求，固定股票代码，拉取集合竞价信息（基本用于自选股）
typedef  std::list<std::string> ReqOptionalBiddingBlock ;//指定股票代码（带市场的统一代码）


//个股
struct ReqBatchBidding
{
    SortField	sort_field ; //BiddingBlockSortType 根据用户所选择的字段进行排序，大体也就5-6个属性
    bool	is_desc ;					    // true 表示降序
    INT32	index = 0;		  			 // 初始index位置
    INT32	count_range= 0;		 //请求获取股票信息的条数
};

//集合竞价排行请求的响应
struct BiddingInfo{
    QVector<StockBid>    stk;	//范围方式批量拉取股票基础信息
    INT32      total_cnt = 0;  //当前排行的所有数量
};

//发送请求，固定股票代码，拉取集合竞价信息（基本用于自选股）
typedef  std::list<std::string> ReqOptionalBidding;//指定股票代码（带市场的统一代码）

}



#endif // BIDDINGDEF_H
