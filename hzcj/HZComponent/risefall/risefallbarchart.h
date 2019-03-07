/********************************************************************
created:
author: dk.zhang
purpose:
*********************************************************************/
#ifndef RISEFALLBARCHART_H
#define RISEFALLBARCHART_H

#define LEGEND_MARKS_HEIGHT  36
#define LEGEND_MARKS_WIDTH   35

#define CHART_BAR_WIDTH          40
#define CHART_MARK_DISTANCE  29

#define CHART_BAR_MIN_DISTANCE      1

#define CHART_SPACING                          11
#define CHART_VERT_SCALE_WIDTH      40

#define  LEGEND_DISTANCE                     70
#define  LEGEND_RECT_WIDTH                5
#define  LEGEND_TEXT_OFFSE                6

#include <QWidget>
#include <QFont>
#include <QDebug>
#include "datamodeldefs/stockinfodef.h"

struct RiseFallBarUint
{
    int         value = 0;
    QString label;
    QColor  color;
    float      maxAlpha = 0.2f;
    float      minAlpha  = 0.2f;
};

typedef QVector<RiseFallBarUint> RiseFallBarSet;
typedef QVector<RiseFallBarUint> RiseFallLegendSet;

struct RiseFallBarData
{
    RiseFallBarSet       barSet;
    int                          maxValue;
};

class BaseRiseFallChart : public QObject
{
public:
    enum   PaintAction{NODRAW = 0,DRAW,OBJDRAW,REDRAW};
    explicit BaseRiseFallChart(QObject *parent = 0);

    void initial(QWidget *wgRect);

    void setData(const HZData::StockRiseFallInfo &risefallInfo);

    void draw();
    void objDraw();
    void reDraw();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void paintOnWidget(QWidget *wg);

    virtual void drawPixmap(QPixmap *pix) = 0;
    virtual void calData(const HZData::StockRiseFallInfo &risefallInfo) = 0;

protected:
    PaintAction  m_paintAction;
    QPixmap      m_pixmap;

    QWidget     *m_wgRect;
    QSize          m_graphSize;

    QFont  m_txtfont;
};

//图像区
class RiseFallBarChart : public BaseRiseFallChart
{
public:
    explicit RiseFallBarChart(QObject *parent = 0);

protected:
    void drawPixmap(QPixmap *pix);
    void calData(const HZData::StockRiseFallInfo &risefallInfo);

private:
    struct XScaleUnit{
        XScaleUnit(){}
        XScaleUnit(float dis,float w):distance(dis),width(w){}
        float distance;  //像素
        float width;      //Bar 宽度
    };
    struct YScaleUnit
    {
        int    dvalue;
        float distance; //像素
        int    num;
        int    max;
     };
    void drawBar(QPainter *pt,const XScaleUnit &XscaleUnit,const RiseFallBarSet &barSet,int maxValue,int totalHeihgt,int txtHeight);
    void drawVertScale(QPainter *pt,YScaleUnit Yunit,int txtHeight,int graphWidth);

    void      initData();

    XScaleUnit getXScaleUnit(int size,int totalWidth, int barWidth = CHART_BAR_WIDTH,int minDis = CHART_BAR_MIN_DISTANCE);
    YScaleUnit getYScaleUnit(int max,int totalHeight,int height      = CHART_MARK_DISTANCE);

private:
    RiseFallBarData     m_risefallBarData;
    YScaleUnit             m_YScaleUnit;
};

//legend 标识区
class RiseFallLegendChart : public BaseRiseFallChart
{
public:
    explicit RiseFallLegendChart(QObject *parent = 0);

protected:
    void drawPixmap(QPixmap *pix);
    void calData(const HZData::StockRiseFallInfo &risefallInfo);

private:
    void initData();

private:
    RiseFallLegendSet    m_riseFallLegendSet;
};
#endif // RISEFALLBARCHART_H
