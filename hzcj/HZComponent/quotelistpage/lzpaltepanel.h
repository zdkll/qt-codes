/********************************************************************
created:2018.10/17
author: juntang.xie
purpose:行情沪深综合 领涨板块面板
*********************************************************************/
#ifndef LZPALTEPANEL_H
#define LZPALTEPANEL_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"
#pragma execution_character_set("utf-8")

class QLabel;
class RiseFallTradioWidget;
class ActivityLabel;
class LzPaltePanel:public BaseWidget
{
    Q_OBJECT
public:
    explicit LzPaltePanel(QWidget *parent = nullptr);

    void setPlateName(const QString &strName);

    void updateData(const HZData::Block &block);

private:
    void createWg();

    void updateText();
    //更新文本颜色
    void updateStockField(const HzQuoteFormat::QuoteItem& item,QLabel* pLabel);

private:
    ActivityLabel *m_pNameLabel;
    QLabel *m_pRateLabel;
    HZData::Block     m_block;
    RiseFallTradioWidget *m_pTradioWidget;
    QString m_strCode;
};

#endif // LZPALTEPANEL_H
