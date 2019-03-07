#include "xaxis.h"
#include "chartdef.h"
#include "basechart.h"
#include "imagelayer.h"

XAxis::XAxis(BaseChart* pchart):
      m_chart(pchart),
      m_crossXindex(-1)
{
    m_crossXindex = 0;
    resetDefaultVaue();
}

int XAxis::xCanvasPTxToIndex(int pt_x)
{
    int nRet = -1;
    if(isInXaxis(pt_x))
    {
        const QVector<double>&  data = m_chart->getScreenData().m_updateTime;
        if(pt_x == m_rc.left())
            return 0;
        if(pt_x == m_rc.right())
            return data.size()-1;
        nRet = (pt_x-m_rc.left())/m_singleAreaWidth;
        if(nRet >= data.size())
        {
            nRet = data.size()-1;
        }

    }
    return nRet;
}

int XAxis::xCanvasIndexToPTx(int index,bool bMid)
{
    int nRet = -1;
    double count = index + 1;
    nRet = m_rc.left() + count* m_singleAreaWidth;
    //返回每根中间位置
    if(bMid)
        nRet -= m_singleAreaWidth/2.0;
    if(nRet < m_rc.left())
        nRet = m_rc.left();
    if(nRet > m_rc.right())
        nRet = m_rc.right();
    return nRet;
}

qint64 XAxis::xCanvasPTxToTime(int pt_x)
{
    qint64 nRet = -1;
    int nIndex = xCanvasPTxToIndex(pt_x);
    if(nIndex != -1)
    {
        const QVector<double>&  data = m_chart->getScreenData().m_updateTime;
        if(nIndex >=0 && nIndex < data.size())
        {
            nRet = data[nIndex];
        }
    }
    return nRet;
}

qint64 XAxis::xCanvasIndexToTime(int nIndex)
{
    qint64 nRet = -1;
    if(nIndex != -1)
    {
        const QVector<double>&  data = m_chart->getScreenData().m_updateTime;
        if(nIndex >=0 && nIndex < data.size())
        {
            nRet = data[nIndex];
        }
    }
    return nRet;
}

double XAxis::xGetPriceByIndex(int index)
{
    double price  = 0;
    const QVector<double> priceList = m_chart->getScreenData().m_closePrice;
    if(priceList.isEmpty() == false && index < priceList.size() && index >= 0)
    {
        price  =  priceList[index];
    }
    return price;
}

//#include <QDebug>
//int XAxis::xCanvasTimeToPTx(qint64 t)
//{
//    int nRet = -1;
//    const std::map<qint64, int> &timeToIndexMap = m_chart->getTimeToIndexMap();
//    auto it = timeToIndexMap.find(t);
//    if(it != timeToIndexMap.end())
//    {
//        int nIndex = it->second;
//        qDebug() << "XAxis-nIndex:" <<  nIndex;
//        int count = nIndex - m_begin + 1;
//        qDebug() << "XAxis-count:" <<  count;
//        nRet = m_rc.left() + count* m_singleAreaWidth - m_singleAreaWidth/2; //返回每根中间位置
//        qDebug() << "XAxis-y-pt:" <<  nRet;
//    }
//    return nRet;
//}

void XAxis::setoffset(int pos)
{
    m_offset = pos;
}

int XAxis::getoffset()
{
    return m_offset;
}

int XAxis::getCount()
{
    return m_count;
}

void XAxis::setSingleAreaWidth(double width)
{
    m_singleAreaWidth = width;
//    if(width > KlineMaxWidth)
//        m_singleAreaWidth = KlineMaxWidth;
//    if(width < KlineMinWidth)
//        m_singleAreaWidth = KlineMinWidth;
}

double XAxis::getSingleAreaWidth()
{
    return m_singleAreaWidth;
}

qint64 XAxis::getBeginTime()
{
    return m_beginTime;
}

qint64 XAxis::getEndTime()
{
    return m_endTime;
}

int XAxis::getEndIndex()
{
    return m_end;
}

int XAxis::getBeginIndex()
{
    return m_begin;
}

bool XAxis::recalc(bool bReCalc)
{
    bool bret  =false;
    if(m_chart)
    {
        m_rc = m_chart->getChartMainArea();
        const QVector<double>&  data = m_chart->getScreenData().m_updateTime;
        if(data.isEmpty() == false)
        {
            if(bReCalc)
            {
                m_singleAreaWidth =((double)m_rc.width())/data.size();
                 //m_count = data.size();
            }/*else
            {
                m_count = (double)m_rc.width() / m_singleAreaWidth;
            }*/
            m_count = data.size();
            m_beginTime = data[0];
            m_endTime = data[data.size() -1];

            m_end = 0;
            m_begin= 0;
        }
        bret = true;
    }
    return bret;
}

QRect XAxis::getRc()
{
    return m_chart->getChartMainArea();
}

double XAxis::calcDataSize()
{
    return (double)m_chart->getChartMainArea().width()/m_singleAreaWidth;
}

int XAxis::calcDataSize(double fwidth)
{
    return m_chart->getChartMainArea().width()/fwidth;
}

void XAxis::setCurrCrossXIndex(int index)
{
    m_crossXindex = index;
}

int XAxis::getCurrCrossXIndex()
{
    return m_crossXindex;
}

Events<XAxisDataChangeCallBack>& XAxis::getObserverMgr()
{
    return m_observerMgr;
}

BaseChart *XAxis::getChart()
{
    return m_chart;
}

void XAxis::resetDefaultVaue()
{
    m_offset = 0;
    m_begin = m_end = 0;
    m_beginTime = m_beginTime = -1;
    m_rc = m_chart->getChartMainArea();
    m_singleAreaWidth = KlineDefaultWidth;
    if(m_chart->getAttr()&ChartThumbnailChart)
        m_singleAreaWidth = KlineThumDefaultWidth;
    m_count = 100;
    if(m_rc.isEmpty() == false)
    {
        m_count = m_rc.width()/m_singleAreaWidth;
    }
}

bool XAxis::isInXaxis(const int &pt_x)
{
    bool bRet = false;
    if(pt_x >= m_rc.left() && pt_x < m_rc.right() || pt_x >= m_rc.left() && pt_x <= m_rc.right())
        bRet = true;
    return bRet;
}
