#include "relatedstockpanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStyle>

#include "activitylabel.h"
#include "pubmsgdef.h"

RelatedStockPanel::RelatedStockPanel(QWidget *parent):
    BaseWidget(parent),
    m_pMainLayout(new QVBoxLayout(this)),
    m_strCode("")
{
    createWg();
}

void RelatedStockPanel::updateData(const HZData::AllStock &allStock)
{
    m_strCode = QString::fromStdString(allStock.stock_code);
    m_allStock = allStock;
    updateText();
}

void RelatedStockPanel::updateData(const HZData::StockMoney &stockMoney)
{
    INT64 money_netput = 0;

    if(INVALID_INT64 == stockMoney.money_one_day.maxord_money.money_netput)
    {
        money_netput += 0;
    }
    else
    {
        money_netput += stockMoney.money_one_day.maxord_money.money_netput;
    }

    if(INVALID_INT64 == stockMoney.money_one_day.bigord_money.money_netput)
    {
        money_netput += 0;
    }
    else
    {
        money_netput += stockMoney.money_one_day.bigord_money.money_netput;
    }

    if(INVALID_INT64 == stockMoney.money_one_day.midord_money.money_netput)
    {
        money_netput += 0;
    }
    else
    {
        money_netput += stockMoney.money_one_day.midord_money.money_netput;
    }

    if(INVALID_INT64 == stockMoney.money_one_day.minord_money.money_netput)
    {
        money_netput += 0;
    }
    else
    {
        money_netput += stockMoney.money_one_day.minord_money.money_netput;
    }

    HzQuoteFormat::QuoteItem item;
    if(0 == money_netput)
    {
        item.strValue = "--";
        item.dirc   = HzQuoteFormat::quote_unknown;
    }
    else
    {
        if(money_netput > 0)
        {
            item.dirc   = HzQuoteFormat::quote_up;
        }
        else
        {
            item.dirc   = HzQuoteFormat::quote_down;
        }
        item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(money_netput, 2);

    }

    updateStockField(item, m_pNetBuyLabel);
}

void RelatedStockPanel::createWg()
{
    QWidget *widget = new QWidget;


    widget->setFixedHeight(115);
    widget->setObjectName("content");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

//    layout->setContentsMargins(73, 35, 0, 0);

    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(10);

    QHBoxLayout *playout1 = new QHBoxLayout;
    playout1->setSpacing(0);
    playout1->setMargin(0);

    QHBoxLayout *playout2 = new QHBoxLayout;
    playout2->setSpacing(5);
    playout2->setMargin(0);

    m_pNameLabel = new ActivityLabel(Default_Item_String);
    m_pCodeLabel = new QLabel(Default_Item_String);
    m_pRateLabel = new QLabel(Default_Item_String);
    m_pNetBuyLabel = new QLabel(Default_Item_String);
    m_pThrRateLabel = new QLabel(Default_Item_String);

    m_pNameLabel->setCursor(QCursor(Qt::PointingHandCursor));
    m_pNameLabel->setObjectName("name");
    m_pCodeLabel->setObjectName("code");
    m_pRateLabel->setObjectName("rate");
    m_pNetBuyLabel->setObjectName("rate");
    m_pThrRateLabel->setObjectName("rate");


    playout1->addWidget(m_pNameLabel, 0, Qt::AlignHCenter);
    playout1->addSpacing(8);
    playout1->addWidget(m_pCodeLabel, 0, Qt::AlignHCenter);
    playout1->addSpacing(15);
    playout1->addWidget(m_pRateLabel, 0, Qt::AlignHCenter);

    playout2->addWidget(new QLabel("主动净买"), 0, Qt::AlignHCenter);
    playout2->addSpacing(7);
    playout2->addWidget(m_pNetBuyLabel, 0, Qt::AlignHCenter);
    playout2->addSpacing(27);
    playout2->addWidget(new QLabel("三日涨幅"), 0, Qt::AlignHCenter);
    playout2->addSpacing(7);
    playout2->addWidget(m_pThrRateLabel, 0, Qt::AlignHCenter);

    layout->addSpacing(36);
    layout->addLayout(playout1);
    layout->setAlignment(playout1, Qt::AlignCenter);
    layout->addSpacing(15);

    layout->addLayout(playout2);
    layout->setAlignment(playout2, Qt::AlignCenter);
    layout->addStretch();
    widget->setLayout(layout);

    m_pMainLayout->addWidget(widget);

    connect(m_pNameLabel, &ActivityLabel::clicked, [&]
    {
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_strCode);
        sendMessage(ROOT_FRAME,params,true);
    });
}

void RelatedStockPanel::updateText()
{
    QString strName, strCode;

    strName = QString::fromStdString(m_allStock.stock_name);
    strCode = QString::fromStdString(m_allStock.stock_code).remove(0, 2);

    m_pNameLabel->setText(strName);
    m_pCodeLabel->setText(strCode);

    HzQuoteFormat::QuoteItem item;

    item = HzQuoteFormat::QuoteFormat::formatChange(m_allStock.up_down_price);

    QString strValue = item.strValue;

    item = HzQuoteFormat::QuoteFormat::formatChange(m_allStock.rate,DECIMAL_POINT);

    item.strValue = item.strValue + "%";

    item.strValue = strValue + " " + item.strValue;

    updateStockField(item, m_pRateLabel);


    item = HzQuoteFormat::QuoteFormat::formatChange(m_allStock.three_rate,DECIMAL_POINT);

    item.strValue += "%";

    updateStockField(item, m_pThrRateLabel);

}

void RelatedStockPanel::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}
