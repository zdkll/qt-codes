#include "minchartwidget.h"
#include <QDebug>
#include <QBoxLayout>
#include "subtab.h"
#include "mincanvas.h"
#include "klinecanvas.h"

#define MinChartWidgetMin 0
#define MinChartWidgetKline 1

MinChartWidget::MinChartWidget(QWidget *parent, QString instName):
    BaseWidget(parent, instName)
{
    createWg();
    m_period= HZData::KLINE_PERIOD_MIN;
}

QVector<ChartItemInfo> getMinChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    item.attr = MinChartShowObjCode|MinTitleShowCanvasName|MinTitleShowAvgData|MinChartDrawRealLine;
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithPlusBt|ChartTitleWithSubBt;
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 1;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);

    return info;
}

QVector<ChartItemInfo> getKlineChartInfo()
{
    QVector<ChartItemInfo> info;
    ChartItemInfo item;
    item.chartClass = "KlineChart";
    item.titleClass = "ChartTitle";
    item.titleAttr = ChartTitleWithPlusBt|ChartTitleWithSubBt;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 3;
    item.chartClass = "KlineTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

void MinChartWidget::createWg()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);

    pMainLayout->addWidget(createTab());
    pMainLayout->addSpacing(1);
    pMainLayout->addWidget(createBody());
}

void MinChartWidget::OnStockCodeChange()
{
    m_minWnd->setBaseInfo(m_stockCode);
    m_klineWnd->setBaseInfo(m_stockCode,m_period);
}

void MinChartWidget::afterActive()
{
    BaseWidget* pWnd = (BaseWidget*)m_pStackWnd->currentWidget();
    if(pWnd)
    {
        pWnd->afterActive();
    }
}

void MinChartWidget::afterDeactive()
{
    BaseWidget* pWnd = (BaseWidget*)m_pStackWnd->currentWidget();
    if(pWnd)
    {
        pWnd->afterDeactive();
    }
}

QWidget *MinChartWidget::createTab()
{
    m_table = new CSubTab();
    m_table->setContentsMargins(0,0,0,0);
    m_table->setPacing(1);
    m_table->setObjectName("subtab");
    m_table->addButton(QStringLiteral("分时"),HZData::KLINE_PERIOD_MIN);
    m_table->addButton(QStringLiteral("5分钟"),HZData::KLINE_PERIOD_5MIN);
    m_table->addButton(QStringLiteral("15分钟"),HZData::KLINE_PERIOD_15MIN);
    m_table->addButton(QStringLiteral("60分钟"),HZData::KLINE_PERIOD_HOUR);
    m_table->addButton(QStringLiteral("日K"),HZData::KLINE_PERIOD_DAY);
    m_table->addButton(QStringLiteral("周K"),HZData::KLINE_PERIOD_WEEK);
    m_table->addButton(QStringLiteral("月K"),HZData::KLINE_PERIOD_MONTH);

    m_table->setStretchLastType(CSubTab::StretchLastType::Empty_Strecth);
    connect(m_table,SIGNAL(currentButtonChanged(int)),this,SLOT(slotsSwitchWnd(int)));

    QHBoxLayout* pLayout = new QHBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->addSpacing(2);
    pLayout->addWidget(m_table);
    QWidget* pWnd = new QWidget;
    pWnd->setFixedHeight(26);
    pWnd->setObjectName("tab");
    pWnd->setLayout(pLayout);
    return pWnd;
}

QWidget * MinChartWidget::createBody()
{
    m_pStackWnd = new QStackedWidget(this);
    m_minWnd = new MinCanvas(m_pStackWnd);
    m_klineWnd = new KlineCanvas(m_pStackWnd);
    m_pStackWnd->addWidget(m_minWnd);
    m_pStackWnd->addWidget(m_klineWnd);

    m_klineWnd->setBaseInfo(m_stockCode,m_period);
    m_klineWnd->setChartsInfo(getKlineChartInfo());
    m_klineWnd->createWg();

    m_minWnd->setBaseInfo(m_stockCode);
    m_minWnd->setChartsInfo(getMinChartInfo());
    m_minWnd->createWg();
    return m_pStackWnd;
}

void MinChartWidget::slotsSwitchWnd(int nId)
{
    if(HZData::KLINE_PERIOD_MIN == nId) //处理分时
    {
        if(!m_minWnd->isVisible())
        {
            m_klineWnd->afterDeactive();
            m_pStackWnd->setCurrentIndex(MinChartWidgetMin);
        }
        m_minWnd->setBaseInfo(m_stockCode);
        m_minWnd->afterActive();

    }else //K线
    {
        if(!m_klineWnd->isVisible())
        {
            m_minWnd->afterDeactive();
            m_pStackWnd->setCurrentIndex(MinChartWidgetKline);
        }
        m_period = nId;
        m_klineWnd->setBaseInfo(m_stockCode,nId);
        m_klineWnd->afterActive();

    }
}
