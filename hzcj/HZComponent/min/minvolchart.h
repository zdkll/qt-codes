#ifndef MINVOLCHART_H
#define MINVOLCHART_H
/********************************************************************
created:
author: min.cheng
purpose: 分时成交量绘制
*********************************************************************/
#include "basechart.h"
#include "datamodeldef.h"
#include "datakline.h"

class ImageLayer;
class MinVolChart : public BaseChart
{
public:
    MinVolChart(ImageLayer* layer,DataKLine &vKlineData);

    void update();

    //十字光标 左侧格式化方法
    virtual QString leftTipInfoFormat(double value);

    //十字光标右侧格式化方法
    virtual QString rightTipInfoFormat(double value);

    //清理绘制数据
    void clearDrawData();
private:
    void redraw();
    void onResize();
    void onCreate();

    void drawMain(QPainter &painter);
    void drawText(QPainter &painter);
    void drawGrid(QPainter &painter);
    DataKLine &m_vKlineData;
};

#endif // MINVOLCHART_H
