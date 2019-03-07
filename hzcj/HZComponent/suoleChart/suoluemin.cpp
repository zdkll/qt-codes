#include "suoluemin.h"
#include <QMessageBox>

#include "pubmsgdef.h"

SuolueMin::SuolueMin(SuolueInfo info, QWidget *parent):
    SuolueBase(info,parent),
    m_minChart(nullptr)
{
    if(info.data.Is<QMap<int,QString>>())
    {
        m_codeMap = info.data.AnyCast<QMap<int,QString>>();
        auto it = m_codeMap.find(info.defaultId);
        if(it != m_codeMap.end())
        {
            m_obj = it.value();
        }
    }
}

SuolueMin::~SuolueMin()
{

}

void SuolueMin::afterActive()
{
    m_minChart->afterActive();
}

void SuolueMin::afterDeactive()
{
    m_minChart->afterDeactive();
}

QWidget *SuolueMin::createContentWnd()
{
    m_minChart = new MinCanvas(this);
    if(!m_obj.isEmpty())
    {
       m_minChart->setBaseInfo(m_obj);
       m_minChart->setChartsInfo(getChartInfo());
       m_minChart->createWg();
    }
    return m_minChart;
}

void SuolueMin::onClickItem(const SuolueItem &item)
{
    auto it =  m_codeMap.find(item.id);
    if(it != m_codeMap.end())
    {
        m_obj =it.value();
        m_minChart->switchStockEx(m_obj);
    }
}

void SuolueMin::onClickMore()
{
    QMessageBox::information(this, "test", QStringLiteral("onClickMore"));
}

QVector<ChartItemInfo> SuolueMin::getChartInfo()
{
    QVector<ChartItemInfo> info;
    ChartItemInfo item;
    item.margins.topMargin = 10;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    info.push_back(item);

    item = ChartItemInfo();
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);
    return info;
}

