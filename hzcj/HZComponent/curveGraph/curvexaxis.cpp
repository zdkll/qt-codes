#include "curvexaxis.h"

CurveXaxis::CurveXaxis(const CurveDataList &data):
    m_data(data)
{

}

void CurveXaxis::setTotalCount(double count)
{
    m_totoalCount = count;
}

int CurveXaxis::getTotalCount()
{
    return m_totoalCount;
}

void CurveXaxis::setItemWidth(double width)
{
    m_ItemWidth = width;
}

int CurveXaxis::xCanvasPtxToIndex(int pt_x)
{
    int nRet = INT_MAX;
    if(!m_data.data.isEmpty())
    {
        int num = (pt_x-m_rc.left())/m_ItemWidth;
        nRet = num;
        if(nRet >= m_data.data.size())
        {
            nRet = m_data.data.size()-1;
        }
    }
    return nRet;
}

int CurveXaxis::xCanvasIndexToPTx(int index)
{
    int nRet = -1;
    int count = index+ 1 ;
    nRet = m_rc.left() + count* m_ItemWidth - m_ItemWidth/2.0; //返回每根中间位置
    return nRet;
}

void CurveXaxis::updateData()
{
    calc();
}

void CurveXaxis::resize(const QRectF& rc,const QRectF& wndRc)
{
     m_rc = rc;
     calc();
}

void CurveXaxis::calc()
{
    if(!m_data.data.isEmpty())
    {
        m_ItemWidth = m_rc.width()/m_totoalCount;
    }
}
