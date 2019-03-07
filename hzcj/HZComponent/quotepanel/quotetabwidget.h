/********************************************************************
created: 2018.9.26
author: juntang.xie
purpose: 盘口，资金 财务页面
*********************************************************************/
#ifndef QUOTETABWIDGET_H
#define QUOTETABWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "hzchildwndmgr.h"

class QStackedWidget;
class CSubTab;
class QuotePanel;
class FundHandicap;
class HZFinanceMarster;
class QuoteTabWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit QuoteTabWidget(QWidget *parent = nullptr, QString instName = "");

    ~QuoteTabWidget();

    void afterActive();

    void afterDeactive();

    //窗口创建
    void createWg();
    //更新数据
    void updateStock(const QVector<HZData::AllStock>& data);
signals:

private slots:
    //tab切换
    void slotsSwitchWnd(int id);
private:
    //股票合约变化
    void OnStockCodeChange();
    //创建 TAB 区域
    QWidget* createTabWnd();
    //创建底部stackWnd区域
    QWidget* createStackWnd();

    //创建盘口区域
    ChildWndItem createQuotePanel();
    //创建资金区域
    ChildWndItem createFundPanel();
    //创建财务区域
    ChildWndItem createFinancePanel();

private:
    HzChildWndMgr<QuoteTabWidget>* m_childWndMgr;
    //标题tab
    CSubTab* m_table;
    //盘口
    QuotePanel* m_quotePanel = nullptr;
    //资金
    FundHandicap *m_fundHandicap = nullptr;
    //财务
    HZFinanceMarster *m_financeGraph = nullptr;
};

#endif // QUOTETABWIDGET_H
