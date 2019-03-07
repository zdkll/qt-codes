/********************************************************************
created:2018.10/22
author: juntang.xie
purpose:指数盘口
*********************************************************************/
#ifndef INDEXHANDICAPWIDGET_H
#define INDEXHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "mincanvas.h"
#include "klinecanvas.h"

class HandicapTitleWidget;
class IndexQuoteWidget;
class IndexRiseFallWidget;
class ZhubiHandicapView;
class IndexHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit IndexHandicapWidget(QWidget *parent = nullptr);

    ~IndexHandicapWidget();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
private:
    //窗口创建
    void createWg();
    //合约变化
    void OnStockCodeChange();
    //创建 指数代码 名称展示区域
    QWidget* createQuoteTitleWnd();

    //创建 指数行情区域
    QWidget* createQuoteBodyWnd();

    //创建 指涨平跌区域
    QWidget* createRiseFallWnd();

    //创建 分时图
    QWidget* createMinWnd();

    //创建 K线图
    QWidget* createKlineWnd();

    //创建 分笔窗口
    QWidget *createFenbiWnd();

    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);
    //请求股票
    void subDyna();
    void cancelSub();

    QVector<ChartItemInfo> getMinChartInfo();

    QVector<ChartItemInfo> getKlineChartInfo();

private slots:
    //逐笔弹窗
    void soltButtonSwitcTick();
private:
    //定义ID
    ReqId m_nReq;
    HandicapTitleWidget *m_handicapTitleWidget;
    IndexQuoteWidget *m_indexQuoteWidget;
    IndexRiseFallWidget *m_indexRiseFallWidget;
    MinCanvas* m_minWnd;
    KlineCanvas* m_klineWnd;
    int m_period;

    ZhubiHandicapView  *m_zhubiHandicapView;
};

#endif // INDEXHANDICAPWIDGET_H
