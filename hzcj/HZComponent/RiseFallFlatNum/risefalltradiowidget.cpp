#include "risefalltradiowidget.h"
#include <QPainter>
#include "datacontroller.h"

RiseFallTradioWidget::RiseFallTradioWidget(QWidget *parent):
    BaseWidget(parent),
    m_subReqId(INVALID_REQID),
    m_strCode(""),
    m_nTotal(0),
    m_upRate(0),
    m_flatRate(0),
    m_downRate(0)
{
    this->setFixedHeight(20);
}

void RiseFallTradioWidget::setStockCode(const QString &stockCode)
{
    m_strCode = stockCode;
    subRiseFallInfo();
}

void RiseFallTradioWidget::paintEvent(QPaintEvent *e)
{

    QPainter pt(this);

    QRect rect = this->rect();
    QRect leftRc, centerRc, rightRc;
    int nWidth = rect.width();

    //画矩形
    leftRc.setRect(rect.x(), rect.y(), nWidth*m_upRate, rect.height());
    pt.fillRect(leftRc, m_colorWidget.upRectColor());
    centerRc.setRect(rect.x() + nWidth*m_upRate, rect.y(), nWidth*m_flatRate, rect.height());
    pt.fillRect(centerRc, m_colorWidget.equalRectColor());
    rightRc.setRect(rect.x() + nWidth*m_upRate + nWidth*m_flatRate, rect.y(), nWidth*m_downRate, rect.height());
    pt.fillRect(rightRc, m_colorWidget.downRectColor());

    //画边框
    if(0 != m_nTotal)
    {
        QPoint leftPt, rightPt;

        leftPt = leftRc.topLeft();
        rightPt = rightRc.bottomRight();

        if(leftRc.contains(leftPt))
        {
            pt.setPen(m_colorWidget.upBorderColor());
        }
        else if(centerRc.contains(leftPt))
        {
            pt.setPen(m_colorWidget.equalBorderColor());
        }
        else if(rightRc.contains(leftPt))
        {
            pt.setPen(m_colorWidget.downBorderColor());
        }
        //        pt.setPen(m_colorWidget.upBorderColor());
        pt.drawLine(leftRc.left(), leftRc.top(), leftRc.left(), leftRc.bottom());
        pt.drawLine(leftRc.left(), leftRc.top(), leftRc.right(), leftRc.top());
        pt.drawLine(leftRc.left(), leftRc.bottom(), leftRc.right(), leftRc.bottom());


        pt.setPen(m_colorWidget.equalBorderColor());
        pt.drawLine(centerRc.left(), centerRc.top(), centerRc.right(), centerRc.top());
        pt.drawLine(centerRc.left(), centerRc.bottom(), centerRc.right(), centerRc.bottom());


        pt.setPen(m_colorWidget.downBorderColor());
        pt.drawLine(rightRc.left(), rightRc.top(), rightRc.right(), rightRc.top());
        pt.drawLine(rightRc.left(), rightRc.bottom(), rightRc.right(), rightRc.bottom());
        if(leftRc.contains(rightPt))
        {
            pt.setPen(m_colorWidget.upBorderColor());
        }
        else if(centerRc.contains(rightPt))
        {
            pt.setPen(m_colorWidget.equalBorderColor());
        }
        else if(rightRc.contains(rightPt))
        {
            pt.setPen(m_colorWidget.downBorderColor());
        }
        pt.drawLine(rightRc.right(), rightRc.top(), rightRc.right(), rightRc.bottom());


        //画文字
        QRect rt;
        QFont font;
        font.setBold(true);
        font.setFamily("Arial");
        font.setPixelSize(13);
        pt.setFont(font);
        pt.setPen(m_colorWidget.upColor());
        rt.setRect(3, rect.top()+3, 100, 100);
        if(0 != m_nValues[0])
            pt.drawText(rt, QString::number(m_nValues[0]));

        QFontMetrics f(font);
        int nPos = (nWidth*m_flatRate - f.width(QString::number(m_nValues[1]))) / 2;
        pt.setPen(m_colorWidget.numColor());
        rt.setRect(nWidth*m_upRate + nPos, rt.top(), 100, 100);
        if(0 != m_nValues[1])
            pt.drawText(rt, QString::number(m_nValues[1]));
        pt.setPen(m_colorWidget.downColor());
        rt.setRect(nWidth  - f.width(QString::number(m_nValues[2])) - 3, rt.top(), 100, 100);
        if(0 != m_nValues[2])
            pt.drawText(rt, QString::number(m_nValues[2]));
    }
}

void RiseFallTradioWidget::subRiseFallInfo()
{
    calcelSub();
    HZData::ReqStockRiseFallInfo req = m_strCode.toStdString();
    onRiseFallInfoArrived fun = std::bind(&RiseFallTradioWidget::onSubRiseFallInfoArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //开始订阅
    m_subReqId = gHZDataController->subRiseFallInfo(req, fun);
}

void RiseFallTradioWidget::calcelSub()
{
    if(INVALID_REQID != m_subReqId)
    {
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void RiseFallTradioWidget::onSubRiseFallInfoArrived(ReqId reqId, const HZData::StockRiseFallInfo &riseFallInfo, ErrCode errCode)
{
    if(0 != errCode)
        return;
    this->setData(riseFallInfo);
}

void RiseFallTradioWidget::setData(const HZData::StockRiseFallInfo &risefallInfo)
{
    m_nValues[0] = risefallInfo.up_num;
    m_nValues[1] = risefallInfo.flat_num;
    m_nValues[2] = risefallInfo.down_num;

    m_nTotal = m_nValues[0] + m_nValues[1] + m_nValues[2];
    if(0 != m_nTotal)
    {
        m_upRate = (float)m_nValues[0] / (float)m_nTotal;
        m_flatRate = (float)m_nValues[1] / (float)m_nTotal;
        m_downRate = (float)m_nValues[2] / (float)m_nTotal;
    }

    this->update();
}

void RiseFallTradioWidget::updateData(INT64 *riseFall)
{
    if( INVALID_UINT32 == riseFall[0] || INVALID_UINT32 == riseFall[1]
            || INVALID_UINT32 == riseFall[2])
        return;
    m_nValues[0] = riseFall[0];
    m_nValues[1] = riseFall[1];
    m_nValues[2] = riseFall[2];

    m_nTotal = m_nValues[0] + m_nValues[1] + m_nValues[2];
    if(0 != m_nTotal)
    {
        m_upRate = (float)m_nValues[0] / (float)m_nTotal;
        m_flatRate = (float)m_nValues[1] / (float)m_nTotal;
        m_downRate = (float)m_nValues[2] / (float)m_nTotal;
    }

    this->update();
}

