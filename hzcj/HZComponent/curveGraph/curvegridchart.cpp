#include "curvegridchart.h"
#include <QPainter>

CurveGridChart::CurveGridChart(ImageLayer *imagLayer, const CurveDataList &data, CurveChartMargin margin):
    CurveChart(imagLayer,data,margin)
{

}

void CurveGridChart::setGridNum(int row, int col)
{
    m_gridRow  =row;
    m_gridCol = col;
}

void CurveGridChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    CurveChart::resize(rc,wndRc);
    calcRc();
    drawGrid();
}

const QVector<QLineF> &CurveGridChart::getChartGridLines() const
{
    return m_chartGridLines;
}

const QVector<int> &CurveGridChart::getVerPtxs() const
{
    return m_verPtxs;
}

void CurveGridChart::calcRc()
{
    //主图区域网格
    m_chartGridLines.clear();
    QPointF pt1,pt2;
    int height = m_rc.height()/m_gridRow;
    pt1.setX(m_rc.left());
    pt2.setX(m_rc.right());
    m_chartGridLines.clear();
    for(int i =0;i < m_gridRow;i++)
    {
        pt1.setY(height*i+m_rc.top());
        pt2.setY(pt1.y());
        m_chartGridLines.push_back(QLineF(pt1,pt2));
    }
    m_chartGridLines.push_back(QLineF(m_rc.bottomLeft(),m_rc.bottomRight()));

    m_verPtxs.clear();
    int width = m_rc.width()/m_gridCol;
    for(int i=1;i< m_gridCol;i++)
    {
        m_verPtxs.push_back(m_rc.left()+i*width);
    }
}

void CurveGridChart::drawGrid()
{
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_layerId);
    QPainter painter(drawLayer.pix);
    QPen pen;
    pen.setColor(m_style->getGridClr());
    painter.setPen(pen);
    //绘制主图区域网格
    painter.drawLine(m_rc.topLeft(),m_rc.bottomLeft());
    painter.drawLine(m_rc.topRight(),m_rc.bottomRight());
    painter.drawLines(m_chartGridLines);

    for(int i=0;i < m_verPtxs.size();i++)
    {
        painter.drawLine(QPoint(m_verPtxs[i],m_rc.top()),
                         QPoint(m_verPtxs[i],m_rc.bottom()));
    }
}
