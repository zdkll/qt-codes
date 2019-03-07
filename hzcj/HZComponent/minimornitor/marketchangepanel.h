/********************************************************************
created:  2018.8
author: juntang.xie
purpose: MarketChangePanel 板块异动整体面板
*********************************************************************/
#ifndef MARKETCHANGEPANEL_H
#define MARKETCHANGEPANEL_H
#include "basewidget.h"
#include "platemove.h"
#include "chartdef.h"

class MinCanvas;
class MarketChangePanel:public BaseWidget
{
    Q_OBJECT
public:
    MarketChangePanel(BaseWidget *parent = nullptr);

    /**
     * @brief 对象激活
     */
    virtual void afterActive();

    /**
     * @brief 取消激活
     */
    virtual void afterDeactive();

private:
    QVector<ChartItemInfo> getMinChartInfo();
private:
    HZPlateMoveMarster *m_ChangedPanel;
    MinCanvas *m_pMinWidget;
};

#endif // MARKETCHANGEPANEL_H
