/********************************************************************
created:  2018.8
author: juntang.xie
purpose: MiniTitleBar 迷你盯盘标题栏
*********************************************************************/
#ifndef MINITITLEBAR_H
#define MINITITLEBAR_H
#include "basewidget.h"

class MiniTitleBar:public BaseWidget
{
    Q_OBJECT
public:
    explicit MiniTitleBar(BaseWidget *parent = nullptr);

private:
    void createWg();
private slots:
    void onPlayBtnClicked();
signals:
    void clickClose();
    void maxiMize();
    void minMizeClicked();
    void playBtnClicked();
private:
    bool m_bPlay;
};

#endif // MINITITLEBAR_H
