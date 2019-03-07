#include "updownwidget.h"
#include <QPainter>
#include "datacontroller.h"


UpDownWidget::UpDownWidget(QWidget *parent):
    BaseWidget(parent),
    m_actived(false)
{
    this->setFixedHeight(70);
//    this->setFixedWidth(500);
    initData();
}

void UpDownWidget::setData(const HZData::StockRiseFallInfo &risefallInfo)
{

    //legend
    m_riseFallLegendSet[0].value = risefallInfo.up_num;
    m_riseFallLegendSet[1].value = risefallInfo.limit_up_num;
    m_riseFallLegendSet[2].value = risefallInfo.down_num;
    m_riseFallLegendSet[3].value = risefallInfo.limit_down_num;
    m_riseFallLegendSet[4].value = risefallInfo.flat_num;
    m_riseFallLegendSet[5].value = risefallInfo.stop_num;

    this->update();
}

void UpDownWidget::afterActive()
{
    m_actived = true;
    if(!m_stockCode.isEmpty())
        subRiseFallInfo();
}

void UpDownWidget::afterDeactive()
{
    m_actived = false;
    cancelSub();
}

void UpDownWidget::setStockCode(const QString &stockCode)
{
    m_stockCode = stockCode;

//    subRiseFallInfo();
    if(m_actived){
        subRiseFallInfo();
    }
}

void UpDownWidget::paintEvent(QPaintEvent *e)
{
    QPainter pt(this);
    QFont font, numfont;
    font.setFamily("SimSun");
    font.setPixelSize(12);
    QFontMetrics fontMetrics(font);

    numfont.setFamily("Arial");
    numfont.setPixelSize(12);
    QFontMetrics numMetrics(numfont);

//    pt.setFont(font);
    int rectY = LEGEND_TEXT_HEIGHT + fontMetrics.height()/2;
    int textY = LEGEND_TEXT_HEIGHT;
    int x = 0;
    for(int i=0;i<m_riseFallLegendSet.size();i++){

        x = LEGEND_DISTANCE*i;

        if(0 == i || 2 == i || 4 == i)
        {
            //rect
            pt.setPen(Qt::NoPen);
            if(0 == i)
            {
                pt.setBrush(m_colorWidget.upColor());
            }
            else if(2 == i)
            {
                pt.setBrush(m_colorWidget.downColor());
            }
            else
            {
                pt.setBrush(m_colorWidget.equalColor());
            }

            pt.drawRect(x + 18,rectY,LEGEND_RECT_WIDTH,LEGEND_RECT_WIDTH);
        }

        font.setFamily("SimSun");
        pt.setFont(font);
        //text
        pt.setPen(m_colorWidget.contexColor());
        QRect rect(x + LEGEND_TEXT_OFFSE + 18, textY, fontMetrics.width(m_riseFallLegendSet[i].label), fontMetrics.height());
        pt.drawText(rect, m_riseFallLegendSet[i].label);

        //value
        if(0==i || 1==i)
        {
            pt.setPen(m_colorWidget.upColor());
        }
        else if(2==i || 3==i)
        {
            pt.setPen(m_colorWidget.downColor());
        }
        else
        {
            pt.setPen(m_colorWidget.equalColor());
        }

        font.setFamily("Arial");
        pt.setFont(font);

        rect.setRect(x + LEGEND_TEXT_OFFSE+fontMetrics.width(m_riseFallLegendSet[i].label)+ 18, textY, numMetrics.width(QString::number(m_riseFallLegendSet[i].value)), numMetrics.height());
        pt.drawText(rect, QString::number(m_riseFallLegendSet[i].value));
    }

    float total = m_riseFallLegendSet[0].value + m_riseFallLegendSet[2].value + m_riseFallLegendSet[4].value;
    float upRate = m_riseFallLegendSet[0].value / total;
    float downRate = m_riseFallLegendSet[2].value / total;
    float  equalRate= m_riseFallLegendSet[4].value / total;
    QRect rc = this->rect();
    int nWidth = rc.width()-16;

    QRect leftRc, centerRc, rightRc;
    leftRc.setRect(8, 40, nWidth*upRate, 20);
    centerRc.setRect(8 + nWidth*upRate, 40, nWidth*equalRate, 20);
    rightRc.setRect(8 + nWidth*upRate + nWidth*equalRate, 40, nWidth*downRate, 20);
    //画矩形
    pt.fillRect(leftRc, m_colorWidget.upRectColor());
    pt.fillRect(centerRc, m_colorWidget.equalRectColor());
    pt.fillRect(rightRc, m_colorWidget.downRectColor());

    //画边框
    if(0 != total)
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
//        pt.setPen("#9a1f1f");
        pt.drawLine(8, 40, 8, 60);
        pt.drawLine(8, 40, 8 + nWidth*upRate, 40);
        pt.drawLine(8, 60, 8 + nWidth*upRate, 60);

        pt.setPen(m_colorWidget.equalBorderColor());
        pt.drawLine(8 + nWidth*upRate, 40, 8 + nWidth*upRate + nWidth*equalRate, 40);
        pt.drawLine(8 + nWidth*upRate, 60, 8 + nWidth*upRate + nWidth*equalRate, 60);


        pt.setPen(m_colorWidget.downBorderColor());
        pt.drawLine(8 + nWidth*upRate + nWidth*equalRate, 40, 8 + nWidth*upRate + nWidth*equalRate + nWidth*downRate, 40);
        pt.drawLine(8 + nWidth*upRate + nWidth*equalRate, 60, 8 + nWidth*upRate + nWidth*equalRate + nWidth*downRate, 60);
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
        pt.drawLine(8 + nWidth*upRate + nWidth*equalRate + nWidth*downRate, 40, 8 + nWidth*upRate + nWidth*equalRate + nWidth*downRate, 60);
    }

    //文字
    QRect rt;
    font.setBold(true);
    font.setFamily("Arial");
    font.setPixelSize(13);
    pt.setFont(font);
    pt.setPen(m_colorWidget.upColor());
    rt.setRect(8+2, 43, 100, 100);
    pt.drawText(rt, QString::number(m_riseFallLegendSet[0].value));

    QFontMetrics f(font);
    int nPos = (nWidth*equalRate - f.width(QString::number(m_riseFallLegendSet[4].value))) / 2;
    pt.setPen(m_colorWidget.numColor());
    rt.setRect(8 + nWidth*upRate + nPos, 43, 100, 100);
    pt.drawText(rt, QString::number(m_riseFallLegendSet[4].value));
    pt.setPen(m_colorWidget.downColor());
    rt.setRect(8 + nWidth - 2 - f.width(QString::number(m_riseFallLegendSet[2].value)), 43, 100, 100);
    pt.drawText(rt, QString::number(m_riseFallLegendSet[2].value));
}

void UpDownWidget::initData()
{
    m_riseFallLegendSet.resize(6);

    QStringList legendList;

    legendList << "上涨：" << "涨停：" << "下跌："
                  << "跌停：" << "平家：" << "停牌：";
    //legendset
    for(int i=0;i<legendList.size();i++)
        m_riseFallLegendSet[i].label = legendList[i];
}

void UpDownWidget::subRiseFallInfo()
{
    cancelSub();
    HZData::ReqStockRiseFallInfo req  = m_stockCode.toStdString();

    onRiseFallInfoArrived  fun= std::bind(&UpDownWidget::onSubRiseFallInfoArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    //开始订阅
   m_subReqId = gHZDataController->subRiseFallInfo(req,fun);
}

void UpDownWidget::cancelSub()
{
    if(m_subReqId != INVALID_REQID){
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void UpDownWidget::onSubRiseFallInfoArrived(ReqId reqId, const HZData::StockRiseFallInfo &riseFallInfo, ErrCode errCode)
{
    if(errCode != 0)
        return;
    this->setData(riseFallInfo);
}
