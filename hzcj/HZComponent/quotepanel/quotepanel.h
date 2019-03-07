#ifndef QUOTEPANEL_H
#define QUOTEPANEL_H
/********************************************************************
created:
author: min.cheng  juntang.xie
purpose:盘口窗口 包括五档 逐笔等
*********************************************************************/
#include "basewidget.h"
#include "datacontroller.h"
#include "scrollwidget.h"
class BuySellFiveWidget;
class ZhubiHandicapView;
//盘口窗口
//class QuotePanel:public BaseWidget
class QuotePanel:public ScrollWidget
{
    Q_OBJECT
public:
    explicit QuotePanel(QWidget *parent = 0,QString instName="");

    ~QuotePanel();

    void createWg();

    void updateStock(const QVector<HZData::AllStock>& data);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
public slots:
    //逐笔弹窗
    void soltButtonSwitcTick();
private:
    void OnStockCodeChange();
    QVector<HZData::AllStock> m_data;
    BuySellFiveWidget *m_buySellFiveWidget;
    ZhubiHandicapView  *m_zhubiHandicapView;
};

#endif // QUOTEPANEL_H
