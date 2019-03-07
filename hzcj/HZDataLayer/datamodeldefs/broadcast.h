#ifndef BROADCAST_H
#define BROADCAST_H

#include "datamodelpublic.h"

QT_BEGIN_NAMESPACE
namespace HZData
{
//交易日切换消息
struct  TradeDayMsg
{
    INT32 trade_day = INVALID_INT32; //当前交易日
    INT32 trade_type = INVALID_INT32; //交易所类型 A股 B股 港美股...
};

}

#endif // BROADCAST_H
