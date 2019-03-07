/********************************************************************
created:2019.03.01
author: juntang.xie
purpose: 策略相关个股窗口
*********************************************************************/
#ifndef STRATEGYRELATEDSTOCKSWIDGET_H
#define STRATEGYRELATEDSTOCKSWIDGET_H
#include "basewidget.h"
#include "hzinfocenter.h"
#include "datacontroller.h"

class RelatedStockPanel;
class StrategyRelatedStocksWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit StrategyRelatedStocksWidget(QWidget *parent = nullptr);

    void updatedata(const TopicInvestDetail& data);

    virtual void afterDeactive();
private:
    void createWg();

    //数据请求回调
    void onStockOptionalDataArrived(ReqId  id , char *data, int num, ErrCode errCode);
    //请求股票
    void subDyna();
    //取消订阅
    void cancelSub();

    //订阅资金数据
    void subOptionalFund();
    //资金请求回调
    void optionalFundArrived(ReqId id, const char *data,int num, ErrCode  errCode);

    void cancelSubFund();

private:
    TopicInvestDetail m_deTailData;
    int m_nReq, m_nRedFund;
    QMap<std::string,RelatedStockPanel*> m_panelMap;
    QVector<RelatedStockPanel*> m_panelVec;
};

#endif // STRATEGYRELATEDSTOCKSWIDGET_H
