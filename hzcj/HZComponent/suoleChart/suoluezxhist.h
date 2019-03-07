/********************************************************************
created:
author: dk.zhang
purpose: 自选股和历史浏览缩略图
*********************************************************************/
#ifndef SUOLUEZXHIST_H
#define SUOLUEZXHIST_H

#include "SuolueBase.h"


class QStackedWidget;
class ZXStockDynaView;
class HistStockDynaView;
class SuolueZxHist : public SuolueBase
{
    Q_OBJECT
public:
    SuolueZxHist(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    //创建内容区域窗口
    QWidget* createContentWnd();

    //导航点击以后触发
    void onClickItem(const SuolueItem& item);

    virtual void onFocusOut();

    //创建标题区域
    virtual SuolueTitleBase* createTitleWnd();
public slots:
    void slotItemSeleced(const QString &stockCode);
private:
  ZXStockDynaView   *m_zxTableView;
  HistStockDynaView *m_histTableView;
  QStackedWidget     *m_stackedWidget;
};

#endif // SUOLUEZXHIST_H
