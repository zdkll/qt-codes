/********************************************************************
created:2018.11
author: juntang.xie
purpose:龙虎榜
*********************************************************************/
#ifndef LHLISTHANDICAPWIDGET_H
#define LHLISTHANDICAPWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#include "hzinfocenter.h"

#pragma execution_character_set("utf-8")
class MinChartWidget;
class LhTitieWidget;
class LhRiseFallWidget;
class LhBuySellView;
class LhbBottomWidget;
class QVBoxLayout;
class LhListHandicapWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit LhListHandicapWidget(QWidget *parent = nullptr);

    ~LhListHandicapWidget();

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

    //设置日期
    void setDateTime(qint64 date);


//protected:
//    virtual void paintEvent(QPaintEvent *event);


protected slots:
    void slotsBuySellViewClickRow(const QString &strID);
    void onCurrentIndex(int index);
private:
    //合约变化
    void OnStockCodeChange();
    //创建窗口
    void createWg();
    //订阅动态行情
    void subDyna();
    //取消订阅
    void cancelSub();
    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);
    //请求龙虎榜单详情
    void reqStockLhbDetial();
    //龙虎榜数据回调
    void onLhbDataArrived(const InfoRet& infoRet, const StockLhbDetialList &data);


    void deleteWidget();

private:
//    //定义ID
    ReqId m_nReq;
    LhTitieWidget *m_titleWidget;
//    LhRiseFallWidget *m_riseFallWidget;
//    LhBuySellView *m_lhBuyView;
//    LhBuySellView *m_lhSellView;
    LhbBottomWidget *m_lhBottomWidget;
    StockLhbDetialList m_lhDetailList;
    qint64 m_dateTime;
    MinChartWidget *m_minChartWidget;
    QWidget *m_bottomWidget;
    QVBoxLayout *m_bottomLayout;

    QVector<LhbBottomWidget*> m_lhbWidgetVec;
};

#endif // LHLISTHANDICAPWIDGET_H
