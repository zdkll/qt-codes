#include "risefallbarview.h"

#include <QBoxLayout>
#include <functional>

#include "risefallbarchart.h"
#include "datacontroller.h"

RiseFallBarView::RiseFallBarView(QWidget *parent)
    : BaseWidget(parent,"risefallbarview")
    ,m_actived(false),m_subReqId(INVALID_REQID)
{
    m_stockCode = "000000";
    QVBoxLayout *mainLayout = new  QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(CHART_SPACING);

    m_legendRect = new QWidget(this);
    m_legendRect->setFixedHeight(LEGEND_MARKS_HEIGHT);
    mainLayout->addWidget(m_legendRect);

    m_graphRect = new QWidget(this);
    mainLayout->addWidget(m_graphRect);

    m_risefallBarChart = new RiseFallBarChart(this);
    m_risefallLegendChart = new RiseFallLegendChart(this);
    m_risefallBarChart->initial(m_graphRect);
    m_risefallLegendChart->initial(m_legendRect);

    this->setLayout(mainLayout);
}

void RiseFallBarView::setData(const HZData::StockRiseFallInfo &risefallInfo)
{
    m_risefallBarChart->setData(risefallInfo);
    m_risefallLegendChart->setData(risefallInfo);
}

void RiseFallBarView::afterActive()
{
    m_actived = true;
    if(!m_stockCode.isEmpty())
        subRiseFallInfo();
}

void RiseFallBarView::afterDeactive()
{
    m_actived = false;
    cancelSub();
}

void RiseFallBarView::setStockCode(const QString &stockCode)
{
    m_stockCode = stockCode;
    if(m_actived){
        subRiseFallInfo();
    }
}

void  RiseFallBarView::subRiseFallInfo()
{
    cancelSub();
    HZData::ReqStockRiseFallInfo req  = m_stockCode.toStdString();

    onRiseFallInfoArrived  fun= std::bind(&RiseFallBarView::onSubRiseFallInfoArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    //开始订阅
   m_subReqId = gHZDataController->subRiseFallInfo(req,fun);
}
void  RiseFallBarView::cancelSub()
{
    if(m_subReqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void RiseFallBarView::onSubRiseFallInfoArrived(ReqId ,const HZData::StockRiseFallInfo &riseFallInfo,ErrCode errCode)
{
    if(errCode != 0)  return;

    m_risefallBarChart->setData(riseFallInfo);
    m_risefallLegendChart->setData(riseFallInfo);
}
