/********************************************************************
created:2019.1
author: jdxchina
purpose: 公用数据定义
*********************************************************************/
#ifndef WEEKLYDEF_H
#define WEEKLYDEF_H

#include <QtCore>
#include <QtGlobal>

#include <hzinfodata.h>

#define CapitalChange_Limit_Val     Qt::UserRole
#define CapitalChange_Limit_Max     Qt::UserRole+1
#define CapitalChange_Limit_Min     Qt::UserRole+2
#define CapitalChange_TEST          Qt::UserRole+3
#define CapitalChange_ModelIndex    Qt::UserRole+4
#define Weekly_Table_StockCode   Qt::UserRole+6
#define Weekly_Table_Column_Sort    Qt::UserRole+7
#define Weekly_Table_UpDnData     Qt::UserRole+9

#define StealingSummary_Template "%1本周发生%2次盘中异动,发生%3件公司大事"

class WeeklyWndUpdate
{
public:
    ~WeeklyWndUpdate(){}

   virtual void updateFromContent(const QVariant &var) = 0;
};

enum
{
    CurWeeklys_Type = 0x1001,//本周资金异动
    CurUpDowns_Type,//本周涨跌
    StealingSummary_Type,//盯盘总结
};

//本周周报
struct STCurWeeklyData
{
    struct WeekData
    {
        QString name;//当天
        qreal num;//当天数值
    };

    qreal opt_up;//自选股上涨
    qreal index_up;//大盘上涨
    QVector<OptionalWeekly::STSummary::STIndexAndOptionRateDaily> selstks; //自选股
    QVector<OptionalWeekly::STSummary::STIndexAndOptionRateDaily> markets; //大盘
};

//本周资金异动
struct STCapitalChange
{
    QString name;//名称
    qlonglong amount;//金额
    qint64 chg; //异动数值
};

//本周涨跌
struct STCurUpDowns
{
    int order;
    QString stockcode;
    QString stockname;
    qreal profit;
    qreal ups;
};

//盯盘总结
struct STStealingSummary
{
    QString stockcode;
    QString stockname;
    QString des;
    qreal ups;
};


Q_DECLARE_METATYPE(STCapitalChange)
Q_DECLARE_METATYPE(STCurUpDowns)
Q_DECLARE_METATYPE(STStealingSummary)
#endif // WEEKLYDEF_H
