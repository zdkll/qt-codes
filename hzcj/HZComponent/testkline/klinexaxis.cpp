#include "klinexaxis.h"
#include "klinewidget.h"


KlineXaxis::KlineXaxis(QObject *parent):
    QObject(parent)
{
    m_parentWnd = qobject_cast<KlineWidget *>(parent);

}

//void KlineXaxis::calc()
//{
//    m_nItemWidth =  m_rc.width() / m_data->m_updateTime.size();
//}

int KlineXaxis::xPtxToIndex(int pt_x)
{
    int nRet = -1;
    m_data = m_parentWnd->getKlineData();
    if(!m_data->m_updateTime.isEmpty())
    {
        nRet = (pt_x - m_rc.left()) / m_singleAreaWidth;

        if(nRet >= m_data->m_updateTime.size() -1)
            nRet =  m_data->m_updateTime.size() -1;
    }

    return nRet;
}

int KlineXaxis::xIndexToPtx(int index)
{
    int nRet = -1;
    int nCount = index + 1;
    nRet = m_rc.left() + nCount *m_singleAreaWidth - m_singleAreaWidth / 2;
    return nRet;
}

void KlineXaxis::resize(QRectF rc)
{
    m_rc = rc;
//    calc();
}

void KlineXaxis::setSingleAreaWidth(float &width)
{
    m_singleAreaWidth = width;
}
