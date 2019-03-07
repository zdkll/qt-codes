/********************************************************************
created:  2018.8
author: juntang.xie
purpose: MiniNaveBar 迷你盯盘导航栏
*********************************************************************/
#ifndef MININAVEBAR_H
#define MININAVEBAR_H
#include "basewidget.h"

class MiniNaveBar:public BaseWidget
{
    Q_OBJECT
public:
    MiniNaveBar(BaseWidget *parent = nullptr);
private:
    void createWg();

signals:
    void clicked(int nIndex);
};

#endif // MININAVEBAR_H
