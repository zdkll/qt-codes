#include "smttimechart.h"
#include <QDateTime>
#include <QPainter>
SmtTimeChart::SmtTimeChart(ImageLayer *imagLayer, const CurveDataList &data, CurveChartMargin margin):
    CurveChart(imagLayer,data,margin)
{

}

void SmtTimeChart::updateData()
{
    CurveChart::updateData();
    drawTimes();
}

void SmtTimeChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    CurveChart::resize(rc,wndRc);
    drawTimes();
}

void SmtTimeChart::drawTimes()
{
    m_imageLayer->clear(m_layerId);
    QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
    QPen pen;
    pen.setColor(m_style->getTitleTextClr());
    dc.setPen(pen);
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(11);
    font.setStyleStrategy(QFont::NoAntialias);
    dc.setFont(font);

    double width = m_rc.width()/8;
    double xPos = m_rc.left();
    int index = 0;
    QString strTime;
    QRectF rc = m_rc;
    for(int i = 0;i < 8;i++)
    {
         rc.setLeft(xPos);
         rc.setRight(xPos+width);
         index  = m_xaxis->xCanvasPtxToIndex(xPos + width/2);
         if(index != INT_MAX)
         {
             strTime = QDateTime::fromTime_t(m_data.data[index].time).toString("yyyy-MM-dd");
             dc.drawText(rc,Qt::AlignCenter,strTime);
         }
         xPos += width;
    }
}
