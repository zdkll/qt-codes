#ifndef CURVEGRIDCHART_H
#define CURVEGRIDCHART_H
/********************************************************************
created:
author: min.cheng
purpose: 曲线网格绘制
*********************************************************************/
#include "curvechart.h"
class CurveGridChart:public CurveChart
{
public:
    CurveGridChart(ImageLayer* imagLayer,const CurveDataList& data,CurveChartMargin margin =  CurveChartMargin(0,0,0,0));

    void setGridNum(int row,int col);

    void resize(const QRectF& rc,const QRectF& wndRc);

    const QVector<QLineF>& getChartGridLines() const;

    const QVector<int>& getVerPtxs() const;
protected:
    void calcRc();
    void drawGrid();
    QVector<QLineF> m_chartGridLines; //网格水平线
    QVector<int> m_verPtxs; //垂直点
    int m_gridRow = 6;
    int m_gridCol = 5;
};

#endif // CURVEGRIDCHART_H
