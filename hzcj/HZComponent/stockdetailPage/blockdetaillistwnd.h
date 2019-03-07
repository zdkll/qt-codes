#ifndef BLOCKDETAILLISTWND_H
#define BLOCKDETAILLISTWND_H
/********************************************************************
created:
author: min.cheng
purpose: 板块左侧窗口
*********************************************************************/
#include "stockdetailbasewnd.h"
class QButtonGroup;
class QLabel;
class NaviListView;
class NaviBlockListView;
enum BkDetailSortDir
{
    bk_detial_sort_up = 0,
    bk_detail_sort_down
};

class BlockDetaillistWnd:public StockBaseDetailListWnd
{
    Q_OBJECT
public:
    explicit BlockDetaillistWnd(QWidget *parent = 0,QString instName="");
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    virtual void slotsObjChange(const QString& obj);
    //排序点击
    void slotsButtonClicked(int id);
    void slotsSwitchUpDownStock(bool isUp, int step);
protected:
    virtual NaviListView* createView();
    //外面发送当前股票切换
    virtual void OnStockCodeChange();
    //创建标题
    QWidget *createTitle();
    //创建body
    QWidget* createBody();
protected:
    QButtonGroup*  m_pButtonGroup;
    NaviListView* m_pView;
    QLabel* m_pLabel;
    int m_filter;
};

#endif // BLOCKDETAILLISTWND_H
