/********************************************************************
created:
author: dk.zhang
purpose:资金盘口窗口
*********************************************************************/
#ifndef FUNDHANDICAP_H
#define FUNDHANDICAP_H

#include "scrollwidget.h"
#include "hzcomponent_global.h"

class FundFlowWidget;
class LeaderIncDecWidget;
class LeaderNetputWidget;
class  FundHandicap : public ScrollWidget
{
    Q_OBJECT
public:
    FundHandicap(QWidget *parent = 0);

    void setStock(const QString& stock);

    void afterActive();

    void afterDeactive();

   void setContentFixedWidth(int width);

private:
   void OnStockCodeChange();
    void createWg();
    FundFlowWidget        *m_fundFlowWidget; //资金流向
    LeaderIncDecWidget  *m_incDecWidget;//30日主力增减仓
    LeaderNetputWidget  *m_leaderNetputWidget;//主力净流入
    QWidget                   *m_contentWg;
};

#endif // FUNDHANDICAP_H
