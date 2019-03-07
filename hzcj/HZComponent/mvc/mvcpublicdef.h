/********************************************************************
created:2018/6/20
author: dk.zhang
purpose: 行情列表 相关定义文件
*********************************************************************/
#ifndef  MVCPUBLICDEF_H
#define MVCPUBLICDEF_H

#include <QColor>
#include <QFont>
#include <QPixmap>


#define Default_Row_Height  22

#define  Fixed_Default_Min_Row  20
#define  Fixed_Default_Max_Row 50
#define  Fixed_Plus_Row             10

#define  Stock_Data_Role                 Qt::UserRole
#define  Stock_Data_Role1               Qt::UserRole+1
#define  Stock_Data_Role2               Qt::UserRole+2
#define  Stock_Data_Role3               Qt::UserRole+3

#define  Tick_Data_Role                    Qt::UserRole+4
#define  Tick_Data_Direct_Role         Qt::UserRole+5

//资金表头
#define  Money_Header_Data_Role               Qt::UserRole+6
#define  Money_Header_Alignment_Role       Qt::UserRole+7
#define  Money_Header_IsSortIndex_Role     Qt::UserRole+8
#define  Money_Header_SortOrder_Role       Qt::UserRole+9

//预估字段
#define  Yugu_Data_Role                              Qt::UserRole+10

//相似K线表头
#define  SimK_Header_Data_Role               Qt::UserRole+6
#define  SimK_Header_Alignment_Role       Qt::UserRole+7
#define  SimK_Header_IsSortIndex_Role     Qt::UserRole+8
#define  SimK_Header_SortOrder_Role       Qt::UserRole+9


//自定义字段名称
#define Stock_No_Role                    Qt::UserRole+100
#define Stock_Name_Role                Qt::UserRole+101
#define Stock_Code_Role                 Qt::UserRole+102
#define Stock_LastPrice_Role           Qt::UserRole+103
#define Stock_UpDownPrice_Role     Qt::UserRole+104
#define Stock_Rate_Role                  Qt::UserRole+105

//分价表
#define Stock_Price_Role                    Qt::UserRole+150
#define Stock_Pre_Close_Price_Role  Qt::UserRole+151
#define Stock_Sell_Volume_Role         Qt::UserRole+152
#define Stock_Buy_Volume_Role         Qt::UserRole+153
#define Stock_Volume_Role                 Qt::UserRole+154

#define Stock_Max_Volume_Role         Qt::UserRole+155
//总的交易量
#define Stock_Total_Volume_Role       Qt::UserRole+156

//主力净流入
#define LeaderNetput_Data_Role             Qt::UserRole+200  //主力净流入字段数值
#define LeaderNetput_Direct_Role            Qt::UserRole+201 //主力净流入买卖方向


//基本的表格样式属性结构
struct  StyleProperties
{
    QColor  riseColor; //上涨
    QColor  plainColor;//平
    QColor  fallColor;//下跌
    QColor  normalColor;//普通颜色
    QColor  gridLineColor;//网格线颜色

   QColor  numberColor;
   QColor  textColor;

   QColor bpColorFirst; //备用颜色1

   //额外pixmap
   QPixmap filePixmap,fileHoverPixmap; //文件
};

enum ViewType
{
    SLView        = 0,  //缩略
    OverallView = 1   //全景图
};

//新股日历类型
enum XgrlType
{
    Xgrl_Xgsg= 0,//新股申购
    Xgrl_Xgyg = 1,//新股预告
    Xgrl_Jrsg = 2,//今日申购
    Xgrl_Jrss = 3,//今日上市
    Xgrl_Yfxdss = 4,//已发行待上市
    Xgrl_Xgssbx = 5,//新股上市表现
};

enum LocalStockType
{
    ZXStock = 0,
    HistStock = 1
};


#endif // MVCPUBLICDEF_H
