#include "screen.h"

#include <QPainter>


void ScreenRect::initRect()
{
    setBeginPt(QPoint(0,0));
}

void ScreenRect::setBeginPt(const QPoint &pt)
{
    m_beginPt = pt;
    m_endPt    = pt;
    this->setTopLeft(m_beginPt);
    this->setBottomRight(m_endPt);
}

void ScreenRect::setEndPt(const QPoint &pt)
{
    m_endPt = pt;

    //更新左上角和右下角坐标
    int x1,x2,y1,y2;
    (m_beginPt.x()<=m_endPt.x())?(x1 = m_beginPt.x(),x2=m_endPt.x()):(x1 = m_endPt.x(),x2=m_beginPt.x());
    (m_beginPt.y()<=m_endPt.y())?(y1= m_beginPt.y(),y2=m_endPt.y()):(y1= m_endPt.y(),y2=m_beginPt.y());

    this->setTopLeft(QPoint(x1,y1));
    this->setBottomRight(QPoint(x2,y2));
}
