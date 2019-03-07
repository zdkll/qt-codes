#include "lzpaltepanel.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QStyle>
#include <QDebug>
#include "risefalltradiowidget.h"
#include "activitylabel.h"
#include "pubmsgdef.h"

LzPaltePanel::LzPaltePanel(QWidget *parent):
    BaseWidget(parent),
    m_strCode("")
{
    createWg();
}

void LzPaltePanel::setPlateName(const QString &strName)
{
    m_pNameLabel->setText(strName);
}

void LzPaltePanel::updateData(const HZData::Block &block)
{
    m_strCode = QString::fromStdString(block.stock_code);

    INT64 risefall[3];
    risefall[0] = block.rise;
    risefall[1] = block.flat;
    risefall[2] = block.fall;
    m_pTradioWidget->updateData(risefall);

    m_block = block;
    updateText();
}

void LzPaltePanel::createWg()
{
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    m_pNameLabel = new ActivityLabel("通信运营");

    m_pNameLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_pRateLabel = new QLabel("10.0%");

    m_pNameLabel->setObjectName("name");

    m_pRateLabel->setObjectName("rate");

    m_pTradioWidget = new RiseFallTradioWidget;

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(13, 0, 14, 0);

    pLayout->addWidget(m_pNameLabel);
    pLayout->addStretch();
    pLayout->addWidget(m_pRateLabel);

    QHBoxLayout *ratelayout = new QHBoxLayout;
    ratelayout->setSpacing(0);
    ratelayout->setContentsMargins(9, 0, 9, 0);
    ratelayout->addWidget(m_pTradioWidget);

    mainlayout->addSpacing(15);
    mainlayout->addLayout(pLayout);
    mainlayout->addSpacing(15);
    mainlayout->addLayout(ratelayout);

    mainlayout->addStretch();

    connect(m_pNameLabel, &ActivityLabel::clicked, [&]
    {
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_strCode);
        sendMessage(ROOT_FRAME,params,true);
    });
}

void LzPaltePanel::updateText()
{
    m_pNameLabel->setText(QString::fromStdString(m_block.stock_name));

    HzQuoteFormat::QuoteItem item;

    item = HzQuoteFormat::QuoteFormat::formatChange(m_block.rate,DECIMAL_POINT);

    item.strValue = item.strValue + "%";
    updateStockField(item, m_pRateLabel);

}

void LzPaltePanel::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}
