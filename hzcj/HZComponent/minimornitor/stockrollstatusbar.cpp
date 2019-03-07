#include "stockrollstatusbar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QVariant>
#include <QStyle>
#include <QTimer>
#include "datasubcenter.h"
#include "activitylabel.h"
#include "pubmsgdef.h"

#define  Property_UpDown             "updown"
#define  Property_UpDown_Plain   0
#define  Property_UpDown_Up      1
#define  Property_UpDown_Down  2

StockRollStatusBar::StockRollStatusBar(BaseWidget *parent):
    BaseWidget(parent),
    m_bFlg(false),
    m_pDataProvider(new DataSubCenter(this)),
    m_timer(new QTimer(this))
{

    createWg();
    m_timer->setInterval(3000);
    connect(m_pDataProvider, &DataSubCenter::subStockArrived, this, &StockRollStatusBar::onSubStockArrived);
    connect(m_timer, &QTimer::timeout, this, &StockRollStatusBar::onTimerOut);
}

void StockRollStatusBar::afterActive()
{
    std::list<string> lists;
    lists.push_back("sh000001");
    lists.push_back("sz399001");
    lists.push_back("sz399006");
    lists.push_back("sz399005");

    list<string>::iterator it;
    for(it = lists.begin(); it != lists.end(); it++)
    {
        HZData::SLStock slStock;

        slStock.stock_code = *it;
        m_stockVec.append(slStock);
    }


    m_pDataProvider->startSub(lists);


    m_nIndex = 0;

    if(!m_bFlg)
    {
        if(!m_timer->isActive())
            m_timer->start();
    }
}

void StockRollStatusBar::afterDeactive()
{
    m_pDataProvider->cancelSub();
    if(m_timer->isActive())
        m_timer->stop();
}

void StockRollStatusBar::createWg()
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setSpacing(6);
    pLayout->setContentsMargins(6,0,0,0);
    QPushButton *btn = new QPushButton;
    btn->setObjectName("rivet");

    QFormLayout *pFormLayout = new QFormLayout;
    pFormLayout->setSpacing(0);
    pFormLayout->setMargin(0);
    m_pLabelName = new ActivityLabel(QStringLiteral("上证指数:"));
    m_pLabelText = new ActivityLabel(QStringLiteral("-- -- --"));

    m_pLabelName->setCursor(QCursor(Qt::PointingHandCursor));
    m_pLabelText->setCursor(QCursor(Qt::PointingHandCursor));

    m_pLabelName->setObjectName("name");
    m_pLabelText->setObjectName("text");

    pFormLayout->addRow(m_pLabelName, m_pLabelText);

    pFormLayout->setFormAlignment(Qt::AlignVCenter);

    pLayout->addWidget(btn, 0, Qt::AlignLeft);

    pLayout->addLayout(pFormLayout, 1);

    btn->setProperty("fixed", m_bFlg);
    connect(btn, &QPushButton::clicked, this, &StockRollStatusBar::onBtnClicked);

    connect(m_pLabelName, &ActivityLabel::clicked, this, &StockRollStatusBar::onLabelClicked);
    connect(m_pLabelText, &ActivityLabel::clicked, this, &StockRollStatusBar::onLabelClicked);
}

int StockRollStatusBar::m_nIndex = 0;
void StockRollStatusBar::setIndexText()
{
    if(m_stockVec.isEmpty())
        return;
    QString strLast_pice = m_stockVec[m_nIndex].last_price == INVALID_DOUBLE?Default_Item_String:QString::number(m_stockVec[m_nIndex].last_price, 'f', 2);
    QString strUp_down_price = m_stockVec[m_nIndex].up_down_price == INVALID_DOUBLE?Default_Item_String:QString::number(m_stockVec[m_nIndex].up_down_price, 'f', 2);
    QString  str_identity = m_stockVec[m_nIndex].rate == INVALID_DOUBLE?"":(m_stockVec[m_nIndex].rate < -DBL_EPSILON)?QStringLiteral("↓"):QStringLiteral("↑");
    str_identity = (  -DBL_EPSILON <= m_stockVec[m_nIndex].rate &&  m_stockVec[m_nIndex].rate <= DBL_EPSILON)?"":str_identity;
    QString strRate        = m_stockVec[m_nIndex].rate == INVALID_DOUBLE? Default_Item_String:QString("%1%").arg(m_stockVec[m_nIndex].rate, 0,'f', 2);

    QString strText = strLast_pice + str_identity + "   " + strUp_down_price + "   " + strRate;

    int updown =  0;
    if(m_stockVec[m_nIndex].rate != INVALID_DOUBLE){
        double new_up_down  = QString::number(m_stockVec[m_nIndex].up_down_price,'f',2).toDouble();
        if(new_up_down > DBL_EPSILON)
            updown= Property_UpDown_Up;
        else if(new_up_down<-DBL_EPSILON)
            updown= Property_UpDown_Down;
    }

    m_pLabelName->setText(QString::fromStdString(m_stockVec[m_nIndex].stock_name) + ": ");

    m_pLabelText->setProperty(Property_UpDown,updown);
    m_pLabelText->style()->unpolish(m_pLabelText);
    m_pLabelText->style()->polish(m_pLabelText);
    m_pLabelText->update();
    m_pLabelText->setText(strText);

    m_stockCode = QString::fromStdString(m_stockVec[m_nIndex].stock_code);

}

void StockRollStatusBar::onBtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if(!m_bFlg)
    {
        m_bFlg = true;
        if(m_timer->isActive())
            m_timer->stop();
    }
    else
    {
        m_bFlg = false;
        if(!m_timer->isActive())
            m_timer->start();
    }

    btn->setProperty("fixed", m_bFlg);
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);
    btn->update();
}

void StockRollStatusBar::onSubStockArrived(char *data, int num)
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

    setIndexText();
}

void StockRollStatusBar::onTimerOut()
{
    setIndexText();
    if(++m_nIndex >= m_stockVec.size())
    {
        m_nIndex = 0;
    }
}

void StockRollStatusBar::onLabelClicked()
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(STOCK_ID,m_stockCode);
    sendMessage(ROOT_FRAME,params,getGlobalPageId()== stock_detail_page?false:true);

}






