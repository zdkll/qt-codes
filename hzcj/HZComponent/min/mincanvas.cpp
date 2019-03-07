#include "mincanvas.h"
#include "hzcfg.h"
#include "minchart.h"
#include "indicatorchart.h"
#include "xaxis.h"
#include "pubmsgdef.h"

MinCanvas::MinCanvas(QWidget *parent):BaseCanvas(parent)
{
    m_obj = "sh600000";
    m_day = -1;
    m_indiMap = Hzcfg::getInstance()->getIndiIndexMap();

}

void MinCanvas::setBaseInfo(const QString &obj,int day)
{
    m_obj = obj;
    m_day = day;
}

void MinCanvas::switchStockEx(const QString &obj)
{
   m_obj = obj;
   Minchart* pChart = getMinChart();
   if(pChart)
   {
       pChart->switchStock(m_obj,m_day);
   }
}

void MinCanvas::keyPressEvent(QKeyEvent *event)
{
    BaseCanvas::keyPressEvent(event);
    if(Qt::Key_Return == event->key() && m_bSwitchKlinAnMinEnable)
    {
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_obj);
        params.emplace(PERIOD_TYPE,QString::number(TabMinType)); //可选 周期; //可选 周期
        sendMessage(ROOT_FRAME,params,true);
    }else if(Qt::Key_Left == event->key() || Qt::Key_Right == event->key())
    {
        postCrossWndInfo(true);
    }
}

ChartCrossDlg *MinCanvas::createCrossDlg()
{
    ChartCrossDlg * dlg = nullptr;
    if(m_bFlag)
    {
        dlg = new ChartCrossDlg(new CrossPanelMin(this),this);
        dlg->setFixedHeight(150);
    }
    return dlg;
}

void MinCanvas::setAttachIndi(int indi)
{
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        pChart->setIndi(indi);
    }
}

int MinCanvas::getAttachIndi()
{
    int id  = -1;
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        id = pChart->getIndiId();
    }
    return id;
}

void MinCanvas::setRecordSwitchPage(bool bRecord)
{
    m_recordPage = bRecord;
}

void MinCanvas::switchAttachIndicator(int id)
{
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        IndiChartInfo info;
        info.indiId = id;
        info.obj = m_obj;
        info.period = HZData::KLINE_PERIOD_MIN;
        pChart->switchIndi(info);
    }
}

void MinCanvas::mouseMoveEvent(QMouseEvent *event)
{
    BaseCanvas::mouseMoveEvent(event);
    postCrossWndInfo(false);
}

void MinCanvas::switchStock(const QString &obj, int daysIndex)
{
    m_obj = obj;
    if(daysIndex < 0)
        m_day = daysIndex;
    else
        m_day= -1;
    Minchart* pChart = getMinChart();
    if(pChart)
    {
        pChart->switchStock(m_obj,daysIndex);
    }
}

void MinCanvas::adjustMultip(int num)
{
//    int currNum = 0;
//    for(auto it = m_areaConfigInfo.begin();it != m_areaConfigInfo.end();it++)
//    {
//        if(it->first <= Area5)
//            currNum++;
//    }
//    if(num < currNum)
//    {
//        int type = Area5;
//        for(int i = currNum - num;i > 0 && type > mainArea;)
//        {
//            auto iter = m_areaConfigInfo.find((AreaIndex)type--);
//            if(iter != m_areaConfigInfo.end())
//            {
//                slotContainerClose((AreaIndex)iter->first);
//                i--;
//            }
//        }
//    }else if(num  > currNum)
//    {
//        int mainId = INT_MAX;
//        std::vector<int> attachIndiList;
//        getIndiIdList(mainId,attachIndiList);
//        if(mainId == INT_MAX || m_indiMap.empty() || attachIndiList.empty())
//            return;
//        unsigned int indiSize = attachIndiList.size()-1;
//        for(int i = currNum;i < num;i++)
//        {
//            ContainerInfo item;
//            item.containerClass = "ChartContainer";
//            item.chartClass = "IndicatorChart";
//            item.bFixed = true;
//            item.nHeight = 80;
//            if(indiSize >= 0 && indiSize < attachIndiList.size())
//                item.cmd = getIndiCmd(attachIndiList[indiSize--]);
//            addContainer(item);
//        }
//    }
}

void MinCanvas::afterActive()
{
    switchStock(m_obj,m_day);
    BaseCanvas::afterActive();
}

void MinCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_bSwitchKlinAnMinEnable)
    {
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_obj);
        params.emplace(PERIOD_TYPE,QString::number(TabMinType)); //可选 周期 //可选 周期
        bool bRecord = false;
        if(m_recordPage)
            bRecord = true;
        sendMessage(ROOT_FRAME,params,bRecord);
    }
//    Minchart* pMainChart = getMinChart();
//    if(pMainChart)
//    {
//        const QVector<HZData::HZKline>& klineVec =  pMainChart->getKlineData();
//        int i = pMainChart->getXAxis()->xCanvasPTxToIndex(event->pos().x());
//        if(i >= 0)
//        {
//            emit signalDbClicked(klineVec[i],i == klineVec.size() -1?true:false,false);
//        }
//    }
   BaseCanvas::mouseDoubleClickEvent(event);
}

void MinCanvas::postCrossWndInfo(bool bKeyEvent)
{
    Minchart* pMainChart = getMinChart();
    if(pMainChart)
    {
        const QVector<HZData::HZKline>& klineVec =  pMainChart->getKlineData();
        int i = pMainChart->getXAxis()->getCurrCrossXIndex();
        if(i < klineVec.size() && i >= 0)
        {
            HZData::HZKline kline =klineVec[i];
            proceCrossWndInfoChange(kline,i == klineVec.size()-1?true:false,bKeyEvent);
        }
    }
}

QString MinCanvas::getBaseCmd(const QString &obj, int day)
{
    MessageParams params;
    params["obj"] = obj;
    params["day"] = QString::number(day);
    QString msg = HZObject::createMsg(SwitchStockInfo,params);
    return msg;
}

QString MinCanvas::getIndiCmd(int id)
{
    MessageParams params;
    params["id"] = QString::number(id);
    params["obj"] = m_obj;
    params["period"] = QString::number(1);
    params["day"] = QString::number(m_day);
    QString msg = HZObject::createMsg(SwitchIndicatorInfo,params);
    return msg;
}

void MinCanvas::getIndiIdList(int &mainId, std::vector<int> &attachIndiList)
{
    mainId = INT_MAX;
    for(auto it = m_indiMap.begin();it != m_indiMap.end();it++)
    {
        IndiItemInfo& info = it->second;
        if(info.drawArea == 0)
        {
            mainId = info.id;
        }else
        {
            attachIndiList.push_back(info.id);
        }
    }
}

Minchart *MinCanvas::getMinChart()
{
    Minchart * pChart = nullptr;
    if(m_mainChart)
    {
        pChart =  (Minchart*)m_mainChart.get();
    }
    return pChart;
}
