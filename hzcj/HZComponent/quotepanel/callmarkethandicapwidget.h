/********************************************************************
created:2018.10
author: juntang.xie
purpose:集合竞价盘口
*********************************************************************/
#ifndef CALLMARKETHANDICAPWIDGET_H
#define CALLMARKETHANDICAPWIDGET_H
#include "scrollwidget.h"
#include "datacontroller.h"

class HandicapTitleWidget;
class HandicapCentralWidget;
class QuoteTabWidget;
class MinChartWidget;
class BiddingCanvas;
class CallMarketHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit CallMarketHandicapWidget(QWidget* parent= nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
private:
    void createWg();
    //合约变化
    void OnStockCodeChange();

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
    QuoteTabWidget *m_quoteTabWidget;
    MinChartWidget *m_minChartWidget;
    BiddingCanvas* m_biddingWidget;
};

#endif // CALLMARKETHANDICAPWIDGET_H
