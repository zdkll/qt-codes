/********************************************************************
created:2018.10/23
author: juntang.xie
purpose:指数详情盘口
*********************************************************************/
#ifndef INDEXDETAILHANDICAPWIDGET_H
#define INDEXDETAILHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"

class HandicapTitleWidget;
class IndexDetailQuoteWidget;
class IndexRiseFallWidget;
class FundHandicap;
class IndexDetailHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit IndexDetailHandicapWidget(QWidget *parent = nullptr);

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
    IndexDetailQuoteWidget *m_pIndexDetailQuoteWidget;
    IndexRiseFallWidget *m_indexRiseFallWidget;
    //资金
    FundHandicap *m_fundHandicap;
};

#endif // INDEXDETAILHANDICAPWIDGET_H
