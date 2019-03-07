#include "yaxis.h"
#include "basechart.h"

YAxis::YAxis(BaseChart* pchart):m_chart(pchart)
{
    resetDefaultValue();
}

double YAxis::yCanvasPTyToValue(double pt_y)
{
    double fRet = DBL_MAX;
    if(m_ratio != 0)
    {
        double fy = (double)m_rc.bottom()-pt_y;
        if(fy ==0)
        {
            fy = 1.0f;
        }
        fRet = m_minValue + fy*m_ratio;
    }
    return fRet;
}

double YAxis::yCanvasValueToPt(double value)
{
    double fRet = 0;
    if(m_ratio != 0)
    {
        fRet = (value-m_minValue)/m_ratio;
        fRet = (double)m_rc.bottom() - fRet;
    }
    if(fRet > m_rc.bottom())
        fRet = m_rc.bottom();
    else if(fRet < m_rc.top())
        fRet = m_rc.top();
    return fRet;
}

QRect YAxis::getRc()
{
    return m_chart->getChartMainArea();
}
#include <QDebug>
bool YAxis::recalc()
{
    bool bRet = false;
    if(m_chart)
    {
        m_rc = m_chart->getChartMainArea();

        m_chart->getMaxMinRange(m_maxValue,m_minValue);
        //qDebug() << "YAxis:: max min" << m_maxValue << m_minValue;
        if(m_maxValue != DBL_MIN && m_minValue != DBL_MAX)
        {
            double fHeight = (double)m_rc.height();
            m_ratio = (m_maxValue - m_minValue)/fHeight;
            if(m_ratio == 0)
                m_ratio = 1.0f;
            bRet = true;
        }
    }
    return bRet;
}

void YAxis::resetDefaultValue()
{
    m_ratio = 0;
    m_maxValue = DBL_MAX;
    m_minValue = DBL_MIN;
}

BaseChart *YAxis::getChart()
{
    return m_chart;
}

double YAxis::getMax()
{
    return m_maxValue;
}

double YAxis::getMin()
{
    return m_minValue;
}

bool YAxis::isInYaxis(const int &pt_y)
{
    bool bRet = false;
    if(pt_y >= m_rc.bottom() && pt_y < m_rc.top() || pt_y > m_rc.bottom() && pt_y <= m_rc.top())
        bRet = true;
    return bRet;
}
