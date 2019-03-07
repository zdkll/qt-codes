/********************************************************************
created: 2018.9.25
author: juntang.xie
purpose: 自选股详情页面右侧盘口展示界面  基础行情盘口，资金 财务 。。。
*********************************************************************/
#ifndef OPTIONALSTOCKQUOTEWIDGET_H
#define OPTIONALSTOCKQUOTEWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"

class HandicapTitleWidget;
class HandicapCentralWidget;
class HandicapBottomWidget;
class QuoteTabWidget;
class MinChartWidget;

class OptionalStockQuoteWidget: public BaseWidget
{
    Q_OBJECT
public:
    explicit OptionalStockQuoteWidget(QWidget *parent = 0,QString instName="");

    //窗口创建
    void createWg();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
signals:

private slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    void slotsObjChange(const QString& obj);
private:
    //合约变化
    void OnStockCodeChange();
    //创建 股票代码 名称展示区域
    QWidget* createQuoteTitleWnd();
    //分时页面模块
    QWidget* createMinLineWidget();
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
private:
    //定义ID
    int m_nReq;
    HandicapTitleWidget *m_handicapTitleWidget;
    HandicapCentralWidget *m_handicapCentralWidget;
    HandicapBottomWidget *m_handicapBottomWidget;
    QuoteTabWidget *m_quoteTabWidget;
    MinChartWidget *m_minChartWidget;
};


#endif // OPTIONALSTOCKQUOTEWIDGET_H
