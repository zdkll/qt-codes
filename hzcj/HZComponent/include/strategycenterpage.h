#ifndef STRATEGYCENTERPAGE_H
#define STRATEGYCENTERPAGE_H
/********************************************************************
created:
author: min.cheng jt.xie
purpose:策略中心
*********************************************************************/
#include "hzcomponent_global.h"
#include  "basepage.h"
#include "hzinfocenter.h"
#include "strategytodaychancewnd.h"
class DynaDataProvider;
//class StrategyRelationStocksWnd;
class StrategyTitleWnd;
class StrategyRelatedStocksWidget;
class HZNewsInfo;
class StrategyRelatedInfowidget;
class HZCOMPONENTSHARED_EXPORT StrategyCenterPage: public BasePage
{
Q_OBJECT
public:
    explicit StrategyCenterPage(QWidget *parent = 0,QString instName="");

    ~StrategyCenterPage();

    void afterActive();

    void afterDeactive();

public slots:
    void slotClickId(QString topic_id,QString codeName);
private:
    void createWg();
    void onTopicInvestDetail(const InfoRet& ret ,const TopicInvestDetail& data);

    void cancleInfoReq();
private:
    DynaDataProvider* m_pProvider = nullptr;
    DynaDataProvider* m_pLongTermProvider = nullptr;
    StrategyTodayChanceWnd* m_ptodyChanceWnd = nullptr;
    StrategyTodayChanceWnd* m_pLongTermChanceWnd = nullptr;
//    StrategyRelationStocksWnd* m_relationStocks;
    StrategyRelatedStocksWidget* m_relatedStockWidget;
    StrategyTitleWnd* m_pTitleWnd;
    StrategyRelatedInfowidget *m_pInfoWnd;
    int m_reqId = -1;
    bool m_bFirstActive = true;
};

#endif // STRATEGYCENTERPAGE_H
