/********************************************************************
created:  2018
author: juntang.xie
purpose:通用的状态栏基类，整体面板布局
*********************************************************************/
#ifndef STATUSBAR_H
#define STATUSBAR_H
#include <QWidget>
#include  "hzcontrols_global.h"
#include "basewidget.h"

class QGridLayout;
class  HZCONTROLSSHARED_EXPORT StatusBar : public  BaseWidget
{
    Q_OBJECT
public:
    StatusBar(QWidget *parent = 0,QString instName="");
    virtual ~StatusBar(){}

    //获取对应位置的面板
    QWidget *widgetAtPosition(const int &row,const int &column);

protected:
    enum rowId{
        zeroRow,
        firstRow
    };

    enum colId{
        zeroCol,
        firstCol,
        secondCol,
        thirdCol
    };



private:
    void createWg();

private:
    QGridLayout  *m_pMainLayout;
};



#endif // STATUSBAR_H
