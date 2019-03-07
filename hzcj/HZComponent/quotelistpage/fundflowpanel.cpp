#include "fundflowpanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStyle>

#include "activitylabel.h"
#include "pubmsgdef.h"

FundFlowPanel::FundFlowPanel(QWidget *parent):
    BaseWidget(parent),
    m_pMainLayout(new QVBoxLayout(this)),
    m_strCode("")
{
    createWg();
}

void FundFlowPanel::updateData(const HZData::StockMoney &money)
{
    m_strCode = QString::fromStdString(money.stock_code);
    m_stockMoney = money;
    updateText();
}

void FundFlowPanel::createWg()
{
    QWidget *widget = new QWidget;


    widget->setFixedHeight(115);
    widget->setObjectName("content");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(12);

    QHBoxLayout *playout1 = new QHBoxLayout;
    playout1->setSpacing(7);
    playout1->setMargin(0);

    QHBoxLayout *playout2 = new QHBoxLayout;
    playout2->setSpacing(5);
    playout2->setMargin(0);

    m_pNameLabel = new ActivityLabel(Default_Item_String);
    m_pCodeLabel = new QLabel(Default_Item_String);
    m_pRateLabel = new QLabel(Default_Item_String);
    m_pFlowLabel = new QLabel("净买入");
    m_pAmountLabel = new QLabel(Default_Item_String);

    m_pNameLabel->setCursor(QCursor(Qt::PointingHandCursor));
    m_pNameLabel->setObjectName("name");
    m_pCodeLabel->setObjectName("code");
    m_pRateLabel->setObjectName("rate");
    m_pFlowLabel->setObjectName("flow");
    m_pAmountLabel->setObjectName("amount");

    playout1->addWidget(m_pNameLabel, 0, Qt::AlignHCenter);
    playout1->addWidget(m_pCodeLabel, 0, Qt::AlignHCenter);
    playout2->addWidget(m_pFlowLabel, 0, Qt::AlignHCenter);
    playout2->addWidget(m_pAmountLabel, 0, Qt::AlignHCenter);

    layout->addSpacing(23);
    layout->addLayout(playout1);
    layout->setAlignment(playout1, Qt::AlignCenter);
    layout->addSpacing(13);
    layout->addWidget(m_pRateLabel, 0, Qt::AlignCenter);
    layout->addSpacing(13);
    //    layout->addWidget(m_pAmountLabel, 0, Qt::AlignCenter);
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

void FundFlowPanel::updateText()
{
    QString strName, strCode;

    strName = QString::fromStdString(m_stockMoney.stock_name);
    strCode = QString::fromStdString(m_stockMoney.stock_code).remove(0, 2);
    //    m_pNameLabel->setText(strName + " " + strCode);

    m_pNameLabel->setText(strName);
    m_pCodeLabel->setText(strCode);

    HzQuoteFormat::QuoteItem item;

//    double pre_close_price  = m_stockMoney.money_one_day.pre_close_price;
//    double last_price  = m_stockMoney.money_one_day.last_price;

//    if(pre_close_price == INVALID_DOUBLE && last_price == INVALID_DOUBLE)
//    {
//            item.strValue = "--";
//            item.dirc = HzQuoteFormat::QuoteDirc::quote_unknown;
//    }
//    else
//    {
//        item = HzQuoteFormat::QuoteFormat::formatChange((m_stockMoney.money_one_day.pre_close_price-m_stockMoney.money_one_day.pre_close_price),DECIMAL_POINT);
//    }

    item = HzQuoteFormat::QuoteFormat::formatChange(m_stockMoney.money_one_day.up_down_price);

    QString strValue = item.strValue;

    item = HzQuoteFormat::QuoteFormat::formatChange(m_stockMoney.money_one_day.rate,DECIMAL_POINT);

    item.strValue = item.strValue + "%";

    item.strValue = strValue + " " + item.strValue;

    updateStockField(item, m_pRateLabel);

    //净买入zdk
    INT64 netput = m_stockMoney.money_one_day.leader_money.money_netput;

    if(netput == INVALID_INT64)
    {
        m_pAmountLabel->setProperty("direct", "3");
    }
    else if(netput > 0)
    {
        m_pAmountLabel->setProperty("direct", "0");
    }
    else if(netput < 0)
    {
        m_pAmountLabel->setProperty("direct", "1");
    }
    else if(0 == netput)
    {
        m_pAmountLabel->setProperty("direct", "2");
    }

    m_pAmountLabel->style()->unpolish(m_pAmountLabel);
    m_pAmountLabel->style()->polish(m_pAmountLabel);
    m_pAmountLabel->update();

    QString strNetPut = HzQuoteFormat::StringUnitFormat::toFormatString(netput);

    m_pAmountLabel->setText(strNetPut);

}

void FundFlowPanel::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}
