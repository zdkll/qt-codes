/********************************************************************
created:  2018.9
author: juntang.xie
purpose: StockRollStatusBar 迷你盯盘底部状态栏面板
*********************************************************************/
#ifndef STOCKROLLSTATUSBAR_H
#define STOCKROLLSTATUSBAR_H
#include "basewidget.h"
#include "datacontroller.h"

class StatusBarIndexMgr;
class QTimer;
class QLabel;
class DataSubCenter;
class ActivityLabel;
class StockRollStatusBar:public BaseWidget
{
    Q_OBJECT
public:
    StockRollStatusBar(BaseWidget *parent = nullptr);

    /**
     * @brief 对象激活
     */
    virtual void afterActive();

    /**
     * @brief 取消激活
     */
    virtual void afterDeactive();

private:
    void createWg();

    void setIndexText();

private slots:
    void onBtnClicked();
    void onSubStockArrived(char *data, int num);
    void onTimerOut();
    void onLabelClicked();
private:
    bool m_bFlg;
    DataSubCenter *m_pDataProvider;
    QVector<HZData::SLStock > m_stockVec;
    QTimer* m_timer;
    ActivityLabel* m_pLabelName;
    ActivityLabel *m_pLabelText;
    static int m_nIndex;
};





#endif // STOCKROLLSTATUSBAR_H
