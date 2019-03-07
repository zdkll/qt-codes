#include "risefallflatbasewidget.h"
#include <QPainter>
#include <QDebug>
RiseFallFlatBaseWidget::RiseFallFlatBaseWidget(QWidget *parent, QString instName)
    :BaseWidget(parent,instName)
{
    m_dataChanged = false;
}

void RiseFallFlatBaseWidget::slotsItemRiseFallFlatChanged(const QString& bkStock,int rise, int fall, int flat)
{
    if(m_stockCode == bkStock && m_rise == rise && m_fall == fall && m_flat == flat)
        return;
    m_dataChanged = true;
    m_stockCode = bkStock;
    m_max = rise;
    m_rise = rise;
    m_fall = fall;
    m_flat = flat;
    if(m_max < m_fall)
        m_max = m_fall;
    if(m_max < m_flat)
        m_max = m_flat;
    m_total = m_rise+m_fall+m_flat;

    m_riseRate = m_total==0?0:m_rise/m_total;
    m_fallRate = m_total==0?0:m_fall/m_total;
    m_flatRate = m_total==0?0:m_flat/m_total;
    this->update();
}

void RiseFallFlatBaseWidget::paintEvent(QPaintEvent *e)
{
    BaseWidget::paintEvent(e);
    qDebug() << "RiseFallFlatBaseWidget::paintEvent";
    draw();
}

void RiseFallFlatBaseWidget::draw()
{

}

RiseFallFlatRectChartWnd::RiseFallFlatRectChartWnd(QWidget *parent, QString instName)
    :RiseFallFlatBaseWidget(parent,instName)
{

}

void RiseFallFlatRectChartWnd::draw()
{
    if(!m_dataChanged)
        return;
    QRectF rc = getRc();
    QRectF wndRc = rc;
    QPainter painter(this);

//    QPen pen1(Qt::red);
//    pen1.setm_width(3);
//    painter.setPen(pen1);
//    painter.drawRect(m_chartRc);
//    painter.drawRect(rc);

    //计算绘图区域
    QRectF rcRise = m_rcRiseText;
    QRectF rcFall = m_rcFallText;
    QRectF rcFlat = m_rcFlatText;

    QPen pen(m_clrText);
    painter.setPen(pen);
    QFont font;
    font.setFamily("SimSun");
    font.setPixelSize(12);
    painter.setFont(font);

    //绘制文本
    painter.drawText(m_rcRiseText,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("上涨"));
    painter.drawText(m_rcFallText,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("下跌"));
    painter.drawText(m_rcFlatText,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("平家"));

    //绘制文本柱状分割线
    pen.setColor(m_clrLine);
    painter.setPen(pen);
    painter.drawLine(m_linePt1,m_linePt2);

    //绘制柱状
    double rate = 0;
    if(m_max != INT_MIN)
        rate = (double)m_chartRc.height()/m_max;
    double chartHight = 0;
    if(m_rise > 0)
    {
        chartHight =(double)m_rise*rate;
        rcRise.setBottom(m_chartRc.bottom());
        rcRise.setTop(rcRise.bottom()-chartHight);
        painter.fillRect(rcRise,m_upClr);

        //绘制文本
        rc = rcRise;
        rc.setTop(wndRc.top());
        rc.setBottom(rcRise.top()-5);
        pen.setColor(m_upClrText);
        font.setFamily("Arial");
        font.setPixelSize(13);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(pen);
        painter.drawText(rc,Qt::AlignHCenter|Qt::AlignBottom,QString::number(m_rise));

    }
    if(m_fall > 0)
    {
        chartHight =(double)m_fall*rate;
        rcFall.setBottom(m_chartRc.bottom());
        rcFall.setTop(rcFall.bottom()-chartHight);
        painter.fillRect(rcFall,m_downClr);

        //绘制文本
        rc = rcFall;
        rc.setTop(wndRc.top());
        rc.setBottom(rcFall.top()-5);
        pen.setColor(m_downClrText);
        font.setFamily("Arial");
        font.setPixelSize(13);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(pen);
        painter.drawText(rc,Qt::AlignHCenter|Qt::AlignBottom,QString::number(m_fall));
    }
    if(m_flat > 0)
    {
        chartHight =(double)m_flat*rate;
        rcFlat.setBottom(m_chartRc.bottom());
        rcFlat.setTop(rcFlat.bottom()-chartHight);
        painter.fillRect(rcFlat,m_equalClrText);

        //绘制文本
        rc = rcFlat;
        rc.setTop(wndRc.top());
        rc.setBottom(rcFlat.top()-5);
        pen.setColor(m_equalClrText);
        font.setFamily("Arial");
        font.setPixelSize(13);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(pen);
        painter.drawText(rc,Qt::AlignHCenter|Qt::AlignBottom,QString::number(m_flat));
    }
}

void RiseFallFlatRectChartWnd::resizeEvent(QResizeEvent *event)
{
    QRectF rc =  getRc();
    m_chartRc = rc;
    m_chartRc.setLeft(rc.left()+25);
    m_chartRc.setRight(rc.right()-25);
    m_chartRc.setTop(rc.top()+25);
    m_chartRc.setBottom(rc.bottom()-34);
    m_width =  m_chartRc.width()/11;


    rc = m_chartRc;
    rc.setTop(rc.bottom()+5);
    rc.setBottom(rc.top()+34);
    m_rcRiseText = rc;
    m_rcRiseText.setRight(m_rcRiseText.left()+m_width*3);
    m_rcFallText = rc;
    m_rcFallText.setLeft(m_rcRiseText.right()+m_width);
    m_rcFallText.setRight(m_rcFallText.left()+m_width*3);
    m_rcFlatText = rc;
    m_rcFlatText.setLeft(m_rcFallText.right()+m_width);
    m_rcFlatText.setRight(m_rcFlatText.left()+m_width*3);

    m_linePt1 = m_chartRc.bottomLeft();
    m_linePt2 = m_chartRc.bottomRight();
    m_linePt1.setX(m_linePt1.x()-10);
    m_linePt2.setX(m_linePt2.x()+10);
    m_dataChanged  = true;
}

QRect RiseFallFlatRectChartWnd::getRc()
{
   QRect rc = geometry();
   int hight = rc.height();
   rc.setTop(0);
   rc.setBottom(hight);
   return rc;
}
