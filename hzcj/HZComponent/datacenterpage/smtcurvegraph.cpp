#include "smtcurvegraph.h"
#include <QMoveEvent>
#include <QKeyEvent>
#include "smttimechart.h"
#include "curvecrosslinechart.h"
#include "smttitlechart.h"
SmtCurveGraphCanvas::SmtCurveGraphCanvas(QWidget *parent):CurveCanvas(parent)
{
    setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

void SmtCurveGraphCanvas::setDatas(const StockRzrqDetailList &datas)
{
    StockRzrqDetailList tempDatas = datas;
    if(!tempDatas.dataList.isEmpty())
    {
        std::sort(tempDatas.dataList.begin(),tempDatas.dataList.end(),[&](const StockRzrqDetailItem& a,const StockRzrqDetailItem& b){
            return a.date < b.date;
        });
    }
    m_rzyeData=CurveDataList();
    m_rqyeData=CurveDataList();
    if(!tempDatas.dataList.isEmpty())
    {
        CurveDataItem item;
        for(int i=0;i <tempDatas.dataList.size();i++)
        {
           item.time = tempDatas.dataList[i].date;
           item.price = tempDatas.dataList[i].rzye;
           if(m_rzyeData.max < item.price)
               m_rzyeData.max = item.price;
           if(m_rzyeData.min > item.price)
               m_rzyeData.min = item.price;
           m_rzyeData.data.push_back(item);

           item.average = tempDatas.dataList[i].rqye;
           if(m_rqyeData.max < item.average)
               m_rqyeData.max = item.average;
           if(m_rqyeData.min > item.average)
               m_rqyeData.min = item.average;
           m_rqyeData.data.push_back(item);
        }
    }
}

void SmtCurveGraphCanvas::setDatas(const StockMarketRzrqList &datas)
{
    StockMarketRzrqList  tempDatas= datas;
    if(!tempDatas.dataList.isEmpty())
    {
        std::sort(tempDatas.dataList.begin(),tempDatas.dataList.end(),[&](const StockMarketRzrqItem& a,const StockMarketRzrqItem& b){
            return a.date < b.date;
        });
    }
    m_rzyeData=CurveDataList();
    m_rqyeData=CurveDataList();
    if(!tempDatas.dataList.isEmpty())
    {
        CurveDataItem item;
        for(int i=0;i <tempDatas.dataList.size();i++)
        {
           item.time = tempDatas.dataList[i].date;
           item.price = tempDatas.dataList[i].rzye;
           if(m_rzyeData.max < item.price)
               m_rzyeData.max = item.price;
           if(m_rzyeData.min > item.price)
               m_rzyeData.min = item.price;
           m_rzyeData.data.push_back(item);

           item.average = tempDatas.dataList[i].rqye;
           if(m_rqyeData.max < item.average)
               m_rqyeData.max = item.average;
           if(m_rqyeData.min > item.average)
               m_rqyeData.min = item.average;
           m_rqyeData.data.push_back(item);
        }
    }
}

void SmtCurveGraphCanvas::afterActive()
{
    dataUpdate();
    this->setFocus();
}

void SmtCurveGraphCanvas::afterDeactive()
{
}

void SmtCurveGraphCanvas::createWg()
{
    QVector<CrossInfo> lineInfo;
    //添加标题区域
    m_titleStaticChart  = createTitleStaticChart();
    if(m_titleStaticChart)
    {
        m_veObserverList.push_back(m_titleStaticChart);
    }
    //添加网格
    m_gridChart = createGridChart();
    if(m_gridChart)
        m_veObserverList.push_back(m_gridChart);

    //融资曲线
    m_lineRzyeChart = createRzyeLineChart();
    if(m_lineRzyeChart)
    {
        m_veObserverList.push_back(m_lineRzyeChart);
        m_lineRzyeChart->setGridChart(m_gridChart);
        CrossInfo item;
        item.m_xaxis = m_lineRzyeChart->getXaxis();
        item.m_yaxis = m_lineRzyeChart->getYaxis();
        item.axisDrawType =  AXIS_LEFT_DRAW_TYPE;
        item.data = &m_rzyeData;
        lineInfo.push_back(item);
    }

    //融券曲线
    m_lineRqyeChart = createRqyeLineChart();
    if(m_lineRqyeChart)
    {
        m_veObserverList.push_back(m_lineRqyeChart);
        m_lineRqyeChart->setGridChart(m_gridChart);
        CrossInfo item;
        item.m_xaxis = m_lineRqyeChart->getXaxis();
        item.m_yaxis = m_lineRqyeChart->getYaxis();
        item.axisDrawType = AXIS_RIGHT_DRAW_TYPE;
        item.data = &m_rqyeData;
        item.isAverage = true;
        lineInfo.push_back(item);
    }

    //时间轴
    m_timeChart  =  createTimelistChart();
    if(m_timeChart)
    {
        m_veObserverList.push_back(m_timeChart);
    }

    m_crossLine = createCrossLineChart();
    if(m_crossLine)
    {
        m_crossLine->setCrossInfo(lineInfo);
    }


     m_xaxis = m_lineRzyeChart->getXaxis();
     m_yaxis = m_lineRzyeChart->getYaxis();
}

void SmtCurveGraphCanvas::setGridNum(int row, int col)
{
    m_row = row;
    m_col = col;
}

CurveGridChart *SmtCurveGraphCanvas::createGridChart()
{
    CurveChartMargin margin(0,0,8,8);
    m_gridChart = new CurveGridChart(m_imageLayer,m_rzyeData,margin);
    m_gridChart->setGridNum(m_row,m_col);
    return m_gridChart;
}

CurveLineChart *SmtCurveGraphCanvas::createRzyeLineChart()
{
    CurveChartMargin margin(0,0,8,8);
    m_lineRzyeChart = new CurveLineChart(m_imageLayer,m_rzyeData,margin);
    m_lineRzyeChart->setAxisWidht(m_leftAxisWidth,0);
    return m_lineRzyeChart;
}

CurveLineChart *SmtCurveGraphCanvas::createRqyeLineChart()
{
    CurveChartMargin margin(0,0,8,8);
    m_lineRqyeChart = new CurveLineChart(m_imageLayer,m_rqyeData,margin);
    m_lineRqyeChart->setAxisWidht(0,m_rightAxisWidth);
    return m_lineRqyeChart;
}

SmtTimeChart *SmtCurveGraphCanvas::createTimelistChart()
{
    m_timeChart = new SmtTimeChart(m_imageLayer,m_rqyeData);
    return m_timeChart;
}

CurveCrossLineChart *SmtCurveGraphCanvas::createCrossLineChart()
{
    CurveChartMargin margin(0,0,8,0);
    m_crossLine =  new CurveCrossLineChart(m_imageLayer,m_rzyeData,margin);
    return m_crossLine;
}

SmtTitleStaticChart *SmtCurveGraphCanvas::createTitleStaticChart()
{
    m_titleStaticChart = new SmtTitleStaticChart(m_imageLayer,m_rzyeData);
    return m_titleStaticChart;
}

void SmtCurveGraphCanvas::resizeEvent(QResizeEvent *event)
{
    int timeChartHight = 0;
    if(m_timeChart)
    {
        timeChartHight = 24;
    }
    QRectF rc = getRc();
    QRectF lineRc = rc;
    int titleHight = 0;
    if(m_titleStaticChart)
    {
        titleHight = 30;
        lineRc.setLeft(rc.left()+m_leftAxisWidth);
        lineRc.setRight(rc.right()-m_rightAxisWidth);
        lineRc.setBottom(rc.top()+titleHight);
        m_titleStaticChart->resize(lineRc,rc);

    }
    if(m_gridChart)
    {
        QRectF gridRc = rc;
        gridRc.setTop(rc.top()+titleHight);
        gridRc.setLeft(rc.left()+m_leftAxisWidth);
        gridRc.setRight(rc.right()-m_rightAxisWidth);
        gridRc.setBottom(rc.bottom()-timeChartHight);
        m_gridChart->resize(gridRc,rc);
    }

    lineRc = rc;
    if(m_lineRzyeChart)
    {
        lineRc.setTop(rc.top()+titleHight);
        lineRc.setLeft(rc.left()+m_leftAxisWidth);
        lineRc.setRight(rc.right()-m_rightAxisWidth);
        lineRc.setBottom(rc.bottom()-timeChartHight);
        m_lineRzyeChart->resize(lineRc,rc);
    }

    lineRc = rc;
    if(m_lineRqyeChart)
    {
        lineRc.setTop(rc.top()+titleHight);
        lineRc.setLeft(rc.left()+m_leftAxisWidth);
        lineRc.setRight(rc.right()-m_rightAxisWidth);
        lineRc.setBottom(rc.bottom()-timeChartHight);
        m_lineRqyeChart->resize(lineRc,rc);
    }

    lineRc = rc;
    if(m_timeChart)
    {
        lineRc.setLeft(rc.left()+m_leftAxisWidth);
        lineRc.setRight(rc.right()-m_rightAxisWidth);
        lineRc.setTop(rc.bottom()-timeChartHight);
        m_timeChart->resize(lineRc,rc);
    }
    lineRc = rc;
    if(m_crossLine)
    {
        lineRc.setTop(rc.top()+titleHight);
        lineRc.setLeft(rc.left()+m_leftAxisWidth);
        lineRc.setRight(rc.right()-m_rightAxisWidth);
        m_crossLine->resize(lineRc,rc);
    }
}

void SmtCurveGraphCanvas::keyPressEvent(QKeyEvent *e)
{
    int nIndex;
    float xPos, yPos;
    QRectF rc;
    if(m_timeChart)
        rc = m_timeChart->getRc();
    if(Qt::Key_Left == e->key() && !m_rzyeData.data.isEmpty())
    {
        if(m_mousePt.x() <= rc.left())
            return;
        xPos = m_mousePt.x() - 1;
        nIndex = m_xaxis->xCanvasPtxToIndex(xPos);
        yPos = m_yaxis->canvasValueToPt(m_rzyeData.data[nIndex].price);
        m_mousePt.setX(xPos);
        m_mousePt.setY(yPos);
        m_crossLine->move(m_mousePt, rc);
    }
    else if(Qt::Key_Right == e->key() && !m_rzyeData.data.isEmpty())
    {
        if(m_mousePt.x() >= rc.right())
            return;
        xPos = m_mousePt.x() + 1;
        nIndex = m_xaxis->xCanvasPtxToIndex(xPos);
        yPos = m_yaxis->canvasValueToPt(m_rzyeData.data[nIndex].price);
        m_mousePt.setX(xPos);
        m_mousePt.setY(yPos);
        m_crossLine->move(m_mousePt,rc);
    }
}

//void SmtCurveGraphCanvas::keyReleaseEvent(QKeyEvent *e)
//{

//}

void SmtCurveGraphCanvas::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePt = event->pos();
    QRectF rc;
    if(m_timeChart)
        rc = m_timeChart->getRc();
    if(m_crossLine)
    {
        m_crossLine->move(event->pos(),rc);
    }
}

