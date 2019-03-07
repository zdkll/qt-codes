#ifndef KPOINT_H
#define KPOINT_H

#include <QtCore/qnamespace.h>
#include <QPoint>

struct KPoint
{
    KPoint(){}
    KPoint(const QPoint&pt):sPt(pt){}
    KPoint(const QPoint&pt,quint32  t,float   p):sPt(pt),tx(t),py(p){}

    QPoint  sPt; //屏幕坐标
    quint32    tx;//时间戳
    float         py;//价格
};

#endif // KPOINT_H
