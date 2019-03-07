#ifndef CROSSLINECHART_H
#define CROSSLINECHART_H
/********************************************************************
created: 2018/4/20
author: min.cheng
purpose: 十字光标绘制区域
*********************************************************************/
#include "basechart.h"

class CrossLineChart : public BaseChart
{
    Q_OBJECT
public:
    CrossLineChart(ImageLayer* layer);
signals:

protected slots:

protected:
    void redraw();
    bool moveCrossLine(const QPoint& pos,int index =-1);
    void mainChartXaixsChnage();

    //通过父类需函数来处理系统事件
    void onShow(bool bshow);
    bool enterEventEx(QEvent *event);
    bool leaveEventEx(QEvent *event);
    bool keyPressEx(QKeyEvent *event);
    bool mouseMoveEx(QMouseEvent *event);
protected:
    QPoint m_currCrossPt;
};

#endif // CROSSLINECHART_H
