#include "minvolchart.h"
#include "basecanvas.h"
#include "imagelayer.h"
#include "yaxis.h"
#include "xaxis.h"
#include "quoteformat.h"

MinVolChart::MinVolChart(ImageLayer* layer,DataKLine &vKlineData):
    BaseChart(layer),
    m_vKlineData(vKlineData)
{

}

void MinVolChart::update()
{
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    if(!m_vKlineData.m_data.isEmpty())
    {
        QVector<double>::iterator biggest = std::max_element(std::begin(m_vKlineData.m_volume), std::end(m_vKlineData.m_volume));
        QVector<double>::iterator littlegest = std::min_element(std::begin(m_vKlineData.m_volume), std::end(m_vKlineData.m_volume));
        m_chartMax = *biggest;
        m_chartmin = *littlegest;
        m_yAxis->recalc();
    }
    redraw();
}

QString MinVolChart::leftTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX && m_vKlineData.m_data.isEmpty() == false)
    {
        if(value > 1000)
        {
            retString = HzQuoteFormat::StringUnitFormat::toFormatString(value);

        }else
        {
           retString = QString::number(value, 'f', 2);
        }
    }
    return retString;
}

QString MinVolChart::rightTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX && m_vKlineData.m_data.isEmpty() == false)
    {
        if(value > 10000)
        {
            retString = HzQuoteFormat::StringUnitFormat::toFormatString(value);

        }else
        {
           retString = QString::number(value, 'f', 2);
        }
    }
    return retString;
}

void MinVolChart::clearDrawData()
{
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawText(painter);
}

void MinVolChart::onResize()
{
    BaseChart::onResize();
    redraw();
}

void MinVolChart::onCreate()
{
    m_yAxis = std::shared_ptr<YAxis>(new YAxis(this));
}

void MinVolChart::drawMain(QPainter &painter)
{
    BaseCanvas* pBaseCanvas = (BaseCanvas*)m_imageLayer->getWnd();
    QColor upClr = pBaseCanvas->getUpClr();
    QColor downClr = pBaseCanvas->getDownClr();
    double val = 0;
    double xPos = -1;
    QVector<QLineF> upVec,downVec;
    int k_status = KlineStatusDown;
    QPointF pt1,pt2;
    double top = m_yAxis->getRc().top()+1;
    pt1.setY(m_yAxis->getRc().bottom());
    if(!m_vKlineData.m_volume.isEmpty())
    {
        for(int j = 0;j < m_vKlineData.m_volume.size();j++)
        {
            val = m_vKlineData.m_volume[j];
            if(val != -1)
            {
                val = m_yAxis->yCanvasValueToPt(val);
                if(val == 0)
                    continue;
                else if(val < top)
                    val = top;
                xPos = m_xAxis->xCanvasIndexToPTx(j);
                pt1.setX(xPos);
                pt2.setX(xPos);
                pt2.setY(val);
                if(m_xAxis->getChart()->getStatusFromIndex(j,k_status))
                {
                    if(k_status == KlineStatusDown)
                    {
                        downVec.push_back(QLineF(pt1,pt2));
                    }else
                    {
                        upVec.push_back(QLineF(pt1,pt2));
                    }
                }
            }
        }
        if(upVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(upClr);
            painter.setPen(pen);
            painter.drawLines(upVec);
        }
        if(downVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(downClr);
            painter.setPen(pen);
            painter.drawLines(downVec);
        }
    }
}

void MinVolChart::redraw()
{
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter dc(drawLayer.pix);

    drawGrid(dc);
    drawText(dc);
    drawMain(dc);
}

void MinVolChart::drawText(QPainter &painter)
{
    BaseCanvas* pCanvas = (BaseCanvas*)m_imageLayer->getWnd();
    QRect chartRc = getChartMainArea();
    double row = 2;
    double gridHeight = chartRc.height()/2;
    QPoint pt1(chartRc.topLeft()),pt2(chartRc.topRight());
    QRect leftRc = getLeftAxisArea(),
          rightRc = getRightAxisArea();


    QPen pen;
    QFont ft;
    ft.setFamily(pCanvas->getYaxisFontFamily());
    ft.setPixelSize(pCanvas->getYaxisFontSize());
    ft.setStyleStrategy(QFont::NoAntialias);
    painter.setFont(ft);
    pen.setWidth(1);
    double tempVal = DBL_MAX;
    double topOffset = gridHeight/2.0;
    double bottomOffset = gridHeight/2.0;
    for(int i = 0;i <= row;i++)
    {
        if(abs(chartRc.bottom() - pt1.y()) < gridHeight/2)
        {
            pt1.setY(chartRc.bottom());
            pt2.setY(chartRc.bottom());
        }
        tempVal = m_yAxis->yCanvasPTyToValue(pt1.y());
        if(i == 0)
        {
            topOffset = 3;
            bottomOffset = 10;
        }else if(i == row)
        {
            topOffset = 10;
            bottomOffset = 3;
        }else
        {
            topOffset = 10;
            bottomOffset = 10;
        }
        //绘制左边坐标轴
        QRect tempRc = leftRc;
        tempRc.setTop(pt1.y()-topOffset);
        tempRc.setBottom(pt1.y() + bottomOffset);
        pen.setColor(pCanvas->getEqualClr());
        painter.setPen(pen);
        painter.drawText(tempRc,Qt::AlignCenter,leftTipInfoFormat(tempVal));

        //绘制右边坐标轴
        tempRc = rightRc;
        tempRc.setTop(pt2.y()-topOffset);
        tempRc.setBottom(pt2.y() + bottomOffset);
        painter.drawText(tempRc,Qt::AlignCenter,rightTipInfoFormat(tempVal));

        pt1.setY(pt1.y() + gridHeight);
        pt2.setY(pt2.y() + gridHeight);
    }
}

void MinVolChart::drawGrid(QPainter &painter)
{
    QRect rc = getChartMainArea();
    BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
    QPen pen(pWnd->getGridClr(),1);
    painter.setPen(pen);
    //painter.drawRect(rc);
    painter.drawLine(rc.left(),rc.top()+rc.height()/2,rc.right(),rc.top()+rc.height()/2);
}
