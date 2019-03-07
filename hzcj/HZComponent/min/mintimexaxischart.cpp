#include "minTimeXaxisChart.h"
#include <QDateTime>
#include "imagelayer.h"
#include "xaxis.h"
#include "yaxis.h"
#include "minchart.h"
#include "mincanvas.h"
#include "chartmodle.h"


MinTimeXaxisChart::MinTimeXaxisChart(ImageLayer *layer):BaseChart(layer)
{

}

QString MinTimeXaxisChart::formatTime(double tm1,const QString& format)
{

    QDateTime dateTime;
    dateTime.setTime_t(tm1);
    return dateTime.toString(format);
}

void MinTimeXaxisChart::mainChartXaixsChnage(const QString &msg)
{
    redraw();
}

bool MinTimeXaxisChart::mouseMoveEx(QMouseEvent *event)
{
    return true;
}

void MinTimeXaxisChart::onResize()
{
    BaseChart::onResize();
    redraw();
}

void MinTimeXaxisChart::redraw()
{
    MinCanvas* pCanvas = (MinCanvas*)m_imageLayer->getWnd();
    Minchart* pChart = (Minchart*)m_xAxis->getChart();
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter;
    painter.begin(drawLayer.pix);
    QPen pen;
    pen.setColor(pCanvas->getXaxisTextClr());
    QFont font;
    font.setFamily(pCanvas->getXaxisFontFamily());
    font.setPixelSize(pCanvas->getXaxisFontSize());
    font.setStyleStrategy(QFont::NoAntialias);

    painter.setFont(font);
    painter.setPen(pen);


    const QString& obj = pChart->getObj();
    const DataKLine& data = pChart->getMinData();
    const QVector<int>& dayListIndex = pChart->getDayIndexList();
    int day = pChart->getDayCount();
    int index = 0;
    double pos = 0.0;
    QRectF rc = m_rc;
    QString strTime;

    //时间刻度点
    QVector<int> timePos;
    //绘制除当日外的时间轴
    for(int i=0;i < dayListIndex.size();i++)
    {
        if(i == dayListIndex.size()-1)
            break; //最后一天特殊绘制坐标轴
        pos = m_xAxis->xCanvasIndexToPTx(index);
        rc.setLeft(pos);
        rc.setRight(pos + 50);
        if(dayListIndex[i] < data.m_updateTime.size())
            strTime = formatTime(data.m_updateTime[dayListIndex[i]]);
        painter.drawText(rc,Qt::AlignLeft|Qt::AlignVCenter,strTime);
        index = dayListIndex[i];
        timePos.push_back(rc.left());
    }

    //绘制当日时间轴
    double col = day* ChartModle::getInstance()->getTradeNode(obj).size() * 2;
    double gridWidth = pChart->getChartMainArea().width()/col;

    //格式化当天日期
    strTime = "--/--";
    if(!data.m_updateTime.isEmpty())
        strTime = formatTime(data.m_updateTime[data.m_updateTime.size()-1]);
    QVector<QString> timeList = ChartModle::getInstance()->getMinXaxisStrTimeList(obj);
    timeList.push_front(strTime);

    pos = m_xAxis->xCanvasIndexToPTx(index);
    for(int i = 0;i < timeList.size();i++)
    {
        rc.setLeft(pos-25);
        rc.setRight(pos + 25);
        painter.drawText(rc,Qt::AlignCenter,timeList[i]);
        timePos.push_back(rc.left());
        pos += gridWidth*2;
        if(day >= 5) //显示大于等于5日数据时，其他交易时刻就不用再绘制了
            break;
    }

    //绘制刻度
    QPoint pt1,pt2;
    pen.setWidth(1);
    pen.setColor(QColor("#030303"));
    painter.setPen(pen);
    if(timePos.size() >=2)
    {
        for(int i=0;i < timePos.size();i++)
        {
            if(i==0)
            {
                rc = getChartMainArea();
                pt1.setX(rc.left());
                pt1.setY(m_rc.top());

                pt2.setX(rc.right());
                pt2.setY(m_rc.top());
                painter.drawLine(pt1,pt2);
            }else
            {
                pt1.setX(timePos[i]);
                pt1.setY(m_rc.top());

                pt2.setX(timePos[i]);
                pt2.setY(m_rc.top()+12);
                painter.drawLine(pt1,pt2);
            }
        }
    }
    painter.end();
}
