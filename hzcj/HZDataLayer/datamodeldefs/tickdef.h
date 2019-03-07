#ifndef TICKDEF_H
#define TICKDEF_H

#include <QVector>

#include "datamodelpublic.h"


//===================协议返回==============================
namespace HZData {

//逐笔交易的信息记录———————————
typedef struct tagTick
{
    UINT32	trading_day;		//交易日
    // UINT64	seq;					// 前32时间+ 后32自增

    double   	price;		    //成交价
    UINT64	volume	;		//成交量
    INT32	   access_way;	//指明逐笔交易的方向，买入还是卖出   1表示卖，2表示买
    INT32     update_time; //交易时间
    double    turnover = INVALID_DOUBLE;	//成交额
    tagTick()
    {
        this->trading_day =INVALID_UINT32 ;	//交易日
        this->price =INVALID_DOUBLE ;           //成交价
        this->volume =INVALID_UINT64;		    //现量
        this->access_way =INVALID_INT32 ;		//性质
        this->update_time  = INVALID_INT32;    //时间
    }
}HZTick;

struct  Zhubi
{
    int totalCount = 0;
    QVector<HZData::HZTick>  ticks;
};


//分价信息
struct PriceTick
{
    double    price = INVALID_DOUBLE;

    UINT64  buy_volume = 0; //买入成交量
    UINT64  sell_volume = 0; //卖出成交量
    UINT64  volume =   0; //总成交量
};

//总的分价表信息
struct PriceTickInfo
{
    int totalCount = 0;
    UINT64 volume = INVALID_UINT64;
    UINT64 max_volume = INVALID_UINT64;
    double pre_close_price = INVALID_DOUBLE;
    QVector<HZData::PriceTick>  priceTicks;
};


//==================  请求=====================================
//逐笔类型:成交量或者主力净流入
enum eZhubiType
{
    zhubi_type_total = 1, //全部逐笔
    zhubi_type_money = 2 //主力资金逐笔
};

//逐笔
struct ReqTickInfo
{
    std::string   stock_code;   //股票代码
    INT32         update_time; //时间
    UINT32      count;       //拉取数量
    bool            is_desc;     //true 表示降序(倒拉)
    eZhubiType zb_type = zhubi_type_total; //逐笔类型
};

struct ReqPriceTickInfo
{
    std::string   stock_code;   //股票代码
    INT32         index;            //序号
    UINT32      count;           //拉取数量
};

}

#endif // TICKDEF_H
