/********************************************************************
created:  2018.9
author: juntang.xie
purpose: SelfStockPanel 自选股面板
*********************************************************************/
#ifndef SELFSTOCKPANEL_H
#define SELFSTOCKPANEL_H
#include "basewidget.h"
#include "subtab.h"
#include "chartdef.h"

class SelfStockWiget;
class MinTrendWiget;
class MinCanvas;
class KlineCanvas;
class QStackedWidget;
class SLOptionTableView;

class SelfStockPanel:public BaseWidget
{
    Q_OBJECT
public:
    SelfStockPanel(BaseWidget *parent = nullptr);
    virtual void afterActive();
    virtual void afterDeactive();

    //获取自选股行情
//QVector<HZData::AllStock> stocks();
signals:
    //自选股数据更新
    void stockChanged();

private:
    void createWg();

    //自选股列表
    SLOptionTableView *m_zxTableView;
    MinTrendWiget       *m_minKlineWnd;
};

/**
  * @brief 自选股列表类
  */
class SelfStockWiget:public BaseWidget
{
    Q_OBJECT
public:
    SelfStockWiget(BaseWidget *parent = nullptr);
private:
    void createWg();
};

class QLabel;
class QPushButton;
/**
  * @brief 分时走势图类
  */
class MinTrendWiget:public BaseWidget
{
    Q_OBJECT
public:
    MinTrendWiget(BaseWidget *parent = nullptr);
    void afterActive();
    void afterDeactive();
private:
    void createWg();
    CSubTab *addCSubTab(CSubTab::CSubTabType subTabType = CSubTab::Root_Tab, QString instName=QString());
public slots:
    void tabCurrentBtnChanged(int id);
    void switchObj(const QString& obj);
    void slotsZxStockChanged(const int count);
private:
    QVector<ChartItemInfo> getMinChartInfo();

    QVector<ChartItemInfo> getKlineChartInfo();
private:
    bool m_bKline;
    QLabel *m_pTitleLabel;
    MinCanvas *m_pMinWidget;
    KlineCanvas* m_pKlineWidget;
    QStackedWidget* m_stackWnd;
    QPushButton * m_pBtn;
    QString m_obj;
    QString m_objName;
};

#endif // SELFSTOCKPANEL_H
