#ifndef SMTCURVEGRAPH_H
#define SMTCURVEGRAPH_H
/********************************************************************
created:
author: min.cheng
purpose: 创建融资融券曲线
*********************************************************************/
#include "hzinfocenter.h"
#include "curvecanvas.h"
class SmtTitleStaticChart;
class SmtTimeChart;
class CurveCrossLineChart;
class SmtCurveGraphCanvas: public CurveCanvas
{
    Q_OBJECT
public:
    SmtCurveGraphCanvas(QWidget *parent);

    void setDatas(const StockRzrqDetailList& datas);

    void setDatas(const StockMarketRzrqList& datas);

    void afterActive();

    void afterDeactive();

    void createWg();

    void setGridNum(int row,int col);
protected:
    //背景网格
    CurveGridChart* createGridChart();

    //融资余额
    CurveLineChart* createRzyeLineChart();

    //融券余额
    CurveLineChart* createRqyeLineChart();

    //时间轴绘制
    SmtTimeChart* createTimelistChart();

    //创建十字光标
    CurveCrossLineChart* createCrossLineChart();

    //标题区域绘制
    SmtTitleStaticChart* createTitleStaticChart();

    void resizeEvent(QResizeEvent *event);

    virtual void keyPressEvent(QKeyEvent *e);

//    virtual void keyReleaseEvent(QKeyEvent *e);


    void mouseMoveEvent(QMouseEvent *event);
private:
    //绘制网格背景
    CurveGridChart* m_gridChart = nullptr;
    //融资曲线
    CurveLineChart* m_lineRzyeChart = nullptr;
    //融券曲线
    CurveLineChart* m_lineRqyeChart = nullptr;
    //时间轴
    SmtTimeChart* m_timeChart = nullptr;
    //十字光标
    CurveCrossLineChart* m_crossLine = nullptr;
    //标题区域
    SmtTitleStaticChart* m_titleStaticChart = nullptr;
protected:
    CurveDataList m_rzyeData;
    CurveDataList m_rqyeData;
    int m_row = 8;
    int m_col = 5;
    QPoint m_mousePt;

    CurveXaxis* m_xaxis = nullptr;
    CurveYaxis* m_yaxis = nullptr;
};

#endif // SMTCURVEGRAPH_H
