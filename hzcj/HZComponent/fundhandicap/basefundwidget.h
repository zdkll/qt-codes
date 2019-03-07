/********************************************************************
created:
author: dk.zhang
purpose: 资金子窗口基类
*********************************************************************/
#ifndef BASEFUNDWIDGET_H
#define BASEFUNDWIDGET_H

#include "basewidget.h"

class BaseFundWidget : public BaseWidget
{
public:
    BaseFundWidget(QWidget *parent = 0,QString instName="");



    void setStock(const QString &stock);

protected:
    virtual void stockChanged(const QString &/*stockCode*/) = 0;

private:

};

#endif // BASEFUNDWIDGET_H
