#include "selfstockrollpanel.h"
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QStyle>
#include <QTimer>
#include <QDebug>
#include "datasubcenter.h"

SelfStockRollPanel::SelfStockRollPanel(BaseWidget *parent):
    BaseWidget(parent),
    m_pTextName(new QLabel(this)),
    m_pTextCode(new QLabel(this)),
    m_pTextPrice(new QLabel(this)),
    m_pArrow(new QLabel(this)),
    m_pTextChange(new QLabel(this)),
    m_pTextChangeRate(new QLabel(this)),
    m_pDataSubCenter(new DataSubCenter(this)),
    m_timer(new QTimer(this)),
    m_bFlg(true)
{

    createWg();

    setObjName();
    m_timer->setInterval(3000);
    connect(m_pDataSubCenter, &DataSubCenter::subStockArrived, this, &SelfStockRollPanel::onSubStockArrived);
    connect(m_timer, &QTimer::timeout, this, &SelfStockRollPanel::onTimeOut);
}

void SelfStockRollPanel::setStockCode(const QString strCode)
{
    m_strCode = strCode;
}

void SelfStockRollPanel::setStockCode(const std::list<string> lists)
{
    m_lists = lists;

    m_stockVec.clear();

    list<string>::iterator it;
    for(it = m_lists.begin(); it != m_lists.end(); it++)
    {
        HZData::SLStock slStock;

        slStock.stock_code = *it;
        m_stockVec.append(slStock);
    }
}

void SelfStockRollPanel::afterActive()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }


    m_nIndex = 0;

    m_pDataSubCenter->startSub(m_lists);

    if(m_lists.size() == 0)
    {
        m_stockVec.clear();
        updateText();
    }

    if(m_bFlg && m_lists.size() > 1)
    {
        m_timer->start();
    }
}

void SelfStockRollPanel::afterDeactive()
{
    m_pDataSubCenter->cancelSub();
    if(m_timer->isActive())
        m_timer->stop();
}

void SelfStockRollPanel::createWg()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    QHBoxLayout *pHLayout1 = new QHBoxLayout;
    QHBoxLayout *pHLayout2 = new QHBoxLayout;
    QFormLayout *formLayout = new QFormLayout;
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);
    pHLayout1->setSpacing(6);
    pHLayout1->setContentsMargins(14,0,0,0);
    pHLayout2->setSpacing(6);
    pHLayout2->setContentsMargins(14,0,0,0);
    formLayout->setSpacing(4);
    formLayout->setMargin(0);
    pHLayout1->addWidget(m_pTextName);
    pHLayout1->addWidget(m_pTextCode);
    pHLayout1->addStretch();
    formLayout->addRow(m_pTextPrice, m_pArrow);
    formLayout->setAlignment(m_pArrow, Qt::AlignVCenter);
    pHLayout2->addLayout(formLayout);
    pHLayout2->addWidget(m_pTextChange);
    pHLayout2->addWidget(m_pTextChangeRate);
    pHLayout2->addStretch();
    pMainLayout->addLayout(pHLayout1);
    pMainLayout->addLayout(pHLayout2);
}

void SelfStockRollPanel::setObjName()
{
    m_pTextName->setObjectName("name");
    m_pTextCode->setObjectName("code");
    m_pTextPrice->setObjectName("price");
    m_pArrow->setObjectName("arrow");
    m_pTextChange->setObjectName("change");
    m_pTextChangeRate->setObjectName("changerate");
}
int SelfStockRollPanel::m_nIndex = 0;
void SelfStockRollPanel::updateText()
{

    HZData::SLStock stock;
    if(!m_stockVec.isEmpty())
        stock = m_stockVec[m_nIndex];

    HzQuoteFormat::QuoteItem item;

    //股票名称
    m_pTextName->setText(QString::fromStdString(stock.stock_name));

    //股票代码
    setTextCode(stock);

    //股票价格
    item = HzQuoteFormat::QuoteFormat::formatPrice(stock.last_price,stock.pre_close_price);
    updateStockField(item,m_pTextPrice);

    if(item.dirc == HzQuoteFormat::quote_up)
    {
        m_pArrow->setProperty("state", "rise");
    }
    else if(item.dirc == HzQuoteFormat::quote_down)
    {
        m_pArrow->setProperty("state", "fall");
    }
    else
    {
        m_pArrow->setProperty("state", "unknown");
    }

    m_pArrow->style()->unpolish(m_pArrow);
    m_pArrow->style()->polish(m_pArrow);
    m_pArrow->update();

    //涨跌
    item = HzQuoteFormat::QuoteFormat::formatChange(stock.up_down_price,DECIMAL_POINT);
    updateStockField(item, m_pTextChange);

    //涨跌幅
    item = HzQuoteFormat::QuoteFormat::formatChangeRate(stock.rate,stock.last_price,stock.pre_close_price,DECIMAL_POINT);

    updateStockField(item, m_pTextChangeRate);
}

void SelfStockRollPanel::updateStockField(const HzQuoteFormat::QuoteItem &item, QLabel *pLabel)
{
    pLabel->setText(item.strValue);
    pLabel->setProperty("QuotePanelDirc",QString::number(item.dirc));
    pLabel->style()->unpolish(pLabel);
    pLabel->style()->polish(pLabel);
    pLabel->update();
}

void SelfStockRollPanel::setTextCode()
{
    QString strcode = m_strCode;
    QString str = strcode.left(2).toUpper();
    strcode.remove(0,2);
    strcode = strcode + "." + str;
    m_pTextCode->setText(strcode);
}

void SelfStockRollPanel::setTextCode(const HZData::SLStock &stock)
{
    QString strcode = QString::fromStdString(stock.stock_code);
    QString str = strcode.left(2).toUpper();
    strcode.remove(0,2);
    strcode = strcode + "." + str;
    m_pTextCode->setText(strcode);
}


void SelfStockRollPanel::onSubStockArrived(char *data, int num)
{
    HZData::SLStock *slData = (HZData::SLStock*)data;


    for(int i = 0; i < num ; i ++)
    {
        for(int j = 0; j < m_stockVec.size(); j++)
        {
            if(m_stockVec[j].stock_code == slData[i].stock_code)
            {
                m_stockVec[j] = slData[i];
            }
        }
    }

    updateText();
}

void SelfStockRollPanel::onTimeOut()
{
    updateText();
    if(++m_nIndex >= m_stockVec.size())
    {
        m_nIndex = 0;
    }
}

void SelfStockRollPanel::onPlayBtnClicked()
{

    if(!m_bFlg)
    {
        m_bFlg = true;
        m_timer->start();
    }
    else
    {
        m_bFlg = false;
        m_timer->stop();
    }
}
