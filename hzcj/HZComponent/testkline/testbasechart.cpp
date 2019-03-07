#include "testbasechart.h"

TestBaseChart::TestBaseChart(QObject* parent, TestLayerManager *layer):
    TestDrawable(parent, layer)
{

}

void TestBaseChart::setXaxis(TestXaxis *xAxis)
{
    if(m_xAxis != xAxis)
    {
        m_xAxis = xAxis;
    }
}

TestXaxis *TestBaseChart::getXaxis() const
{
    return m_xAxis;
}

void TestBaseChart::setYaxis(TestYaxis *yAxis)
{
    if(m_yAxis != yAxis)
    {
        m_yAxis = yAxis;
    }
}

TestYaxis *TestBaseChart::getYaxis() const
{
    return m_yAxis;
}

