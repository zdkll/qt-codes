#ifndef SCREEN_H
#define SCREEN_H

#include <QRect>

class ScreenRect : public QRect
{
public:
    ScreenRect():QRect(){}
    ScreenRect(const QPoint &topLeft, const QPoint &bottomRight):QRect(topLeft,bottomRight){}
    ScreenRect(const QPoint &topLeft, const QSize &size):QRect(topLeft,size){}
    ScreenRect(int x, int y, int width, int height):QRect(x,y,width,height){}

    void initRect();
    void setBeginPt(const QPoint& pt);
    void setEndPt(const QPoint& pt);

private:
    QPoint  m_beginPt,m_endPt;
};

#endif // SCREEN_H
