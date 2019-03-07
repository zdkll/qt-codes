/********************************************************************
created:2018.11/23
author: juntang.xie
purpose:非重要指数详情盘口
*********************************************************************/
#ifndef NORMALINDEXLHANDICAPWIDGET_H
#define NORMALINDEXLHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"

class HandicapTitleWidget;
class IndexDetailQuoteWidget;
class ZhubiHandicapView;
class NormalIndexHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit NormalIndexHandicapWidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

private:
    //合约变化
    void OnStockCodeChange();
    //创建窗口
    void createWg();
    //创建逐笔面板
    QWidget* createZhuBiPanel();
    //订阅动态行情
    void subDyna();
    //取消订阅
    void cancelSub();
    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);

private slots:
    void soltButtonSwitcTick();

private:
    //定义ID
    ReqId m_nReq;
    HandicapTitleWidget* m_handicapTitleWidget;
    IndexDetailQuoteWidget *m_pIndexDetailQuoteWidget;
    //逐笔
    ZhubiHandicapView *m_zhubiHandicapView;
};

#endif // NORMALINDEXLHANDICAPWIDGET_H
