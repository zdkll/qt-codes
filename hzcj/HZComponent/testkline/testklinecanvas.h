/********************************************************************
created:   2019.1.14
author:  jt.xie
purpose: K线画布
*********************************************************************/
#ifndef TESTKLINECANVAS_H
#define TESTKLINECANVAS_H
#include "testbasecanvas.h"

class TestKlineChart;
class TestKlineCanvas:public TestBaseCanvas
{
    Q_OBJECT
public:
    explicit TestKlineCanvas(QWidget *parent = nullptr);


    //设置股票代码和周期
    void setBaseInfo(const QString &strCode, int nPeriod);

private:
    TestKlineChart* getKlineChart();

};

#endif // TESTKLINECANVAS_H
