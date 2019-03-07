#include "klinewidget.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include "chartdef.h"
#include "klinexaxis.h"
#include "klineyaxis.h"
#include "klineinfowidget.h"


KlineWidget::KlineWidget(QWidget *parent):
    BaseWidget(parent),
    m_xAxis(new KlineXaxis(this)),
    m_yAxis(new KlineYaxis(this)),
    m_infoWidget(new KlineInfoWidget)
{
    this->setMouseTracking(true);
    m_period = HZData::KLINE_PERIOD_DAY;
    m_stockCode = "sh600000";
//    m_stockCode = "sz300752";
//    m_stockCode = "sz300724";
    m_klineData = new DataKLine;

    m_cacheData = new DataKLine;

    m_noffset = 0;

    m_singleKlineWidth = KLINESINGLEWIDTH;

    m_xAxis->setSingleAreaWidth(m_singleKlineWidth);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &KlineWidget::onTimeOut);

    timer->start(2000);
//    QTimer::singleShot(1000, this, SLOT(onTimeOut()));
}

KlineWidget::~KlineWidget()
{
    if(m_klineData)
    {
        delete m_klineData;
        m_klineData = nullptr;
    }
    if(m_cacheData)
    {
        delete m_cacheData;
        m_cacheData = nullptr;
    }

    if(m_infoWidget)
    {
        delete m_infoWidget;
        m_infoWidget = nullptr;
    }

}

void KlineWidget::afterActive()
{
    startReq();
}

DataKLine *KlineWidget::getKlineData()
{
    return m_klineData;
}

void KlineWidget::getMaxMinRange(float &max, float &min)
{
      max = m_maxValue;
      min = m_minValue;
}

void KlineWidget::paintEvent(QPaintEvent *e)
{
    BaseWidget::paintEvent(e);
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("#0c1215"));
    drawGrid(&painter);
    drawYAxis(&painter);
    drawKline(&painter);
    drawCross(&painter);
    drawXAxis(&painter);
    drawYAxisPoint(&painter);
}

//void KlineWidget::showEvent(QShowEvent *event)
//{
//    //    startReq();
//}

void KlineWidget::resizeEvent(QResizeEvent *e)
{
    startReq();

    m_xAxis->resize(this->getDrawRect());
    m_yAxis->resize(this->getDrawRect());
}

void KlineWidget::mouseMoveEvent(QMouseEvent *e)
{
    BaseWidget::mouseMoveEvent(e);
    if(-1 != m_pressPosX)
    {
        int nDistance = e->pos().x() - m_pressPosX;

        int noffset = nDistance / m_singleKlineWidth;

        if(nDistance < 0 && abs(nDistance) >= m_singleKlineWidth)  //向左移动
        {
            dragLeft(-noffset);
        }
        else if(nDistance > 0 && abs(nDistance) >= m_singleKlineWidth)  //向右移动
        {
            dragRight(noffset);
        }

        m_pressPosX = e->pos().x();
    }
    m_mousePt = e->pos();

    QRectF rc = this->getDrawRect();
    QPoint pt;
    pt.setX(rc.topLeft().x());
    pt.setY(rc.topLeft().y());
    if(rc.contains(m_mousePt))
    {
        m_infoWidget->show();
        pt = mapToGlobal(pt);
        m_infoWidget->move(pt);
    }
//    else
//    {
//        m_infoWidget->hide();
//    }

    this->update();
}

void KlineWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_pressPosX = e->pos().x();
    }
}

void KlineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_pressPosX = -1;
}

void KlineWidget::keyPressEvent(QKeyEvent *e)
{
    if(Qt::Key_Up == e->key())
    {
        keyUp();
    }
    else if(Qt::Key_Down == e->key())
    {
        keyDown();
    }
    else if(Qt::Key_Left == e->key())
    {
        QRectF rc = this->getDrawRect();

        int nIndex = (m_mousePt.x() - rc.left()) / m_singleKlineWidth;

        if(nIndex <= 0)
        {
            nIndex = 1;
            dragRight(1);
        }

        m_mousePt.setX(rc.left() + nIndex*m_singleKlineWidth - m_singleKlineWidth / 2);

        int nPos  = m_xAxis->xPtxToIndex(m_mousePt.x());

        m_mousePt.setY(m_yAxis->yValueToPt(m_klineData->m_closePrice[nPos]));

        this->update();
    }
    else if(Qt::Key_Right == e->key())
    {
        QRectF rc = this->getDrawRect();

        int nIndex = (m_mousePt.x() - rc.left()) / m_singleKlineWidth + 1;

        if(nIndex >= m_reqInfo.nCount - 1)
        {
            nIndex = m_reqInfo.nCount- 1;
            dragLeft(1);
        }

        m_mousePt.setX(rc.left() + nIndex*m_singleKlineWidth + m_singleKlineWidth*1 / 2);

        int nPos = m_xAxis->xPtxToIndex(m_mousePt.x());

        m_mousePt.setY(m_yAxis->yValueToPt(m_klineData->m_closePrice[nPos]));

        this->update();
    }
}

void KlineWidget::drawGrid(QPainter *painter)
{
    QRectF  rc = this->getDrawRect();


//    rc.setLeft(rc.left() + YAXISWIDTH);
//    rc.setRight(rc.right() - YAXISWIDTH);
//    rc.setTop(rc.top() + XAXISHEIGHT);
//    rc.setBottom(rc.bottom() - XAXISHEIGHT);

    int nCol = XAXISSEPARATE;
    int nRow = rc.height() / YAXISPERHEIGHT;

    float width = rc.width() / nCol;

    float height = YAXISPERHEIGHT;

    QVector<QLineF> vVec, hVec;
    QPointF ptTop(rc.topLeft()), ptBottom(rc.bottomLeft());
    for(int i = 0; i < nCol; i++)
    {
        ptTop.setX(rc.x()+ i*width);
        ptBottom.setX(rc.x() + i*width);

        vVec.append(QLineF(ptTop, ptBottom));
    }

    QPointF ptLeft(rc.topLeft()), ptRight(rc.topRight());
    for(int i = 0; i < nRow; i++)
    {
        ptLeft.setY(rc.y() + i*height);
        ptRight.setY(rc.y() + i*height);
        hVec.append(QLineF(ptLeft, ptRight));
    }

    QPen pen;
    pen.setColor("#121917");
    painter->setPen(pen);
    painter->drawLines(vVec);
    painter->drawLines(hVec);
    painter->drawRect(rc);
}

void KlineWidget::drawYAxis(QPainter *painter)
{
    QRectF rc = this->rect();
    QRectF drawRc = this->getDrawRect();
    QRectF leftRc = rc, rightRc = rc;

    leftRc.setRight(leftRc.left() + YAXISWIDTH);
    rightRc.setLeft(rightRc.right() - YAXISWIDTH);

    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(11);

    QFontMetrics metrics(font);

    QPen pen;
    pen.setColor("#b1bbc3");
    painter->setPen(pen);

    float fontHeight = metrics.height() / 2.0;

    int nRow = getDrawRect().height() / YAXISPERHEIGHT;

    int yPos;
    QString strPrice;

    for(int i = 0; i < nRow; i++)
    {
        yPos = drawRc.y() + i*YAXISPERHEIGHT;
        leftRc.setY(drawRc.y() + i*YAXISPERHEIGHT - fontHeight);

        strPrice = QString::number(m_yAxis->yPtToValue(yPos), 'f', 2);

        painter->drawText(leftRc, Qt::AlignHCenter,  strPrice);

        rightRc.setY(drawRc.y() + i*YAXISPERHEIGHT - fontHeight);
        painter->drawText(rightRc, Qt::AlignHCenter,  strPrice);

//        pen.setStyle(Qt::DashDotLine);
//        pen.setColor("#0c1215");
//        painter->setPen(pen);
//        painter->fillRect(leftRc, QColor("#0c1215"));
//        painter->fillRect(rightRc, QColor("#0c1215"));
//        pen.setStyle(Qt::DashDotLine);
//        pen.setColor("#414141");
//        painter->drawRect(leftRc);
//        painter->drawRect(rightRc);
    }
}

void KlineWidget::drawKline(QPainter *painter)
{
  float fwidth = m_singleKlineWidth / 2;

  QRectF drawRc = this->getDrawRect();

  QVector<double>::iterator max = std::max_element(std::begin(m_klineData->m_highPrice),std::end(m_klineData->m_highPrice));
  QVector<double>::iterator min = std::min_element(std::begin(m_klineData->m_lowPrice), std::end(m_klineData->m_lowPrice));

  float fMax = *max, fMin = *min;

  float fValue = this->getDrawRect().height() / (fMax - fMin);
  QVector<QLineF> upLineVec, downLineVec, equalLineVec;
  QVector<QRectF> upRcVec, downRcVec;

  QPointF hPt, lPt, oPt, cPt;
  float xPos;
  for(int i = 0; i < m_klineData->m_updateTime.size(); i++)
  {
//      xPos =  drawRc.left() + (i+1)*m_singleKlineWidth - fwidth;

      xPos = m_xAxis->xIndexToPtx(i);

      hPt.setX(xPos);
      hPt.setY(m_yAxis->yValueToPt(m_klineData->m_highPrice[i]));
//      hPt.setY(drawRc.bottom() - (m_klineData->m_highPrice[i] - fMin)*fValue);

      lPt.setX(xPos);
      lPt.setY(m_yAxis->yValueToPt(m_klineData->m_lowPrice[i]));
//      lPt.setY(drawRc.bottom() -  (m_klineData->m_lowPrice[i] - fMin)*fValue);

      oPt.setX(xPos);
      oPt.setY(m_yAxis->yValueToPt(m_klineData->m_openPrice[i]));
//      oPt.setY(drawRc.bottom() -  (m_klineData->m_openPrice[i] - fMin)*fValue);

      cPt.setX(xPos);
      cPt.setY(m_yAxis->yValueToPt(m_klineData->m_closePrice[i]));
//      cPt.setY(drawRc.bottom() -  (m_klineData->m_closePrice[i] - fMin)*fValue);
      int nResult = compareFloat(m_klineData->m_openPrice[i], m_klineData->m_closePrice[i]);

      if(nResult < 0)   //开盘价小于收盘价  阳线
      {
          upLineVec.append(QLineF(cPt, hPt));
          upLineVec.append(QLineF(oPt, lPt));

          QRectF rc;
          rc.setLeft(cPt.x() - fwidth / 2.0);
          rc.setRight(rc.x() + fwidth);
          rc.setTop(cPt.y());
          rc.setBottom(oPt.y());

          upRcVec.append(rc);
      }
      else if(nResult > 0) //开盘价大于收盘价  阴线
      {
            downLineVec.append(QLineF(lPt, hPt));
            QRectF rc;
            rc.setLeft(cPt.x() - fwidth / 2.0);
            rc.setRight(rc.x() + fwidth);
            rc.setTop(oPt.y());
            rc.setBottom(cPt.y());
            downRcVec.append(rc);
      }
      else
      {
            equalLineVec.append(QLineF(lPt, hPt));
            QPointF p1, p2;
            p1.setX(oPt.x() - fwidth / 2.0);
            p1.setY(oPt.y());
            p2.setX(p1.x() + fwidth);
            p2.setY(oPt.y());
            equalLineVec.append(QLineF(p1, p2));
      }

  }
    QPen pen;
    if(!upLineVec.isEmpty())
    {
        pen.setColor(QColor("#e62727"));
        painter->setPen(pen);
        painter->drawLines(upLineVec);
        painter->drawRects(upRcVec);
    }

    if(!downLineVec.isEmpty())
    {
        pen.setColor(QColor("#0aaa38"));
        painter->setPen(pen);
        painter->drawLines(downLineVec);
        painter->setBrush(QBrush("#0aaa38"));
        painter->drawRects(downRcVec);
    }
    if(!equalLineVec.isEmpty())
    {
        pen.setColor(QColor("#b1bbc3"));
        painter->setPen(pen);
        painter->drawLines(equalLineVec);
    }

}

void KlineWidget::drawCross(QPainter *painter)
{
    QRectF rc = this->getDrawRect();
    if(rc.contains(m_mousePt))
    {
        QPen pen;
        pen.setColor(QColor("#414141"));
        pen.setStyle(Qt::DotLine);
        pen.setWidth(1);
        painter->setPen(pen);

        QPointF p1(rc.left(), m_mousePt.y()),p2(rc.right(), m_mousePt.y());

        painter->drawLine(p1, p2);

        p1.setX(m_mousePt.x());
        p1.setY(rc.top());
        p2.setX(m_mousePt.x());
        p2.setY(rc.bottom());
        painter->drawLine(p1, p2);

    }
}

void KlineWidget::drawYAxisPoint(QPainter *painter)
{
    QRectF rc = this->rect();
    QRectF drawRc = this->getDrawRect();
    QRectF leftRc, rightRc, timeRc;


    if(drawRc.contains(m_mousePt))
    {
        leftRc.setRight(rc.left() + YAXISWIDTH);
        leftRc.setWidth(YAXISWIDTH);
        leftRc.setTop(m_mousePt.y()-10);
        leftRc.setBottom(m_mousePt.y() + 10);
        rightRc.setLeft(rc.right() - YAXISWIDTH);
        rightRc.setWidth(YAXISWIDTH);
        rightRc.setTop(m_mousePt.y()-10);
        rightRc.setBottom(m_mousePt.y() + 10);


        timeRc.setTop(drawRc.bottom());
        timeRc.setHeight(30);
        timeRc.setLeft(m_mousePt.x() - 30);
        timeRc.setRight(m_mousePt.x() + 30);

        QString strPrice, strTime;
        strPrice = QString::number(m_yAxis->yPtToValue(m_mousePt.y()), 'f', 2);

        int index = m_xAxis->xPtxToIndex(m_mousePt.x());
        strTime = QDateTime::fromTime_t(m_klineData->m_updateTime[index]).toString("yyyy/MM/dd");

        QPen pen;
        pen.setStyle(Qt::DotLine);

        pen.setColor("#414141");
        painter->setBrush(QColor("#0c1215"));
        painter->setPen(pen);
        painter->drawRect(leftRc);
        painter->drawRect(rightRc);
        painter->drawRect(timeRc);

        pen.setColor("#414141");

        painter->drawText(leftRc, Qt::AlignCenter, strPrice);
        painter->drawText(rightRc, Qt::AlignCenter, strPrice);
        painter->drawText(timeRc, Qt::AlignCenter, strTime);
    }

}

void KlineWidget::drawXAxis(QPainter *painter)
{
    QRectF rc = this->getDrawRect();

    float fWidth = rc.width() / XAXISSEPARATE;


    QPen pen;
    pen.setColor("#b1bbc3");
    painter->setPen(pen);

    QString strTime;
    QRectF textRc;
    textRc.setTop(rc.bottom());
    if(!m_klineData->m_updateTime.isEmpty())
    {
        for(int i = 0; i < XAXISSEPARATE; i++)
        {
            int index = m_xAxis->xPtxToIndex(rc.left() + fWidth*i);
            if(index == -1)
                return;
            strTime = QDateTime::fromTime_t(m_klineData->m_updateTime[index]).toString("yyyy/MM");

            textRc.setHeight(15);
            textRc.setLeft(rc.left() + fWidth*i - 30);
            textRc.setRight(rc.left() + fWidth*i + 30);
            painter->drawText(textRc, strTime);
        }
    }
}

QRectF KlineWidget::getDrawRect()
{
    QRectF rc = this->rect();
    rc.setLeft(rc.left() + YAXISWIDTH);
    rc.setRight(rc.right() - YAXISWIDTH);
    rc.setTop(rc.top() + XAXISHEIGHT);
    rc.setBottom(rc.bottom() - XAXISHEIGHT);

    return rc;
}

void KlineWidget::startReq()
{

//    if(!m_cacheData->m_updateTime.isEmpty() && m_cacheData->m_updateTime.size() == m_klineData->m_updateTime.size())
//    {
//            m_singleKlineWidth = this->getDrawRect().width() / m_cacheData->m_updateTime.size();
//            m_xAxis->setSingleAreaWidth(m_singleKlineWidth);
//    }

    HZData::ReqKlineInfo reqInfo;
    reqInfo.stock_name = m_stockCode.toStdString();
    reqInfo.kline_type = m_period;
    reqInfo.start_time = -1;
    reqInfo.count = this->getDrawRect().width() / m_singleKlineWidth;
    reqInfo.is_desc = true;


    m_reqInfo.nCount = reqInfo.count;

    OnKLineArrived fun = std::bind(&KlineWidget::onKlineDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);


    int id = gHZDataController->requestKline(reqInfo, fun);

}

void KlineWidget::reqKline(HZData::ReqKlineInfo reqInfo)
{
    if(!reqInfo.is_desc)
    {
        getScreenData();
        return;
    }
    else
    {
        OnKLineArrived fun = std::bind(&KlineWidget::onKlineDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        int index = -1;
        if(reqInfo.start_time != -1 && !m_cacheData->m_upsertTime.isEmpty())
        {
            index = m_cacheData->m_upsertTime.lastIndexOf(reqInfo.start_time);
        }

        if(-1 != index)
        {
            //反方向请求（缩放/右拖/第一次请求）
            //本地缓存数据够计算K线和绘制或则如果服务器数据已经到头了不发生真正请求
            if(index + 1 - reqInfo.count >=0)
            {
                getScreenData();
                return;
            }
            else
            {
                reqInfo.start_time = m_cacheData->m_upsertTime[0];
            }
        }


        int id = gHZDataController->requestKline(reqInfo, fun);
    }
}

void KlineWidget::onKlineDataArrived(ReqId id, DataKLine *dataKline, ErrCode errCode)
{
    if( 0 == errCode)
    {
//        mergeData(dataKline);

        if(dataKline->m_data.isEmpty())
        {
            getScreenData();
//            m_isEnd = true;
        }
        else
        {
            mergeData(dataKline);
        }

        qDebug() << "KlineWidget::onKlineDataArrived";

    }
}

void KlineWidget::dragLeft(int noffset)
{

    int nScreenNum = this->getDrawRect().width() / m_singleKlineWidth;

    if(m_cacheData->m_data.size() >= nScreenNum )
    {
        HZData::ReqKlineInfo reqInfo;
        reqInfo.stock_name = m_stockCode.toStdString();
        reqInfo.kline_type = m_period;
        reqInfo.start_time = m_klineData->m_upsertTime[noffset];
        reqInfo.count = nScreenNum;
        reqInfo.is_desc = false;

        noffset += m_noffset;

        if(noffset > 0)
            noffset = 0;

        m_reqInfo.nCount = reqInfo.count;
        m_reqInfo.noffset = noffset;

        reqKline(reqInfo);
    }
}

void KlineWidget::dragRight(int noffset)
{
    int nScreenNum = this->getDrawRect().width() / m_singleKlineWidth;

    if(m_cacheData->m_data.size() >= nScreenNum)
    {

        int nIndex = m_klineData->m_upsertTime.size() - 1 - noffset;

        if(nIndex < 0)
            nIndex = 0;

        HZData::ReqKlineInfo reqInfo;
        reqInfo.stock_name = m_stockCode.toStdString();
        reqInfo.kline_type = m_period;
        reqInfo.start_time = m_klineData->m_upsertTime[nIndex];
        reqInfo.count = nScreenNum;
        reqInfo.is_desc = true;

        m_reqInfo.noffset = m_noffset - noffset;
        m_reqInfo.nCount = reqInfo.count;

        reqKline(reqInfo);

    }
}

void KlineWidget::keyUp()
{
    if(!m_klineData->m_upsertTime.isEmpty())
    {
        int nCount = m_reqInfo.nCount;


        nCount /= 2;
        m_reqInfo.nCount = nCount;
        HZData::ReqKlineInfo reqInfo;
        reqInfo.stock_name = m_stockCode.toStdString();
        reqInfo.kline_type = m_period;
        reqInfo.start_time = m_klineData->m_upsertTime.last();
        reqInfo.count = nCount;
        reqInfo.is_desc = true;

        m_singleKlineWidth = this->getDrawRect().width() / nCount;

        if(m_singleKlineWidth > KLINEMAXWIDTH)
        {
            m_singleKlineWidth =  KLINEMAXWIDTH;

            m_reqInfo.nCount = this->getDrawRect().width() / m_singleKlineWidth;
        }

        m_xAxis->setSingleAreaWidth(m_singleKlineWidth);

        reqKline(reqInfo);
    }
}

void KlineWidget::keyDown()
{
    if(!m_klineData->m_upsertTime.isEmpty())
    {
        if(m_reqInfo.nCount >= m_cacheData->m_upsertTime.size())
        {
            m_reqInfo.nCount = m_cacheData->m_upsertTime.size();
        }

        int nCount = m_reqInfo.nCount;
        nCount *=2;
        HZData::ReqKlineInfo reqInfo;
        reqInfo.stock_name = m_stockCode.toStdString();
        reqInfo.kline_type = m_period;
        reqInfo.start_time = m_klineData->m_upsertTime.last();
        reqInfo.count = nCount;
        reqInfo.is_desc = true;


        m_singleKlineWidth = this->getDrawRect().width() / nCount;

        m_xAxis->setSingleAreaWidth(m_singleKlineWidth);

        m_reqInfo.nCount = reqInfo.count;
        reqKline(reqInfo);

    }
}

void KlineWidget::mergeData(DataKLine *src)
{
    if(m_cacheData->m_data.isEmpty())
    {
        *m_cacheData = *src;

        getScreenData();

        return;
    }
    else
    {
        if(src->m_upsertTime[0] < m_cacheData->m_updateTime[0] && src->m_upsertTime.last() > m_cacheData->m_upsertTime.last())
        {
            *m_cacheData = *src;
        }
        else if(src->m_upsertTime[0] < m_cacheData->m_upsertTime[0])
        {
            int num = src->m_upsertTime.size() + m_cacheData->m_upsertTime.size();

            int nIndex = src->m_upsertTime.lastIndexOf(m_cacheData->m_upsertTime[0]);

            QVector<HZData::HZKline>    data;  //原始数据
            QVector<std::string>     tradingDay;		//交易日
            QVector<double>     updateTime;//时间
            QVector<double>     upsertTime;
            QVector<double>     highPrice; //最高价
            QVector<double>     openPrice ; //开盘价
            QVector<double>	    lowPrice; //最低价
            QVector<double>     closePrice	;//

            QVector<double>      volume ;		//成交量
            QVector<double>	   turnover ;		//成交额
            QVector<double>      preClosePrice ;		// 昨收价
            QVector<double>      averPrice ;// 均价 成交额/成交量

            data.reserve(num);
            tradingDay.reserve(num);
            updateTime.reserve(num);
            upsertTime.reserve(num);
            highPrice.reserve(num);
            openPrice.reserve(num);
            lowPrice.reserve(num);
            closePrice.reserve(num);
            volume.reserve(num);
            turnover.reserve(num);
            preClosePrice.reserve(num);
            averPrice.reserve(num);

            data.append(src->m_data.mid(0, nIndex));
            data.append(m_cacheData->m_data);
            m_cacheData->m_data.swap(data);

            tradingDay.append(src->m_tradingDay.mid(0, nIndex));
            tradingDay.append(m_cacheData->m_tradingDay);
            m_cacheData->m_tradingDay.swap(tradingDay);

            updateTime.append(src->m_updateTime.mid(0, nIndex));
            updateTime.append(m_cacheData->m_updateTime);
            m_cacheData->m_updateTime.swap(updateTime);

            upsertTime.append(src->m_upsertTime.mid(0, nIndex));
            upsertTime.append(m_cacheData->m_upsertTime);
            m_cacheData->m_upsertTime.swap(upsertTime);


            highPrice.append(src->m_highPrice.mid(0, nIndex));
            highPrice.append(m_cacheData->m_highPrice);
            m_cacheData->m_highPrice.swap(highPrice);

            openPrice.append(src->m_openPrice.mid(0, nIndex));
            openPrice.append(m_cacheData->m_openPrice);
            m_cacheData->m_openPrice.swap(openPrice);

            lowPrice.append(src->m_lowPrice.mid(0, nIndex));
            lowPrice.append(m_cacheData->m_lowPrice);
            m_cacheData->m_lowPrice.swap(lowPrice);

            closePrice.append(src->m_closePrice.mid(0, nIndex));
            closePrice.append(m_cacheData->m_closePrice);
            m_cacheData->m_closePrice.swap(closePrice);

            volume.append(src->m_volume.mid(0, nIndex));
            volume.append(m_cacheData->m_volume);
            m_cacheData->m_volume.swap(volume);

            turnover.append(src->m_turnover.mid(0, nIndex));
            turnover.append(m_cacheData->m_turnover);
            m_cacheData->m_turnover.swap(turnover);

            preClosePrice.append(src->m_preClosePrice.mid(0, nIndex));
            preClosePrice.append(m_cacheData->m_preClosePrice);
            m_cacheData->m_preClosePrice.swap(preClosePrice);

            averPrice.append(src->m_averPrice.mid(0, nIndex));
            averPrice.append(m_cacheData->m_averPrice);
            m_cacheData->m_averPrice.swap(averPrice);

            m_cacheData->clearSig = src->clearSig;
        }


        getScreenData();
    }
}

void KlineWidget::getScreenData()
{
    if(!m_cacheData->m_data.isEmpty())
    {
        int end = m_cacheData->m_data.size() + m_reqInfo.noffset;

        m_noffset = m_reqInfo.noffset;;

        int begin = end - m_reqInfo.nCount;

        if(begin < 0)
        {
            begin = 0;
        }

        m_cacheData->mid(begin, m_reqInfo.nCount, *m_klineData);

        if(0 == begin)
        {
            m_noffset = m_klineData->m_upsertTime.size() - m_cacheData->m_upsertTime.size();
        }


//        if(m_isDown)
//        {

//            m_singleKlineWidth = this->getDrawRect().width() / m_klineData->m_updateTime.size();

//            m_xAxis->setSingleAreaWidth(m_singleKlineWidth);

//            m_isDown = false;
//        }



        QVector<double>::iterator max = std::max_element(std::begin(m_klineData->m_highPrice),std::end(m_klineData->m_highPrice));
        QVector<double>::iterator min = std::min_element(std::begin(m_klineData->m_lowPrice), std::end(m_klineData->m_lowPrice));


        m_maxValue = *max;
        m_minValue = *min;

        m_yAxis->calc();

        this->update();
    }
}

void KlineWidget::onTimeOut()
{
    if(m_bFlg)
    {
        startReq();
        m_bFlg = false;
    }

}
