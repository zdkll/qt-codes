#include "basecanvas.h"
#include <QTimer>
#include "imagelayer.h"
#include "ChartFactory.h"
#include "yaxis.h"
#include "xaxis.h"
#include "basechart.h"
#include "imagelayer.h"
#include "indicatorchart.h"
#include "charttitle.h"

BaseCanvas::BaseCanvas(QWidget *parent) : BaseWidget(parent)
{
    m_crossDlg = nullptr;
    m_mainChart = nullptr;
    m_repaint= false;
    setMouseTracking(true);
    m_imgLayer = new ImageLayer(this);
}

BaseCanvas::~BaseCanvas()
{
    m_selectedChart = m_mainChart = m_crossChart = nullptr;
    m_sletModelchartVec.clear();
    m_chartVec.clear();
    if(m_imgLayer)
    {
        delete m_imgLayer;
        m_imgLayer = nullptr;
    }
}

void BaseCanvas::setChartsInfo(const QVector<ChartItemInfo> &info)
{
    m_chartsInfo = info;
}

std::shared_ptr<BaseChart> BaseCanvas::createChart(QRect rc, const ChartItemInfo &info, std::shared_ptr<XAxis> pXAxis)
{
    std::shared_ptr<BaseChart> pChart = nullptr;
    std::shared_ptr<ChartTitle> pChartTitle = nullptr;
    if(info.chartClass.isEmpty() == false)
    {
        std::string name = info.chartClass.toStdString();
        pChart = ChartFactory::getIoc()->ResolveShared<BaseChart>(name,m_imgLayer);
        pChart->createProvider(info.chartProvider);
        if(!info.titleClass.isEmpty())
        {
            name = info.titleClass.toStdString();
            pChartTitle = ChartFactory::getIoc()->ResolveShared<ChartTitle>(name,m_imgLayer);
            pChartTitle->setAttr(info.titleAttr);
            QRect titleRc = rc;
            titleRc.setBottom(rc.top()+ContainerTitleHeight);
            pChartTitle->setRc(titleRc,getContainerAreaRc());

        }
        if(pChart)
        {
            if(pXAxis)
            {
                pChart->setXAxis(pXAxis);
            }
            if(pChartTitle)
            {
                connect(pChartTitle.get(), SIGNAL(signalButtonClose()), pChart.get(),SLOT(soltButtonClose()));
                connect(pChartTitle.get(), SIGNAL(signalButtonParamSet()), pChart.get(),SLOT(soltButtonParamSet()));
                connect(pChartTitle.get(), SIGNAL(signalButtonPlus()), pChart.get(),SLOT(soltButtonPlus()));
                connect(pChartTitle.get(), SIGNAL(signalButtonSub()), pChart.get(),SLOT(soltButtonSub()));
                connect(pChartTitle.get(), SIGNAL(signalRestorationComBoxChanged(int)), this,SLOT(slotsDrTypeChanged(int)));
                connect(pChartTitle.get(), SIGNAL(signalIndiSeltComBoxChanged(int)), pChart.get(),SLOT(soltIndiSeltComBoxChanged(int)));
                pChart->setTitle(pChartTitle);
                pChartTitle->createEx();
            }
            pChart->setMargins(info.margins);
            pChart->setRc(rc,getContainerAreaRc());
            pChart->setAttr(info.attr);
            pChart->setType(info.type);
            pChart->createEx();
        }
        if(info.type == ChartAttachType)
        {
            IndicatorChart* pIndiChart = nullptr;
            QString className = pChart->metaObject()->className();
            if(className == "IndicatorChart")
            {
                pIndiChart = (IndicatorChart*)pChart.get();
                pIndiChart->setIndi(info.indiId);
            }
        }
    }
    return pChart;
}

void BaseCanvas::adjustSelectedArea(const QPoint &pos)
{
    std::shared_ptr<BaseChart> pChart = nullptr;
    for(int i =0 ;i < m_sletModelchartVec.size();i++)
    {
        if(m_sletModelchartVec[i].pChart->getRc().contains(pos))
        {
            pChart = m_sletModelchartVec[i].pChart;
            break;
        }
    }
    if(pChart && pChart != m_selectedChart)
    {
        m_selectedChart = pChart;
        update();
        OnAreaSeletedChange();
    }
}

void BaseCanvas::adjustCrossLineYAxis(const QPoint &pos)
{
    if(m_crossChart == nullptr)
        return;
    for(int i =0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && m_chartVec[i]->adjustCrossLineYAxis(m_crossChart,pos))
            break;
    }
}

void BaseCanvas::adjustCrossDlg(const QPoint &currPos)
{
    if(m_crossDlg && m_crossDlg->isVisible())
    {
       QRect rc = m_crossDlg->geometry();
       if(currPos.x() > rc.left() && currPos.x() < rc.right())//在信息窗口范围类
       {
           QRect tempRc = m_mainChart->getChartMainArea();
           int mid = tempRc.left()+tempRc.width()/2;
           QPoint pt;
           if(currPos.x() < mid) //信息小窗在主窗口左边
           {
               pt = tempRc.topRight();
               pt.setX(pt.x()-rc.width());
           }else //信息小窗在主窗口右边
           {
               pt = tempRc.topLeft();
           }
           m_crossDlg->move(pt);
       }
    }
}

int getHeightFromFixed(const QVector<ChartItemInfo>& info)
{
    int ret = 0;
    for(int i =0;i < info.size();i++)
    {
        if(info[i].bShow && info[i].nHeight != INT_MAX)
        {
            ret += info[i].nHeight;
            ret += info[i].padding;
        }
    }
    return ret;
}

QVector<QRect> BaseCanvas::calcChartRc(const QVector<ChartItemInfo> &info)
{
    QVector<QRect> retVec;
    QRect rc = getContainerAreaRc();
    QRect tempRc;
    int fixedHeight = getHeightFromFixed(info);
    int pos = rc.top();
    for(int i=0;i < info.size();i++)
    {

        if(info[i].bShow)
        {
            tempRc.setRight(rc.right());
            tempRc.setLeft(rc.left());
            if(info[i].nHeight != INT_MAX)
            {
                tempRc.setTop(pos+info[i].padding);
                tempRc.setBottom(tempRc.top()+ info[i].nHeight);
                pos = tempRc.bottom();
            }else if(info[i].bFillArea)
            {
                tempRc.setTop(rc.top() +info[i].padding);
                tempRc.setBottom(rc.bottom());
            }
            else
            {
                tempRc.setTop(rc.top()+info[i].padding);
                tempRc.setBottom(rc.bottom() - fixedHeight);
                pos = tempRc.bottom();
            }
            retVec.push_back(tempRc);
        }
    }
    return retVec;
}

void BaseCanvas::createWg()
{
    //开启定时器
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    timer->setInterval(500);
    timer->start();
    std::shared_ptr<XAxis> pXAxis = nullptr;
    QVector<QRect> vec = calcChartRc(m_chartsInfo);
    std::shared_ptr<BaseChart> pChart = nullptr;
    for(int i=0;i < m_chartsInfo.size();i++)
    {
        if(m_chartsInfo[i].bShow)
        {
            pChart = createChart(vec[i],m_chartsInfo[i],pXAxis);
            m_chartsInfo[i].id = pChart->getId();
            if(m_chartsInfo[i].type == ChartMainType)
            {
               m_mainChart = pChart;
               pXAxis = m_mainChart->getXAxis();

            }else if(m_chartsInfo[i].type == ChartCrossType)
            {
                m_crossChart = pChart;
            }
            m_chartVec.push_back(pChart);
            if(m_chartsInfo[i].bSelectedStyle)
            {
                ChartData data;
                data.pChart = pChart;
                data.info = m_chartsInfo[i];
                if(m_sletModelchartVec.isEmpty())
                {
                    m_selectedChart = pChart;
                }
                m_sletModelchartVec.push_back(data);
            }
        }
    }
    m_crossDlg = createCrossDlg();
    if(m_crossDlg && m_bFlag)
    {
        m_crossDlg->setFixedWidth(120);
        m_crossDlg->move(m_mainChart->getChartMainArea().topLeft());
        m_crossDlg->hide();
       // m_crossDlg->show();
    }
    OnCreateEx();
}

QVector<int> BaseCanvas::addAttachCharts(const QVector<ChartItemInfo> &chartsInfo)
{
    QVector<int> idList;
    if(m_mainChart == nullptr)
        return idList;
    int pos = 0;
    for(int i = m_chartsInfo.size()-1;i >=0;i--)
    {
        if(m_chartsInfo[i].type == ChartAttachType)
        {
           pos =i;
           for(int j = 0;j < chartsInfo.size();j++)
           {
               m_chartsInfo.insert(pos+j,chartsInfo[j]);
           }
           break;
        }
    }
    QVector<QRect> vec = calcChartRc(m_chartsInfo);
    std::shared_ptr<BaseChart> pChart = nullptr;
    for(int i=pos,num=0;i < m_chartsInfo.size();i++,num++)
    {
        if(num >= chartsInfo.size())
            break;
        if(m_chartsInfo[i].bShow && m_chartsInfo[i].type == ChartAttachType)
        {
            pChart = createChart(vec[i],m_chartsInfo[i],m_mainChart->getXAxis());
            m_chartsInfo[i].id = pChart->getId();
            idList.push_back(pChart->getId());
            m_chartVec.insert(pos,pChart);
            if(m_chartsInfo[i].bSelectedStyle)
            {
                ChartData data;
                data.pChart = pChart;
                data.info = m_chartsInfo[i];
                if(m_sletModelchartVec.isEmpty())
                {
                    m_selectedChart = pChart;
                }
                m_sletModelchartVec.push_back(data);
            }
        }
    }
    //调整，重绘
    resize();
    return idList;
}

void BaseCanvas::removeAttachCharts(const QVector<int> &idList,bool bResize)
{
    //删除所有附图
    for(auto it =  m_chartsInfo.begin();it != m_chartsInfo.end();)
    {
        if(idList.indexOf(it->id) != -1)
        {
            it = m_chartsInfo.erase(it);
            continue;
        }
        it++;
    }

    for(auto it = m_chartVec.begin();it != m_chartVec.end();)
    {
        std::shared_ptr<BaseChart> pChart = *it;
        if(idList.indexOf(pChart->getId()) != -1)
        {
            it = m_chartVec.erase(it);
            continue;
        }
        it++;
    }

    for(auto it = m_sletModelchartVec.begin();it != m_sletModelchartVec.end();)
    {
        if(idList.indexOf(it->info.id) != -1)
        {
            m_sletModelchartVec.erase(it);
            continue;
        }
        it++;
    }

    if(m_selectedChart && idList.indexOf(m_selectedChart->getId()) != -1)
        m_selectedChart = nullptr;
    if(bResize)
        resize();
}

QRect BaseCanvas::getContainerAreaRc(QResizeEvent *event)
{
    return getWndRc();
}

IndicatorChart* BaseCanvas::getSlectedIndiChart()
{
    IndicatorChart* pChart = nullptr;
    if(m_selectedChart)
    {
        BaseChart* pBaseChart = m_selectedChart.get();
        QString className = pBaseChart->metaObject()->className();
        if(className == "IndicatorChart")
        {
            pChart = (IndicatorChart*)pBaseChart;
        }
    }
    return pChart;
}

void BaseCanvas::proceCrossWndInfoChange(const HZData::HZKline &info, bool bEndIndex, bool bKeyEvent)
{
    emit signalCrossWndInfoChange(info,bEndIndex,bKeyEvent);
    if(m_crossDlg)
        m_crossDlg->setTipData(info);
}

void BaseCanvas::setCreateCrossDlgFlag(bool bFlag)
{
    m_bFlag = bFlag;
}

void BaseCanvas::setEnterAndDbClickSwitchKlineMin(bool bEnable)
{
    m_bSwitchKlinAnMinEnable = bEnable;
}

void BaseCanvas::switchDrType()
{
    if(m_mainChart && m_mainChart->getTitle())
    {
       std::shared_ptr<ChartTitle> pTitle =  m_mainChart->getTitle();
       if(pTitle)
       {
           pTitle->switchDrType();
           emit signalDrTypeChanged(m_mainChart->getDrType(),false);
       }
    }
}

void BaseCanvas::setDrType(RestorationType type)
{
    if(m_mainChart)
        m_mainChart->setDrType(type);
}

RestorationType BaseCanvas::getDrType()
{
    RestorationType type = RestorationNone;
    if(m_mainChart)
       type = m_mainChart->getDrType();
    return type;
}

void BaseCanvas::setAttachIndi(int indi)
{

}

int BaseCanvas::getAttachIndi()
{
    return -1;
}

QRect BaseCanvas::getWndRc()
{
    return QRect(0,0,this->width(),this->height());
}

void BaseCanvas::OnAreaSeletedChange()
{
    IndicatorChart* pChart = getSlectedIndiChart();
    if(pChart)
    {
        emit signalAttachIndiSeletedChange(pChart->getIndiId());
    }
}

void BaseCanvas::OnCreateEx()
{

}

void BaseCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(m_selectedChart)
    {
       QRect rc = m_selectedChart->getRc();
       QPen pen1;
       pen1.setColor(getContainerBorderClr());
       pen1.setStyle(Qt::PenStyle::SolidLine);
       pen1.setWidth(1);
       painter.setPen(pen1);
       painter.drawRect(rc);
    }
    painter.fillRect(getWndRc(),getBkClr());
    if(m_imgLayer)
        m_imgLayer->repaint();
    BaseWidget::paintEvent(event);
}

void BaseCanvas::resizeEvent(QResizeEvent *event)
{
    resize();
    BaseWidget::resizeEvent(event);
}

void BaseCanvas::mouseMoveEvent(QMouseEvent *event)
{
    adjustCrossLineYAxis(event->pos());
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && m_chartVec[i]->mouseMoveEx(event) == false)
            break;
    }
    adjustCrossDlg(event->pos());
    BaseWidget::mouseMoveEvent(event);
}

void BaseCanvas::mousePressEvent(QMouseEvent *event)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->mousePressEx(event))
            break;
    }
    adjustSelectedArea(event->pos());
    BaseWidget::mousePressEvent(event);
}

void BaseCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->mouseReleaseEx(event))
            break;
    }
    BaseWidget::mouseReleaseEvent(event);
}

void BaseCanvas::enterEvent(QEvent *event)
{
    this->setFocus();
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->enterEventEx(event))
            break;
    }
    if(m_crossDlg && m_crossDlg->isHidden() && m_bCrossIsShow)
    {
        m_crossDlg->show();
    }
    BaseWidget::enterEvent(event);
}

void BaseCanvas::leaveEvent(QEvent *event)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->leaveEventEx(event))
            break;
    }
    if(m_crossDlg && m_crossDlg->isVisible())
    {
        m_crossDlg->hide();
    }
    if(m_mainChart)
        m_mainChart->getXAxis()->setCurrCrossXIndex(-1);
    BaseWidget::leaveEvent(event);
}

void BaseCanvas::keyPressEvent(QKeyEvent *event)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->keyPressEx(event))
            break;
    }
    BaseWidget::keyPressEvent(event);
}

void BaseCanvas::keyReleaseEvent(QKeyEvent *event)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx() && false == m_chartVec[i]->keyReleaseEx(event))
            break;
    }
    BaseWidget::keyReleaseEvent(event);
}

void BaseCanvas::showEvent(QShowEvent *e)
{
    if(m_crossDlg && m_crossDlg->isVisible())
    {
        m_crossDlg->hide();
    }
    m_bCrossIsShow = false;
    BaseWidget::showEvent(e);
}

void BaseCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_crossDlg)
    {
        if(m_crossDlg->isHidden())
            m_crossDlg->show();
        adjustCrossDlg(event->pos());
        m_bCrossIsShow = true;
    }
    BaseWidget::mouseDoubleClickEvent(event);
}

void BaseCanvas::paintImmet()
{
    this->update();
}

void BaseCanvas::repainterOnTime()
{
    m_repaint = true;
}

void BaseCanvas::afterActive()
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx())
            m_chartVec[i]->afterActive();
    }
}

void BaseCanvas::afterDeactive()
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx())
            m_chartVec[i]->afterDeactive();
    }
}

ChartCrossDlg *BaseCanvas::createCrossDlg()
{
    return nullptr;
}

void BaseCanvas::onTimerOut()
{
    if(m_repaint)
    {
        this->update();
        m_repaint = false;
    }
}

void BaseCanvas::switchAttachIndicator(int id)
{

}

void BaseCanvas::slotsDrTypeChanged(int id)
{
    if(m_mainChart)
      m_mainChart->soltRestorationComBoxChanged(id);
    emit signalDrTypeChanged(id,true);//先发通知
}

void BaseCanvas::showCrossLine(bool bShow)
{
    if(m_crossChart)
    {
        m_crossChart->showEx(bShow);
        update();
    }
}

bool BaseCanvas::isShowCrossLine()
{
    bool bShow = false;
    if(m_crossChart)
    {
        bShow = m_crossChart->isShowEx();
    }
    return bShow;
}

void BaseCanvas::hideChart(bool bHide, int type)
{
    for(int i=0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->type())
        {
            if(m_chartVec[i]->isShowEx() && bHide ||
               m_chartVec[i]->isShowEx() == false && bHide == false)
            {
                 m_chartVec[i]->showEx(!bHide);
                 if(i < m_chartsInfo.size())
                 {
                     m_chartsInfo[i].bShow = false;
                 }
                 resize();
                 break;
            }
        }
    }
}

void BaseCanvas::resize()
{
    QRect rc = getContainerAreaRc();
    QVector<QRect> vec = calcChartRc(m_chartsInfo);
    for(int i =0;i < m_chartVec.size();i++)
    {
        if(m_chartVec[i]->isShowEx())
        {
            m_chartVec[i]->setRc(vec[i],rc);
            m_chartVec[i]->resize();
        }
    }
}
