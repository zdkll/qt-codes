/********************************************************************
created:2019.03/01
author: juntang.xie
purpose:策略页面相关个股面板
*********************************************************************/
#ifndef RelatedStockPanel_H
#define RelatedStockPanel_H
#include "basewidget.h"
#include "datacontroller.h"
#include "quoteformat.h"
#pragma execution_character_set("utf-8")

class QVBoxLayout;
class QLabel;
class ActivityLabel;
class RelatedStockPanel:public BaseWidget
{
    Q_OBJECT
public:
    explicit RelatedStockPanel(QWidget *parent = nullptr);

    void updateData(const HZData::AllStock &allStock);
    void updateData(const HZData::StockMoney &stockMoney);

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
    QLabel *m_pNetBuyLabel;
    QLabel *m_pThrRateLabel;

    HZData::AllStock m_allStock;
    QString m_strCode;
};

#endif // RelatedStockPanel_H
