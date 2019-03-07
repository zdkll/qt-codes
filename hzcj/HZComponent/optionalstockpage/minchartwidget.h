/********************************************************************
created: 2018.9.26
author: juntang.xie  程敏
purpose: 分时页面/K线页面
*********************************************************************/
#ifndef MINCHARTWIDGET_H
#define MINCHARTWIDGET_H
#include <QStackedWidget>
#include "basewidget.h"
#pragma execution_character_set("utf-8")

class CSubTab;
class MinCanvas;
class KlineCanvas;
class MinChartWidget:public BaseWidget
{
    Q_OBJECT

public:
    explicit MinChartWidget(QWidget *parent = nullptr, QString instName = "");

    //窗体激活时调用
    void afterActive();
    //窗体退出时调用
    void afterDeactive();
private:
    QWidget* createTab();

    QWidget * createBody();
private slots:
    void slotsSwitchWnd(int nId);
private:
    void createWg();
    void OnStockCodeChange();
private:
    //标题tab
    CSubTab* m_table;
    QStackedWidget* m_pStackWnd;
    MinCanvas* m_minWnd;
    KlineCanvas* m_klineWnd;
    int m_period;
};

#endif // MINCHARTWIDGET_H
