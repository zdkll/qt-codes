#ifndef STOCKDETAILQUOTEPANELWND_H
#define STOCKDETAILQUOTEPANELWND_H
/********************************************************************
created:
author: min.cheng
purpose: 个股详情页面右侧盘口展示界面  基础行情盘口，资金 财务 。。。
*********************************************************************/
#include "stockdetailbasewnd.h"
#include "datacontroller.h"

enum QuotePanelType
{
    QuotePanelTab = 0,
    FundPanelTab,
    FinancePanelTab,
};

class QStackedWidget;
class HandicapTitleWidget;
class HandicapCentralWidget;
class HandicapBottomWidget;
class HZFinanceMarster;
class QuoteTabWidget;


class FundHandicap;
class QuotePanel;

class StockDetailQuotePanelWnd: public StockDetailBaseWnd
{
    Q_OBJECT
public:
    explicit StockDetailQuotePanelWnd(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
signals:

public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
private:
    //创建 股票代码 名称展示区域
    QWidget* createQuoteTitleWnd();
    //创建 基础行情展示区域 高开低收成交量等
    QWidget* createQuoteBaseDataWnd();
    //创建 附件行情展示区域 每股收益 市盈率等
    QWidget* createQuoteExtraDataWnd();  
    //创建切换页面
    QWidget* createQuoteTabWidget();
    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);
    //请求股票
    void subDyna();
    //取消订阅
    void cancelSub();
    //股票合约变化
    void OnStockCodeChange();
private:
    //合约
    QString m_obj;
    //定义ID
    int m_nReq;
    HandicapTitleWidget *m_handicapTitleWidget;
    HandicapCentralWidget *m_handicapCentralWidget;
    HandicapBottomWidget *m_handicapBottomWidget;
    QuoteTabWidget *m_quoteTabWidget;
};

#endif // STOCKDETAILQUOTEPANELWND_H
