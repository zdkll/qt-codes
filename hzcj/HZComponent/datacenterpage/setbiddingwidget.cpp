#include "setbiddingwidget.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QScrollArea>
#include <QDebug>

#define LeftTextWidth 30

//namespace ColorSet {
//QColor lineColor("#2f2f2f");
//QColor riselineColor("#9a1f1f");
//QColor riseColor("#350c0f");
//QColor falllineColor("#127f3a");
//QColor fallColor("#0b2e18");
//QColor textColor("#adadad");
//QColor riseNumColor("#e62727");
//QColor fallNumColor("#0aaa38");
//}
//using namespace ColorSet;



SetBiddingChart::SetBiddingChart(QWidget *parent):
    BaseWidget(parent),
    m_pix(nullptr)
{
    m_textFont.setFamily("SimSun");
    m_textFont.setPixelSize(12);

    m_numFont.setFamily("Arial");
    m_numFont.setPixelSize(13);
    m_numFont.setBold(true);

    m_riseData.biddingSet =  SetBiddingSet(10);
    m_fallData.biddingSet =  SetBiddingSet(10);

//    m_riseData.biddingSet.clear();
//    m_fallData.biddingSet.clear();

//    updateData();
}

SetBiddingChart::~SetBiddingChart()
{
    if(m_pix)
        delete m_pix;
}

void SetBiddingChart::updateData()
{
//    m_riseData.biddingSet =  SetBiddingSet(10);
//    m_fallData.biddingSet =  SetBiddingSet(10);
    float riseRates[] = { 0.066, 0.065, 0.055, 0.045, 0.035, 0.022, 0.011, 0.088, 0.089, 0.073};
    float fallRates[] = { -0.066, -0.065, -0.055, -0.045, -0.035, -0.022, -0.011,-0.099, -0.089, -0.073};

    QStringList nameList = {QStringLiteral("独角兽"), QStringLiteral("送转预期"),\
                           QStringLiteral("次新股"), QStringLiteral("航空航天"),\
                           QStringLiteral("木业家具"), QStringLiteral("券商信托"),\
                           QStringLiteral("数字中国"), QStringLiteral("创投"),\
                           QStringLiteral("软件服务"), QStringLiteral("昨日连板")};

    QList<float> riselist;
    QList<float> falllist;

    for(int i = 0; i < 10; i++)
    {
       riselist << riseRates[i];
       falllist << fallRates[i];
    }

    qSort(riselist.begin(), riselist.end(), qGreater<float>());

    m_riseData.maxValue = riselist[0];

    m_riseData.strMaxScale =  QString::number( riselist[0]*100 , 'f', 1);

    m_riseData.strHalfScale =  QString::number( riselist[0]*100 / 2 , 'f', 1);

    qSort(falllist.begin(), falllist.end(), qGreater<float>());

    m_fallData.maxValue = qAbs(falllist.last());

    m_fallData.strMaxScale =  QString::number(falllist.last()*100 , 'f', 1);

    m_fallData.strHalfScale =  QString::number(falllist.last() *100/ 2 , 'f', 1);

    for(int i = 0; i < riselist.size(); i++)
    {
        m_riseData.biddingSet[i].rate = riselist[i];
        m_riseData.biddingSet[i].strRate = QString::number(riselist[i]*100, 'f', 2) + "%";
        m_riseData.biddingSet[i].strName = nameList[i];
    }


    for(int i = 0; i < falllist.size(); i++)
    {
        m_fallData.biddingSet[i].rate = qAbs(falllist[i]);
        m_fallData.biddingSet[i].strRate = QString::number(falllist[i]*100, 'f', 2) + "%";
        m_fallData.biddingSet[i].strName = nameList[i];
    }

}

void SetBiddingChart::updateData(const BiddingBlockInfo &info, RateDir rateDir)
{

    if(RiseDir == rateDir)
    {
        transformRiseData(info);
    }
    else
    {
        transformFallData(info);
    }

    if(m_fallData.maxValue != INVALID_FLOAT)
    {
        if(m_riseData.maxValue > qAbs(m_fallData.maxValue))
        {
            m_fallData.maxValue = -m_riseData.maxValue;

            m_fallData.strMaxScale =  QString::number(-m_riseData.maxValue , 'f', 1);

            m_fallData.strHalfScale =  QString::number(-m_riseData.maxValue / 2 , 'f', 1);
        }
        else
        {
            m_riseData.maxValue = qAbs(m_fallData.maxValue);
            m_riseData.strMaxScale =  QString::number(m_riseData.maxValue , 'f', 1);

            m_riseData.strHalfScale =  QString::number(m_riseData.maxValue / 2 , 'f', 1);
        }
        this->update();
    }
}

void SetBiddingChart::paintEvent(QPaintEvent *event)
{
    BaseWidget::paintEvent(event);

//    QPainter painter(this);

//    painter.drawPixmap(this->rect(), *m_pix);

    draw();
}

//void SetBiddingChart::resizeEvent(QResizeEvent *e)
//{
//    if(m_pix)
//    {
//        delete  m_pix;
//    }

//    QSize size = this->size();

//    m_pix = new QPixmap(this->size());

//    m_pix->fill(Qt::transparent);

//    draw();

//}

void SetBiddingChart::draw()
{


    if(m_riseData.maxValue == INVALID_FLOAT || m_fallData.maxValue == INVALID_FLOAT)
        return;

    QFontMetrics textMetrics(m_textFont);
    QFontMetrics numMetrics(m_numFont);


    QPixmap pix(this->size());
//    QPainter painter(m_pix);

    pix.fill(Qt::transparent);

    QPainter painter(&pix);

    int nWidth = this->width();
    int nHeight = this->height();

    int upTextHeight = numMetrics.height();  //预留上涨涨幅数字高度
    //绘图区域高度   总高度-上涨涨幅数字高度-下方文字高度-下方数字高度
    int nChartHeight = nHeight - numMetrics.height() - textMetrics.height() - upTextHeight;

    //高度四等分
    float nAvgHeight = (float) nChartHeight / 4;
    //宽度20等分   LeftTextWidth为左侧文字宽度
    float nAvgWidth = (float)(nWidth - LeftTextWidth) / 20;

    //单位涨幅对应高度
    float riseUnitHeight = nAvgHeight * 2 / m_riseData.maxValue;
    //单位跌幅对应高度
    float fallUnitHeight = nAvgHeight * 2 / m_fallData.maxValue;


    painter.setPen(m_styledWidget.lineColor());
    //画坐标线
    for(int i = 0; i < 5; i++)
    {
        //垂直方向upTextHeight为起点
        painter.drawLine(LeftTextWidth,  nAvgHeight*i + upTextHeight, nWidth, nAvgHeight*i + upTextHeight);
    }

    painter.drawLine(LeftTextWidth, upTextHeight,  LeftTextWidth, nChartHeight + upTextHeight);
    painter.drawLine(nWidth - 1, upTextHeight, nWidth - 1, nChartHeight+ upTextHeight);


    //画左侧数据
    QRect leftRc;

    painter.setFont(m_numFont);
    painter.setPen(m_styledWidget.textColor());
    leftRc.setRect(LeftTextWidth - 5 - numMetrics.width(m_riseData.strMaxScale), upTextHeight / 2, numMetrics.width(m_riseData.strMaxScale), numMetrics.height());

    painter.drawText(leftRc, m_riseData.strMaxScale);

    leftRc.setRect(LeftTextWidth - 5 - numMetrics.width(m_riseData.strHalfScale), upTextHeight / 2 +  nAvgHeight, numMetrics.width(m_riseData.strHalfScale), numMetrics.height());

    painter.drawText(leftRc, m_riseData.strHalfScale);

    leftRc.setRect(LeftTextWidth - 5 - numMetrics.width("0"), upTextHeight / 2 +  nAvgHeight*2, numMetrics.width("0"), numMetrics.height());

    painter.drawText(leftRc, "0");

    leftRc.setRect(LeftTextWidth - 5 - numMetrics.width(m_fallData.strHalfScale), upTextHeight / 2 +  nAvgHeight*3, numMetrics.width(m_fallData.strMaxScale), numMetrics.height());

    painter.drawText(leftRc, m_fallData.strHalfScale);

    leftRc.setRect(LeftTextWidth - 5 - numMetrics.width(m_fallData.strMaxScale), upTextHeight / 2 +  nAvgHeight*4, numMetrics.width(m_fallData.strMaxScale), numMetrics.height());

    painter.drawText(leftRc, m_fallData.strMaxScale);


    //画矩形  上涨矩形
    QRect rect;
//    painter.setPen(m_styledWidget.riseBorderColor());
//    painter.setBrush(m_styledWidget.riseBrushColor());
    for(int i = 0; i < 10; i++)
    {
        if(0 == m_riseData.biddingSet[i].rate)
            continue;
        else if(m_riseData.biddingSet[i].rate > 0)
        {
            painter.setPen(m_styledWidget.riseBorderColor());
            painter.setBrush(m_styledWidget.riseBrushColor());
        }
        else
        {
            painter.setPen(m_styledWidget.fallBorderColor());
            painter.setBrush(m_styledWidget.fallBrushColor());
        }
        rect.setRect(LeftTextWidth + nAvgWidth / 4 + nAvgWidth*i, upTextHeight + 2*nAvgHeight, \
                     nAvgWidth / 2, -riseUnitHeight*m_riseData.biddingSet[i].rate);
        painter.drawRect(rect);
    }

    //下跌矩形
    for(int i = 0; i < 10; i++)
    {
        if(0 == m_fallData.biddingSet[i].rate)
            continue;
        else if(m_fallData.biddingSet[i].rate > 0)
        {
            painter.setPen(m_styledWidget.riseBorderColor());
            painter.setBrush(m_styledWidget.riseBrushColor());
        }
        else
        {
            painter.setPen(m_styledWidget.fallBorderColor());
            painter.setBrush(m_styledWidget.fallBrushColor());
        }
        rect.setRect(LeftTextWidth + nAvgWidth / 4 + nAvgWidth*(10+i), upTextHeight + 2*nAvgHeight, \
                     nAvgWidth / 2, fallUnitHeight*m_fallData.biddingSet[i].rate);
        painter.drawRect(rect);
    }

    //画涨幅
    //上涨涨幅
    QRect rateRc;
    for(int i = 0; i < 10; i++)
    {
        if(m_riseData.biddingSet[i].rate > 0)
        {
            painter.setPen(m_styledWidget.riseNumColor());
            rateRc.setRect(LeftTextWidth + (nAvgWidth - numMetrics.width(m_riseData.biddingSet[i].strRate)) / 2 + nAvgWidth*i , \
                           upTextHeight + 2*nAvgHeight - numMetrics.height() - riseUnitHeight*m_riseData.biddingSet[i].rate, \
                           numMetrics.width(m_riseData.biddingSet[i].strRate), numMetrics.height());
        }
        else
        {
            painter.setPen(m_styledWidget.fallNumColor());
            rateRc.setRect(LeftTextWidth + (nAvgWidth - numMetrics.width(m_riseData.biddingSet[i].strRate)) / 2 + nAvgWidth*i, \
                           upTextHeight + 2*nAvgHeight + 2 - riseUnitHeight*m_riseData.biddingSet[i].rate, \
                           numMetrics.width(m_riseData.biddingSet[i].strRate), numMetrics.height());
        }
        painter.drawText(rateRc, m_riseData.biddingSet[i].strRate);
    }
    //下跌涨幅
    painter.setPen(m_styledWidget.fallNumColor());
    for(int i = 0; i < 10; i++)
    {
        if(m_fallData.biddingSet[i].rate > 0)
        {
            painter.setPen(m_styledWidget.riseNumColor());
            rateRc.setRect(LeftTextWidth + (nAvgWidth - numMetrics.width(m_fallData.biddingSet[i].strRate)) / 2 + nAvgWidth*(i + 10) , \
//                           upTextHeight + 2*nAvgHeight + 2,
                            upTextHeight + 2*nAvgHeight - numMetrics.height() - riseUnitHeight*m_fallData.biddingSet[i].rate, \
                           numMetrics.width(m_fallData.biddingSet[i].strRate), numMetrics.height());

//            upTextHeight + 2*nAvgHeight - numMetrics.height() - riseUnitHeight*m_riseData.biddingSet[i].rate
//            upTextHeight + 2*nAvgHeight + numMetrics.height() - riseUnitHeight*m_fallData.biddingSet[i].rate,
        }
        else
        {
            painter.setPen(m_styledWidget.fallNumColor());
            rateRc.setRect(LeftTextWidth + (nAvgWidth - numMetrics.width(m_fallData.biddingSet[i].strRate)) / 2 + nAvgWidth*(i + 10) , \
                           upTextHeight + 2*nAvgHeight + 2 + fallUnitHeight*m_fallData.biddingSet[i].rate, \
                           numMetrics.width(m_fallData.biddingSet[i].strRate), numMetrics.height());
        }
        painter.drawText(rateRc, m_fallData.biddingSet[i].strRate);
    }


    //画下方文字
    QRect textRc;
    painter.setPen(m_styledWidget.textColor());
    painter.setFont(m_textFont);
    QString strText;
    for(int i = 0; i < 20; i++)
    {
        if(i < 10)
            strText = m_riseData.biddingSet[i].strName;
        else
            strText = m_fallData.biddingSet[i - 10].strName;
        textRc.setRect(LeftTextWidth + (nAvgWidth - textMetrics.width(strText)) / 2 + nAvgWidth*i , nHeight - textMetrics.height(), \
                     textMetrics.width(strText), textMetrics.height());
        painter.drawText(textRc, strText);
    }


    //结束画笔
    painter.end();

    //开始在窗口上画，画什么呢？
    painter.begin(this);
    //前方开始装车的过程，最后一股气打包全部带走，送到窗口
    //北京颜色发生了改变，怎么变回来呢？
    painter.drawPixmap(QPoint(0, 0), pix);


}

void SetBiddingChart::transformRiseData(const BiddingBlockInfo &info)
{
    QVector<BlockBid> bidVec;
    float tmpRate = 0.0f;

    if(!info.stk.isEmpty())
    {
        bidVec.append(info.stk);
    }
    else
    {
        bidVec.resize(10);
    }


    qSort(bidVec.begin(), bidVec.end(),
          [](const BlockBid &ItemA ,const BlockBid &ItemB ){
                        return ItemA.rate > ItemB.rate;});

    for(int i = 0; i < bidVec.size(); i++)
    {
        if(INVALID_DOUBLE == bidVec[i].rate){
            m_riseData.biddingSet[i].rate = tmpRate;
        }
        else
        {
            m_riseData.biddingSet[i].rate = bidVec[i].rate;
        }
        m_riseData.biddingSet[i].strRate = QString::number(m_riseData.biddingSet[i].rate, 'f', 2) + "%";
        m_riseData.biddingSet[i].strName = QString::fromStdString(bidVec[i].block_name);
    }

    if(bidVec[0].rate == INVALID_DOUBLE)
    {
        m_riseData.maxValue = tmpRate;
    }
    else
    {
        m_riseData.maxValue = bidVec[0].rate;
    }

    m_riseData.strMaxScale =  QString::number(m_riseData.maxValue , 'f', 1);

    m_riseData.strHalfScale =  QString::number(m_riseData.maxValue / 2 , 'f', 1);


}

void SetBiddingChart::transformFallData(const BiddingBlockInfo &info)
{
    QVector<BlockBid> bidVec;
    float tmpRate = 0.0f;

    if(!info.stk.isEmpty())
    {
        bidVec.append(info.stk);
    }
    else
    {
        bidVec.resize(10);
    }


    qSort(bidVec.begin(), bidVec.end(),
          [](const BlockBid &ItemA ,const BlockBid &ItemB ){
        return ItemA.rate > ItemB.rate;});


    for(int i = 0; i < bidVec.size(); i++)
    {
        if(INVALID_DOUBLE == bidVec[i].rate){
            m_fallData.biddingSet[i].rate = tmpRate;
        }
        else
        {
            m_fallData.biddingSet[i].rate = bidVec[i].rate;
        }

        m_fallData.biddingSet[i].strRate = QString::number(bidVec[i].rate, 'f', 2) + "%";
        m_fallData.biddingSet[i].strName = QString::fromStdString(bidVec[i].block_name);
    }

    if(bidVec.last().rate == INVALID_DOUBLE)
    {
        m_fallData.maxValue = tmpRate;
    }
    else
    {
        m_fallData.maxValue = m_fallData.biddingSet.last().rate;
    }

    m_fallData.strMaxScale =  QString::number(m_fallData.maxValue , 'f', 1);

    m_fallData.strHalfScale =  QString::number(m_fallData.maxValue / 2 , 'f', 1);



}


SetBiddingWidget::SetBiddingWidget(QWidget *parent):
    ScrollWidget(parent),
    m_nRiseRed(INVALID_REQID),
    m_nFallRed(INVALID_REQID)
{
    createWg();
}

void SetBiddingWidget::afterActive()
{
    startSub();
}

void SetBiddingWidget::afterDeactive()
{
    cancelSub();
}



void SetBiddingWidget::createWg()
{
    QWidget *mainWidget = new QWidget(this);

    mainWidget->setObjectName("mainwidget");

    m_subwidget = new SetBiddingChart;
    QVBoxLayout *sublayout = new QVBoxLayout;
    sublayout->setSpacing(0);
    sublayout->setContentsMargins(42, 34, 18, 39);

    sublayout->addWidget(m_subwidget);

    mainWidget->setLayout(sublayout);


    m_subwidget->setFixedWidth(1440);

    this->setWidget(mainWidget);
}

void SetBiddingWidget::startSub()
{
    ReqBatchBiddingBlock req;

    req.sort_field = bid_block_rate;
    req.index = 0;
    req.is_desc = false;
    req.count_range = 10;

    OnBiddingBlockArrived fun = std::bind(&SetBiddingWidget::onDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_nRiseRed = gHZDataController->subBatchBiddingBlock(req, fun);


    req.is_desc = true;

    m_nFallRed = gHZDataController->subBatchBiddingBlock(req, fun);
}

void SetBiddingWidget::cancelSub()
{
    if(m_nRiseRed != INVALID_REQID)
    {
        gHZDataController->cancelSub(m_nRiseRed);
        m_nRiseRed  = INVALID_REQID;
    }

    if(m_nFallRed != INVALID_REQID)
    {
        gHZDataController->cancelSub(m_nFallRed);
        m_nFallRed  = INVALID_REQID;
    }
}

void SetBiddingWidget::onDataArrived(ReqId id, const BiddingBlockInfo &info, ErrCode errCode)
{
    Q_UNUSED(id)

    if(0 != errCode)
        return;
    if(id == m_nRiseRed)
    {
        m_subwidget->updateData(info);
    }
    else if(id == m_nFallRed)
    {
        m_subwidget->updateData(info, FallDir);
    }
}


