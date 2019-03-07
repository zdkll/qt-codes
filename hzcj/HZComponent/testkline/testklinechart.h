#ifndef TESTKLINECHART_H
#define TESTKLINECHART_H
#include "testbasechart.h"


class TestKlineChart:public TestBaseChart
{
    Q_OBJECT
public:
    explicit TestKlineChart(QObject *parent = nullptr, TestLayerManager *layer = nullptr);



protected:
    //createEx()调用后，自动调用
    virtual void onCreate();

};

#endif // TESTKLINECHART_H
