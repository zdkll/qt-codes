/********************************************************************
created:2018.10.17
author: juntang.xie
purpose: 张平跌小组件
*********************************************************************/
#ifndef RISEFALLTRADIOWIDGET_H
#define RISEFALLTRADIOWIDGET_H
#include "basewidget.h"
#include "datamodeldefs/stockinfodef.h"
#include "updownwidget.h"
#include "datacontroller.h"


class RiseFallTradioWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit RiseFallTradioWidget(QWidget *parent = nullptr);

    void setStockCode(const QString &stockCode);

    void updateData(INT64 *riseFall);

protected:
    virtual void paintEvent(QPaintEvent *e);


private:
    void subRiseFallInfo();
    void calcelSub();
    void onSubRiseFallInfoArrived(ReqId reqId, const HZData::StockRiseFallInfo &riseFallInfo, ErrCode errCode);
    void setData(const HZData::StockRiseFallInfo &risefallInfo);
private:
    ReqId m_subReqId;
    QString m_strCode;
    INT64 m_nValues[3];
    INT64 m_nTotal;
    float m_upRate;
    float m_flatRate;
    float m_downRate;
    ColorWidget m_colorWidget;
};

#endif // RISEFALLTRADIOWIDGET_H
