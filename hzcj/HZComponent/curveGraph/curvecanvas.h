#ifndef CURVECANVAS_H
#define CURVECANVAS_H
/********************************************************************
created:
author: min.cheng
purpose: 曲线窗口画布
*********************************************************************/
#include "curvegraphstyle.h"
#include "curvedef.h"
#include "curvegridchart.h"
#include "curvelinechart.h"
#include "imagelayer.h"

class CurveCanvas:public CurveGraphStyle
{
    Q_OBJECT
public:
    CurveCanvas(QWidget *parent);

    ~CurveCanvas();

    virtual void createWg();

    void setAxisWidth(int left,int right);
protected:
    virtual void paintEvent(QPaintEvent *event);
    //获取窗口大小
    QRectF getRc();
    //更新数据
    virtual void dataUpdate();
protected:
    QVector<CurveObserver*> m_veObserverList;
    int m_reqId = INT_MAX;
    int m_leftAxisWidth = 80;
    int m_rightAxisWidth = 80;
    //图层管理器
    ImageLayer* m_imageLayer;
};

#endif // CURVECANVAS_H
