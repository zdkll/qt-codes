#include "curvelinechart.h"
#include "curvegridchart.h"
#include <QPainter>

#include "quoteformat.h"

CurveLineChart::CurveLineChart(ImageLayer *imagLayer, const CurveDataList &data, CurveChartMargin margin)
    :CurveChart(imagLayer,data,margin)
{
    m_gradDcId = m_imageLayer->createLayout(QRect(m_rc.left(),m_rc.top(),m_rc.width(),m_rc.height())).type;
}

void CurveLineChart::updateData()
{
    CurveChart::updateData();
    drawLine();
    drawAxis();
}

void CurveLineChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    CurveChart::resize(rc,wndRc);
    QRect tempRc(wndRc.left(),wndRc.top(),wndRc.width(),wndRc.height());
    m_imageLayer->resize(tempRc,m_gradDcId);
    drawLine();
    drawAxis();
}

void CurveLineChart::setAxisWidht(int left, int right)
{
    m_leftAxisWidth  = left;
    m_rightAxisWidth = right;
}

void CurveLineChart::setGridChart(CurveGridChart *pChart)
{
    m_pGrid = pChart;
}

void CurveLineChart::drawLine()
{
    m_imageLayer->clear(m_layerId);
    m_imageLayer->clear(m_gradDcId);
    QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
    QPainter gradDc(m_imageLayer->getLayout(m_gradDcId).pix);

    QVector<QPointF> pts;
    QVector<QPointF> avPts;
    QPainterPath path;
    double  minPty = DBL_MAX;
    double firstPtx = 0;
    QVector<QVector<QPointF>> lines;
    double xPos = 0;
    QPointF cPt,avPt;
    int dataNum = m_data.data.size();

    for(int i = 0;i < dataNum;i++)
    {
        xPos = m_xaxis->xCanvasIndexToPTx(i);
        if(i == 0)
            xPos = m_rc.left();
        else if(m_xaxis->getTotalCount()-1 == i)
            xPos = m_rc.right();
        if(m_data.data[i].price != DBL_MAX)
        {
            cPt.setX(xPos);
            cPt.setY(m_yaxis->canvasValueToPt(m_data.data[i].price));
            pts.push_back(cPt);
        }
        if(m_data.data[i].average != DBL_MAX)
        {
            avPt.setX(xPos);
            avPt.setY(m_yaxis->canvasValueToPt(m_data.data[i].average));
            avPts.push_back(avPt);
        }

        if(minPty > cPt.y())
            minPty = cPt.y();
        if(i == 0)
        {
            path.moveTo(cPt);
            firstPtx = cPt.x();
        }else
        {
           path.lineTo(cPt);
        }
        if(dataNum  == 1) //防止一根数据画不出来的问题
        {
            if(m_data.data[i].price != DBL_MAX)
            {
                cPt.setX(xPos+0.5);
                pts.push_back(cPt);
            }

            if(m_data.data[i].average != DBL_MAX)
            {
                avPt.setX(xPos+0.5);
                avPts.push_back(cPt);
            }
        }
        lines.push_back(pts);
    }
    QPen pen;
    // 价格曲线
    if(!pts.isEmpty())
    {
        pen.setColor(m_style->getMinLineClr());
        dc.setPen(pen);
        dc.drawPolyline(pts.data(),pts.size());
    }

    //均价曲线
    if(!avPts.isEmpty())
    {
        pen.setColor(m_style->getMinAvgLineClr());
        dc.setPen(pen);
        dc.drawPolyline(avPts.data(),avPts.size());
    }

    //透明渐变区域绘制
    if(!pts.isEmpty())
    {
        QRectF rc = m_rc;
        if(minPty != DBL_MAX)
        {
           path.lineTo(cPt.x(),rc.bottom());
           path.lineTo(firstPtx,rc.bottom());
           rc.setTop(minPty);
        }
        QLinearGradient linearGradient(rc.bottomLeft(),rc.topLeft());
        QColor gradClr = m_style->getGradClr();
        gradClr.setAlpha(0);
        linearGradient.setColorAt(0,gradClr);
        gradClr.setAlpha(60);
        linearGradient.setColorAt(1,gradClr);
        linearGradient.setSpread(QGradient::PadSpread);
        gradDc.setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);
        gradDc.setPen(Qt::NoPen);
        gradDc.setRenderHint(QPainter::Antialiasing, true);
        gradDc.setBrush(linearGradient);
        gradDc.setClipPath(path);
        gradDc.drawRect(rc);
    }
}

void CurveLineChart::drawAxis()
{
    if(m_pGrid)
    {
        const QVector<QLineF>& lines = m_pGrid->getChartGridLines() ;
        QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
        QPen pen;
        pen.setColor(m_style->getAxisTextClr());
        dc.setPen(pen);
        QFont font;
        font.setFamily("Arial");
        font.setPixelSize(11);
        font.setStyleStrategy(QFont::NoAntialias);
        dc.setFont(font);
        QRectF leftRc;
        leftRc.setLeft(m_wndRc.left());
        leftRc.setRight(m_rc.left());
        leftRc.setTop(m_rc.top());
        leftRc.setBottom(m_rc.bottom());
        QRectF rightRc = leftRc;
        rightRc.setLeft(m_rc.right());
        rightRc.setRight(m_wndRc.right());
        double topPrice = 0;
        QString format;
        for(int i = 0;i < lines.size();i++)
        {
            topPrice = m_yaxis->canvasPtToValue(lines[i].p1().y());
            format = HzQuoteFormat::StringUnitFormat::toFormatString(topPrice);
            //绘制左边
            if(m_leftAxisWidth != 0)
            {
                leftRc.setTop(lines[i].p1().y()-50);
                leftRc.setBottom(lines[i].p1().y()+50);
                dc.drawText(leftRc,Qt::AlignCenter,format);
            }
            //绘制右边
            if(m_rightAxisWidth != 0)
            {
                rightRc.setTop(lines[i].p1().y()-50);
                rightRc.setBottom(lines[i].p1().y()+50);
                dc.drawText(rightRc,Qt::AlignCenter,format);
            }
        }
    }
}
