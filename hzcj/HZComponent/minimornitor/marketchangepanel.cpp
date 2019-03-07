#include "marketchangepanel.h"
#include <QVBoxLayout>
#include "mincanvas.h"

MarketChangePanel::MarketChangePanel(BaseWidget *parent):
    BaseWidget(parent)
  ,m_ChangedPanel(new HZPlateMoveMarster)
  ,m_pMinWidget(new MinCanvas(this))
{
    m_pMinWidget->setRecordSwitchPage(false);
    setLayout(new QVBoxLayout(this));
    m_pMinWidget->setFixedHeight(202);
    layout()->addWidget(m_pMinWidget);
    layout()->addWidget(m_ChangedPanel);
    layout()->setContentsMargins(0,0,0,0);

    m_pMinWidget->setBaseInfo("sh000001");
    m_pMinWidget->setChartsInfo(getMinChartInfo());
    m_pMinWidget->createWg();
}

void MarketChangePanel::afterActive()
{
    m_pMinWidget->afterActive();
    m_ChangedPanel->afterActive();
}

void MarketChangePanel::afterDeactive()
{
    m_pMinWidget->afterDeactive();
    m_ChangedPanel->afterDeactive();
}

QVector<ChartItemInfo> MarketChangePanel::getMinChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "MinBlockAbnormal";
    item.margins.topMargin = 10;
    item.margins.rightMargin = 15;
    item.attr = ChartHideRightXaxis;
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.margins.rightMargin = 15;
    item.attr = ChartHideRightXaxis;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    item.attr = ChartHideRightXaxis;
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}
