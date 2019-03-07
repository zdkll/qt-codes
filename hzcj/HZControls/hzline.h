/********************************************************************
created: 2018/04/12
author: dk.zhang
purpose:界面布局分割线，支持水平和垂直两种方式
*********************************************************************/
#ifndef HZLINE_H
#define HZLINE_H

#include "hzcontrols_global.h"
#include <QFrame>

class HZCONTROLSSHARED_EXPORT HZLine : public QFrame
{
    Q_OBJECT
public:
    HZLine(QWidget *parent = 0);
    HZLine(Qt::Orientation orientation ,QWidget *parent = 0);
    ~HZLine(){}

    void setHZlineWidth(const int &lineWidth);

protected:
//    void paintEvent(QPaintEvent *e);

private:
    void createLine();

private:
    Qt::Orientation m_orientation;
    int                   m_hzLineWidth;
};

#endif // HZLINE_H
