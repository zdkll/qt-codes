#ifndef CURVECROSSLINECHART_H
#define CURVECROSSLINECHART_H
/********************************************************************
created:
author: min.cheng
purpose: 中间十字线光标
*********************************************************************/
#include "curvechart.h"

#define AXIS_LEFT_DRAW_TYPE 0x0001
#define AXIS_RIGHT_DRAW_TYPE 0x0002
//绘图需要信息
struct CrossInfo
{
    CurveXaxis* m_xaxis=nullptr;
    CurveYaxis* m_yaxis=nullptr;
    int axisDrawType =0;
    CurveDataList* data=nullptr;
    bool isAverage = false;
};

class CurveCrossLineChart:public CurveChart
{
public:
    CurveCrossLineChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    void resize(const QRectF& rc,const QRectF& wndRc);

    void setCrossInfo(QVector<CrossInfo>& lineInfo);

    void move(const QPoint& pt,const QRectF& timeRc);

    void mouseLeave();

    virtual void drawLine();
private:
    QPoint m_currPt;
    QRectF m_timeRc;
    QVector<CrossInfo> m_crossInfo;
};

#endif // CURVECROSSLINECHART_H
