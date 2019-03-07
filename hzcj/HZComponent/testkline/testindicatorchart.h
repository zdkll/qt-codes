#ifndef TESTINDICATORCHART_H
#define TESTINDICATORCHART_H
#include "testbasechart.h"


class TestIndicatorChart:public TestBaseChart
{
public:
    TestIndicatorChart(QObject* parent = nullptr, TestLayerManager* layer = nullptr);
};

#endif // TESTINDICATORCHART_H
