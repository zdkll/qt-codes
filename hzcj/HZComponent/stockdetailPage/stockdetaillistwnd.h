#ifndef STOCKDETAILLISTWND_H
#define STOCKDETAILLISTWND_H
/********************************************************************
created:
author: min.cheng
purpose: 个股详情页面用于展示 自选，最近浏览 资金异动界面 页面左边布局
*********************************************************************/
#include "stockdetailbasewnd.h"

class  NaviZXListView;
class  NaviZXHistListView;
class FundChangeWidget;
enum ListWndType
{
    OptionTab = 0,
    RecentlyBrowsingTab,
    FundChnge,
};

class MyBaseWidget :public BaseWidget
{
public:
    MyBaseWidget(QWidget *parent = 0):BaseWidget(parent,"test"){}
protected:
    void paintEvent(QPaintEvent *e);
};

class StockDetailListWnd: public StockBaseDetailListWnd
{
    Q_OBJECT
public:
    explicit StockDetailListWnd(QWidget *parent = 0,QString instName="");
    //相关板块成分股界面双击跳转,默认跳转到最近浏览页面
    void slotsObjChange(const QString& obj);
public slots:
    //tab切换
    void slotsSwitchWnd(int id);
    void slotsSwitchUpDownStock(bool, int);
protected:
    void OnStockCodeChange();
    //创建标题
    CSubTab* createSubAndItems();
    //创建body
    virtual QWidget* createBody();
protected:
    NaviZXListView       *m_naviZXListView = nullptr;
    NaviZXHistListView *m_naviHistListView = nullptr;
    FundChangeWidget* m_fundChangeView = nullptr;
};

#endif // STOCKDETAILLISTWND_H
