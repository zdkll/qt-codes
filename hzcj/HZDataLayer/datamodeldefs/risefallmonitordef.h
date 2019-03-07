#ifndef RISEFALLMONITORDEF_H
#define RISEFALLMONITORDEF_H

#include <string>
#include "datamodeldef.h"

//涨停监测相关定义
namespace HZData
{
//涨停监测类型
enum RFMonitorType
{
    RFM_type_zt = 1,	    //涨停
    RFM_type_dt = 2,	//跌停
    RFM_type_zthb = 3,	//涨停开板
    RFM_type_dtkb = 4,	//跌停开板
    RFM_type_lb = 5,	    //连板
    RFM_type_zb = 6,   //炸板
    RFM_type_yestoday_zt = 7//昨日涨停
};

//涨跌统计排序类型
enum RiseFallMSortType
{
    rfm_sort_invalid = 0,
    rfm_sort_fb_rise = 1,   //涨幅
    rfm_sort_fb_time = 2, //封板时间
    rfm_sort_zt_volume = 3, //涨停封单
    rfm_sort_dt_volumn = 4, //跌停停封单
    rfm_sort_dt_time = 5, //跌停时间
    rfm_sort_zb_time = 6, //炸板时间
    rfm_sort_zb_num = 7, //炸板次数
    rfm_sort_main_funds = 8,//主力资金流入
    rfm_sort_zt_open_time = 9, //涨停打开时间
    rfm_sort_dt_open_time = 10//跌停打开时间
};

//涨停快照
struct RiseFallData
{
    UINT32 message_type = INVALID_UINT32;		        //消息类型
    std::string stock_code ;		    //股票代码
    std::string stock_type  ;		    //股票类型
    std::string stock_name  ;		    //股票名称
    double rise = INVALID_DOUBLE;//涨幅
    std::string plate ;			           //板块
    UINT64 up_limit_rate_time = INVALID_UINT64;             //封板时间
    std::string continuous_up_limit_rate ;         //涨停连板
    std::string continuous_lower_limit_rate  ;         //跌停连板
    UINT64 up_limit_volume = INVALID_UINT64;               //涨停封单
    UINT64 lower_limit_volume = INVALID_UINT64;                //跌停停封单
    UINT64 lower_limit_rate_time = INVALID_UINT64;               //跌停时间
    UINT64 fried_time = INVALID_UINT64;                  //炸板时间
    UINT64 fried_num = INVALID_UINT64;                 //炸板次数
    INT64   main_funds = INVALID_INT64;              //主力资金流入
    UINT64 to_up_limit_time = INVALID_UINT64;             //涨停打开时间
    UINT64 to_lower_limit_time = INVALID_UINT64;          //跌停打开时间
    std::string history_continuous;            //往日连扳
    UINT64 update_time_sec = INVALID_UINT64;		   //成交时间
    UINT32 tradingday = INVALID_UINT32;			         //交易日
    double last_price = INVALID_DOUBLE;
};


//涨跌统计数量信息
struct RiseFallMonitorNumInfo
{
    INT32 real_time_num = INVALID_INT32;         			//实时涨停数
    UINT32 natural_num = INVALID_UINT32;          				//自然数
    UINT32 continuous_num = INVALID_UINT32;   				//连板数
    UINT32 aword_plate = INVALID_UINT32;         				//一字板
    float sealing_plate_rate = INVALID_FLOAT;  				//封板率
    UINT32 fried_plate = INVALID_UINT32;      						//炸板数
    float yeaterday_limit_rate = INVALID_FLOAT;   			//昨日涨停
    float yeaterday_high_open_rate = INVALID_FLOAT;  	//昨日高开
    float yeaterday_profit_rate = INVALID_FLOAT;   			//昨日收益率
    float yeaterday_continuous_limit_rate = INVALID_FLOAT;  	//昨日连板个股涨跌幅
    float yeaterday_continuous_high_open_rate = INVALID_FLOAT; //昨日连板高开率
    float yeaterday_continuous_profit_rate = INVALID_FLOAT;//昨日连板获利率
    UINT64 update_time_sec = INVALID_UINT64;		   				//成交时间
    UINT32   trading_day = INVALID_UINT32;			          		       //交易日
};

struct ReqBatchRfmInfo
{
    RFMonitorType      m_type;	//涨跌统计的分类
    SortField sort_type;	//涨跌统计排序类型
    bool	     is_desc ;				// true 表示降序
    INT32	index = 0;		  		// 初始index位置
    INT32	count_range = 0;	//请求获取股票信息的条数
};

struct RiseFallMonitorInfo
{
    QVector<RiseFallData>  rfmData;
    int totalCount = 0;
};

}

#endif // RISEFALLMONITORDEF_H
