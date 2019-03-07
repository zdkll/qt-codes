#include "chartcrossdlg.h"
#include <QFormLayout>
#include <QWidget>
#include <QLabel>
#include <QDateTime>
#include <QDebug>
#include <QStyle>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>

ChartCrossDlg::ChartCrossDlg(CrossPanel* pPanel,QWidget *parent)
    :BaseWidget(parent),m_pPanel(pPanel)
{
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setFixedSize(CROSS_DLG_WIDTH,CROSS_KLINE_DLG_HEIGHT);
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    pLayout->addWidget(m_pPanel);
    this->setLayout(pLayout);
}

void ChartCrossDlg::setTipData(const HZData::HZKline &crossTipInfo)
{
    if(m_pPanel)
    {
        m_pPanel->updateInfo(crossTipInfo);
    }
}


CrossPanel::CrossPanel(QWidget *parent)
    :BaseWidget(parent)
{
    createWg();
}

void CrossPanel::createWg()
{
    QGridLayout* pLayout = new QGridLayout();
    pLayout->setSpacing(0);
    pLayout->setMargin(10);
    QVector<HzQuoteFormat::QuoteItem> nameList;
    QVector<HzQuoteFormat::QuoteItem> valList;
    convertNameItems(nameList);
    convertValueItems(valList);
    if(nameList.isEmpty() == false && nameList.size() == valList.size())
    {
        QFont ft;
        for(int i = 0;i < nameList.size();i++)
        {
            //字段名称列
            QLabel *pLab = new QLabel();
            pLab->setFixedHeight(20);
            pLab->setText(nameList[i].strValue);
            pLab->setObjectName("filedName");
            pLayout->addWidget(pLab,i,0,Qt::AlignLeft);

            //字段值列
            pLab = new QLabel();
            pLab->setFixedHeight(20);
            pLab->setText(valList[i].strValue);
            pLab->setObjectName("filedValue");
            pLayout->addWidget(pLab,i,1,Qt::AlignRight);

            //保持值字段列，用于数据更新
            m_labVec.push_back(pLab);
        }
        this->setLayout(pLayout);
    }
}

void CrossPanel::updateInfo(const HZData::HZKline &crossTipInfo)
{
    m_tipInfo = crossTipInfo;
    QVector<HzQuoteFormat::QuoteItem> valList;
    convertValueItems(valList);
    if(valList.size() != m_labVec.size())
        return;
    QLabel *pLab = nullptr;
    for(int i=0;i < valList.size();i++)
    {
        pLab = m_labVec[i];
        pLab->setText(valList[i].strValue);
        QString strDirc = "quote_up";
        switch (valList[i].dirc) {
        case HzQuoteFormat::quote_up:
            strDirc = "quote_up";
            break;
        case HzQuoteFormat::quote_down:
            strDirc = "quote_down";
            break;
        case HzQuoteFormat::quote_equal:
        case HzQuoteFormat::quote_unknown:
        {
            strDirc = "quote_equal";
        }
            break;
        default:
            break;
        }
        pLab->setProperty("QuoteDirc", strDirc);
        pLab->style()->unpolish(pLab);
        pLab->style()->polish(pLab);
        pLab->update();
    }
    this->update();
}

CrossPanelKline::CrossPanelKline(QWidget *parent):CrossPanel(parent)
{
    createWg();
    //this->setFixedSize(CROSS_DLG_WIDTH,CROSS_KLINE_DLG_HEIGHT);
}

void CrossPanelKline::convertNameItems(QVector<HzQuoteFormat::QuoteItem> &nameList)
{
    //K线   日期 开盘  最高       最低     收盘   涨跌:“收盘-上一根收盘价”  涨跌幅 成交量(总手)  金额
    HzQuoteFormat::QuoteItem item;
    item.strValue = QStringLiteral("日期");
    nameList.push_back(item);

    item.strValue = QStringLiteral("开盘");
    nameList.push_back(item);

    item.strValue = QStringLiteral("最高");
    nameList.push_back(item);

    item.strValue = QStringLiteral("最低");
    nameList.push_back(item);

    item.strValue = QStringLiteral("收盘");
    nameList.push_back(item);

    item.strValue = QStringLiteral("涨跌");
    nameList.push_back(item);

    item.strValue = QStringLiteral("涨跌幅");
    nameList.push_back(item);

    item.strValue = QStringLiteral("均价");
    nameList.push_back(item);

    item.strValue = QStringLiteral("成交量");
    nameList.push_back(item);

    item.strValue = QStringLiteral("金额");
    nameList.push_back(item);
}

void CrossPanelKline::convertValueItems(QVector<HzQuoteFormat::QuoteItem> &valList)
{
    //K线   日期 开盘  最高       最低     收盘   涨跌:“收盘-上一根收盘价”  涨跌幅 成交量(总手)  金额
    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::formatTime(m_tipInfo.update_time,"yyyy-MM-dd");
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.open_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.high_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.low_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatChange(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.avg_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(m_tipInfo.volume);
    //item = HzQuoteFormat::QuoteFormat::formatVolume(m_tipInfo.volume);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatTurnover(m_tipInfo.turnover);
    valList.push_back(item);
}

CrossPanelMin::CrossPanelMin(QWidget *parent):CrossPanel(parent)
{
    createWg();
    //this->setFixedSize(CROSS_DLG_WIDTH,CROSS_MIN_DLG_HEIGHT);
}

void CrossPanelMin::convertNameItems(QVector<HzQuoteFormat::QuoteItem> &nameList)
{
    //分时: 时间 价格:"收盘价"  涨跌：“收盘价-昨收价”  涨跌幅："涨跌/ 昨收" 均价 金额 总手
    //分时--------------------------------------------------
    HzQuoteFormat::QuoteItem item;
    item.strValue = QStringLiteral("时间");
    nameList.push_back(item);

    item.strValue = QStringLiteral("价格");
    nameList.push_back(item);

    item.strValue = QStringLiteral("涨跌");
    nameList.push_back(item);

    item.strValue = QStringLiteral("涨跌幅");
    nameList.push_back(item);

    item.strValue = QStringLiteral("均价");
    nameList.push_back(item);

    item.strValue = QStringLiteral("金额");
    nameList.push_back(item);

    item.strValue = QStringLiteral("现手");
    nameList.push_back(item);
}

void CrossPanelMin::convertValueItems(QVector<HzQuoteFormat::QuoteItem> &valList)
{
    //分时: 时间 价格:"收盘价"  涨跌：“收盘价-昨收价”  涨跌幅："涨跌/ 昨收" 均价 金额 总手
    //分时--------------------------------------------------
    HzQuoteFormat::QuoteItem item;
    item = HzQuoteFormat::QuoteFormat::formatTime(m_tipInfo.update_time,"hh:mm");
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatChange(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_tipInfo.close_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatPrice(m_tipInfo.avg_price,m_tipInfo.pre_close_price);
    valList.push_back(item);

    item = HzQuoteFormat::QuoteFormat::formatTurnover(m_tipInfo.turnover);
    valList.push_back(item);

    item.strValue = HzQuoteFormat::StringUnitFormat::toFormatString(m_tipInfo.volume);
    item.strValue += QStringLiteral("手");
    //item = HzQuoteFormat::QuoteFormat::formatVolume(m_tipInfo.volume);
    valList.push_back(item);
}
