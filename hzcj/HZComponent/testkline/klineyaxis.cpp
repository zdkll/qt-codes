#include "klineyaxis.h"
#include "klinewidget.h"

KlineYaxis::KlineYaxis(QObject *parent):
    QObject(parent)
{
    m_parentWnd = qobject_cast<KlineWidget *>(parent);
}

void KlineYaxis::calc()
{

    m_parentWnd->getMaxMinRange(m_maxValue, m_minValue);

    m_ratio = (m_maxValue - m_minValue) / m_rc.height();
    if(m_ratio > -1e-6 && m_ratio < 1e-6)
        m_ratio = 1.0;

}

void KlineYaxis::resize(QRectF rc)
{
    m_rc = rc;

    calc();
}

float KlineYaxis::yPtToValue(float pt_y)
{
    float fValue = -1;

    fValue = (m_rc.bottom() - pt_y)*m_ratio + m_minValue;

    return fValue;
}

float KlineYaxis::yValueToPt(float value)
{
    float fPt = -1;

    fPt = m_rc.bottom() - (value - m_minValue) / m_ratio;

    return fPt;
}
