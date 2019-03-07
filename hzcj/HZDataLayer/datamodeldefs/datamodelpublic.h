#ifndef DATAMODELPUBLIC_H
#define DATAMODELPUBLIC_H

#include <stdio.h>
#include <string>
#include <list>

#include <QtGlobal>
#include <QString>

#include "hzpublic.hpp"

//ReqId---------------------------------------
typedef uint ReqId;       //请求订阅ID
typedef int   ReqType;   //请求订阅ID类型
typedef int  ErrCode;

//列排序字段值-通用
typedef quint32 SortField;

//默认显示
#define  Default_Item_String "--"
//非法的REQ
#define  INVALID_REQID             -1
#define  MIN_REQID                   100
#define  MAX_REQID                  UINT32_MAX

//===================协议 Enumeration==============================
QT_BEGIN_NAMESPACE

namespace HZData
{
enum ServiceType
{
    None_Service    = 0,
    Sub_Batch_Dyna = 1,
    Req_Batch_Dyna = 2,
    Sub_Opt_Dyna = 3,
    Req_Opt_Dyna = 4,

    //逐笔
    Sub_TickInfo = 5,
    Req_TickInfo= 6,

    //K线分时
    Sub_Kline = 7,
    Req_Kline= 8,
    Sub_TimeKline = 9,
    Req_TimeKline= 10,

    //集合竞价K线
    Sub_BidKline= 11,
    Req_BidKline= 12,

    Sub_Batch_Money= 13,
    Req_Batch_Money =14,

    Sub_Opt_Money = 15,
    Req_Opt_Money = 16,

    //搜索请求
    Req_SearchCode = 17,

    //板块异动
    Sub_BlockAbnormal = 18,
    Req_BlockAbnormal = 19,

    //资金异动
    Sub_Batch_FundAbnormal = 20,
    Req_Batch_FundAbnormal = 21,
    Sub_Opt_FundAbnormal = 22,
    Req_Opt_FundAbnormal = 23,

    //板块集合竞价
    Sub_Batch_BiddingBlock = 24,
    Req_Batch_BiddingBlock = 25,
    Sub_Opt_BiddingBlock = 26,
    Req_Opt_BiddingBlock = 27,

    //个股集合竞价
    Sub_Batch_Bidding = 28,
    Req_Batch_Bidding = 29,
    Sub_Opt_Bidding = 30,
    Req_Opt_Bidding = 31,

    //异动热门板块
    Sub_Abnormal_Block = 32,
    Req_Abnormal_Block = 33,

    //涨跌只有订阅
    Sub_Rise_Fall  = 100,

    //个股短线
    Sub_ShortMotion = 101,
    Req_ShortMotion = 102,

    //涨停监测
    Sub_RfmNumInfo   = 103,
    Req_RfmNumInfo   = 104,
    Sub_RFMonitorInfo = 105,
    Req_RFMonitorInfo = 106,

    //分价表
    Sub_PriceTick = 107,
    Req_PriceTick = 108,

    //主力增减仓
    Sub_IncrDec_Postion = 109,
    Req_IncrDec_Postion = 110,

    Sub_LeaderNetput  = 111,
    Req_LeaderNetput  = 112,

    //指标
    Sub_Zhibiao = 113,
    Req_Zhibiao = 114,

    //盘中异动
    Sub_MidAbnormal = 115,
    Req_MidAbnormal = 116,

    //其他
    Req_Time = 200 //授时
};

//订阅服务的推送频率
enum SubPeriod
{
    FreqDefault = 0,
    OnceIn1Second = 1,
    OnceIn2Second = 2,
    OnceIn3Second = 3,
    OnceIn5Second = 5,
    OnceIn10Second = 10,
    OnceIn30Second = 30,
    OnceIn60Second = 60
};

//设备类型枚举
enum DeviceType {
    device_pc 		= 1, //pc登陆
    device_web		= 2,//web登陆
    device_android  = 3, //android登陆
    device_ios 		= 4, //ios登陆
    device_ipad 	= 5, //ipad登陆
};

//交易状态
enum TradeStatus
{
    DealStatusS = 0,  //开市前
    DealStatusC = 1,  //集合竞价
    DealStatusT = 2 , //交易
    DealStatusB = 3,  //休市
    DealStatusE = 4,  //收盘
    DealStatusP = 5   //停牌
};

//请求类型
enum  CmdType
{
    None = 0,
    Cmd_ReqTime = 101, //授时

    //用户登录
    Cmd_Kick_User = 1003,

    //行情消息序列号，规定为10001~14999
    Cmd_StockCode =  10001,
    Cmd_BatchStock = 10003,
    Cmd_OptionalStock = 10004,
    Cmd_Kline           = 10005,
    Cmd_Zhubi          = 10006,
    Cmd_OptionalMoney= 10007,      //MoneyOptionalReq
    Cmd_BatchMoney 	= 10008,		//BatchGetMoneyReq
    Cmd_TimeKline        = 10009,		//TimeKlineReq
    Cmd_RiseFallInfo    	=	10010,		//GetStockRiseFallInfoRequest
    //集合竞价信息
    Cmd_OptionalBidding =   10011,     		//BiddingOptionalRequest
    Cmd_BatchBidding =	10012,				//BatchGetBiddingRequest
    Cmd_OptionalBiddingBlock =   10013,       //BiddingBlockOptionalRequest
    Cmd_BatchBiddingBlock =	10014,		//BatchGetBiddingBlockRequest

    Cmd_BiddingKline     =	 10015,//集合竞价K线,GetBiddingQuoteKlineRequest
    Cmd_BatchFundAbnormal =	10018,//BatchGetMoneyAbnormalRequest
    Cmd_BlockAbnormal  =	10019,  //BlockAbnormalRequest
    Cmd_OptionalFundAbnormal =	10020,//MoneyAbnormalOptionalRequest
    Cmd_ShortMotion  =	10021,  //BatchShortMotionRequest短线异动

    Cmd_RFMNumInfo  = 10022,      	//RFMonitorNumInfoOptionalRequest
    Cmd_RFMonitorInfo = 10023,		//BatchRFMonitorRequest

    Cmd_GetFenjia	 = 10025,	 //GetFenJiaRequest分价
    Cmd_GetLeaderIncDecPos = 10026,	//MoneyFieldLineRequest
    Cmd_Mid_Abnormal  = 10027,     //盘中异动MidAbnormalRequest
    Cmd_Abnormal_Block = 10028,    //BlockMoneyAbnoramlPCRequest  PC版的板块异动

    //搜索消息序列号，规定为15000~15999
    CMD_Search_Start = 1500,
    Cmd_Query_Code  = 15001,	//QueryCodeRequest
    CMD_Search_End =  15999,

    //publish---------------------------------------------------
    Cmd_Publish = 17001,
    CMD_MSG_PUBLISH_START  = 17000,
    //CMD_MSG_PUBLISH		         = 17001,		//PublishRequest
    CMD_MSG_PUBLISH_END  = 17999,


    //指标  为了区别不同的用户权限，指标分成了四大类，用的结构体都一样
    CMD_MSG_ZHIBIAO_START = 18000,
    Cmd_Zhibiao_Hjjj = 18001,	  // 指标 黄金狙击：见底出击，筹码分布，趋势拐点	GetZhibiaoRequest
    Cmd_Zhibiao_Dcxg = 18002,// 指标 多彩选股：黄蓝区间，庄家控盘，操盘提醒	GetZhibiaoRequest
    Cmd_Zhibiao_Byzs = 18003,// 指标 博弈致胜：海平面，心情曲线				GetZhibiaoRequest
    Cmd_Zhibiao_Bdww = 18004,// 指标 波段为王：多空区间，底部根基，顶部搭建	GetZhibiaoRequest
    CMD_MSG_ZHIBIAO_END = 18049,

    //广播类消息
    CMD_MSG_BROAD_START  		  = 19000,

    Cmd_Trade_Day		  = 19001,		//BroadCastTradeDay

    CMD_MSG_BROAD_END             = 19999,
};

//kline K线类型- proto
enum KlineType
{
    KLINE_PERIOD_MIN = 1,
    KLINE_PERIOD_5MIN = 5,
    KLINE_PERIOD_15MIN = 15,
    KLINE_PERIOD_HOUR = 60,
    KLINE_PERIOD_DAY = 1440,
    KLINE_PERIOD_WEEK = 10080,
    KLINE_PERIOD_MONTH = 43200
};

//股票信息Filter
enum  FilterFlag
{
    MarketAll = 0,              //所有
    MarketSZ = 1,		//深圳市场
    MarketSH = 2,		//上海市场
    MarketCY = 3,		//创业板
    MarketZX = 4,		//中小板
    MarketFX = 5,		//风险板
    BlockIndustry = 100,      //行业板块
    BlockConcept = 101,   //概念板块
    BlockRegion = 102,		//地域板块
    BlockWhole = 103,		//全部板块
    IndexSH = 200,			//上证系列指数
    IndexSZ = 201,			//深圳系列指数
    IndexInner = 202,		//国内指数
    IndexOutter = 203,		//国外指数
    IndexStocks  = 204,		//指数成分股
    //本地自定义
    IndexZH = 501, //重要指数

    //本地增加-自选股
    ZXStocks    = 1001
};


//股票信息排序字段-proto-stock_baseinfo
enum DynaSortField
{
    none_sort   = 0,

    ///**************个股********************///
    highest_price = 1,	//最高价排序序号
    lowest_price = 2,//最低价排序序号
    last_price = 3,		//最新价排序序号
    averag_price = 4,//均价排序序号
    rise_fall_price = 5,	//涨跌额排序序号
    rise_fall_rate = 6,	//涨跌幅排序序号
    bid_price = 7,		//买入价排序序号
    ask_price = 8,		//卖出价排序序号
    volume = 9,			//成交量排序序号
    turnover = 10,		//成交额排序序号
    cur_volume = 11,	//现手排序序号
    turnover_rate = 12, 	//换手率排序序号

    pre_close_price = 13,//昨收价
    open_price = 14,//开盘价
    pes = 15, //市盈率
    np_volume = 16,//内盘
    wp_volume = 17,//外盘
    ratio = 18,//量比
    committee = 19,//委比
    buy_sell_buy = 20,//委差
    pbv = 21,//市净率
    tot_stock = 22,//总股本
    cir_stock = 23,//流通股本
    tot_value = 24,//总市值
    cir_value = 25,//流通市值

    //7/27 新增
    nwp_rate =26,       //内盘外盘比例排序序号
    sa = 27,             //振幅排序序号
    speed_rate = 28,     //涨速排序序号
    three_rate = 29,      //3日涨幅排序序号
    three_tuv_rate = 31,  //3日换手率排序序号
    five_tuv_rate = 32,    //5日换手率排序序号
    ped              = 33,    //动态市盈率
    five_rate = 34,        //5日涨幅排序序号
    security_name = 35, //股票名称排序序号
    year_rate = 36,          //年初至今涨幅

    ///**************板块********************///
    block_rate = 101,						//板块涨跌幅排序序号
    block_up_down_ratio = 102,		//涨跌比排序序号
    block_turnover_rate = 103,		//换手率排序序号
    block_turnover = 104,				//板块成交额排序序号
    block_tot_value = 105,				//板块总市值排序序号
    block_cir_value = 106,				//板块流通市值排序序号
    block_avg_stock = 107,				//板块平均股本排序序号
    block_volume = 108,					//板块成交量排序序号
    block_upper = 109,					//板块上涨个数排序序号
    block_down = 110,                   //成分股下跌个数排序序号
    //7/27 新增
    block_speed_rate = 111,       			//涨速排序序号
    block_three_rate = 112,      				//3日涨幅排序序号
    block_three_tuv_rate = 114,  				 //3日换手率排序序号
    block_six_tuv_rate = 115,     				//5日换手率排序序号
    block_rise_day_num = 116,				//连涨天数
    block_five_rate = 117,       				 //5日涨幅
    block_block_name = 118,    				 //板块名称排序序号
    block_year_rate = 119,                  //年初至今涨跌排序序号

    ///**************资金********************///
    money_last_price = 201,		     //最新价排序序号
    money_rate = 202,	                 //涨跌比排序序号
    money_auction_volume = 203,	 //集合竞价成交量排序序号
    leader_money_input = 204,		 //主力资金流入排序序号
    leader_money_output = 205,	     //主力资金流出排序序号
    leader_money_netput = 206,	     //主力资金净流入排序序号
    leader_money_rate = 207,		     //主力资金净占比排序序号
    maxord_money_input = 208,		 //超大单资金流入排序序号
    maxord_money_output = 209,	 //超大单资金流出排序序号
    maxord_money_netput = 210,	 //超大单资金净流入排序序号
    maxord_money_rate = 211,		 //超大单资金净占比排序序号
    bigord_money_input = 212,	     //大单资金流入排序序号
    bigord_money_output = 213,	     //大单资金流出排序序号
    bigord_money_netput = 214,     //大单资金净流入排序序号
    bigord_money_rate = 215,	         //大单资金净占比排序序号
    midord_money_input = 216,	     //中单资金流入排序序号
    midord_money_output = 217,	 //中单资金流出排序序号
    midord_money_netput = 218,	 //中单资金净流入排序序号
    midord_money_rate = 219,		 //中单资金净占比排序序号
    minord_money_input = 220,	     //小单资金流入排序序号
    minord_money_output = 221,	 //小单资金流出排序序号
    minord_money_netput = 222,	 //小单资金净流入排序序号
    minord_money_rate = 223,     	 //小单资金净占比排序序号
    three_leader_money_netput = 224,	 //3日主力资金净流入排序序号
    five_leader_money_netput = 225,	     //5日主力资金净流入排序序号
    auction_turnover = 226,		   //集合竞价成交额排序序号
    money_security_name = 227,  //股票名称或板块名称排序序号
    money_up_down_price = 228 //资金涨跌额
};



//股票搜索方式
enum SearchType
{
    FuzzyQuery = 0,				   //模糊查询
    CodeQuery = 1,					//code查询
    SampleQuery = 2, 				//简拼查询
    //FullQuery = 3 ,					//全拼查询
    //ChineseQuery = 4 			//中文查询
};

//股票码
struct StockCode
{
    std::string stock_code;
    std::string stock_name;
};

struct StockTable
{
    QString stock_code;
    QString stock_name;
};

struct Ret
{
    INT32       err_code = 0;
    std::string err_msg = "";
};

}
QT_END_NAMESPACE

#endif // DATAMODELPUBLIC_H
