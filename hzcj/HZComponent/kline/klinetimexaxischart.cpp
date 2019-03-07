#include "klinetimexaxischart.h"
#include <QDateTime>


#include "imagelayer.h"
#include "xaxis.h"
#include "yaxis.h"
#include "klinecanvas.h"
#include "klinechart.h"


KlineTimeXaxisChart::KlineTimeXaxisChart(ImageLayer *layer):BaseChart(layer)
{

}

void KlineTimeXaxisChart::drawText(QPainter &painter)
{
    //时间刻度点
    QVector<int> timePos;
    const QVector<double>& timeList = getXAxis()->getChart()->getScreenData().m_updateTime;
    if (timeList.isEmpty())
        return;
    QRect rc = getChartMainArea();
    int col = rc.width()/KlineTimeXaxisMinWidth;
    double fWidth = rc.width()/col;
    QFontMetrics metrix = painter.fontMetrics();
    int nHeight = metrix.height();
    QString strFormat,strTime;
    int index = 0;
    qint64 pre_time = -1,curr_time=-1;
    KlineCanvas* pKline = (KlineCanvas*)m_imageLayer->getWnd();
    painter.setPen(QColor(pKline->getXaxisTextClr()));
    QRect textRc = rc;
    //循环画第二个到第 XaxisSeparate - 1个点
    for(int i = 0; i < col+1; i++)
    {
        if(i != 0)
        {
            index = i-1;
        }
        pre_time = getXAxis()->xCanvasPTxToTime(fWidth*index+rc.left());
        curr_time = getXAxis()->xCanvasPTxToTime(fWidth*i+rc.left());
        if(pre_time == -1 || curr_time == -1 ||(i != 0 && pre_time == curr_time))
            break;
        strFormat = formatTime(curr_time,pre_time);
        strTime = QDateTime::fromTime_t(curr_time).toString(strFormat);
        textRc.setLeft(fWidth*i+rc.left());
        textRc.setRight(textRc.left()+fWidth);
        painter.drawText(textRc,Qt::AlignCenter,strTime);
        //painter.drawText(fWidth*i+rc.left(), rc.top()+ nHeight / 2 ,strTime);
        timePos.push_back(fWidth*i+rc.left()-5);
    }

    //绘制刻度
    QPoint pt1,pt2;
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(pKline->getXaxisScaleClr()));
    painter.setPen(pen);
    if(timePos.size() >=2)
    {
        for(int i=0;i < timePos.size();i++)
        {
            if(i==0)
            {
                rc = getChartMainArea();
                pt1.setX(rc.left());
                pt1.setY(rc.top());

                pt2.setX(rc.right());
                pt2.setY(rc.top());
                painter.drawLine(pt1,pt2);
            }else
            {
                pt1.setX(timePos[i]);
                pt1.setY(rc.top());

                pt2.setX(timePos[i]);
                pt2.setY(rc.top()+12);
                painter.drawLine(pt1,pt2);
            }
        }
    }

}

QString KlineTimeXaxisChart::formatTime(double tm1, double tm2)
{
    int nPeriod = getPeriod();

    QDateTime dateTime1 = QDateTime::fromTime_t(tm1);
    QDateTime dateTime2 = QDateTime::fromTime_t(tm2);
    int year1, year2, month1, month2,day1, day2;

    year1 = dateTime1.date().year();
    year2 = dateTime2.date().year();
    month1 = dateTime1.date().month();
    month2 = dateTime2.date().month();
    day1 = dateTime1.date().day();
    day2 = dateTime2.date().day();

    if( HZData::KLINE_PERIOD_MIN <= nPeriod && nPeriod <= HZData::KLINE_PERIOD_HOUR)
    {

        if(year1 != year2)
        {
            return "yyyy/MM";
        }
        else if(day1 != day2)
        {
            return "MM/dd";
        }
        else
        {
            return "hh:mm";
        }
    }
    else if(HZData::KLINE_PERIOD_DAY <= nPeriod && nPeriod <= HZData::KLINE_PERIOD_WEEK)
    {
        if(month1 != month2 || year1 != year2)
        {
            return "yyyy/MM";
        }else
        {
            return "dd";
        }
    }
    else if(HZData::KLINE_PERIOD_MONTH <= nPeriod && nPeriod <= HZData::KLINE_PERIOD_MONTH)
    {
        return "yyyy/MM";
    }
    return "";
}

int KlineTimeXaxisChart::getPeriod()
{
    KlineChart* pChart = (KlineChart*)getXAxis()->getChart();
    return pChart->getPeriod();
}

void KlineTimeXaxisChart::mainChartXaixsChnage(const QString &msg)
{
    redraw();
}

void KlineTimeXaxisChart::onResize()
{
    BaseChart::onResize();
    redraw();
}

void KlineTimeXaxisChart::redraw()
{
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter;
    painter.begin(drawLayer.pix);
    drawText(painter);
    painter.end();
}
