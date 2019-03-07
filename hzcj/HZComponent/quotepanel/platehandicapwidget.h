/********************************************************************
created:2018.10/23
author: juntang.xie
purpose:板块盘口
*********************************************************************/
#ifndef PLATEHANDICAPWIDGET_H
#define PLATEHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "scrollwidget.h"

class HandicapTitleWidget;
class MinChartWidget;
class PlateDetailQuoteWidget;
class IndexRiseFallWidget;
class QuoteTabWidget;
class PlateHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit PlateHandicapWidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

private:
    //合约变化
    void OnStockCodeChange();
    //创建窗口
    void createWg();

    //创建成分股标题
    QWidget* createStockTitle();

    //订阅动态行情
    void subPlateDyna();
    //取消订阅
    void cancelPlateSub();

    //订阅动态行情
    void subStockDyna();
    //取消订阅
    void cancelStockSub();


    //数据返回回调函数
    void onPlateDynaArrived(ReqId  id , char *data, int num, ErrCode errCode);
    void onStockDynaArrived(ReqId  id , char *data, int num, ErrCode errCode);

private:
    //定义ID
    ReqId m_nReq;
    ReqId m_nStockReq;
    HandicapTitleWidget* m_plateTitleWidget;
    MinChartWidget *m_plateChartWidget;
    PlateDetailQuoteWidget* m_plateQuoteWidget;
    IndexRiseFallWidget* m_plateRiseFallWidget;
    HandicapTitleWidget* m_stockTitleWidget;
    MinChartWidget *m_stockChartWidget;
    QuoteTabWidget *m_stockTabWidget;
};

#endif // PLATEHANDICAPWIDGET_H
