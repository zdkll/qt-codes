#ifndef FUNDFLOWDEF_H
#define FUNDFLOWDEF_H

#include "datamodeldef.h"

#include <QFont>
#include <QColor>

struct FundOrder
{
    INT64 input = 0;
    INT64 output = 0;
    INT64 netput = 0;
};

struct FundInOutData
{
    FundOrder  fundOrders[4];  //超大单，大单，中单和小单
    INT64         totalInput = 0;   //超大单，大单，中单和小单总流入
    INT64         totalOutput = 0;//超大单，大单，中单和小单总流出
    INT64         totalNetput = 0; //超大单，大单，中单和小单净流入
    INT64         totalInOut =0;    //总资金流
    INT64         maxTotalFund = 0;//最大资金流向
    INT64         maxNetput = 0;  //最大净流入
    INT64         leaderNetput = INVALID_INT64;
};

//资金颜色类型
enum FundColor
{
    Normal = 0,
    Input,
    Output1,
    Output2,
    Output3
};

struct  FundItem
{
    QString  label;  //名称
    QString  unit = QStringLiteral("元");  //单位
    QString  value; //值
    double   ratio = 0;    //百分比
    FundColor     fundColor = Normal;
    QColor   color;
};


enum PaintAction
{
    NODRAW = 0,
    REDRAW = 1,
    OBJDRAW = 2,
    DRAW    = 3
};

//资金流样式
struct FundFlowStyledProperties
{
    QFont  bigCharFont;
    QFont  charFont;

};

struct  IncrDecPosData
{
    //原始数据
    QVector<HZData::IncrDecPostion> incDecPos;
    long long max = 0,min = 0;
    int    max_index,min_index;
    int           uintType;   //单位
};

struct NeputData
{
    QString    label;
    INT64      netput = INVALID_INT64;
};

#endif // FUNDFLOWDEF_H
