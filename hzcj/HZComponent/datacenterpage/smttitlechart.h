#ifndef SMTTITLECHART_H
#define SMTTITLECHART_H
/********************************************************************
created:
author: min.cheng
purpose: 标题区域绘制
*********************************************************************/
#include "curvechart.h"

//融券融券静态文本
class SmtTitleStaticChart:public CurveChart
{
public:
    SmtTitleStaticChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    void resize(const QRectF& rc,const QRectF& wndRc);
private:
    void draw();
};

#endif // SMTTITLECHART_H
