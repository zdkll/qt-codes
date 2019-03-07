/********************************************************************
created:
author: dk.zhang
purpose:可滚动区域窗口
*********************************************************************/
#ifndef SCROLLWIDGET_H
#define SCROLLWIDGET_H

#include "basewidget.h"

class QScrollArea;
class HZCONTROLSSHARED_EXPORT ScrollWidget : public BaseWidget
{
    Q_OBJECT
public:
    ScrollWidget(QWidget *parent = 0,QString instName="");

   QScrollArea *scrollArea(){return m_scrollArea;}

   void setWidget(QWidget *widget);

private:
    QScrollArea *m_scrollArea;
};

#endif // SCROLLWIDGET_H
