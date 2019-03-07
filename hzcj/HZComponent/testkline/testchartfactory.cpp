#include "testchartfactory.h"
#include "testindicatorchart.h"
#include "testklinechart.h"
#include "testlayermanager.h"

IocContainer *testChartFactory::getIoc()
{
    static testChartFactory instance;
    return &instance.m_ioc;
}

testChartFactory::testChartFactory()
{
    m_ioc.RegisterInherit<TestBaseChart, TestKlineChart, QObject*, TestLayerManager*>("TestKlineChart");
    m_ioc.RegisterInherit<TestBaseChart, TestIndicatorChart, QObject*, TestLayerManager*>("TestIndicatorChart");
}
