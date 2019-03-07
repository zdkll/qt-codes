#ifndef MINTIMEXAXISCHART_H
#define MINTIMEXAXISCHART_H
/********************************************************************
created: 2018/4/20
author: min.cheng
purpose: 分时X时间轴绘制区域
*********************************************************************/
#include "basechart.h"

class MinTimeXaxisChart  : public BaseChart
{
    Q_OBJECT
public:
    MinTimeXaxisChart(ImageLayer* layer);
signals:

protected slots:

protected:
    QString formatTime(double tm1,const QString& format = "MM/dd");
    void mainChartXaixsChnage(const QString& msg);
    bool mouseMoveEx(QMouseEvent *event);
    void onResize();
    void redraw();
};

#endif // MINTIMEXAXISCHART_H
