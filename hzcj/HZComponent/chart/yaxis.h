#ifndef YAXIS_H
#define YAXIS_H
/********************************************************************
created: 2018/4/4
author: min.cheng
purpose: Y坐标轴
*********************************************************************/
#include <qvariant.h>
#include <objbase.h>

#include "drawable.h"

class BaseChart;

class YAxis:public QObject
{
    Q_OBJECT
public:
    YAxis(BaseChart* pchart);

    //画布Y轴坐标点到价格值之间的转换
    double yCanvasPTyToValue(double pt_y);

    //价格值到画布Y轴坐标点的转换
    double yCanvasValueToPt(double value);

    //获取chart的绘图区域
    QRect getRc();

    //当数据/界面大小/偏移等发生跟新后重新计算位置和转换坐标
    bool recalc();

    //Y轴坐标系的初始化
    void resetDefaultValue();

    //获取当前Y轴对应的绘图区域
    BaseChart* getChart();

    //显示区域的最大值
    double getMax();

    //显示区域的最小值
    double getMin();
signals:

protected slots:

protected:
    bool isInYaxis(const int& pt_y);
protected:
    QRect m_rc;
    double m_ratio;
    double m_maxValue;
    double m_minValue;
    BaseChart* m_chart;
};

#endif // YAXIS_H
