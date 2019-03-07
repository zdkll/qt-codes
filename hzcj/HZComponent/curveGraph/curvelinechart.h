#ifndef CURVELINECHART_H
#define CURVELINECHART_H
/********************************************************************
created:
author: min.cheng
purpose: 曲线线条绘制
*********************************************************************/
#include "curvechart.h"
class CurveGridChart;
class CurveLineChart:public CurveChart
{
public:
    CurveLineChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    void updateData();

    void resize(const QRectF& rc,const QRectF& wndRc);

    void setAxisWidht(int left,int right);

    void setGridChart(CurveGridChart* pChart);
protected:
    virtual void drawLine();
    virtual void drawAxis();
    int m_gradDcId = -1;
    int m_leftAxisWidth = 0;
    int m_rightAxisWidth = 0;
    CurveGridChart* m_pGrid = nullptr;
};

#endif // CURVELINECHART_H
