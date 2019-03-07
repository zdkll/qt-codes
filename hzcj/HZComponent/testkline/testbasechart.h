#ifndef TESTBASECHART_H
#define TESTBASECHART_H
#include "testdrawable.h"
#include "chartdef.h"

class TestXaxis;
class TestYaxis;
class TestBaseChart:public TestDrawable
{
    Q_OBJECT
public:
    explicit TestBaseChart(QObject* parent = nullptr, TestLayerManager *layer = nullptr);


    //x轴设置和获取
    void setXaxis(TestXaxis *xAxis = nullptr);
    TestXaxis *getXaxis() const;

    //y轴设置和获取
    void setYaxis(TestYaxis *yAxis = nullptr);
    TestYaxis* getYaxis() const;

    //设置区域边界
    void setMargins(const ChartMargins &margins){m_margins = margins;}
    ChartMargins getMargins() const{ return m_margins;}

protected:
    TestXaxis *m_xAxis = nullptr;
    TestYaxis *m_yAxis = nullptr;

    //边距
    ChartMargins m_margins;
};

#endif // TESTBASECHART_H
