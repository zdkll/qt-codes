/********************************************************************
created:   2019.1.14
author:  jt.xie
purpose: 画布基类
*********************************************************************/
#ifndef TESTBASECANVAS_H
#define TESTBASECANVAS_H
#include "basewidget.h"
#include "chartdef.h"

class TestBaseChart;
class TestLayerManager;
class TestXaxis;
class TestBaseCanvas:public BaseWidget
{
    Q_OBJECT
public:
    explicit TestBaseCanvas(QWidget *parent = nullptr);

    void setChartsInfo(const QVector<ChartItemInfo> &info);

    void createWg();

private:
    //计算chart区域
    QVector<QRect> calcChartRc(const QVector<ChartItemInfo> &info);

    //获取固定高度
    int getHeightFromFixed(const QVector<ChartItemInfo> &info);
    //创建chart
    TestBaseChart* createChart(QRect rc, const ChartItemInfo &info, TestXaxis *pXaxis);

protected:
    //主图
    TestBaseChart* m_mainChart = nullptr;
    //图表容器
    QVector<TestBaseChart *> m_chartVec;

    //图层管理
    TestLayerManager* m_layerManager = nullptr;

    //图表信息
    QVector<ChartItemInfo> m_chartsInfo;
};

#endif // TESTBASECANVAS_H
