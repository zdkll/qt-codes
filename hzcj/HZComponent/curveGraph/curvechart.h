#ifndef CURVECHART_H
#define CURVECHART_H
/********************************************************************
created:
author: min.cheng
purpose: 图层区域
*********************************************************************/
#include "imagelayer.h"
#include "curvedef.h"
#include "curvegraphstyle.h"
#include "curvexaxis.h"
#include "curveyaxis.h"
class CurveChart:public CurveObserver
{
public:
    CurveChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    ~CurveChart();

    virtual void updateData();

    virtual void resize(const QRectF& rc,const QRectF& wndRc);

    void setXaxis(CurveXaxis* xaxis);

    CurveXaxis* getXaxis();

    void setYaxis(CurveYaxis* yaxis);

    CurveYaxis* getYaxis();

    QRectF getRc();
protected:
    ImageLayer* m_imageLayer = nullptr;
    const CurveDataList& m_data;
    int m_layerId = -1;
    CurveChartMargin m_margin;
    QRectF m_rc;
    QRectF m_wndRc;
    CurveGraphStyle* m_style;
    CurveXaxis* m_xaxis = nullptr;
    CurveYaxis* m_yaxis = nullptr;
};

#endif // CURVECHART_H
