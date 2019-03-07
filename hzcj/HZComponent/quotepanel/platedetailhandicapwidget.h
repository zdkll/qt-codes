/********************************************************************
created:2018.10/22
author: juntang.xie
purpose:板块详情盘口
*********************************************************************/
#ifndef PLATEDETAILHANDICAPWIDGET_H
#define PLATEDETAILHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"


class HandicapTitleWidget;
class PlateDetailQuoteWidget;
class IndexRiseFallWidget;
class FundHandicap;
class PlateDetailHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit PlateDetailHandicapWidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

private:
    //合约变化
    void OnStockCodeChange();
    //创建窗口
    void createWg();
    //创建资金区域
    QWidget* createFundPanel();
    //订阅动态行情
    void subDyna();
    //取消订阅
    void cancelSub();
    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);


private:
    //定义ID
    ReqId m_nReq;
    HandicapTitleWidget* m_handicapTitleWidget;
    PlateDetailQuoteWidget *m_plateQuoteWidget;
    IndexRiseFallWidget *m_indexRiseFallWidget;
    //资金
    FundHandicap *m_fundHandicap;
};

#endif // PLATEDETAILHANDICAPWIDGET_H
