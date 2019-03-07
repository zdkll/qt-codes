#include "Minchart.h"
#include <qgridlayout.h>
#include <QPolygonF>
#include <QtAlgorithms>

#include "hzobject.h"
#include "minprovider.h"
#include "xaxis.h"
#include "yaxis.h"
#include "imagelayer.h"
#include "render.h"
#include "chartmodle.h"
#include "mincanvas.h"
#include "httpdata.h"
#include "minvolchart.h"
#include "charttitle.h"
#include "quoteformat.h"


Minchart::Minchart(ImageLayer* layer):BaseChart(layer)
{
    m_drawAvgPrice =false;
    m_historyTime = 0;
    m_gradientLayer = -1;
    m_klineReqId = -1;
    m_klineSubId = -1;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_dayIndex = -1;
    m_preClose  = DBL_MAX;
    m_volChart = nullptr;
}

Minchart::~Minchart()
{
    cancle(m_klineReqId);
    cancle(m_klineSubId);
    if (m_provider)
    {
        delete m_provider;
        m_provider = nullptr;
    }
    if(m_volChart)
        delete m_volChart;
    m_volChart = nullptr;
}

void Minchart::dataResponse(int id,int type)
{
    // qDebug() << "KlineChart::dataResponse: " << id;
     if(m_xAxis == nullptr)
         return;
     if(type == KlineReqType && (m_klineReqId == id || id == INT_MAX))
     {
         m_klineReqId = -1;
         m_chartMax = DBL_MIN;
         m_chartmin = DBL_MAX;
         m_vKlineData.clear();
         m_dayIndexList.clear();
         int day = m_dayIndex;
         if(m_historyTime != 0)
             day = m_historyTime;
         m_provider->getData(m_vKlineData,m_dayIndexList,m_chartMax ,m_chartmin,day,m_drawAvgPrice);
         if(!m_drawAvgPrice)
             m_vKlineData.m_averPrice.clear();
         m_xAxis->setoffset(0);
         double count = ChartModle::getInstance()->getMinDayCount(m_obj)*abs(m_dayIndex);
         m_xAxis->setSingleAreaWidth((double)getChartMainArea().width()/count);
         if(m_historyTime == 0)
         {
            m_klineSubId = subKline();
         }
         if(m_vKlineData.m_preClosePrice.isEmpty())
              m_preClose = DBL_MAX;
         else
             m_preClose =  m_vKlineData.m_preClosePrice[m_vKlineData.m_preClosePrice.size()-1];
         refreshXYaxis(false);
         redraw();
         //请求来了以后，发送最后一根K线到十字小窗
         MinCanvas* pCanvas = (MinCanvas*)m_imageLayer->getWnd();
         if(!m_vKlineData.m_data.isEmpty())
         {
             if(m_xAxis->getCurrCrossXIndex() == -1)
             {
                 HZData::HZKline kline = m_vKlineData.m_data[m_vKlineData.m_data.size()-1];
                 pCanvas->proceCrossWndInfoChange(kline,true,false);
             }
         }else
         {
             pCanvas->proceCrossWndInfoChange(HZData::HZKline(),true,false);
         }
     }else if(m_klineSubId == id)
     {
         m_chartMax = DBL_MIN;
         m_chartmin = DBL_MAX;
         m_vKlineData.clear();
         m_dayIndexList.clear();
         m_dayIndex = m_provider->getData(m_vKlineData,m_dayIndexList,m_chartMax ,m_chartmin,m_dayIndex,m_drawAvgPrice);
         if(!m_drawAvgPrice)
             m_vKlineData.m_averPrice.clear();
         if(m_vKlineData.m_preClosePrice.isEmpty())
              m_preClose = DBL_MAX;
         else
             m_preClose =  m_vKlineData.m_preClosePrice[m_vKlineData.m_preClosePrice.size()-1];
         m_xAxis->setoffset(0);
         refreshXYaxis(false);
         redraw();
     }

}

void Minchart::errorResponse(int id, int ncode, const QString &errReuslt)
{
    resetDefaultValue();
    clearDrawData();
    MinCanvas* pCanvas = (MinCanvas*)m_imageLayer->getWnd();
    pCanvas->proceCrossWndInfoChange(HZData::HZKline(),true,false);
}

void Minchart::getMaxMinRange(double &max, double &min)
{
    if(m_vKlineData.m_updateTime.isEmpty() ||  m_preClose == DBL_MAX)
    {
        max = DBL_MIN;
        min = DBL_MAX;
    }else
    {
        max = m_chartMax;
        min = m_chartmin;
        if(m_preClose != DBL_MAX)
        {
            max = m_chartMax;
            min = m_chartmin;
            double v1 = abs(m_preClose-m_chartmin);
            double v2 = abs(m_preClose-m_chartMax);
            v1 = v1>v2?v1:v2;
            double rate = v1/m_preClose;
            if(v1  < 0.07)
                v1 = 0.07 ;
            else if(rate < 0.01) //小于1%
            {
                v1 = m_preClose*0.01;
            }
            max = m_preClose + v1;
            min = m_preClose - v1;
        }
        //qDebug() << "Minchart::reCalcMaxMinValueByPreClose preClose: " << m_preClose;
       // qDebug() << "Minchart::reCalcMaxMinValueByPreClose max: " << max << "min: " << min;
    }
}

bool Minchart::adjustCrossLineYAxis(std::shared_ptr<BaseChart> crossChart, const QPoint &pos)
{
    bool bRet = false;
    if(crossChart != nullptr && crossChart.get() != this)
    {
        if(getRc().contains(pos))
        {
            if(!m_volChart->adjustCrossLineYAxis(crossChart,pos))
                crossChart->setYAxis(this->getYAxis());
             bRet = true;
        }
    }
    return bRet;
}

void Minchart::afterActive()
{

}

void Minchart::afterDeactive()
{
    cancle(m_klineReqId);
    cancle(m_klineSubId);
}

const QString &Minchart::getObj() const
{
    return m_obj;
}

const DataKLine &Minchart::getMinData() const
{
    return m_vKlineData;
}

const QVector<int> &Minchart::getDayIndexList() const
{
    return m_dayIndexList;
}

int Minchart::getDayCount()
{
    return abs(m_dayIndex);
}

void Minchart::switchStock(const QString &obj,int day)
{
    //更换周期或合约时，恢復下坐标位置
    resetDefaultValue();
    clearDrawData();
    cancle(m_klineSubId);
    if(obj.isEmpty())
    {
        return ;
    }

    changeAvgDrawFlag(obj);
    m_obj = obj;
    if(day > 0)
    {
        m_historyTime = day;
        m_dayIndex = -1;
    }else
    {
        m_dayIndex = day;
        m_historyTime =0;
    }
    m_klineReqId = reqKline();
}

QRect Minchart::getChartMainArea()
{
    QRect rc = BaseChart::getChartMainArea();
    rc.setBottom(getVolArea().top());
    return rc;
}

QRect Minchart::getRightAxisArea()
{
    QRect rc = BaseChart::getRightAxisArea();
    if(!rc.isEmpty())
    {
        rc.setBottom(getVolArea().top());
    }
    return rc;
}

QRect Minchart::getLeftAxisArea()
{
    QRect rc = BaseChart::getLeftAxisArea();
    if(!rc.isEmpty())
    {
        rc.setBottom(getVolArea().top());
    }
    return rc;
}

bool Minchart::isDrawAvgLine()
{
    return m_drawAvgPrice;
}

void Minchart::soltButtonPlus()
{
    downKey();
}

void Minchart::soltButtonSub()
{
     upKey();
}

QRect Minchart::getVolArea()
{
    QRect rc  = m_rc;
    int height = rc.height()/3;
    rc.setTop(rc.bottom()-height);
    return rc;
}

bool Minchart::getStatusFromIndex(int index, int &status)
{
    KlineStatus type;
    bool bRet = false;
    if(index < m_vKlineData.m_updateTime.size())
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

const QVector<HZData::HZKline> &Minchart::getKlineData()
{
    return m_vKlineData.m_data;
}

QString Minchart::rightTipInfoFormat(double value)
{
    HzQuoteFormat::QuoteItem item = HzQuoteFormat::QuoteFormat::formatChangeRate(value,m_preClose);
    return item.strValue;
}

DataProvider *Minchart::getProvider()
{
    return m_provider;
}

void Minchart::drawMin(QPainter &dc, QPainter &gradDc, const DataKLine &data, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y)
{
    UINT32	trading_day = 0;
    QVector<QVector<QPointF>> avgLines;
    QVector<QVector<QPointF>> lines;
    QVector<QLineF> daysGridRcVec;
    int day = 0;
    QVector<QPointF> pts;
    QVector<QPointF> avgLine;
    QPainterPath path;
    QRectF chartRc  = x->getChart()->getChartMainArea();
    double  minPty = DBL_MAX;
    double firstPtx = 0;
    QPointF cPt,avgPt;
    QRectF daysGridRc = chartRc;
    daysGridRc.setRight(chartRc.left());
    daysGridRc.setBottom(m_rc.bottom());
    QLineF gridLine;
    double xPos = 0;
    int dataNum = data.m_updateTime.size();
    for(int i = 0;i < dataNum;i++)
    {
        xPos = x->xCanvasIndexToPTx(i);
        cPt.setX(xPos);
        cPt.setY(y->yCanvasValueToPt(data.m_closePrice[i]));
        pts.push_back(cPt);

        if(m_drawAvgPrice && data.m_averPrice[i] >= y->getMin() && data.m_averPrice[i] <= y->getMax())
        {
            avgPt.setX(xPos);
            avgPt.setY(y->yCanvasValueToPt(data.m_averPrice[i]));
            avgLine.push_back(avgPt);
        }

        if(minPty > cPt.y())
            minPty = cPt.y();
        if(i == 0)
        {
            path.moveTo(cPt);
            firstPtx = cPt.x();
        }else
        {
           path.lineTo(cPt);
        }
       // qDebug() << "data.m_data[i].trading_day" << "i= " << i << "day:" << data.m_data[i].trading_day;
        //最后一更数据或则交易日切换
        bool crossTrade = 0 != trading_day && data.m_data[i].trading_day != 0 && trading_day != data.m_data[i].trading_day;
        if(i == dataNum-1 || crossTrade)
        {
            if(dataNum  == 1) //防止一根数据画不出来的问题
            {
                cPt.setX(xPos+0.5);
                avgPt.setX(xPos+0.5);
                pts.push_back(cPt);
                avgLine.push_back(avgPt);
            }
            lines.push_back(pts);
            avgLines.push_back(avgLine);
            pts.clear();
            avgLine.clear();

            if(m_dayIndex != -1 && crossTrade) //交易日切换
            {
                //qDebug() << "data.m_data[i].trading_day" <<  data.m_data[i].trading_day;
                gridLine.setP1(QPointF(cPt.x(),chartRc.top()));
                gridLine.setP2(QPointF(cPt.x(),m_rc.bottom()));
                daysGridRcVec.push_back(gridLine);
                day++;
            }
        }
        trading_day = data.m_data[i].trading_day;
    }
    MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
    QPen     pen;
    //多日分时网格
    if(!daysGridRcVec.isEmpty())
    {
        pen.setWidth(1);
        pen.setColor(pMinCanvas->getDaiesGridLineClr());
        dc.setPen(pen);
        dc.drawLines(daysGridRcVec);
    }
    //绘制分时线 均线 多日分时文本
    QFont font;
    font.setFamily(pMinCanvas->getDaiesFontFamily());
    font.setPixelSize(pMinCanvas->getDaiesFontSize());
    font.setStyleStrategy(QFont::NoAntialias);

    dc.setFont(font);
    QString str;

    if(abs(m_dayIndex) == day)
        day--;
    for(int i=0;i < lines.size();i++)
    {
        // 分时线
        pen.setColor(pMinCanvas->getMinLineClr());
        dc.setPen(pen);
        dc.drawPolyline((QPointF*)lines[i].data(),lines[i].size());
        // 多日分时文本
        if(day > 0/*lines.size() > 1 && i + 1 != lines.size()*/)
        {
            QRect rc;
            rc.setLeft(lines[i][0].x()+5);
            rc.setRight(rc.left()+35);
            rc.setTop(chartRc.top());
            rc.setBottom(chartRc.top()+20);
            str = QStringLiteral("前%d日");
            str.sprintf(str.toStdString().c_str(),day--);
            pen.setColor(pMinCanvas->getDaiesTextClr());
            dc.setPen(pen);
            dc.drawText(rc,Qt::AlignCenter,str);
        }
    }

    pen.setColor(pMinCanvas->getMinAvgLineClr());
    dc.setPen(pen);
    for(int i=0;i < avgLines.size();i++)
    {
        // 均线
        dc.drawPolyline(avgLines[i].data(),avgLines[i].size());
    }

    //透明渐变区域绘制
    if(minPty != DBL_MAX)
    {
       path.lineTo(cPt.x(),chartRc.bottom());
       path.lineTo(firstPtx,chartRc.bottom());
       chartRc.setTop(minPty);
    }
    QLinearGradient linearGradient(chartRc.bottomLeft(),chartRc.topLeft());
    linearGradient.setColorAt(0,QColor(54,130,230,0));
    linearGradient.setColorAt(1,QColor(54,130,230,60));
    linearGradient.setSpread(QGradient::PadSpread);
    gradDc.setRenderHints(QPainter::Antialiasing |
                     QPainter::SmoothPixmapTransform);
    gradDc.setPen(Qt::NoPen);
    gradDc.setRenderHint(QPainter::Antialiasing, true);
    gradDc.setBrush(linearGradient);
    gradDc.setClipPath(path);
    gradDc.drawRect(chartRc);

    //绘制最新价动态标签区域图
    if(x && dataNum > 0 && (m_attr & MinChartDrawRealLine))
    {
        double price = data.m_closePrice[dataNum-1];
        int yPos = y->yCanvasValueToPt(price);
        QPointF startPt,endPt;
        startPt.setX(chartRc.left());
        startPt.setY(yPos);
        endPt.setX(chartRc.right());
        endPt.setY(yPos);
        pen.setWidth(1);
        pen.setColor(pMinCanvas->getMinAvgLineClr());
        pen.setStyle(Qt::PenStyle::DashLine);
        dc.setPen(pen);
        dc.drawLine(startPt,endPt);

        int hight =16,lmargin=8;
        QRect yRightTipRect = getRightAxisArea();
        yRightTipRect.setTop(yPos - TimeXaxisHeight/2);
        yRightTipRect.setBottom(yPos + TimeXaxisHeight/2);

        QRect yLeftTipRect = getLeftAxisArea();
        yLeftTipRect.setTop(yPos - TimeXaxisHeight/2);
        yLeftTipRect.setBottom(yPos + TimeXaxisHeight/2);

        yPos = yPos - hight/2;
        pen.setStyle(Qt::PenStyle::SolidLine);
        dc.setPen(pen);
        QBrush brush(pMinCanvas->getBkClr());
        dc.setBrush(brush);
        //左侧框路径
        QPolygonF leftPts;
        leftPts.push_back(QPointF(yLeftTipRect.left(),yPos));
        leftPts.push_back(QPointF(yLeftTipRect.right()-lmargin,yPos));
        leftPts.push_back(QPointF(yLeftTipRect.right(),yPos+hight/2));
        leftPts.push_back(QPointF(yLeftTipRect.right()-lmargin,yPos+hight));
        leftPts.push_back(QPointF(yLeftTipRect.left(),yPos+hight));
        dc.drawPolygon(leftPts);


        //右侧框路径
        QPolygonF rightPts;
        rightPts.push_back(QPointF(yRightTipRect.right(),yPos));
        rightPts.push_back(QPointF(yRightTipRect.left()+lmargin,yPos));
        rightPts.push_back(QPointF(yRightTipRect.left(),yPos+hight/2));
        rightPts.push_back(QPointF(yRightTipRect.left()+lmargin,yPos+hight));
        rightPts.push_back(QPointF(yRightTipRect.right(),yPos+hight));
        dc.drawPolygon(rightPts);

        //标签内文本
        QString strRightPrice = Default_Item_String,strLeftPrice=Default_Item_String;
        strRightPrice = rightTipInfoFormat(price);
        strLeftPrice = leftTipInfoFormat(price);

        pen.setColor(pMinCanvas->getXaxisTextClr());
        dc.setPen(pen);
        dc.drawText(yRightTipRect,Qt::AlignCenter,strRightPrice);
        dc.drawText(yLeftTipRect,Qt::AlignCenter,strLeftPrice);

    }
}

double Minchart::getGridHeight(const QRectF &rc,double& row)
{
    row =  rc.height()/MinGridHight;
    return rc.height()/row;;
}

void Minchart::redraw()
{
    if(m_title)
    {
        m_titleItemList.clear();
        createTitleTextInfo(m_titleItemList,INT_MAX);
        m_title->updateTextItem(m_titleItemList);
    }

    m_imageLayer->clear(m_type);
    m_imageLayer->clear(m_gradientLayer);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    ImgLayerInfo gradLayer =  m_imageLayer->getLayout(m_gradientLayer);
    QPainter painter(drawLayer.pix);
    QPainter gradDc(gradLayer.pix);
    drawYaxis(painter);
    drawGrid(painter);
    drawMin(painter,gradDc,m_vKlineData,m_xAxis,m_yAxis);
    m_volChart->update();
    paintImmet();
}

void Minchart::drawGrid(QPainter &painter)
{
    if(m_gridPosVec.isEmpty())
        return;
    //绘制分时主图区域网格
    QRectF chartRc = getChartMainArea();
    double col = (abs(m_dayIndex) + 1)* ChartModle::getInstance()->getTradeNode(m_obj).size() * 2;
    double gridWidth = chartRc.width()/col;
    QVector<QLineF> hVec,vVec;
    QPointF pt1(chartRc.topLeft()),pt2(chartRc.topRight());
    for(int i = 0;i < m_gridPosVec.size();i++)
    {
        pt1.setY(m_gridPosVec[i]);
        pt2.setY(m_gridPosVec[i]);
        hVec.push_back(QLineF(pt1,pt2));
    }

    pt1 = QPointF(chartRc.left(),m_gridPosVec[0]);
    if(m_volChart)
    {
         pt2 = QPointF(chartRc.left(),m_volChart->getChartMainArea().bottom());
    }else
    {
        pt2 = QPointF(chartRc.left(),m_gridPosVec[m_gridPosVec.size()-1]);
    }
    vVec.push_back(QLineF(pt1,pt2));
    for(int i = 0;i < col;i++)
    {
        pt1.setX(pt1.x() + gridWidth);
        pt2.setX(pt2.x() + gridWidth);
        vVec.push_back(QLineF(pt1,pt2));
    }
    QPen     pen;
    MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
    pen.setColor(pMinCanvas->getGridClr()/*QColor("#12191d")*/);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLines(hVec);
    painter.drawLines(vVec);
    //绘制vol区域网格
    painter.drawRect(m_volChart->getChartMainArea());
}

void Minchart::drawYaxis(QPainter &painter)
{
    m_gridPosVec.clear();
    MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
    QRectF chartRc = getChartMainArea();
    double max,min;
    getMaxMinRange(max,min);
    if(max != DBL_MIN && min != DBL_MAX)
    {
        chartRc.setTop(m_yAxis->yCanvasValueToPt(max));
        chartRc.setBottom(m_yAxis->yCanvasValueToPt(min));
    }

    double gridHeight = 27;//getGridHeight(chartRc,row);
    int row = (chartRc.height()/gridHeight)/2;//2 的整数倍
    row *= 2;
    if(row < 6)//最小八行
        row=6;
    else if(row >14)
        row = 14;
    gridHeight = chartRc.height()/row;

    double midPos = chartRc.top() + chartRc.height()/2.0;
    if(m_preClose != DBL_MAX)
    {
        midPos = m_yAxis->yCanvasValueToPt(m_preClose);
    }
    QRectF leftRc = getLeftAxisArea(),
           rightRc = getRightAxisArea();

    QPen pen;
    QFont font;
    font.setPixelSize(pMinCanvas->getYaxisFontSize());
    font.setFamily(pMinCanvas->getYaxisFontFamily());
    font.setStyleStrategy(QFont::NoAntialias);

    painter.setFont(font);
    pen.setWidth(1);
    QColor clr;
    QString strRate;
    HzQuoteFormat::QuoteItem itemleft;
    HzQuoteFormat::QuoteItem itemright;
    for(int i = 0;i <= row/2;i++)
    {
        if(i == 0) //绘制中轴
        {
            MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
            //绘制左边轴
            pen.setColor(pMinCanvas->getEqualClr());
            painter.setPen(pen);
            itemleft.strValue = HzQuoteFormat::QuoteFormat::numericFormatToString(m_preClose,2);
            painter.drawText(leftRc,Qt::AlignCenter,itemleft.strValue);
            //绘制右边轴
            itemright.strValue = "0.00%";
            if(itemleft.strValue == DEFAULT_STR_PRICE)
                itemright.strValue = DEFAULT_STR_PRICE;
            painter.drawText(rightRc,Qt::AlignCenter,itemright.strValue);
            m_gridPosVec.push_back(midPos);
        }else
        {
            calcRc(midPos - (double)i*gridHeight,m_preClose,gridHeight,clr,itemleft,leftRc,itemright,rightRc);
            m_gridPosVec.push_back(midPos - (double)i*gridHeight);
            strRate  = itemright.strValue;
            //绘制左边上方向轴
            pen.setColor(clr);
            painter.setPen(pen);
            painter.drawText(leftRc,Qt::AlignCenter,itemleft.strValue);
            //绘制右边上方向轴
            painter.drawText(rightRc,Qt::AlignCenter,strRate);


            calcRc(midPos + (double)i*gridHeight,m_preClose,gridHeight,clr,itemleft,leftRc,itemright,rightRc);
            m_gridPosVec.push_back(midPos + (double)i*gridHeight);
            //绘制左边下方向轴
            pen.setColor(clr);
            painter.setPen(pen);
            painter.drawText(leftRc,Qt::AlignCenter,itemleft.strValue);
            //绘制右边下方向轴
            painter.drawText(rightRc,Qt::AlignCenter,strRate);
        }
    }
    qSort(m_gridPosVec.begin(),m_gridPosVec.end());
}

void Minchart::calcRc(double pos,double preClose,double gridHeight,QColor& clr,HzQuoteFormat::QuoteItem& itemleft,QRectF& leftRc,HzQuoteFormat::QuoteItem& itemright,QRectF& rightRc)
{
    MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
    double tempVal = DBL_MAX;
    leftRc.setTop(pos -gridHeight/2.0);
    leftRc.setBottom(pos + gridHeight/2.0);
    tempVal = m_yAxis->yCanvasPTyToValue(pos);
    itemleft = HzQuoteFormat::QuoteFormat::formatPrice(tempVal,preClose);
    switch (itemleft.dirc) {
    case HzQuoteFormat::quote_up:
        clr = pMinCanvas->getUpClr();
        break;
    case HzQuoteFormat::quote_down:
        clr = pMinCanvas->getDownClr();
        break;
    case HzQuoteFormat::quote_equal:
    case HzQuoteFormat::quote_unknown:
        clr = pMinCanvas->getEqualClr();
        break;
    default:
        break;
    };
    //绘制右边
    itemright = HzQuoteFormat::QuoteFormat::formatChangeRate(tempVal,preClose);
    rightRc = rightRc;
    rightRc.setTop(pos-gridHeight/2.0);
    rightRc.setBottom(pos + gridHeight/2.0);
}

bool Minchart::keyPressEx(QKeyEvent *event)
{
    if(event && m_xAxis)
    {
        if(Qt::Key_Up == event->key())
            upKey();
        if(Qt::Key_Down == event->key())
            downKey();

        if(m_title)
        {
            int index  = m_xAxis->getCurrCrossXIndex();
            {
                if(Qt::Key_Left == event->key())
                {
                    m_titleItemList.clear();
                    createTitleTextInfo(m_titleItemList,index-1);
                    m_title->updateTextItem(m_titleItemList);
                }
                if(Qt::Key_Right == event->key())
                {
                    m_titleItemList.clear();
                    createTitleTextInfo(m_titleItemList,index+1);
                    m_title->updateTextItem(m_titleItemList);
                }
            }
        }
    }
    return true;
}

bool Minchart::mouseMoveEx(QMouseEvent *event)
{
    if(m_xAxis && event && m_title)
    {
        QPoint pos = event->pos();
        int index = m_xAxis->getCurrCrossXIndex();
        if(index != -1)
        {
            QRect rc = getChartMainArea();
            if(pos.x() >= rc.left() && pos.x() < rc.right() ||
               pos.x() > rc.left() && pos.x() <= rc.right())
            {
                index = m_xAxis->xCanvasPTxToIndex(pos.x());
                if(index != -1)
                {
                    m_titleItemList.clear();
                    createTitleTextInfo(m_titleItemList,index);
                    m_title->updateTextItem(m_titleItemList);
                }
            }
        }
    }
    return true;
}


void Minchart::onCreate()
{
    //创建一个图层用于绘制渐变区域
    auto info = m_imageLayer->createLayout(m_wndDrawRc);
    m_gradientLayer = info.type;

    m_provider = new MinProvider(this);

    m_xAxis = std::shared_ptr<XAxis>(new XAxis(this));
    m_xAxis->setoffset(0);
    if(m_attr &ChartThumbnailChart)
        m_xAxis->setSingleAreaWidth(KlineThumDefaultWidth);
    else
        m_xAxis->setSingleAreaWidth(KlineDefaultWidth);
    m_yAxis = std::shared_ptr<YAxis>(new YAxis(this));
    this->setTitleChart(m_title);
    m_volChart = new MinVolChart(m_imageLayer,m_vKlineData);
    m_volChart->createProvider("");
    m_volChart->setMargins(ChartMargins(0,0,10,0));
    m_volChart->setXAxis(m_xAxis);
    m_volChart->setRc(getVolArea(),m_wndDrawRc);
    m_volChart->setAttr(m_attr);
    m_volChart->createEx();
}

void Minchart::onResize()
{
    BaseChart::onResize();
    if(m_gradientLayer != -1)
        m_imageLayer->resize(m_wndDrawRc,m_gradientLayer);
    if(m_volChart)
    {
        m_volChart->setRc(getVolArea(),m_wndDrawRc);
        m_volChart->resize();
    }
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_vKlineData.clear();
    m_dayIndexList.clear();
    m_provider->getData(m_vKlineData,m_dayIndexList,m_chartMax ,m_chartmin,m_dayIndex,m_drawAvgPrice);
    m_xAxis->setoffset(0);
    double count = ChartModle::getInstance()->getMinDayCount(m_obj)*abs(m_dayIndex);
    m_xAxis->setSingleAreaWidth((double)getChartMainArea().width()/count);
    //refreshXYaxis(false);

    if(m_yAxis)
    {
        m_yAxis->recalc();
    }
    if(m_xAxis)
    {
        m_xAxis->recalc(false);
    }
    redraw();
}

void Minchart::switchParam(const QString &msg)
{
    QString key;
    MessageParams params;
    if(HZObject::parseMsg(msg,key,params) && m_provider)
    {
        if(SwitchStockInfo == key)
        {
            QString market;
            QString obj;
            int day = -1;
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "market")
                {
                    market = it->second;
                }else if(it->first == "obj")
                {
                    obj = it->second;
                }else if(it->first == "day")
                {
                    day  = it->second.toInt();
                }
            }
            changeAvgDrawFlag(obj);
            if(/*market.isEmpty() || */obj.isEmpty())
            {
                return ;
            }

            //更换周期或合约时，恢復下坐标位置
            if(m_obj != obj)
            {
                resetDefaultValue();
                clearDrawData();
            }
            m_obj = obj;
            //目前测试阶段所以获取本地文件中的分时数据
//            m_obj = "600000";
            if(day > 0)
            {
                m_historyTime = day;
                m_dayIndex = -1;
            }else
            {
                m_dayIndex = day;
                m_historyTime =0;
            }
            m_klineReqId = reqKline();
        }
    }
}

void Minchart::resetDefaultValue()
{
    m_dayIndex = -1;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    m_preClose = DBL_MAX;
    m_vKlineData.clear();
    m_xAxis->resetDefaultVaue();
    m_yAxis->resetDefaultValue();
}

int Minchart::reqKline()
{
    cancle(m_klineSubId);
    MessageParams params;
    params["obj"] = m_obj;
    if(m_historyTime == 0)
        params["day"] = QString::number(m_dayIndex);
    else
        params["day"] = QString::number(m_historyTime);
    QString reqmsg = HZObject::createMsg(KlineReqInfo,params);
    return m_provider->query(reqmsg.toStdString());
}

int Minchart::subKline()
{
    cancle(m_klineSubId);
    MessageParams params;
    params["obj"] = m_obj;
    params["day"] = QString::number(m_dayIndex);
    QString reqmsg = HZObject::createMsg(KlineSubInfo,params);
    return m_provider->query(reqmsg.toStdString());
}

void Minchart::cancle(int &id)
{
    if(m_provider)
    {
        if(id != -1 && id != INT_MAX)
            m_provider->cancle(id);
    }
    id  =-1;
}

void Minchart::upKey()
{
    if(m_dayIndex == -1)
        return;
    m_dayIndex += 1;
    m_klineReqId = reqKline();
}

void Minchart::downKey()
{
    if(MinMaxDay == m_dayIndex)
        return;
    m_dayIndex -= 1;
    m_klineReqId = reqKline();
}

void Minchart::refreshXYaxis(bool bReCalc)
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
        if(m_vKlineData.m_data.empty())
        {
            MessageParams params;
            QString msg = HZObject::createMsg(ChartUiClear,params);
            m_xAxis->getObserverMgr().Notify(msg);
        }
    }
}

QString Minchart::getObjCmd()
{
    MessageParams params;
    params["obj"] = m_obj;
    params["period"] = QString::number(1);
    QString msg = HZObject::createMsg(SwitchStockInfo,params);
    return msg;
}

void Minchart::createTitleTextInfo(std::vector<TitleTextItem> &itemList, int index)
{
    MinCanvas* pMinCanvas = (MinCanvas*)m_imageLayer->getWnd();
    if(m_attr & MinTitleShowObjName)
    {
        TitleTextItem item;
        item.nInterval = 12;
        item.clr = pMinCanvas->getContainerTitleTextClr();
        item.str = QStringLiteral(Default_Item_String);
        item.bBold = true;
        if(!m_obj.isEmpty())
        {
            item.str = Httpdata::instance()->getCodeNameByCode(m_obj);
            if(item.str.isEmpty())
            {
                item.str = Default_Item_String;
            }
        }
        itemList.push_back(item);
    }

    if(m_attr & MinChartShowObjCode)
    {
        TitleTextItem item;
        item.nInterval = 12;
        item.clr = pMinCanvas->getContainerTitleTextClr();
        item.str = QStringLiteral(Default_Item_String);
        if(!m_obj.isEmpty())
        {
            item.str = HzQuoteFormat::QuoteFormat::formatObjCode(m_obj);
            if(item.str.isEmpty())
            {
                item.str = Default_Item_String;
            }
        }
        itemList.push_back(item);
    }

    if(m_attr & MinTitleShowCanvasName)
    {
        TitleTextItem item;
        item.nInterval = 12;
        item.clr = pMinCanvas->getContainerTitleTextClr();
        item.str = QStringLiteral("分时走势");
        itemList.push_back(item);
    }
    if(index < 0)
        index =0;
    else if(index > m_vKlineData.m_data.size() -1)
    {
        index = m_vKlineData.m_data.size() -1;
    }

    if(m_attr & MinTitleShowAvgData)
    {
        TitleTextItem avgItem;
        avgItem.clr = pMinCanvas->getMinAvgLineClr();
        avgItem.nInterval = 12;
        avgItem.str = QStringLiteral("均线 ");
        if(m_vKlineData.m_data.isEmpty() || index >= m_vKlineData.m_data.size() || index < 0)
            avgItem.str.append(Default_Item_String);
        else
        {
            avgItem.str += QString::number(m_vKlineData.m_data[index].avg_price,'f',2);
        }
        itemList.push_back(avgItem);
    }
}

void Minchart::clearDrawData()
{
    m_imageLayer->clear(m_type);
    m_imageLayer->clear(m_gradientLayer);
    MessageParams params;
    QString msg = HZObject::createMsg(ChartUiClear,params);
    m_xAxis->getObserverMgr().Notify(msg);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawYaxis(painter);
    drawGrid(painter);
    m_volChart->clearDrawData();
    paintImmet();
}

void Minchart::changeAvgDrawFlag(const QString &stock)
{
    m_drawAvgPrice = false;
    QMap<QString,HZData::StockTable> out;
    Httpdata::instance()->getListMapByFilter(out,HZData::MarketAll);
    auto it =   out.find(stock);
    if(it != out.end() ||
       stock == "sh000001" ||
       stock == "sz399001" ||
       stock == "sz399006")
    {
        m_drawAvgPrice = true;
    }
}
