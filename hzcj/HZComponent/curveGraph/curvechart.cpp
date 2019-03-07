#include "curvechart.h"

CurveChart::CurveChart(ImageLayer *imagLayer, const CurveDataList &data,CurveChartMargin margin):
    m_imageLayer(imagLayer),
    m_data(data),
    m_margin(margin)

{
    m_xaxis = new CurveXaxis(m_data);
    m_yaxis = new CurveYaxis(m_data);;
    m_style = (CurveGraphStyle*)m_imageLayer->getWnd();
    m_layerId = m_imageLayer->createLayout(QRect(m_rc.left(),m_rc.top(),m_rc.width(),m_rc.height())).type;
}

CurveChart::~CurveChart()
{
    delete m_xaxis;
    m_xaxis = nullptr;
    delete m_yaxis;
    m_yaxis = nullptr;
}

void CurveChart::updateData()
{
    m_xaxis->setTotalCount(m_data.data.size());
    m_xaxis->calc();
    m_xaxis->updateData();
    m_yaxis->updateData();
}

void CurveChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    m_wndRc = wndRc;
    m_rc.setLeft(rc.left()+m_margin.leftPadding);
    m_rc.setRight(rc.right()-m_margin.rightPadding);
    m_rc.setTop(rc.top()+m_margin.topPadding);
    m_rc.setBottom(rc.bottom()-m_margin.bottomPadding);
    QRect tempRc(wndRc.left(),wndRc.top(),wndRc.width(),wndRc.height());
    m_imageLayer->resize(tempRc,m_layerId);
    m_xaxis->resize(m_rc,wndRc);
    m_yaxis->resize(m_rc,wndRc);
}

CurveXaxis *CurveChart::getXaxis()
{
    return m_xaxis;
}

CurveYaxis *CurveChart::getYaxis()
{
    return m_yaxis;
}

QRectF CurveChart::getRc()
{
    return m_rc;
}
