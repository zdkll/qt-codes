#ifndef SHOTLINEMOTIONDEF_H
#define SHOTLINEMOTIONDEF_H

#include "datamodelpublic.h"
#include <string>

//短线异动
namespace HZData
{
//短线异动类型
enum ShortMotionType
{
    OrganizationEat = 0,		//机构吃货
    LargeBid = 1,					//有大买盘
    RocketLaunch = 2,			//火箭发射
    RapidRebound = 3,			//快速反弹
    HighDiving = 4,				//高台跳水
    AccelerateDecline = 5,		//加速下跌
    LargeBuy = 6,					//大笔买入
    LargeSell = 7,					//大笔卖出
    LargeAsk = 8,					//有大卖盘
    UpperLimit = 9,				//封涨停板
    DownLimit = 10,				//封跌停板
    OpenUpperLimit = 11,		//打开涨停
    OpenDownLimit = 12,		//打开跌停
    OrganizationBuy = 13,		//机构买单
    OrganizationSell = 14,		//机构卖单
    OrganizationSpit = 15,		//机构吐货
    BuyNew = 16,   				//买入新单
    SellNew	= 17 				//卖出新单
};

enum ShortMotionClientType
{
    SMCT_Total = 1,		//短线异动总排行
    SMCT_Money = 2		//短线异动资金排行
};

//请求短线异动
struct ReqShortMotionInfo
{
    ShortMotionClientType type;
    bool is_desc;
    INT32  index = 0;
    INT32  count_range = 0;
};

//短线异动
struct ShortMotion
{
    INT32				index;						//位置下标
    ShortMotionType		short_type  ;					//异动类型
    std::string			stock_code ;					//股票代码
    std::string			stock_name ;					//股票名称
    INT64				update_time_sec = INVALID_INT64;			//成交时间
    INT64				detail_vol = INVALID_INT64;					//现手
    double				rate = INVALID_DOUBLE;						//今日涨跌幅
    double				last_price = INVALID_DOUBLE;					//最新价
    INT64				buy_volume = INVALID_INT64;					//买5档总量
    INT64				buy_volume1 = INVALID_INT64;				//买1
    INT64				sell_volume = INVALID_INT64;				//卖5档总量
    INT64				sell_volume1 = INVALID_INT64;				//卖1
};

struct  ShortMotionInfo
{
    int  total_cnt= 0;
    QVector<ShortMotion> shortMotions;
};

}

#endif // SHOTLINEMOTIONDEF_H
