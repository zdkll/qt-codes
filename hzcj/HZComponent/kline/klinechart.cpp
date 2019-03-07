#include "klinechart.h"
#include <qdebug.h>
#include <QThread>

#include "imagelayer.h"
#include "klinedataprovider.h"
#include "xaxis.h"
#include "yaxis.h"
#include "hzobject.h"
#include "render.h"
#include "klinecanvas.h"

KlineChart::KlineChart(ImageLayer *layer):BaseChart(layer)
{
    m_indiId = -1;
    m_pressPosX = INT_MAX;
    m_indiChart = nullptr;
    m_lastReqRet = true;
}

KlineChart::~KlineChart()
{
    if(m_indiChart)
    {
        delete m_indiChart;
        m_indiChart = nullptr;
    }
    if(m_srvIndiChart)
    {
        delete m_srvIndiChart;
        m_srvIndiChart  = nullptr;
    }
    if (m_provider)
	{
        delete m_provider;
        m_provider = nullptr;
	}
}

DataProvider *KlineChart::getProvider()
{
    return m_provider;
}

void KlineChart::dataResponse(int id,int type)
{
    m_lastReqRet = true;
    if(m_xAxis == nullptr || m_xAxis == nullptr)
        return;
    if(type == KlineReqType)
    {
        m_chartMax = DBL_MIN;
        m_chartmin = DBL_MAX;
        m_vKlineData.clear();
        int nOffset = m_provider->getData(m_klineReqParam.offset,m_klineReqParam.count,m_vKlineData,m_chartMax ,m_chartmin,m_maxIndex,m_minIndex);
        m_xAxis->setoffset(nOffset);
        if(m_klineReqParam.reqtype & UpkeyReq || m_klineReqParam.reqtype & DownkeyReq
           ||m_klineReqParam.reqtype & DrReq)
            refreshXYaxis(true);
        else
            refreshXYaxis(false);
        m_klinesubParam = m_klineReqParam;
        qDebug() << "cout" << m_klineReqParam.count <<"offset" << nOffset << "ve count" << m_vKlineData.m_data.size();
        subKline();
        //请求来了以后，发送最后一根K线到十字小窗
        KlineCanvas* pCanvas = (KlineCanvas*)m_imageLayer->getWnd();
        if(!m_vKlineData.m_data.isEmpty())
        {
            if(m_xAxis->getCurrCrossXIndex() == -1)
            {
                HZData::HZKline kline = m_vKlineData.m_data[m_vKlineData.m_data.size()-1];
                if(m_vKlineData.m_data.size() -2 >= 0)
                {
                    kline.pre_close_price = m_vKlineData.m_data[m_vKlineData.m_data.size()-2].close_price;
                }
                pCanvas->proceCrossWndInfoChange(kline,true,false);
            }
        }else
        {
            pCanvas->proceCrossWndInfoChange(HZData::HZKline(),true,false);
        }
    }else
    {
        if(m_xAxis && m_xAxis->getoffset() == 0)
        {
            m_provider->getFlexCountDataBySub(m_vKlineData,m_chartMax ,m_chartmin,m_maxIndex,m_minIndex);
            refreshXYaxis(false);
        }
    }
}

void KlineChart::errorResponse(int id, int ncode, const QString &errReuslt)
{
    qDebug() << "errorResponse" << "code: " << ncode << "errReuslt: " << errReuslt;
    m_lastReqRet = true;
    resetDefaultValue();
    clearDrawData();
    KlineCanvas* pCanvas = (KlineCanvas*)m_imageLayer->getWnd();
    pCanvas->proceCrossWndInfoChange(HZData::HZKline(),true,false);
}

void KlineChart::onCreate()
{
    m_provider = new KlineDataProvider(this);

    m_xAxis = std::shared_ptr<XAxis>(new XAxis(this));
    m_xAxis->setoffset(0);
    if(m_attr &ChartThumbnailChart)
        m_xAxis->setSingleAreaWidth(KlineThumDefaultWidth);
    else
        m_xAxis->setSingleAreaWidth(KlineDefaultWidth);
    m_yAxis = std::shared_ptr<YAxis>(new YAxis(this));

    m_indiChart = new IndicatorChart(m_imageLayer,false,true);
    m_indiChart->createProvider("");
    m_indiChart->setXAxis(m_xAxis,false);
    m_indiChart->setRc(m_rc,m_wndDrawRc);
    if(m_title)
    {
        m_indiChart->setTitleChart(m_title);
    }
    m_indiChart->getObserverMgr().Connect(std::bind(&KlineChart::OnIndiChange,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    m_indiChart->createEx();
}

void KlineChart::getMaxMinRange(double &max, double &min)
{
    m_indiChart->getMaxMinRangeByMainArea(max,min);
    if(max <   m_chartMax)
    {
       max = m_chartMax;
    }
    if(min > m_chartmin)
    {
        min =  m_chartmin;
    }
    qDebug() << "m_indiChart->getMaxMinRangeByMainArea(max,min);: " << max << "  " << min;
    if(m_srvIndiChart)
    {
        double srvMax;double srvMin;
        m_srvIndiChart->getMaxMinRangeByMainArea(srvMax,srvMin);
        qDebug() << "m_srvIndiChart->getMaxMinRangeByMainArea(srvMax,srvMin);: " << srvMax << "  " << srvMin;
        if(max < srvMax)
        {
           max = srvMax;
        }
        if(min > srvMin)
        {
            min = srvMin;
        }
    }
}

int KlineChart::getCacheSize()
{
    return m_provider->getCacheSize();
}

const QVector<HZData::HZKline> &KlineChart::getKlineData()
{
    return m_vKlineData.m_data;
}

int KlineChart::getPeriod()
{
    return m_period;
}

void KlineChart::afterDeactive()
{
    cancleEx();
}

void KlineChart::afterActive()
{
    if(!m_obj.isEmpty())
        switchObj(m_obj,m_period);
    else
    {
        resetDefaultValue();
        clearDrawData();
    }
}

bool KlineChart::getStatusFromIndex(int index, int &status)
{
    KlineStatus type;
    bool bRet = false;
    if(index >= 0 && index < m_vKlineData.m_updateTime.size())
    {
        double preClose = m_vKlineData.m_preClosePrice[m_vKlineData.m_preClosePrice.size()-1];
        double close = m_vKlineData.m_closePrice[index];
        if(index != 0)
            preClose = m_vKlineData.m_closePrice[index-1];
        int nResult = compareFloat(preClose,close);
        if(nResult < 0)
        {
            type = KlineStatusUp;
        }else if(nResult > 0)
        {
            type = KlineStatusDown;
        }else
        {
            type = KlineStatusEqual;
        }
        status = type;
        bRet = true;
    }
    return bRet;
}

void KlineChart::switchObj(const QString &obj, int period)
{
    //更换周期或合约时，恢復下坐标位置
    resetDefaultValue();
    clearDrawData();
    cancleEx();
    if(obj.isEmpty() || period == -1)
    {
        return ;
    }
    m_obj = obj;
    m_period = period;
    IndiChartInfo info;
    info.indiId = m_indiId;
    info.obj = obj;
    info.period = period;

    m_indiChart->setIndiInfo(info);
    //请求K线
    m_klineReqParam.obj = m_obj = obj;
    m_klineReqParam.period = m_period = period;
    m_klineReqParam.count = m_xAxis->calcDataSize();
    qDebug() << "rec widht" <<  this->getChartMainArea().width();
    qDebug() << "m_klineReqParam.count" << m_xAxis->calcDataSize() << "widht" << m_xAxis->getSingleAreaWidth();
    reqKline(m_klineReqParam);
}

void KlineChart::switchIndi(int indi)
{
    IndiChartInfo info;
    info.indiId = indi;
    info.obj = m_obj;
    info.period = m_period;
    m_indiChart->switchIndi(info);
}

bool KlineChart::isDrawCrossLintPt()
{
    return false;
}

void KlineChart::setBaseInfo(const QString &obj, int period, int mainIndi)
{
    m_obj = obj;
    m_period = period;
    m_indiId = mainIndi;
}

void KlineChart::soltButtonClose()
{
    if(m_indiChart)
    {
        m_indiChart->soltButtonClose();
    }
}

void KlineChart::soltButtonParamSet()
{
    if(m_indiChart)
    {
        m_indiChart->soltButtonParamSet();
    }
}

void KlineChart::soltButtonPlus()
{
    upKey();
}

void KlineChart::soltButtonSub()
{
    downKey();
}

void KlineChart::soltRestorationComBoxChanged(int index)
{
    m_drType = (RestorationType)index;
    m_klineReqParam.offset =m_xAxis->getoffset();
    m_klineReqParam.count = m_xAxis->calcDataSize();
    m_klineReqParam.dirc = 1;
    int in = m_vKlineData.m_upsertTime.size()-1;
    if(in < 0)
    {
        in = 0;
    }
    m_klineReqParam.startTime = m_vKlineData.m_upsertTime[in];
    m_klineReqParam.reqtype = DrReq;
    reqKline(m_klineReqParam);;
}

void KlineChart::drawCandle(QPainter &dc, const DataKLine &data, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y)
{
    KlineCanvas* pCanvas = (KlineCanvas*)m_imageLayer->getWnd();
    QColor upClr(pCanvas->getUpClr());
    QColor downClr(pCanvas->getDownClr());
    QColor equalClr(pCanvas->getEqualClr());

    DataKLine* pdata = const_cast<DataKLine*>(&data);
    double fWidth = x->getSingleAreaWidth()* CandleRate;
    int nklineRlt = compareFloat(fWidth,KlineMinWidth);
    bool lastKlineUp =  false;

    QVector<QRectF> upKlineRcs;
    QVector<QLineF> upLines;
    QVector<QRectF> downKlineRcs;
    QVector<QLineF> downLines;
    QVector<QLineF> eualLines;

    double xPos = 0;
    QPointF hPt;
    QPointF lPt;
    QPointF oPt;
    QPointF cPt;
    for(int i=0;i < pdata->m_updateTime.size();i++)
    {
        xPos = i;
        xPos = x->xCanvasIndexToPTx(i);
        hPt.setX(xPos);
        hPt.setY(y->yCanvasValueToPt(pdata->m_highPrice[i]));

        lPt.setX(xPos);
        lPt.setY(y->yCanvasValueToPt(pdata->m_lowPrice[i]));

        oPt.setX(xPos);
        oPt.setY(y->yCanvasValueToPt(pdata->m_openPrice[i]));
        cPt.setX(xPos);
        cPt.setY(y->yCanvasValueToPt(pdata->m_closePrice[i]));
        int nResult = compareFloat(pdata->m_openPrice[i],pdata->m_closePrice[i]);
        if(nResult < 0)
        {
            lastKlineUp =true;
            QPen     pen;
            pen.setColor(upClr);
            dc.setPen(pen);

            //高，低线
            if(nklineRlt>0)
            {
                upLines.push_back(QLineF(cPt,hPt));
                upLines.push_back(QLineF(oPt,lPt));

                //画空闲矩形区域
//                QRectF rc;
//                rc.setTopLeft(QPointF(oPt.x()-fWidth/2.0,cPt.y()));
//                rc.setWidth(fWidth);
//                rc.setHeight(oPt.y() - cPt.y());
//                upKlineRcs.push_back(rc);

                QRectF rc;
                rc.setLeft(oPt.x()-fWidth/2.0);
                rc.setRight(rc.left()+fWidth);
                rc.setTop(cPt.y());
                rc.setBottom(oPt.y());
                upKlineRcs.push_back(rc);

            }else
            {
                upLines.push_back(QLineF(lPt,hPt));
            }

        }else if(nResult > 0) //阴线
        {
            lastKlineUp =false;
            //高，低线
            if(nklineRlt > 0)
            {
                downLines.push_back(QLineF(lPt,hPt));

                QRectF rc;
                rc.setTopLeft(QPointF(oPt.x()-fWidth/2.0,oPt.y()));
                rc.setWidth(fWidth);
                rc.setHeight(cPt.y() - oPt.y());
                downKlineRcs.push_back(rc);
            }else
            {
                downLines.push_back(QLineF(lPt,hPt));
            }

        }else //平行线
        {
            QColor clr;
            if(compareFloat(pdata->m_highPrice[i],pdata->m_lowPrice[i]) == 0)
            {
                if(compareFloat(pdata->m_highPrice[i],pdata->m_lowPrice[i]) != 0)
                    eualLines.push_back(QLineF(lPt,hPt));
                int pos = oPt.x()-fWidth/2;
                eualLines.push_back(QLineF(QPointF(pos,oPt.y()),QPointF(pos+fWidth,oPt.y())));
            }else
            {
                if(lastKlineUp)
                {
                    if(compareFloat(pdata->m_highPrice[i],pdata->m_lowPrice[i]) != 0)
                        downLines.push_back(QLineF(lPt,hPt));
                    int pos = oPt.x()-fWidth/2;
                    downLines.push_back(QLineF(QPointF(pos,oPt.y()),QPointF(pos+fWidth,oPt.y())));
                }else
                {
                    if(compareFloat(pdata->m_highPrice[i],pdata->m_lowPrice[i]) != 0)
                        upLines.push_back(QLineF(lPt,hPt));
                    int pos = oPt.x()-fWidth/2;
                    upLines.push_back(QLineF(QPointF(pos,oPt.y()),QPointF(pos+fWidth,oPt.y())));
                }
            }
        }
    }

    if(upLines.isEmpty() == false)
    {
        QPen     pen;
        pen.setColor(upClr);
        dc.setPen(pen);
        dc.drawLines(upLines);
        if(upKlineRcs.isEmpty() == false)
        {
            dc.drawRects(upKlineRcs);
        }
    }

    if(downLines.isEmpty() == false)
    {
        QPen     pen;
        pen.setColor(downClr);
        dc.setPen(pen);
        dc.drawLines(downLines);
        if(downKlineRcs.isEmpty() == false)
        {
            dc.setBrush(QBrush(downClr));
            dc.drawRects(downKlineRcs);
        }
    }

    if(eualLines.isEmpty() == false)
    {
        QPen     pen;
        pen.setColor(equalClr);
        dc.setPen(pen);
        dc.drawLines(eualLines);
    }
}

double KlineChart::getGridHeight(const QRectF &rc, double &row)
{
    row =  rc.height()/KlineGridHight;
    return rc.height()/row;;
}

void KlineChart::drawGrid(QPainter &painter)
{
    KlineCanvas* pCanvas = (KlineCanvas*)m_imageLayer->getWnd();
    QRectF chartRc = getChartMainArea();
    double col = XaxisSeparate;
    double row = MinGridHSeparate;
    double gridWidth = chartRc.width()/col;
    double gridHeight = getGridHeight(chartRc,row);
    QVector<QLineF> hVec,vVec;
    QPointF pt1(chartRc.topLeft()),pt2(chartRc.topRight());
    for(int i = 0;i < row-1;i++)
    {
        pt1.setY(pt1.y() + gridHeight);
        pt2.setY(pt2.y() + gridHeight);
        hVec.push_back(QLineF(pt1,pt2));
    }

    pt1 = QPointF(chartRc.topLeft());
    pt2 = QPointF(chartRc.bottomLeft());
    for(int i = 0;i < col;i++)
    {
        pt1.setX(pt1.x() + gridWidth);
        pt2.setX(pt2.x() + gridWidth);
        vVec.push_back(QLineF(pt1,pt2));
    }
    QPen     pen;
    pen.setColor(pCanvas->getGridClr());
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLines(hVec);
    painter.drawLines(vVec);
    painter.drawRect(chartRc);
}

void KlineChart::drawYaxis(QPainter &painter)
{
    KlineCanvas* pCanvas = (KlineCanvas*)m_imageLayer->getWnd();
    QRectF chartRc = getChartMainArea();
    double row = MinGridHSeparate;
    double gridHeight = getGridHeight(chartRc,row);
    QPointF pt1(chartRc.topLeft()),pt2(chartRc.topRight());
    QRect leftRc = getLeftAxisArea(),
          rightRc = getRightAxisArea();

    QPen pen;
    QFont ft;
    ft.setFamily(pCanvas->getYaxisFontFamily());
    ft.setPixelSize(pCanvas->getYaxisFontSize());
    ft.setStyleStrategy(QFont::NoAntialias);

    painter.setFont(ft);
    pen.setWidth(1);
    double tempVal = DBL_MAX;
    QString str;
    for(int i = 0;i < row-1;i++)
    {
        pt1.setY(pt1.y() + gridHeight);
        pt2.setY(pt2.y() + gridHeight);
        tempVal = m_yAxis->yCanvasPTyToValue(pt1.y());
        if(tempVal != DBL_MAX)
        {
            str = QString::number(tempVal, 'f', 3);
        }else
        {
            str = Default_Item_String;
        }

        //绘制左边坐标轴
        QRectF tempRc = leftRc;
        tempRc.setTop(pt1.y()-gridHeight/2.0);
        tempRc.setBottom(pt1.y() + gridHeight/2.0);
        pen.setColor(pCanvas->getEqualClr());
        painter.setPen(pen);
        painter.drawText(tempRc,Qt::AlignCenter,str);

        //绘制右边坐标轴
        tempRc = rightRc;
        tempRc.setTop(pt2.y()-gridHeight/2.0);
        tempRc.setBottom(pt2.y() + gridHeight/2.0);
        painter.drawText(tempRc,Qt::AlignCenter,str);
    }
}

void KlineChart::drawMinMaxTip(QPainter& painter)
{
    KlineCanvas* pWnd = (KlineCanvas*)m_imageLayer->getWnd();
    if(m_chartMax == DBL_MAX || m_chartmin == DBL_MIN || m_chartMax  ==  m_chartmin ||
       m_vKlineData.m_data.size() < 2 || m_xAxis == nullptr || m_yAxis == nullptr || m_maxIndex == -1 || m_minIndex == -1)
        return;
    QPointF maxPt,minPt;
    maxPt.setX(m_xAxis->xCanvasIndexToPTx(m_maxIndex));
    maxPt.setY(m_yAxis->yCanvasValueToPt(m_chartMax));

    minPt.setX(m_xAxis->xCanvasIndexToPTx(m_minIndex));
    minPt.setY(m_yAxis->yCanvasValueToPt(m_chartmin));

    QFont font;
    font.setFamily(pWnd->getMaxMinPriceFontFamily());
    font.setPixelSize(pWnd->getMaxMinPriceFontSize());
    font.setStyleStrategy(QFont::NoAntialias);
    painter.setFont(font);
    QFontMetrics fm(font);
    QRectF rc = getChartMainArea();

    QString hstr = QString::number(m_chartMax,'f',3);
    QRectF fmRc = fm.boundingRect(hstr);
    QRectF htextRc = fmRc;
    htextRc.setLeft(maxPt.x());
    htextRc.setTop(maxPt.y());
    htextRc.setWidth(fmRc.width()+3);
    htextRc.setHeight(fmRc.height());
    int width = 20;
    QPointF hpt;
    if(htextRc.right() + width > rc.right())
    {
       width += fmRc.width();
       htextRc.setLeft(htextRc.left()-width);
       htextRc.setRight(htextRc.right()-width);
       hpt = htextRc.topRight();
       hpt.setY(htextRc.top()+htextRc.height()/2);
    }else
    {
        htextRc.setLeft(htextRc.left()+width);
        htextRc.setRight(htextRc.right() + width);
        hpt = htextRc.topLeft();
        hpt.setY(htextRc.top()+htextRc.height()/2);
    }


    QString str = QString::number(m_chartmin,'f',3);
    fmRc = fm.boundingRect(str);
    QRectF textRc = fmRc;
    textRc.setLeft(minPt.x());
    textRc.setTop(minPt.y()-13);
    textRc.setWidth(fmRc.width()+5);
    textRc.setHeight(fmRc.height());
    width = 20;
    QPointF pt;
    if(textRc.left() - width - fmRc.width() < rc.left())
    {
       textRc.setLeft(textRc.left()+width);
       textRc.setRight(textRc.right()+width);
       pt = textRc.topLeft();
       pt.setY(textRc.top()+textRc.height()/2);
    }else
    {
        width += fmRc.width();
        textRc.setLeft(textRc.left() - width);
        textRc.setRight(textRc.right() - width);
        pt = textRc.topRight();
        pt.setY(textRc.top()+textRc.height()/2);

    }

    QPen pen;
    pen.setColor(pWnd->getMaxMinPriceTextClr());
    painter.setPen(pen);
    painter.drawText(textRc,Qt::AlignCenter,str);
    painter.drawText(htextRc,Qt::AlignCenter,hstr);

    pen.setColor(pWnd->getMaxMinPriceLineClr());
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(minPt,pt);
    painter.drawLine(maxPt,hpt);

}

void KlineChart::OnIndiChange(bool bRefresh,int id,BaseChart* pChart)
{
    m_yAxis->recalc();
    m_indiChart->getYAxis()->recalc();
    if(m_srvIndiChart)
        m_srvIndiChart->getYAxis()->recalc();
    if(m_indiChart == pChart && m_srvIndiChart != nullptr)
    {
        QString msg = getObjCmd();
        m_srvIndiChart->mainChartXaixsChnage(msg); //请求计算特色指标
    }else
    {
        redraw();
        m_indiChart->redraw();
        if(m_srvIndiChart)
            m_srvIndiChart->redraw();
        m_imageLayer->getWnd()->update();
    }
}

void KlineChart::clearDrawData()
{
    m_imageLayer->clear(m_type);
    MessageParams params;
    QString msg = HZObject::createMsg(ChartUiClear,params);
    m_xAxis->getObserverMgr().Notify(msg);
    m_indiChart->mainChartXaixsChnage(msg);
    if(m_srvIndiChart)
        m_srvIndiChart->mainChartXaixsChnage(msg);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawGrid(painter);
    drawYaxis(painter);
    paintImmet();
}

bool KlineChart::mouseMoveEx(QMouseEvent *event)
{
    if(event)
    {
        if(m_indiChart)
        {
            m_indiChart->mouseMoveEx(event);
        }
        if(m_srvIndiChart)
        {
            m_srvIndiChart->mouseMoveEx(event);
        }
        //水平拖动
        if(m_pressPosX != INT_MAX && m_xAxis && m_provider)
        {
            int distance =event->pos().x() - m_pressPosX;
            qDebug() << "m_pressPosX: " << m_pressPosX;
            qDebug() << "kline distance: " << distance;
            if(distance < 0 && m_xAxis->getoffset() <= 0 && abs(distance) >= m_xAxis->getSingleAreaWidth()) //向左
            {
                distance *= -1;
                int offset = distance / m_xAxis->getSingleAreaWidth();
                if(offset <= 0)
                    offset = 1;
                qDebug() << "left offest: " << offset;
                dragLeft(offset);
                m_pressPosX = event->pos().x();
            }else if(distance > 0 && distance >= m_xAxis->getSingleAreaWidth())    //向右拉
            {
                int offset = distance / m_xAxis->getSingleAreaWidth();
                if(offset <= 0)
                    offset = 1;
                qDebug() << "m_pressPosX: " << m_pressPosX;
                qDebug() << "right offest: " << offset;
                dragRight(offset);
                m_pressPosX = event->pos().x();
            }
        }
    }
    return true;
}

bool KlineChart::mousePressEx(QMouseEvent *event)
{
    bool bRet  =true;
    if(event && event->button() == Qt::LeftButton)
    {
        m_pressPosX  = INT_MAX;
        if(m_indiChart)
        {
            m_indiChart->mousePressEx(event);
        }
        if(m_srvIndiChart)
        {
            m_srvIndiChart->mousePressEx(event);
        }
        QPoint pos = event->pos();
        QRect rc = getChartMainArea();
        if(pos.x() >= rc.left() && pos.x() < rc.right() ||
           pos.x() > rc.left() && pos.x() <= rc.right())
        {
             m_pressPosX = pos.x();
             qDebug() << "klinechart press button x pos: " << m_pressPosX;
        }
    }
    return bRet;
}

bool KlineChart::mouseReleaseEx(QMouseEvent *event)
{
    if(m_indiChart)
    {
        m_indiChart->mouseReleaseEx(event);
    }
    if(m_srvIndiChart)
    {
        m_srvIndiChart->mouseReleaseEx(event);
    }
    m_pressPosX  = INT_MAX;
    return true;
}

bool KlineChart::keyPressEx(QKeyEvent *event)
{
    if(event)
    {
        if(m_indiChart)
        {
            m_indiChart->keyPressEx(event);
        }
        if(m_srvIndiChart)
        {
            m_srvIndiChart->keyPressEx(event);
        }
        if(Qt::Key_Up == event->key())
            upKey();
        if(Qt::Key_Down == event->key())
            downKey();
        if(Qt::Key_Left == event->key())
        {
            int index  = m_xAxis->getCurrCrossXIndex();
            if(index  == 0)
            {
                dragRight(1);
            }
        }
        if(Qt::Key_Right == event->key())
        {
            int index  = m_xAxis->getCurrCrossXIndex();
            if(index == m_vKlineData.m_updateTime.size() -1)
            {
                dragLeft(1);
            }
        }
    }
    return true;
}

bool KlineChart::keyReleaseEx(QKeyEvent *event)
{
    if(m_indiChart)
    {
        m_indiChart->keyReleaseEx(event);
    }
    if(m_srvIndiChart)
    {
        m_srvIndiChart->keyReleaseEx(event);
    }
    return true;
}

void KlineChart::onResize()
{
    BaseChart::onResize();
    //更换周期或合约时，恢復下坐标位置
    if(m_indiChart)
    {
        m_indiChart->setRc(m_rc,m_wndDrawRc);
        m_indiChart->resize();
    }
    if(m_srvIndiChart)
    {
        QRect rc = m_rc;
        rc.setTop(m_rc.top()+ContainerTitleHeight+TitleAndChartInterval);
        m_srvIndiChart->setRc(rc,m_wndDrawRc);
        m_srvIndiChart->resize();
    }
    paintImmet();
    if(m_xAxis && m_vKlineData.m_upsertTime.isEmpty() == false)
    {
//        double beginTime = m_vKlineData.m_upsertTime[m_vKlineData.m_upsertTime.size()-1];
//        resetDefaultValue();
//        clearDrawData();
//        m_klineReqParam.offset = m_xAxis->getoffset();
//        m_klineReqParam.obj = m_obj;
//        m_klineReqParam.period = m_period;
//        m_klineReqParam.count = m_xAxis->calcDataSize();
//        m_klineReqParam.startTime = beginTime;
//        m_klineReqParam.dirc = 1;
//        reqKline(m_klineReqParam);

        DataKLine* pData = m_provider->getDataCache()->pData;
        if(pData && abs(m_xAxis->getoffset()) < pData->m_upsertTime.size())
        {
            double beginTime = pData->m_upsertTime[abs(m_xAxis->getoffset())];
            m_klineReqParam.count = m_xAxis->calcDataSize();
            m_klineReqParam.startTime = beginTime;
            m_klineReqParam.dirc = 1;
            reqKline(m_klineReqParam);
        }
    }
}

void KlineChart::switchParam(const QString &msg)
{
    QString key;
    MessageParams params;
    if(HZObject::parseMsg(msg,key,params) && m_provider)
    {
        if(SwitchStockInfo == key)
        {
            QString obj;
            int period =-1;
            int indiId = -1;
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "obj")
                {
                    obj = it->second;
                }else if(it->first == "period")
                {
                    period = it->second.toInt();
                }else if(it->first == "indid")
                {
                    indiId = it->second.toInt();
                }
            }

            //更换周期或合约时，恢復下坐标位置
            resetDefaultValue();
            clearDrawData();

            if(obj.isEmpty() || period == -1 || indiId == -1)
            {
                return ;
            }


            IndiChartInfo info;
            info.indiId = indiId;
            info.obj = obj;
            info.period = period;

            m_indiChart->setIndiInfo(info);
            //请求K线
            m_klineReqParam.obj = m_obj = obj;
            m_klineReqParam.period = m_period = period;
            m_klineReqParam.count = m_xAxis->calcDataSize();
            reqKline(m_klineReqParam);
        }else if(SwitchIndicatorInfo == key)
        {
            m_indiChart->switchParam(msg);
        }
    }
}

void KlineChart::redraw()
{
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawGrid(painter);
    drawYaxis(painter);
    drawMinMaxTip(painter);
    drawCandle(painter,m_vKlineData,m_xAxis,m_yAxis);
}

int KlineChart::reqKline(const DataReqInfo& req)
{
    int ret = -1;
    if(m_lastReqRet)
    {
        cancleEx();
        MessageParams params;
        params["obj"] = req.obj;
        params["period"] = QString::number(req.period);
        params["startTime"] = QString::number(req.startTime);
        params["count"] = QString::number(req.count);
        params["dirc"] = QString::number(req.dirc);
        params["reType"]=QString::number(m_drType);
        QString reqmsg = HZObject::createMsg(KlineReqInfo,params);
        ret =  m_provider->query(reqmsg.toStdString());
    }
    return ret;
}

int KlineChart::subKline()
{
    MessageParams params;
    params["obj"] = m_klinesubParam.obj;
    params["period"] = QString::number(m_klinesubParam.period);
    params["reType"]=QString::number(m_drType);
    QString reqmsg = HZObject::createMsg(KlineSubInfo,params);
    return m_provider->query(reqmsg.toStdString());
}

void KlineChart::cancleEx()
{
    if(m_provider)
    {
        m_provider->cancleEx();
    }
}

QString KlineChart::getObjCmd()
{
    MessageParams params;
    params["obj"] = m_obj;
    params["period"] = QString::number(m_period);
    QString msg = HZObject::createMsg(SwitchStockInfo,params);
    return msg;
}

void KlineChart::resetDefaultValue()
{
    m_lastReqRet = true;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_vKlineData.clear();
    m_klineReqParam = DataReqInfo();
    m_xAxis->resetDefaultVaue();
    m_yAxis->resetDefaultValue();
}

void KlineChart::upKey()
{
    double width = m_xAxis->getSingleAreaWidth();
    if(m_xAxis && m_provider && width < KlineMaxWidth)
    {
        int count = m_xAxis->calcDataSize();
        if(m_provider->getCacheSize() < count/2)
        {
            width = width *2;
            if(width > KlineMaxWidth)
                width = KlineMaxWidth;
            m_xAxis->setSingleAreaWidth(width);
            refreshXYaxis(false);
            //redraw();
        }else if(m_vKlineData.m_upsertTime.isEmpty() == false)
        {
            int nCount = m_xAxis->getCount();
            nCount = nCount /2;
            m_klineReqParam.count = nCount;
            m_klineReqParam.startTime = m_vKlineData.m_upsertTime[m_vKlineData.m_upsertTime.size()-1];;
            m_klineReqParam.dirc = 1;
            m_klineReqParam.reqtype = UpkeyReq;
            reqKline(m_klineReqParam);
        }
    }
}

void KlineChart::downKey()
{
    double width = m_xAxis->getSingleAreaWidth();
    if(m_xAxis && m_provider)
    {
        int count = m_xAxis->calcDataSize();
        if(m_provider->getCacheSize() < count)
        {
            if(width > KlineMinWidth)
            {
                width = width /2;
                if(width < KlineMinWidth)
                    width = KlineMinWidth;
                m_xAxis->setSingleAreaWidth(width);
                refreshXYaxis(false);
                //redraw();
            }
        }else if(m_vKlineData.m_upsertTime.isEmpty() == false)
        {
            int nCount = m_xAxis->getCount();
            nCount = nCount *2;
            m_klineReqParam.count = nCount;
            m_klineReqParam.startTime = m_vKlineData.m_upsertTime[m_vKlineData.m_upsertTime.size()-1];;
            m_klineReqParam.dirc = 1;
            m_klineReqParam.reqtype = DownkeyReq;
            reqKline(m_klineReqParam);
        }
    }

}

void KlineChart::dragLeft(int offset)
{   
    int screenKlineNum = m_xAxis->calcDataSize();
    if(offset > 0 && m_provider && m_provider->getCacheSize() >=  screenKlineNum
       && offset < m_vKlineData.m_upsertTime.size()) //发生了偏移
    {
        qDebug() << "KlineChart::dragLeft last offset: " << m_xAxis->getoffset();
        m_klineReqParam.startTime = m_vKlineData.m_upsertTime[offset];
        offset = offset + m_xAxis->getoffset();
        if(offset > 0)
            offset = 0;
        m_klineReqParam.offset = offset;
        m_klineReqParam.count = screenKlineNum;
        m_klineReqParam.dirc = 0;
        m_klineReqParam.reqtype = LeftReq;
        reqKline(m_klineReqParam);
        qDebug() << "KlineChart::dragLeft m_klineReqParam.offset: " << m_klineReqParam.offset;

    }
}

void KlineChart::dragRight(int offset)
{
    int screenKlineNum = m_xAxis->calcDataSize();
    if(offset > 0 && m_provider && m_provider->getCacheSize() >=  screenKlineNum
       && offset < m_vKlineData.m_upsertTime.size()) //发生了偏移
    {
        qDebug() << "KlineChart::dragRight last offset: " << m_xAxis->getoffset();
        int index = m_vKlineData.m_upsertTime.size() - offset -1;
        if(index < 0)
        {
            index = 0;
        }

        m_klineReqParam.offset =m_xAxis->getoffset() - offset;
        m_klineReqParam.count = screenKlineNum;
        m_klineReqParam.dirc = 1;
        m_klineReqParam.startTime = m_vKlineData.m_upsertTime[index];
        m_klineReqParam.reqtype = RightReq;
        reqKline(m_klineReqParam);
        qDebug() << "KlineChart::dragRight m_klineReqParam.offset: " << m_klineReqParam.offset;
    }
}

void KlineChart::refreshXYaxis(bool bReCalc)
{
    if(m_yAxis)
    {
        m_yAxis->recalc();
    }
    if(m_xAxis)
    {
        m_xAxis->recalc(bReCalc);
        QString msg = getObjCmd();
        m_xAxis->getObserverMgr().Notify(msg);
        m_indiChart->mainChartXaixsChnage(msg);
    }
}

void KlineChart::createSrvIndi()
{
    m_srvIndiChart = new IndicatorChart(m_imageLayer,false,true);
    m_srvIndiChart->createProvider("IndicatorSrvProvider");
    m_srvIndiChart->setXAxis(m_xAxis,false);
    QRect rc = m_rc;
    rc.setTop(m_rc.top()+ContainerTitleHeight+TitleAndChartInterval);
    m_srvIndiChart->setRc(rc,m_wndDrawRc);
    srvIndiNotifyId = m_srvIndiChart->getObserverMgr().Connect(std::bind(&KlineChart::OnIndiChange,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    m_srvIndiChart->createEx();
}

void KlineChart::removSrvIndi()
{
    if(m_srvIndiChart)
    {
        m_srvIndiChart->getObserverMgr().Disconnect(srvIndiNotifyId);
        delete m_srvIndiChart;
        m_srvIndiChart = nullptr;
    }
}

void KlineChart::switchSrvIndi(int id)
{
    removSrvIndi();
    createSrvIndi();
    if(m_drType == RestorationNone)
    {
        IndiChartInfo info;
        info.indiId = id;
        info.obj = m_obj;
        info.period = m_period;
        m_srvIndiChart->switchIndi(info);
    }
}

