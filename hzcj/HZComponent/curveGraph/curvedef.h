#ifndef CURVEDEF_H
#define CURVEDEF_H
/********************************************************************
created:
author: min.cheng
purpose 曲线图结构定义
*********************************************************************/
#include <QVector>
#include <QMap>
#include <functional>
#include <QRectF>

//数据定义
struct CurveDataItem
{
    double price = DBL_MAX;
    double average = DBL_MAX;
    qint64 time;
};

struct CurveDataList
{
    double max = DBL_MIN;
    double min = DBL_MAX;
    QVector<CurveDataItem> data;
};

//请求参数
typedef QMap<QString,QString> CurveReqParam;


//数据监听
class CurveObserver
{
public:
    virtual void clearData(){}

    virtual void updateData(){}

    virtual void resize(const QRectF& rc,const QRectF& wndRc){}
};

typedef std::function<void(int id,int retCode,const CurveDataList& data)>  CurveUpdateCallBack;
//绘图区域边界
struct CurveChartMargin
{
    int leftPadding = 0;
    int rightPadding = 0;
    int topPadding =0;
    int bottomPadding=0;

    CurveChartMargin(){}
    CurveChartMargin(int left,int right,int top,int bottom):
        leftPadding(left),
        rightPadding(right),
        topPadding(top),
        bottomPadding(bottom)
    {}
};

#endif // CURVEDEF_H
