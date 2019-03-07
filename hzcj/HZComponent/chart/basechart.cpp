#include "basechart.h"
#include "xaxis.h"
#include "basecanvas.h"
#include "imagelayer.h"
#include "charttitle.h"
#include "basecanvas.h"

BaseChart::BaseChart(ImageLayer* layer):Drawable(layer)
{
    static int id = 0;
    m_id = id++;
    m_attr = 0;
    m_xAxis= nullptr;
    m_yAxis =  nullptr;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_maxIndex = -1;
    m_minIndex = -1;
    m_drType = RestorationNone;
}

BaseChart::~BaseChart()
{
    if(m_xAxis && m_xaxisObserverKey != -1)
    {
        m_xAxis->getObserverMgr().Disconnect(m_xaxisObserverKey);
    }
}

void BaseChart::setXAxis(std::shared_ptr<XAxis> pXAxis,bool bBindObserver)
{
    if(m_xAxis != pXAxis)
    {
        m_xAxis = pXAxis;
        if(bBindObserver)
            m_xaxisObserverKey = m_xAxis->getObserverMgr().Connect(std::bind(&BaseChart::OnXaxisChange,this,std::placeholders::_1));
    }
}

void BaseChart::setYAxis(std::shared_ptr<YAxis> pYAxis)
{
    m_yAxis = pYAxis;
}

std::shared_ptr<XAxis> BaseChart::getXAxis()
{
    return m_xAxis;
}

std::shared_ptr<YAxis> BaseChart::getYAxis()
{
    return m_yAxis;
}

void BaseChart::setTitle(std::shared_ptr<ChartTitle> title)
{
    m_title = title;
}

std::shared_ptr<ChartTitle> BaseChart::getTitle()
{
    return m_title;
}

void BaseChart::setMargins(ChartMargins margins)
{
    m_margins = margins;
}

ChartMargins BaseChart::getMargins()
{
    return m_margins;
}

const DataKLine &BaseChart::getScreenData() const
{
    return m_vKlineData;
}

bool BaseChart::isDrawCrossLintPt()
{
    return true;
}

bool BaseChart::isDrawAvgLine()
{
    return false;
}

DataProvider *BaseChart::getProvider()
{
    return nullptr;
}

void BaseChart::createProvider(const QString& provider)
{

}

int BaseChart::getAttr()
{
    return m_attr;
}

void BaseChart::setAttr(int attr)
{
    m_attr = attr;
}

void BaseChart::setDrType(RestorationType type)
{
    m_drType = type;
}

RestorationType BaseChart::getDrType()
{
    return m_drType;
}
//const std::map<qint64, int> &BaseChart::getTimeToIndexMap() const
//{
//    return timeIndexMap_;
//}

//const std::map<int,qint64> &BaseChart::getIndexToTimeMap() const
//{
//    return indexMap_;
//}

//const std::map<qint64, QVariant> &BaseChart::getChartDataTimeMap() const
//{
//    return timeMap_;
//}

void BaseChart::switchParam(const QString &msg)
{

}

void BaseChart::setParam(const QString &msg)
{

}

QRect BaseChart::getChartMainArea()
{
    QRect rc = m_rc;
    if(m_title)
        rc.setTop(m_title->getRc().bottom()+TitleAndChartInterval);
    int leftWidht = YaxisKlineMinWidth,rightWidth = YaxisKlineMinWidth;
    if(ChartHideRightXaxis&m_attr)
        rightWidth = 0;
    if(ChartHideLeftXaxis &m_attr)
        leftWidht = 0;
    rc.setLeft(m_rc.left() + leftWidht + m_margins.leftMargin);
    rc.setRight(m_rc.right() - rightWidth - m_margins.rightMargin);
    rc.setTop(rc.top() +  m_margins.topMargin);
    rc.setBottom(rc.bottom() -  m_margins.bottomMargin);
    return rc ;
}

QRect BaseChart::getRightAxisArea()
{
    if(ChartHideRightXaxis&m_attr)
        return QRect();
    QRect rc = m_rc;
    if(m_title)
        rc.setTop(m_title->getRc().bottom()+TitleAndChartInterval);
    rc.setRight(m_rc.right()-m_margins.rightMargin);
    rc.setLeft(m_rc.right() - YaxisKlineMinWidth);
    rc.setTop(rc.top() + m_margins.topMargin);
    rc.setBottom(rc.bottom() - m_margins.bottomMargin);
    return rc ;
}

QRect BaseChart::getLeftAxisArea()
{
    if(ChartHideLeftXaxis&m_attr)
        return QRect();
    QRect rc = m_rc;
    if(m_title)
        rc.setTop(m_title->getRc().bottom()+TitleAndChartInterval);
    rc.setLeft(rc.left() + m_margins.leftMargin);
    rc.setRight(rc.left() + YaxisKlineMinWidth);
    rc.setTop(rc.top() +  m_margins.topMargin);
    rc.setBottom(rc.bottom() - m_margins.bottomMargin);
    return rc ;
}

void BaseChart::getMaxMinRange(double &max, double &min)
{
    max = m_chartMax;
    min = m_chartmin;
}

void BaseChart::getMaxMinRangeIndex(int &maxPos, int &minPos)
{
    maxPos = m_maxIndex;
    minPos = m_minIndex;
}

int BaseChart::getCacheSize()
{
    return 0;
}

void BaseChart::redraw()
{

}

void BaseChart::getFormatTimeStr(QString &strFormat)
{
    strFormat = "yyyy/MM/dd hh:mm";
}

bool BaseChart::mouseMoveEx(QMouseEvent *event)
{
    return true;
}

bool BaseChart::mousePressEx(QMouseEvent *event)
{
    return true;
}

bool BaseChart::mouseReleaseEx(QMouseEvent *event)
{
    return true;
}

bool BaseChart::keyPressEx(QKeyEvent *event)
{
    return true;
}

bool BaseChart::keyReleaseEx(QKeyEvent *event)
{
    return true;
}

bool BaseChart::enterEventEx(QEvent *event)
{
    return true;
}

bool BaseChart::leaveEventEx(QEvent *event)
{
    return true;
}

void BaseChart::setTitleChart(std::shared_ptr<ChartTitle> pTitle)
{
    m_title = pTitle;
}

std::shared_ptr<ChartTitle> BaseChart::getTitleChart()
{
    return m_title;
}

QString BaseChart::leftTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX)
    {
        retString = QString::number(value, 'f', 2);
    }
    return retString;
}

QString BaseChart::rightTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX)
    {
        retString = QString::number(value, 'f', 2);
    }
    return retString;
}

bool BaseChart::adjustCrossLineYAxis(std::shared_ptr<BaseChart> crossChart, const QPoint &pos)
{
    //当前不是十字光标区域，且鼠标点是处于当前区域中（因为循环调用的鼠标移动事件，如果不做这个判断，每个区域都会重复给其设置X,Y坐标）
    bool bRet = false;
    if(crossChart != nullptr && crossChart.get() != this)
    {
        if(getRc().contains(pos))
        {
             crossChart->setYAxis(this->getYAxis());
             bRet = true;
        }
    }
    return bRet;
}

void BaseChart::onShow(bool bshow)
{
    if(m_title)
        m_title->showEx(bshow);
}

void BaseChart::onResize()
{
    if(m_title)
    {
        QRect rc = m_rc;
        rc.setBottom(rc.top()+ContainerTitleHeight);
        m_title->setRc(rc,m_wndDrawRc);
        m_title->resize();
    }
}

void BaseChart::afterActive()
{

}

void BaseChart::afterDeactive()
{

}

bool BaseChart::getStatusFromIndex(int index, int &status)
{
    return false;
}

void BaseChart::paintImmet()
{
    BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
    pWnd->paintImmet();
}

void BaseChart::repainterOnTime()
{
    BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
    pWnd->repainterOnTime();
}

void BaseChart::setType(ChartType type)
{
    m_chartType = type;
}

ChartType BaseChart::getType()
{
    return m_chartType;
}

int BaseChart::getId()
{
    return  m_id;
}

void BaseChart::soltButtonClose()
{
//    if(m_title)
//    {
//        BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
//        if(pWnd)
//        {
//            pWnd->hideChart(true,m_type);
//        }
//    }
}

void BaseChart::soltButtonParamSet()
{

}

void BaseChart::soltButtonPlus()
{

}

void BaseChart::soltButtonSub()
{

}

void BaseChart::soltRestorationComBoxChanged(int index)
{

}

void BaseChart::soltIndiSeltComBoxChanged(int index)
{

}

void BaseChart::mainChartXaixsChnage(const QString& msg)
{

}

void BaseChart::dataResponse(int id,int type)
{

}

void BaseChart::errorResponse(int id, int ncode, const QString &errReuslt)
{

}

void BaseChart::OnXaxisChange(QString msg)
{
    mainChartXaixsChnage(msg);
}
