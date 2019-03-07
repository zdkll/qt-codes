#include "curvecrosslinechart.h"
#include "quoteformat.h"
#include <QDateTime>
#include <qfontmetrics.h>
#include <QPainter>
CurveCrossLineChart::CurveCrossLineChart(ImageLayer *imagLayer, const CurveDataList &data, CurveChartMargin margin):
    CurveChart(imagLayer,data,margin)
{

}

void CurveCrossLineChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    CurveChart::resize(rc,wndRc);
    if(m_rc.contains(m_currPt))
    {
        drawLine();
    }
}

void CurveCrossLineChart::setCrossInfo(QVector<CrossInfo> &lineInfo)
{
    m_crossInfo = lineInfo;
}

void CurveCrossLineChart::move(const QPoint &pt,const QRectF& timeRc)
{
    if(m_rc.contains(pt))
    {
        m_currPt = pt;
        m_timeRc = timeRc;
        drawLine();
        m_imageLayer->getWnd()->update();
    }else if(!m_currPt.isNull())
    {
        m_currPt.setX(0);
        m_currPt.setY(0);
        m_imageLayer->clear(m_layerId);
        m_imageLayer->getWnd()->update();
    }
}

void CurveCrossLineChart::mouseLeave()
{
    m_currPt.setX(0);
    m_currPt.setY(0);
    m_imageLayer->clear(m_layerId);
    m_imageLayer->getWnd()->update();
}

void CurveCrossLineChart::drawLine()
{
    m_imageLayer->clear(m_layerId);
    QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(11);
    font.setStyleStrategy(QFont::NoAntialias);
    dc.setFont(font);
    QPen pen;
    pen.setStyle(Qt::PenStyle::DashDotLine);
    pen.setWidth(1);
    pen.setColor(QColor(m_style->getCrossLineClr()));
    dc.setPen(pen);
    //绘制垂直十字线
    QPoint pt1(m_currPt.x(),m_rc.top());
    QPoint pt2(m_currPt.x(),m_rc.bottom());
    dc.drawLine(pt1,pt2);

    //绘制水平十字线
    QPoint pt3(m_rc.left(),m_currPt.y());
    QPoint pt4(m_rc.right(),m_currPt.y());
    dc.drawLine(pt3,pt4);


    int index = 0;
    double price;
    QString strFormat;
    CurveXaxis* xaxis = nullptr;
    for(int i = 0;i < m_crossInfo.size();i++)
    {
        xaxis = m_crossInfo[i].m_xaxis;
        strFormat = Default_Item_String;
        index = m_crossInfo[i].m_xaxis->xCanvasPtxToIndex(m_currPt.x());
        if(index != INT_MAX)
        {
            //绘制交叉点
            price = m_crossInfo[i].isAverage?m_crossInfo[i].data->data[index].average:m_crossInfo[i].data->data[index].price;
            QPoint closePt;
            closePt.setX(m_currPt.x());
            closePt.setY(m_crossInfo[i].m_yaxis->canvasValueToPt(price));
            QColor clr = Qt::red;
            if(m_crossInfo[i].isAverage)
            {
                clr = m_style->getMinAvgLineClr();
            }else
            {
                clr = m_style->getMinLineClr();
            }
            QPen pen;
            pen.setColor(clr);
            QBrush brush(clr);
            dc.setPen(clr);
            dc.setBrush(brush);
            dc.drawEllipse(closePt,3,3);
            dc.setBrush(Qt::NoBrush);
        }
        //绘制左提示文本
        if(m_crossInfo[i].axisDrawType & AXIS_LEFT_DRAW_TYPE)
        {
            price = m_crossInfo[i].m_yaxis->canvasPtToValue(m_currPt.y());
            strFormat = HzQuoteFormat::StringUnitFormat::toFormatString(price);

            QRectF rc;
            rc.setTop(m_currPt.y() - 10);
            rc.setBottom(m_currPt.y() + 10);
            rc.setLeft(m_wndRc.left());
            rc.setRight(m_rc.left());
            pen.setColor(QColor(m_style->getCrossLineClr()));
            dc.setPen(pen);
            dc.fillRect(rc,QColor(m_style->getCrossLineRcBkClr()));
            dc.drawRect(rc);
            pen.setColor(QColor(m_style->getCrossLineTextClr()));
            dc.setPen(pen);
            dc.drawText(rc,Qt::AlignCenter,strFormat);
        }
        //绘制右提示文本
        if(m_crossInfo[i].axisDrawType & AXIS_RIGHT_DRAW_TYPE )
        {
            price = m_crossInfo[i].m_yaxis->canvasPtToValue(m_currPt.y());
            strFormat = HzQuoteFormat::StringUnitFormat::toFormatString(price);

            QRectF rc;
            rc.setTop(m_currPt.y() - 10);
            rc.setBottom(m_currPt.y() + 10);
            rc.setLeft(m_rc.right());
            rc.setRight(m_wndRc.right());
            pen.setColor(QColor(m_style->getCrossLineClr()));
            dc.setPen(pen);
            dc.fillRect(rc,QColor(m_style->getCrossLineRcBkClr()));
            dc.drawRect(rc);
            pen.setColor(QColor(m_style->getCrossLineTextClr()));
            dc.setPen(pen);
            dc.drawText(rc,Qt::AlignCenter,strFormat);
        }
    }

    //绘制时间轴区域 X轴时间提示框
    QString formatStr = "yyyy-MM-dd";
    QString strTime = "--:--";
    if(xaxis)
    {
        index  = xaxis->xCanvasPtxToIndex(m_currPt.x());
        if(index != INT_MAX)
        {
            QDateTime dateTime;
            dateTime.setTime_t(m_data.data[index].time);
            strTime = dateTime.toString(formatStr);
        }
    }
    QFontMetrics fm(font);
    int width = fm.boundingRect(strTime).width()+5;
//    if(width < YaxisKlineMinWidth)
//        width = YaxisKlineMinWidth;

    QRectF xTipRc = m_timeRc;
    xTipRc.setLeft(m_currPt.x()-width/2);
    xTipRc.setRight(xTipRc.left()+ width);


    pen.setStyle(Qt::PenStyle::SolidLine);
    pen.setWidth(1);
    pen.setColor(QColor(m_style->getCrossLineClr()));
    dc.setPen(pen);
    dc.fillRect(xTipRc,QColor(m_style->getCrossLineRcBkClr()));
    dc.drawRect(xTipRc);

    pen.setColor(QColor(m_style->getCrossLineTextClr()));
    dc.setPen(pen);
    dc.drawText(xTipRc,Qt::AlignCenter,strTime);
}
