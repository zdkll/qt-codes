/********************************************************************
created:2018.10/18
author: juntang.xie
purpose:行情沪深综合 资金流向面板
*********************************************************************/
#ifndef FUNDFLOWPANEL_H
#define FUNDFLOWPANEL_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"
#pragma execution_character_set("utf-8")

class QVBoxLayout;
class QLabel;
class ActivityLabel;
class FundFlowPanel:public BaseWidget
{
    Q_OBJECT
public:
    explicit FundFlowPanel(QWidget *parent = nullptr);

    void updateData(const HZData::StockMoney &money);

private:
    void createWg();

    void updateText();

    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);


private:
    QVBoxLayout *m_pMainLayout;
    ActivityLabel *m_pNameLabel;
    QLabel *m_pCodeLabel;
    QLabel *m_pRateLabel;
    QLabel *m_pFlowLabel;
    QLabel *m_pAmountLabel;

    HZData::StockMoney m_stockMoney;
    QString m_strCode;
};

#endif // FUNDFLOWPANEL_H
