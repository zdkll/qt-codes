#include "curveyaxis.h"
CurveYaxis::CurveYaxis(const CurveDataList &data):
    m_data(data)
{

}

double CurveYaxis::canvasPtToValue(double pt_y)
{
    double fRet = DBL_MAX;
    if(!m_data.data.isEmpty())
    {
        double fy = (double)m_rc.bottom()-pt_y;
        if(fy ==0)
        {
            fy = 1.0f;
        }
        fRet = m_data.min + fy*m_ratio;
    }
    return fRet;
}

double CurveYaxis::canvasValueToPt(double value)
{
    double fRet = DBL_MAX;
    if(!m_data.data.isEmpty())
    {
        fRet = m_rc.bottom() - (value-m_data.min)/m_ratio;
    }
    return fRet;
}

void CurveYaxis::updateData()
{
    calc();
}

void CurveYaxis::resize(const QRectF& rc,const QRectF& wndRc)
{
    m_rc = rc;
    calc();
}

void CurveYaxis::calc()
{
    m_ratio = (m_data.max-m_data.min)/m_rc.height();
    if(m_ratio == 0)
        m_ratio = 1.0f;
}
