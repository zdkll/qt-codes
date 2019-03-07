#include "buysellfivewidget.h"
#include <QPainter>
#include "buysellfiveview.h"
#include <QGridLayout>

TradingRatioWidget::TradingRatioWidget(QWidget *parent):
    BaseWidget(parent),
    m_strRise(""),
    m_strDown(""),
    m_allBuyAmount(0),
    m_allSellAmmount(0)
{
    this->setFixedSize(350, 20);
}

TradingRatioWidget::~TradingRatioWidget()
{

}

void TradingRatioWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    formatData(vec);
    update();
}

void TradingRatioWidget::paintEvent(QPaintEvent *event)
{
    BaseWidget::paintEvent(event);

    QRect rc = this->rect();
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

    float width = 0;
    if(0 != m_allBuyAmount + m_allSellAmmount)
    {

         width = m_allBuyAmount * rc.width() / (m_allBuyAmount + m_allSellAmmount);
    }
    painter.fillRect(rc.x(), rc.y(), width, rc.height(), m_styledWidget.riseColor());

    painter.fillRect(rc.x() + width, rc.y(),rc.width() - width, rc.height(), m_styledWidget.downColor());

    painter.setPen(m_styledWidget.textColor());
    QFont font;
    font.setBold(m_styledWidget.textBold());
    font.setFamily(m_styledWidget.textFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter.setFont(font);
    painter.drawText(rc.x() + 7, rc.y() + 4, rc.width(), rc.height(), Qt::AlignLeft,m_strRise);
    painter.drawText(rc.x(), rc.y() + 4, rc.width(), rc.height(), Qt::AlignRight,m_strDown);
}

void TradingRatioWidget::formatData(const QVector<HZData::AllStock> &vec)
{
    m_allBuyAmount = 0;
    m_allSellAmmount = 0;
    if(vec.size())
    {
        for(int i = 0; i < 5; i++)
        {
            if(INVALID_UINT64 == vec[0].stk_buy[i].volume
                    &&INVALID_UINT64 == vec[0].stk_sell[i].volume)
            {
                continue;
            }
            m_allBuyAmount += vec[0].stk_buy[i].volume;

            m_allSellAmmount += vec[0].stk_sell[i].volume;
        }
    }

    if(0 == m_allBuyAmount + m_allSellAmmount)
    {
        m_strRise = "";
        m_strDown = "";
        return;
    }

    float buyRadio = (float)m_allBuyAmount / ((float)m_allBuyAmount + (float)m_allSellAmmount);

    float sellRadio = (float)m_allSellAmmount / ((float)m_allBuyAmount + (float)m_allSellAmmount);

    m_strRise = QString::number(buyRadio *100 , 'f', 2) + "%";
    m_strDown = QString::number(sellRadio *100 , 'f', 2) + "% ";
}


BuySellFiveWidget::BuySellFiveWidget(QWidget *parent):
    BaseWidget(parent),
    m_ratio(new TradingRatioWidget),
    m_buyFiveView(new BuySellFiveView(0)),
    m_sellFiveView(new BuySellFiveView(1))

{
    createWg();
}

BuySellFiveWidget::~BuySellFiveWidget()
{

}

void BuySellFiveWidget::updateData(const QVector<HZData::AllStock> &vec)
{
    m_ratio->updateData(vec);
    m_buyFiveView->updateData(vec);
    m_sellFiveView->updateData(vec);
}

void BuySellFiveWidget::createWg()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(m_ratio, 0, 0, 1, 2);
    mainLayout->addWidget(m_buyFiveView, 1, 0, 1, 1);
    mainLayout->addWidget(m_sellFiveView, 1, 1, 1, 1);
}


