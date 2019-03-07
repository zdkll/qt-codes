/********************************************************************
created:2018.10/17
author: juntang.xie
purpose:行情沪深综合 左侧页面
*********************************************************************/
#ifndef HSZHLEFTWND_H
#define HSZHLEFTWND_H
#include "basewidget.h"
#include "mincanvas.h"
#include "datacontroller.h"
#pragma execution_character_set("utf-8")

class QVBoxLayout;
class QLabel;
/**
  * @brief 沪深综合分时窗体
  */
class HszhMinWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HszhMinWidget(QWidget *parent = nullptr);

    void afterActive();

    void afterDeactive();

    void setTitle(const QString &strName);

    void switchStockEx(const QString &strCode);

private:
    void createTitleBar();
    void createWg();
    QVector<ChartItemInfo> getChartInfo();
    void OnStockCodeChange();

private:
    MinCanvas* m_minChart;
    QLabel *m_pLabelName;
    QVBoxLayout *m_pMainLayout;
};


/**
  * @brief 左侧页面上面窗体
  */
class CSubTab;
class UpDownWidget;
class HszhLeftTopWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit HszhLeftTopWnd(QWidget* parent = nullptr);

    void afterActive();

    void afterDeactive();

private:
    void createWg();
    void createNavBar();
    void createUpDownPanel();
    void createMinWidget();
    void getNavBtn();
    void switchStockEx(int id);

private slots:
    void onButtonChanged(int id);
    void onLeftBtnClicked();
    void onRightBtnClicked();


private:
    QVBoxLayout *m_pMainLaytout;
    CSubTab *m_pSubTab;
    QVector<QPushButton*> m_btnVec;
    HszhMinWidget *m_pleftMinWidget;
    HszhMinWidget *m_prightMinWidget;
    UpDownWidget *m_pUpDownWidget;
};

/**
  * @brief 左侧页面中间窗体  领涨板块
  */
class QTimer;
class LzPaltePanel;
class HszhLeftCenterWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit HszhLeftCenterWnd(QWidget* parent = nullptr);

    void afterActive();
    void afterDeactive();

private:
    void createWg();
    void createTitleBar();
    void createBody();

    void subBatchDyna();
    void cancelSub();
    void onBatchDynaArrived(ReqId id ,const char * data, int num,ErrCode errCode);

private:
    QVBoxLayout         *m_pMainLayout;
    HZData::ReqBatchDynaInfo  m_reqBatchInfo;
    ReqId                      m_reqId;

    QVector<LzPaltePanel *> m_lzPlateVec;
};






/**
  * @brief 左侧页面下面窗体  资金流向
  */
class FundFlowPanel;
class HszhLeftBottomWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit HszhLeftBottomWnd(QWidget* parent = nullptr);

    void afterActive();

    void afterDeactive();

private:
    void createWg();
    void createTitleBar();
    void createBody();
    void subBatchDyna();

    void cancelSub();
    //数据请求回调
    void onBatchFundArrived(ReqId id ,const char *data,int num,ErrCode errCode);

private:
    QVBoxLayout *m_pMainLayout;   
    QTimer *m_timer;
    ReqId    m_reqId;

    HZData::ReqBatchFundInfo  m_reqBatchFunInfo;
    QVector<FundFlowPanel*>   m_pFundPanelVec;
};



/**
  * @brief 左侧页面主窗体
  */
class HszhLeftWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit HszhLeftWnd(QWidget *parent = nullptr);

    void afterActive();

    void afterDeactive();

private:
    void createWg();

private:
    HszhLeftTopWnd *m_pLeftTopWidget;
    HszhLeftCenterWnd *m_pLeftCenterWidget;
    HszhLeftBottomWnd *m_pLeftBottomWidget;
};

#endif // HSZHLEFTWND_H
