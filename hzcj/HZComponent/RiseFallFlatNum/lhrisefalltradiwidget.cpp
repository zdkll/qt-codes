#include "lhrisefalltradiwidget.h"
#include <QPainter>

LhRiseFallTradiWidget::LhRiseFallTradiWidget(QWidget *parent):
   BaseWidget(parent),
   m_nTotal(0)
{
    m_strUpRate = m_strDownRate = "--";

    this->setFixedHeight(20);
}

void LhRiseFallTradiWidget::updateData(const HZData::AllStock &stock)
{
    if( INVALID_UINT32 == stock.rise || INVALID_UINT32 == stock.flat
            || INVALID_UINT32 == stock.fall)
        return;
    m_nValues[0] = stock.rise;
    m_nValues[1] = stock.fall;

    m_nTotal = m_nValues[0] + m_nValues[1];
    if(0 != m_nTotal)
    {
        m_upRate = (float)m_nValues[0] / (float)m_nTotal;
        m_downRate = (float)m_nValues[1] / (float)m_nTotal;

        m_strUpRate = QString::number(m_upRate*100, 'f', 2) + "%";
        m_strDownRate = QString::number(m_downRate*100, 'f', 2) + "%";
    }


    this->update();
}

void LhRiseFallTradiWidget::updateLhData(const StockLhbDetialListItem &data)
{

    StockLhbDetialListItem lhDetail;
    if(!data.reason.isEmpty())
    {
        lhDetail = data;
    }

    if(lhDetail.buy == INVALID_DOUBLE || lhDetail.sell == INVALID_DOUBLE)
        return;

    m_nValues[0] = lhDetail.buy;
    m_nValues[1] = lhDetail.sell;

    m_nTotal = m_nValues[0] + m_nValues[1];
    if(0 != m_nTotal)
    {
        m_upRate = (float)m_nValues[0] / (float)m_nTotal;
        m_downRate = (float)m_nValues[1] / (float)m_nTotal;

        m_strUpRate = QString::number(m_upRate*100, 'f', 2) + "%";
        m_strDownRate = QString::number(m_downRate*100, 'f', 2) + "%";
    }

    this->update();
}

void LhRiseFallTradiWidget::paintEvent(QPaintEvent *e)
{
//    Q_UNUSED(e)

    QPainter pt(this);

    QRect rect = this->rect();
    QRect leftRc, rightRc;
    int nWidth = rect.width();

    //画矩形
    leftRc.setRect(rect.x(), rect.y(), nWidth*m_upRate, rect.height());
    pt.fillRect(leftRc, m_colorWidget.upRectColor());
    rightRc.setRect(rect.x() + nWidth*m_upRate, rect.y(), nWidth - nWidth*m_upRate, rect.height());
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
        else if(rightRc.contains(leftPt))
        {
            pt.setPen(m_colorWidget.downBorderColor());
        }

        pt.drawLine(leftRc.left(), leftRc.top(), leftRc.left(), leftRc.bottom());
        pt.drawLine(leftRc.left(), leftRc.top(), leftRc.right(), leftRc.top());
        pt.drawLine(leftRc.left(), leftRc.bottom(), leftRc.right(), leftRc.bottom());

        pt.setPen(m_colorWidget.downBorderColor());
        pt.drawLine(rightRc.left(), rightRc.top(), rightRc.right(), rightRc.top());
        pt.drawLine(rightRc.left(), rightRc.bottom(), rightRc.right(), rightRc.bottom());
        if(leftRc.contains(rightPt))
        {
            pt.setPen(m_colorWidget.upBorderColor());
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
        rt.setRect(17, rect.top()+2, 200, 20);
        if(0 != m_nValues[0])
        {
            pt.drawText(rt, "买入 "+ m_strUpRate);
        }

        rect.adjust(0, 2, -17,2);
        pt.setPen(m_colorWidget.downColor());
        if(0 != m_nValues[1])
        {
            pt.drawText(rect, Qt::AlignRight, "卖出 "+ m_strDownRate);
        }
    }
}
