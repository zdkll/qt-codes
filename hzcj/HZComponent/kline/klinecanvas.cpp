#include "klinecanvas.h"
#include <qpainter.h>
#include <QTimer>

#include "hzcfg.h"
#include "imagelayer.h"
#include "basechart.h"
#include "indicatorchart.h"
#include "klinechart.h"
#include "xaxis.h"
#include "yaxis.h"
#include "pubmsgdef.h"

#include <QDebug>

KlineCanvas::KlineCanvas(QWidget *parent) :
    BaseCanvas(parent),
    m_press(false)
{
    m_obj = "sh600000";
    m_period = 0;
    m_mainIndiId =INT_MAX;
    m_indiMap = Hzcfg::getInstance()->getIndiIndexMap();
    getIndiIdList(m_mainIndiId,std::vector<int>());
    this->installEventFilter(this);
}

void KlineCanvas::paintEvent(QPaintEvent *event)
{
    BaseCanvas::paintEvent(event);
}

void KlineCanvas::setBaseInfo(const QString &obj, int period)
{
    m_obj = obj;
    m_period = period;
    KlineChart * pWnd = getKlineChart();
    if(pWnd)
        pWnd->setBaseInfo(m_obj,m_period,m_mainIndiId);
}

void KlineCanvas::switchStock(const QString &obj, int period)
{
    m_obj = obj;
    m_period = period;
    getKlineChart()->switchObj(obj,period);
}

void KlineCanvas::switchPeriod(int period)
{
    m_period = period;
    switchStock(m_obj,period);
}

void KlineCanvas::switchMainIndicator(int id)
{
    m_mainIndiId = id;
    getKlineChart()->switchIndi(id);
}

void KlineCanvas::switchAttachIndicator(int id)
{
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        //removeAttachCharts(m_templateIdList);
        IndiChartInfo info;
        info.indiId = id;
        info.obj = m_obj;
        info.period = m_period;
        pChart->switchIndi(info);
    }
}

void KlineCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    KlineChart* pMainChart = getKlineChart();
    if(pMainChart)
    {
        const QVector<HZData::HZKline>& klineVec =  pMainChart->getKlineData();
        int i = pMainChart->getXAxis()->xCanvasPTxToIndex(event->pos().x());
        if(i >= 0)
        {
            std::shared_ptr<YAxis> pyaxis = pMainChart->getYAxis();
            double hightPt = pyaxis->yCanvasValueToPt(klineVec[i].high_price);
            double lowPt = pyaxis->yCanvasValueToPt(klineVec[i].low_price);
            bool bIn = false;
            if(event->pos().y() <= lowPt && event->pos().y() >= hightPt)
            {
                 bIn =true;
            }
            emit signalDbClicked(klineVec[i],i == klineVec.size() -1?true:false,bIn);
        }else if(m_bSwitchKlinAnMinEnable)
        {
            MessageParams params;
            params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
            params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
            params.emplace(STOCK_ID,m_obj);
            params.emplace(PERIOD_TYPE,QString::number(TabKlineDayType));
            sendMessage(ROOT_FRAME,params,true);
        }
    }
   BaseCanvas::mouseDoubleClickEvent(event);
}

void KlineCanvas::mousePressEvent(QMouseEvent *event)
{
    if(event && event->button() == Qt::LeftButton)
        setCursor(Qt::SizeHorCursor);
    BaseCanvas::mousePressEvent(event);
}

void KlineCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    BaseCanvas::mouseReleaseEvent(event);
    if(event && event->button() == Qt::LeftButton)
        setCursor(Qt::ArrowCursor);
}

void KlineCanvas::keyPressEvent(QKeyEvent *event)
{
    BaseCanvas::keyPressEvent(event);
    if(Qt::Key_Return == event->key() && m_bSwitchKlinAnMinEnable)
    {
        MessageParams params;
        params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
        params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        params.emplace(STOCK_ID,m_obj);
        params.emplace(PERIOD_TYPE,QString::number(TabKlineDayType));
        sendMessage(ROOT_FRAME,params,true);
    }else if(Qt::Key_Left == event->key() || Qt::Key_Right == event->key())
    {
        postCrossWndInfo(true);
    }
}

ChartCrossDlg *KlineCanvas::createCrossDlg()
{
    ChartCrossDlg * dlg = nullptr;
    if(m_bFlag)
    {
        dlg = new ChartCrossDlg(new CrossPanelKline(this),this);
        dlg->setFixedHeight(200);
    }
    return dlg;
}

void KlineCanvas::setAttachIndi(int indi)
{
   IndicatorChart* pChart = getSlectedIndiChart();
   if(pChart)
   {
       pChart->setIndi(indi);
   }
}

int KlineCanvas::getAttachIndi()
{
    int id  = -1;
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        id = pChart->getIndiId();
    }
    return id;
}

void KlineCanvas::switchTemplate(const QVector<ChartItemInfo>& attachIndiList,int mainIndi)
{
    //添加附图指标
    removeSrvIndi();
    if(!attachIndiList.isEmpty())
        m_templateIdList = addAttachCharts(attachIndiList);

    //判断在模板中是否有主图
    if(mainIndi != -1)
    {
        getKlineChart()->switchSrvIndi(mainIndi);
    }
}

void KlineCanvas::removeSrvIndi()
{
    if(!m_templateIdList.isEmpty())
    {
        getKlineChart()->removSrvIndi();
        removeAttachCharts(m_templateIdList,true);
        m_templateIdList.clear();
    }
}

void KlineCanvas::mouseMoveEvent(QMouseEvent *event)
{
     BaseCanvas::mouseMoveEvent(event);
     postCrossWndInfo(false);
}

void KlineCanvas::OnCreateEx()
{
    KlineChart * pChart = getKlineChart();
    if(pChart)
    {
        pChart->setBaseInfo(m_obj,m_period,m_mainIndiId);
    }
    //开启定时器
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
//    timer->setInterval(15000);
//    timer->start();
}

void KlineCanvas::getIndiIdList(int &mainId, std::vector<int> &attachIndiList)
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
    mainId = 0;//先临时处理为第一个配置的指标，后期要更改，否则第一个是附图的话，就会有问题
}

KlineChart *KlineCanvas::getKlineChart()
{
    KlineChart * pChart = nullptr;
    if(m_mainChart)
    {
        pChart =  (KlineChart *)m_mainChart.get();
    }
    return pChart;
}

void KlineCanvas::postCrossWndInfo(bool bKeyEvent)
{
    KlineChart* pMainChart = getKlineChart();
    if(pMainChart)
    {
        const QVector<HZData::HZKline>& klineVec =  pMainChart->getKlineData();
        int i = pMainChart->getXAxis()->getCurrCrossXIndex();
        if(i < klineVec.size() && i >= 0)
        {
            HZData::HZKline kline =klineVec[i];
            if(i - 1 >= 0)
            {
                kline.pre_close_price = klineVec[i - 1].close_price;
            }else
            {
                if(klineVec[i].pre_close_price == 0 )
                    kline.pre_close_price = klineVec[i].open_price;
                else
                    kline.pre_close_price = klineVec[i].pre_close_price;
            }
            proceCrossWndInfoChange(kline,i == klineVec.size()-1?true:false,bKeyEvent);
        }
    }
}


QString KlineCanvas::getBaseCmd(const QString &obj, int period,int mainIndiId)
{
    MessageParams params;
    params["obj"] = obj;
    params["period"] = QString::number(period);
    if(mainIndiId != INT_MAX)
        params["indid"] = QString::number(mainIndiId);
    QString msg = HZObject::createMsg(SwitchStockInfo,params);
    return msg;
}

QString KlineCanvas::getIndiCmd(int id)
{
    MessageParams params;
    params["id"] = QString::number(id);
    params["obj"] = m_obj;
    params["period"] = QString::number(m_period);
    QString msg = HZObject::createMsg(SwitchIndicatorInfo,params);
    return msg;
}
