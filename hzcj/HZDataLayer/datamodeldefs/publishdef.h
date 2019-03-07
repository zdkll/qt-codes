#ifndef PUBLISHDEF_H
#define PUBLISHDEF_H

#include "datamodelpublic.h"


//==================  请求=====================================
//2018/08/23 新增订阅推送协议
namespace HZData
{
//订阅服务类型
enum SubServiceType
{
    service_invalid = 0,
    service_stock_block_quote = 1,			//个股板块快照
    service_stock_block_kline = 20,			//个股板块K线
    service_stock_block_zhubi = 30, 		//个股板块逐笔
    service_money_block_quote = 100 	//资金快照
};

struct SubItem
{
    std::string 	      stock_code;	//订阅code
    SubServiceType stype;        //订阅类型
    INT32     			  kline_type = INVALID_INT32; //k线类型
};

struct SubRequest
{
    INT32    	              state;//1订阅，2取消订阅
    std::list<SubItem> items;//订阅的项
};

}

#endif // PUBLISHDEF_H
