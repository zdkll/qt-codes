#ifndef BLOCKDETAILINFOWND_H
#define BLOCKDETAILINFOWND_H
/********************************************************************
created:
author: min.cheng
purpose: 板块详情底部展示窗口  指數底部详情页面
*********************************************************************/
#include "stockdetailbasewnd.h"
class NewsWidget;
class HZNewsInfo;
class HZPlateNewsInfo;
class HZIndexNewsInfo;
class StockDynaView;

class BlockDetailInfoWnd: public StockBaseDetailInfoWnd
{
    Q_OBJECT
public:
    explicit BlockDetailInfoWnd(QWidget *parent = 0,QString instName="");

    //返回TAB数据项
    void getStockDetailSubItemList(QVector<StockDetailSubItem>& list);
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
    //tab切换
    void slotsSwitchWnd(int id);

protected:
    //股票合约变化
    void OnStockCodeChange();
    //创建body
    QWidget* createBody();
private:
    StockDynaView* m_pbView;
    NewsWidget *m_newswidget;
    HZNewsInfo* m_newsInfo;
    HZPlateNewsInfo *mPlateNewsInfo;
    int m_filter;
};


class IndexDetailInfoWnd: public StockBaseDetailInfoWnd
{
    Q_OBJECT
public:
    explicit IndexDetailInfoWnd(QWidget *parent = 0,QString instName="");

    //返回TAB数据项
    void getStockDetailSubItemList(QVector<StockDetailSubItem>& list);
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
    //tab切换
    void slotsSwitchWnd(int id);

protected:
    //股票合约变化
    void OnStockCodeChange();
    //创建body
    QWidget* createBody();
private:
    StockDynaView* m_pbView;
    NewsWidget *m_newswidget;
    HZNewsInfo* m_newsInfo;
    HZIndexNewsInfo *mIndexNews;
    int m_filter;
};

#endif // BLOCKDETAILINFOWND_H
