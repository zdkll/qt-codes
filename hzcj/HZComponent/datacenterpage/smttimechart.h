#ifndef SMTTIMECHART_H
#define SMTTIMECHART_H
/********************************************************************
created:
author: min.cheng
purpose：融资融券时间轴绘制
*********************************************************************/
#include "curvechart.h"
class SmtTimeChart:public CurveChart
{
public:
    SmtTimeChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    void updateData();

    void resize(const QRectF& rc,const QRectF& wndRc);
private:
    void drawTimes();
};

#endif // SMTTIMECHART_H
