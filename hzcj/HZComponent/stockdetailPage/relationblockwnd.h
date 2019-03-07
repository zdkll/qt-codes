#ifndef RELATIONBLOCKWND_H
#define RELATIONBLOCKWND_H
/********************************************************************
created:
author: min.cheng
purpose:相关板块窗口
*********************************************************************/
#include "basewidget.h"
class RelationBlockListView;
class StockDynaView;
class RiseFallFlatBaseWidget;
class RelationBlockWnd: public BaseWidget
{
    Q_OBJECT
public:
    explicit RelationBlockWnd(QWidget *parent = 0,QString instName="");

    void createWg();

    /**
     * @brief 对象激活
     */
    void afterActive();

    /**
     * @brief 取消激活
     */
    void afterDeactive();
signals:
    void signalDbClickObjChange(const QString& obj);
protected slots:
    void slotRightViewDbClicked(const QString& obj);
    void slotLeftViewDbClicked(const QString& obj);
    void slotsItemSelected(const int& filterFlag,const QString &stockCode);
protected:
    /**
     * @brief 合约变化时被调用
     */
    void OnStockCodeChange();

    //创建左侧板块列表
    QWidget* createLeftBkListWnd();
    //创建中间涨跌家数
    QWidget* createMidWnd();
    //创建右侧板块成分股窗口
    QWidget* createRightBkStocksWnd();
private:
    //左侧板块列表视图
    RelationBlockListView* m_leftBkListView;
    //右侧板块成分股视图
    StockDynaView * m_rightBkStocksView;
    //涨跌家数界面
    RiseFallFlatBaseWidget* m_rffWnd;
};

#endif // RELATIONBLOCKWND_H
