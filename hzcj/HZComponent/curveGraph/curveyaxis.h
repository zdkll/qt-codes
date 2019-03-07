#ifndef CURVEYAXIS_H
#define CURVEYAXIS_H
/********************************************************************
created:
author: min.cheng
purpose:图标Y轴坐标
*********************************************************************/
#include <QRectF>
#include "curvedef.h"
class CurveYaxis:public CurveObserver
{
public:
    CurveYaxis(const CurveDataList& data);

    //画布轴坐标点到价格值之间的转换
    double canvasPtToValue(double pt_y);

    //价格值到画布轴坐标点的转换
    double canvasValueToPt(double value);

    void updateData();

    void resize(const QRectF& rc,const QRectF& wndRc);
private:
    void calc();
private:
    const CurveDataList& m_data;
    QRectF m_rc;
    double m_ratio =1.0f;
};

#endif // CURVEYAXIS_H
