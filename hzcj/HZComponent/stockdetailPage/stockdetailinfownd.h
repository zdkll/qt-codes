#ifndef STOCKDETAILINFOWND_H
#define STOCKDETAILINFOWND_H
/********************************************************************
created:
author: min.cheng
purpose: 个股详情页中部底部新闻，公共 研报 相关板块展示界面
*********************************************************************/
#include "stockdetailbasewnd.h"
class NewsWidget;
class HZAnncsInfo;
class HZNewsInfo;
class HZResearchReport;
class RelationBlockWnd;
class StockDetailInfoWnd: public StockBaseDetailInfoWnd
{
Q_OBJECT
public:
    explicit StockDetailInfoWnd(QWidget *parent = 0,QString instName="");

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
protected:
    //创建body
    QWidget* createBody();
    //页面切换
    void switchPage(int id);
protected:
    NewsWidget *m_newswidget;
    HZAnncsInfo* m_anncInfo;
    HZNewsInfo* m_newsInfo;
    HZResearchReport* m_researchReport;
    RelationBlockWnd* m_relationBlockWnd;
};

#endif // STOCKDETAILINFOWND_H
