#include "fundbasechart.h"

#include <QPainter>

FundBaseChart::FundBaseChart(QWidget *parent)
    : QWidget(parent),m_paintAction(DRAW)
{

}

void FundBaseChart::draw()
{
    m_paintAction = DRAW;

    this->update();
}

void FundBaseChart::reDraw()
{
    m_paintAction = REDRAW;

    this->update();
}

void FundBaseChart::paintEvent(QPaintEvent *e)
{
    if(m_size != this->size()){
        m_size = this->size();
        m_paintAction = DRAW;
    }
    if(m_paintAction == DRAW){
        m_pixmap = QPixmap(m_size);
        drawPixmap(&m_pixmap);
        m_paintAction = REDRAW;
    }
    if(m_paintAction !=  NODRAW)
    {
        QPainter pt(this);
        pt.drawPixmap(0,0,m_pixmap);
        pt.end();

        //额外绘制其他
        paintOthers(&pt);
    }

    return QWidget::paintEvent(e);
}
