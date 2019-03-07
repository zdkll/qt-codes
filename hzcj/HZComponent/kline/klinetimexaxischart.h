#ifndef KLINETIMEXAXISCHART_H
#define KLINETIMEXAXISCHART_H
/********************************************************************
created: 2018/4/20
author: min.cheng
purpose: X时间轴绘制区域
*********************************************************************/
#include "basechart.h"

class KlineTimeXaxisChart  : public BaseChart
{
    Q_OBJECT
public:
    KlineTimeXaxisChart(ImageLayer* layer);
signals:

protected slots:

protected:
    void drawText(QPainter &painter);
    QString formatTime(double tm1, double tm2);
    int getPeriod();
    void mainChartXaixsChnage(const QString& msg);
    void onResize();
    void redraw();
};

#endif // TIMEXAXISCHART_H
