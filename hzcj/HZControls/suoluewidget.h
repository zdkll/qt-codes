/********************************************************************
created:
author: dk.zhang
purpose: 具有缩略图样式的空窗口
*********************************************************************/
#ifndef SUOLUEWIDGET_H
#define SUOLUEWIDGET_H

#include "hzcontrols_global.h"
#include "basewidget.h"

class HZCONTROLSSHARED_EXPORT  SuolueWidget : public BaseWidget
{
    Q_OBJECT
public:
    SuolueWidget(QWidget *parent = 0, const QString &instName = "suoluewidget");

};

#endif // SUOLUEWIDGET_H
