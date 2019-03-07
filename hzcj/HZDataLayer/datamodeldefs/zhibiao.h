#ifndef ZHIBIAO_H
#define ZHIBIAO_H

#include <QVector>

#include "datamodelpublic.h"

namespace HZData {

//指标类型-命令类型
enum ZhibiaoMsgType
{
    Zhibiao_Hjjj    = 18001,// 指标 黄金狙击：见底出击，筹码分布，趋势拐点
    Zhibiao_Dcxg = 18002,// 指标 多彩选股：黄蓝区间，庄家控盘，操盘提醒
    Zhibiao_Byzs = 18003,// 指标 博弈致胜：海平面，心情曲线
    Zhibiao_Bdww= 18004// 指标 波段为王：多空区间，底部根基，顶部搭建
};

//指标数据类型
enum ZhibiaoType{
    JDCJ = 1,		// 见底出击
    CMFB = 2,   // 筹码分布
    QSGD = 3,	// 趋势拐点
    HLQJ = 4,	// 黄蓝区间
    ZJKP = 5,		// 庄家控盘
    CPTX = 6,	// 操盘提醒
    HPM = 7,		// 海平面
    XQQX = 8,	// 心情曲线
    DQKJ = 9,	// 多空区间
    DBGJ = 10,	// 底部根基
    DBDJ = 11,	// 顶部搭建
};

//点状结果集
// stype 详细说明
// JDCJ: 趋势类别 1短期, 2中期, 3长期
// CMFB：分集标志 0集, 1分
// QSGD:
// HLQJ: 买卖标志 0买，1卖
// ZJKP: 控盘类型 0无装控盘（白），1开始控盘（黄），2有庄控盘（红），3高度控盘（紫），4主力出货（绿）
// CPTX: 进场标志 0波段进场, 1反弹进场，2超跌进场
// HPM:  0空，1多
//QX:
// DBDJ:
struct ZbDot
{
    INT32 upserttime_time = INVALID_INT32;
    INT32 stype = INVALID_INT32;
};

//柱状图，曲线结果集（曲线时，高低坐标相同, 只给high）
// color_type 详细说明
// JDCJ: 无
// CMFB：无
// QSGD:
// HLQJ: 0黄，1蓝
// ZJKP: 控盘类型 0无装控盘（白），1开始控盘（黄），2有庄控盘（红），3高度控盘（紫），4主力出货（绿）
// CPTX: 0红, 1绿
// HPM:  柱形颜色 0紫，1绿
// XQQX:
// DBDJ: 折线颜色 0白，1红，2紫
struct ZbColumn
{
    INT32 color_type = INVALID_INT32;//颜色类型， 考虑到后期产品可能会更换显示颜色的规则，目前只会按照每种指标，给出对应的类型，不会直接给出颜色
    INT32 upserttime_time = INVALID_INT32;
    INT32 high = INVALID_INT32;	//柱状点的上坐标
    INT32 low = INVALID_INT32;	//柱状点的下坐标
};

//发送请求，获取指标的数据
struct ReqZhibiaoInfo
{
    std::string	stock_code;//股票代码 sh600000
    ZhibiaoMsgType cmdType = Zhibiao_Hjjj;//指标类型
    ZhibiaoType	ztype = JDCJ;//指标类型
    INT32   start_time;     //k线开始时间
    INT32	end_time;		  //k线结束时间
};

//指标拉取，响应
struct ZhibiaoInfo
{
    ZhibiaoType	     ztype;//指标类型
    QVector<ZbDot>         dotInfo;
    QVector<ZbColumn>	colInfo;
};

}


#endif // ZHIBIAO_H
