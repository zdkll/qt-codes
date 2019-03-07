#include "testbasecanvas.h"
#include "testxaxis.h"
#include "testbasechart.h"
#include "testchartfactory.h"
#include "testlayermanager.h"


TestBaseCanvas::TestBaseCanvas(QWidget *parent):
    BaseWidget(parent)
{
    m_layerManager = new TestLayerManager(this);
}

void TestBaseCanvas::setChartsInfo(const QVector<ChartItemInfo> &info)
{
    m_chartsInfo = info;
}

void TestBaseCanvas::createWg()
{
    QVector<QRect> vec = calcChartRc(m_chartsInfo);

    TestXaxis *pxAxis = nullptr;

    TestBaseChart *pChart = nullptr;


    for(int i = 0; i < m_chartsInfo.size(); i++)
    {
        if(m_chartsInfo[i].bShow)
        {
            pChart = createChart(vec[i], m_chartsInfo[i], pxAxis);

            if(m_chartsInfo[i].type == ChartMainType)
            {
                m_mainChart = pChart;
                pxAxis = m_mainChart->getXaxis();
            }

            m_chartVec.append(pChart);
        }
    }


}

QVector<QRect> TestBaseCanvas::calcChartRc(const QVector<ChartItemInfo> &info)
{
    QVector<QRect> retVec;

    QRect tmpRc;

    QRect rc = this->rect();


    tmpRc = rc;

    int nFixHeight = getHeightFromFixed(info);

    int pos = 0;

    for(int i = 0; i < info.size(); i++)
    {
        if(info[i].bShow)
        {
            if(info[i].nHeight != INT_MAX)
            {

                tmpRc.setTop(pos + info[i].padding);
                tmpRc.setBottom(tmpRc.top() + info[i].nHeight);
                pos = tmpRc.bottom();
            }
            else if(info[i].bFillArea)
            {
                tmpRc.setTop(rc.top() + info[i].padding);
            }
            else
            {
                tmpRc.setTop(rc.top() + info[i].padding);
                tmpRc.setBottom(rc.bottom() - nFixHeight);
                pos = tmpRc.bottom();
            }
        }
        retVec.append(tmpRc);
    }

    return retVec;
}

int TestBaseCanvas::getHeightFromFixed(const QVector<ChartItemInfo> &info)
{
    int ret = 0;
    for(int i = 0; i < info.size(); i++)
    {
        if(info[i].bShow && info[i].nHeight != INT_MAX)
        {
                ret += info[i].nHeight + info[i].padding;
        }
    }
    return ret;
}

TestBaseChart *TestBaseCanvas::createChart(QRect rc, const ChartItemInfo &info, TestXaxis *pXaxis)
{
    TestBaseChart *pChart = nullptr;

    if(!info.chartClass.isEmpty())
    {
        std::string strName = info.chartClass.toStdString();
        pChart = testChartFactory::getIoc()->Resolve<TestBaseChart>(strName, this, m_layerManager);

        if(pChart)
        {
            pChart->setMargins(info.margins);
            pChart->setRc(rc, this->rect());
            pChart->createEx();
        }
    }

    return pChart;
}
