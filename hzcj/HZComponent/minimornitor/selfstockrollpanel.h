/********************************************************************
created:  2018.9
author: juntang.xie
purpose: SelfStockRollPanel 迷你顶部自选股滚动播报面板
*********************************************************************/
#ifndef SELFSTOCKROLLPANEL_H
#define SELFSTOCKROLLPANEL_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"
#include <list>
#include <string>
using namespace std;


class QLabel;
class DataSubCenter;
class QTimer;
class SelfStockRollPanel:public BaseWidget
{
    Q_OBJECT
public:
    SelfStockRollPanel(BaseWidget *parent = nullptr);

    void setStockCode(const QString strCode);

    void setStockCode(const std::list<string> lists);

    virtual void afterActive();
    virtual void afterDeactive();

public slots:
    void onPlayBtnClicked();
private:
    void createWg();
    void setObjName();
    void updateText();

    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);
    void setTextCode();

    void setTextCode(const HZData::SLStock &stock);

private slots:
    void onSubStockArrived(char *data, int num);
    void onTimeOut();

private:
    QString m_strCode;
    std::list<string> m_lists;
    QLabel* m_pTextName;
    QLabel* m_pTextCode;
    QLabel* m_pTextPrice;
    QLabel* m_pArrow;
    QLabel* m_pTextChange;
    QLabel* m_pTextChangeRate;
    QVector<HZData::SLStock > m_stockVec;
    DataSubCenter *m_pDataSubCenter;
    QTimer *m_timer;
    static int m_nIndex;
    bool m_bFlg;
};

#endif // SELFSTOCKROLLPANEL_H
