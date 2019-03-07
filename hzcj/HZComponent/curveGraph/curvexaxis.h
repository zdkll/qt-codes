#ifndef CURVEXAXIS_H
#define CURVEXAXIS_H
/********************************************************************
created:
author: min.cheng
purpose: 图标X轴坐标
*********************************************************************/
#include "curvedef.h"
#include <QRectF>
class CurveXaxis:public CurveObserver
{
public:
    CurveXaxis(const CurveDataList& data);

    void setTotalCount(double count);

    int getTotalCount();

    void setItemWidth(double width);

    //画布轴坐标转数据下标
    int xCanvasPtxToIndex(int pt_x);

    //数据的下标转画布的X轴坐标
    int xCanvasIndexToPTx(int index);

    void updateData();

    void resize(const QRectF& rc,const QRectF& wndRc);

    void calc();
private:
    const CurveDataList& m_data;
    QRectF m_rc;
    double m_ItemWidth = 1;
    double m_totoalCount = 1;
};

#endif // CURVEXAXIS_H
