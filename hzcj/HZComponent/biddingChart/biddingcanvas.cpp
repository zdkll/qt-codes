#include "biddingcanvas.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QHBoxLayout>
#include "imagelayer.h"


#define BiddWndAxisWidth 60
#define BiddWndToBottomMargin 5
#define BiddWndTimeHight 20
BiddingTitle::BiddingTitle(QWidget *parent)
    :BiddingStyle(parent)
{
}

void BiddingTitle::updateData(const BiddingItem &data)
{
    m_data = data;
    this->update();
}

void BiddingTitle::resizeEvent(QResizeEvent *event)
{

}

QColor BiddingTitle::getClrByDirc(HzQuoteFormat::QuoteDirc dirc)
{
    QColor clr(m_equalClr);
    switch (dirc) {
    case HzQuoteFormat::quote_up:
        clr = m_upClr;
        break;
    case HzQuoteFormat::quote_down:
        clr = m_downClr;
         break;
    default:
        break;
    }
    return clr;
}

void BiddingTitle::draw()
{
    QPainter painter(this);
    QPen namePen(m_titleTextClr);
    QRect rc(15,0,this->geometry().width()-30,this->geometry().height());
    int nWidth = rc.width()/3;
    int nHight = this->geometry().height();
    //绘制最新价名称
    int left  =rc.left();
    painter.setPen(namePen);
    painter.drawText(QRect(left,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,QStringLiteral("现价："));
    //绘制最新价
    HzQuoteFormat::QuoteItem item = HzQuoteFormat::QuoteFormat::formatPrice(m_data.last_price,m_data.pre_close_price);
    painter.setPen(QPen(getClrByDirc(item.dirc)));
    painter.drawText(QRect(left+35,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,item.strValue);

    //绘制涨跌幅名称
    painter.setPen(namePen);
    left = left+nWidth;
    painter.drawText(QRect(left,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,QStringLiteral("涨幅："));
    //绘制涨跌幅
    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_data.bid_rate*100.0f,m_data.last_price,m_data.pre_close_price);
    painter.setPen(QPen(getClrByDirc(item.dirc)));
    painter.drawText(QRect(left+35,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,item.strValue);

    //绘制成交量名称
    painter.setPen(namePen);
    left = left+nWidth;
    painter.drawText(QRect(left,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,QStringLiteral("成交量："));
    //绘制成交量
    item = HzQuoteFormat::QuoteFormat::formatVolume(m_data.volume);
    if(item.strValue != DEFAULT_STR_PRICE)
    {
        item.strValue += QStringLiteral("手");
    }
    painter.setPen(QPen(getClrByDirc(item.dirc)));
    painter.drawText(QRect(left+45,0,nWidth,nHight),Qt::AlignVCenter|Qt::AlignLeft,item.strValue);
}

void BiddingTitle::paintEvent(QPaintEvent *event)
{
    draw();
}

BiddingChart::BiddingChart(QWidget *parent)
    :BiddingStyle(parent),
      m_imageLayer(new ImageLayer(this))
{
    m_gridAndAxisLayerId = m_imageLayer->createLayout(this->geometry()).type;
    m_chartLayerId = m_imageLayer->createLayout(this->geometry()).type;
    m_gradientLayerId = m_imageLayer->createLayout(this->geometry()).type;
}

void BiddingChart::updateData(BiddingData *pData)
{
    m_data = *pData;
    drawLine();
    this->update();
}

void BiddingChart::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rc(0,0,this->geometry().width(),this->geometry().height());
    painter.fillRect(rc,QColor("#0c1215"));
    m_imageLayer->repaint();
}

void BiddingChart::resizeEvent(QResizeEvent *event)
{
    QRect rc(0,0,this->geometry().width(),this->geometry().height());
    CalcRc(rc);
    m_imageLayer->resize(rc,m_gridAndAxisLayerId);
    m_imageLayer->resize(rc,m_chartLayerId);
    m_imageLayer->resize(rc,m_gradientLayerId);
    drawLine();
    drawGrid();
}

void BiddingChart::drawGrid()
{
    m_imageLayer->clear(m_gridAndAxisLayerId);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_gridAndAxisLayerId);
    QPainter painter(drawLayer.pix);
    QPen pen;
    pen.setColor(getGridClr());
    painter.setPen(pen);
    //绘制主图区域网格
    painter.drawLine(m_chartRc.topLeft(),m_chartRc.bottomLeft());
    painter.drawLine(m_chartRc.topRight(),m_chartRc.bottomRight());
    painter.drawLines(m_chartGridLines);
    //绘制成交量区域网格
    painter.drawRect(m_volumeRc);
    painter.drawLine(QPoint(m_volumeRc.left(),m_volumeRc.top()+m_volumeRc.height()/2),
                     QPoint(m_volumeRc.right(),m_volumeRc.top()+m_volumeRc.height()/2));
    for(int i=0;i < m_verPtxs.size();i++)
    {
        painter.drawLine(QPoint(m_verPtxs[i],m_chartRc.top()),
                         QPoint(m_verPtxs[i],m_chartRc.bottom()));

        painter.drawLine(QPoint(m_verPtxs[i],m_volumeRc.top()),
                         QPoint(m_verPtxs[i],m_volumeRc.bottom()));
    }
    //绘制时间轴
    pen.setColor(getTitleTextClr());
    painter.setPen(pen);
    painter.drawText(m_timeRc,Qt::AlignVCenter|Qt::AlignLeft,"9:15");
    painter.drawText(m_timeRc,Qt::AlignCenter,"9:20");
    painter.drawText(m_timeRc,Qt::AlignVCenter|Qt::AlignRight,"9:25");
    //绘制撮合量
    QRectF rc = m_volumeRc;
    rc.setLeft(0);
    rc.setTop(m_volumeRc.top());
    rc.setRight(m_volumeRc.left());
    painter.drawText(rc,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("撮合量"));
}

double yCanvasPTyToValue(const QRectF& rc,double pt_y,double min,double max,double ratio)
{
    double fRet = DBL_MAX;
    if(ratio != 0)
    {
        double fy = (double)rc.bottom()-pt_y;
        if(fy ==0)
        {
            fy = 1.0f;
        }
        fRet = min + fy*ratio;
    }
    return fRet;
}

int xCanvasIndexToPTx(const QRectF& rc,int index,double singleAreaWidth)
{
    int nRet = -1;
    int count = index + 1;
    nRet = rc.left() + count* singleAreaWidth - singleAreaWidth/2; //返回每根中间位置
    return nRet;
}

double yCanvasValueToPt(const QRectF& rc,double value,double min,double max,double ratio)
{      
    double fRet = 0;
    if(ratio != 0)
    {
        fRet = (value-min)/ratio;
        fRet = (double)rc.bottom() - fRet;
    }
    return fRet;
}

void BiddingChart::calcRc(double pos,double preClose,QColor& clr,HzQuoteFormat::QuoteItem& itemleft,
                          QRectF& leftRc,HzQuoteFormat::QuoteItem& itemright,QRectF& rightRc,double ratio)
{
    double tempVal = DBL_MAX;
    leftRc.setTop(pos -50);
    leftRc.setBottom(pos + 50);
    tempVal = yCanvasPTyToValue(m_chartRc,pos,m_data.minValue,m_data.maxValue,ratio);
    itemleft = HzQuoteFormat::QuoteFormat::formatPrice(tempVal,preClose);
    switch (itemleft.dirc) {
    case HzQuoteFormat::quote_up:
        clr = m_upClr;
        break;
    case HzQuoteFormat::quote_down:
        clr = m_downClr;
        break;
    case HzQuoteFormat::quote_equal:
    case HzQuoteFormat::quote_unknown:
        clr = m_equalClr;
        break;
    default:
        break;
    };
    //绘制右边
    itemright = HzQuoteFormat::QuoteFormat::formatChangeRate(tempVal,preClose);
    rightRc = rightRc;
    rightRc.setTop(pos-50);
    rightRc.setBottom(pos + 50);
}

void BiddingChart::volStick(QPainter &dc,double ratio,double singleAreaWidth)
{
    if(!m_data.data.isEmpty())
    {
        double preClose = m_data.data[0].pre_close_price;
        double val = 0;
        double xPos = m_volumeRc.left();
        QVector<QLineF> upVec,downVec;
        QPointF pt1,pt2;
        double top = m_volumeRc.top()+1;
        pt1.setY(m_volumeRc.bottom());
        for(int j = 0;j < m_data.data.size();j++)
        {
            val = m_data.data[j].volume;
            if(val != INVALID_DOUBLE && val != -1 && val != 0)
            {
                val = yCanvasValueToPt(m_volumeRc,val,m_data.volMinVal,m_data.volMaxVal,ratio);
                if(val == 0)
                    continue;
                else if(val < top)
                    val = top;
                xPos = m_volumeRc.left() +((double)j+1.0)*singleAreaWidth-singleAreaWidth/2.0;
                pt1.setX(xPos);
                pt2.setX(xPos);
                pt2.setY(val);
                if(m_data.data[j].bid_price < preClose)
                {
                    downVec.push_back(QLineF(pt1,pt2));
                }else
                {
                    upVec.push_back(QLineF(pt1,pt2));
                }
                preClose = m_data.data[j].bid_price;
            }
        }
        if(upVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(m_upClr);
            dc.setPen(pen);
            dc.drawLines(upVec);
        }
        if(downVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(m_downClr);
            dc.setPen(pen);
            dc.setBrush(QBrush(m_downClr));
            dc.drawLines(downVec);
        }
    }
}

void BiddingChart::drawLine()
{ 
    QRectF chartRc = m_chartRc;
    double singleAreaWidth = chartRc.width()/201.0;
    double preClose = DBL_MAX;
    double ratio =1.0f;
    QPen     pen;
    if(m_data.data.isEmpty() == false)
    {
        preClose = m_data.data[0].pre_close_price;
        ratio = (m_data.maxValue-m_data.minValue)/chartRc.height();
        if(ratio == 0)
            ratio = 1.0f;
    }
    m_imageLayer->clear(m_chartLayerId);
    m_imageLayer->clear(m_gradientLayerId);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_chartLayerId);
    ImgLayerInfo gradLayer =  m_imageLayer->getLayout(m_gradientLayerId);
    QPainter dc(drawLayer.pix);
    QPainter gradDc(gradLayer.pix);

    //绘制主图坐标轴
    QColor clr;
    QString strValue;
    QRectF leftRc(0,BiddWndToBottomMargin,BiddWndAxisWidth,m_chartRc.height());
    QRectF rightRc(m_chartRc.right(),BiddWndToBottomMargin,BiddWndAxisWidth,m_chartRc.height());
    //绘制价格和百分比标签
    HzQuoteFormat::QuoteItem itemleft;
    HzQuoteFormat::QuoteItem itemright;
    for(int i=0;i < m_chartGridLines.size();i++)
    {
        if(m_chartGridLines.size()/2 == i) //绘制中轴
        {
            //绘制左边轴
            pen.setColor(m_equalClr);
            dc.setPen(pen);
            strValue = HzQuoteFormat::QuoteFormat::numericFormatToString(preClose,2);
            dc.drawText(QRectF(0,m_chartGridLines[i].p1().y()-25,BiddWndAxisWidth,50),
                        Qt::AlignCenter,strValue);
            //绘制右边轴
            strValue = preClose==DBL_MAX?DEFAULT_STR_PRICE:"0.00%";
            dc.drawText(QRectF(m_chartGridLines[i].p2().x(),m_chartGridLines[i].p2().y()-25,BiddWndAxisWidth,50),
                        Qt::AlignCenter,strValue);
        }else
        {
            calcRc(m_chartGridLines[i].p1().y(),preClose,clr,itemleft,leftRc,itemright,rightRc,ratio);
            strValue  = itemright.strValue;
            //绘制左边上方向轴
            pen.setColor(clr);
            dc.setPen(pen);
            dc.drawText(leftRc,Qt::AlignCenter,itemleft.strValue);
            //绘制右边上方向轴
            dc.drawText(rightRc,Qt::AlignCenter,strValue);


            calcRc(m_chartGridLines[i].p1().y(),preClose,clr,itemleft,leftRc,itemright,rightRc,ratio);
            //绘制左边下方向轴
            pen.setColor(clr);
            dc.setPen(pen);
            dc.drawText(leftRc,Qt::AlignCenter,itemleft.strValue);
            //绘制右边下方向轴
            dc.drawText(rightRc,Qt::AlignCenter,strValue);
        }
    }

    if(preClose == DBL_MAX)
        return;
    QVector<QPointF> pts;
    QPainterPath path;
    double  minPty = DBL_MAX;
    double firstPtx = 0;
    QVector<QVector<QPointF>> lines;
    double xPos = 0;
    QPointF cPt;
    int dataNum = m_data.data.size();

    for(int i = 0;i < dataNum;i++)
    {
        if(m_data.data[i].bid_price == 0)
            m_data.data[i].bid_price = m_data.data[i].pre_close_price;
        xPos = chartRc.left()+((double)(i+1)*singleAreaWidth)-singleAreaWidth/2.0;
        cPt.setX(xPos);
        cPt.setY(chartRc.bottom() - (m_data.data[i].bid_price-m_data.minValue)/ratio);
        pts.push_back(cPt);
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
        if(dataNum  == 1) //防止一根数据画不出来的问题
        {
            cPt.setX(xPos+0.5);
            pts.push_back(cPt);
        }
        lines.push_back(pts);
    }
    pen.setColor(m_minLineClr);
    dc.setPen(pen);
    // 分时线
    dc.drawPolyline(pts.data(),pts.size());

    //透明渐变区域绘制
    if(minPty != DBL_MAX)
    {
       path.lineTo(cPt.x(),chartRc.bottom());
       path.lineTo(firstPtx,chartRc.bottom());
       chartRc.setTop(minPty);
    }
    QLinearGradient linearGradient(chartRc.bottomLeft(),chartRc.topLeft());
    linearGradient.setColorAt(0,QColor(230,71,71,1));
    linearGradient.setColorAt(1,QColor(230,71,71,60));
    linearGradient.setSpread(QGradient::PadSpread);
    gradDc.setRenderHints(QPainter::Antialiasing |
                     QPainter::SmoothPixmapTransform);
    gradDc.setPen(Qt::NoPen);
    gradDc.setRenderHint(QPainter::Antialiasing, true);
    gradDc.setBrush(linearGradient);
    gradDc.setClipPath(path);
    gradDc.drawRect(chartRc);

    //绘制成交量
    ratio = (m_data.volMaxVal-m_data.volMinVal)/m_volumeRc.height();
    if(ratio == 0)
        ratio = 1.0f;
    volStick(dc,ratio,singleAreaWidth);

    //绘制量
    pen.setColor(getTitleTextClr());
    dc.setPen(pen);

    double volume = DBL_MAX;
    if(!m_data.data.isEmpty())
        volume = m_data.data[m_data.data.size()-1].volume;
    HzQuoteFormat::QuoteItem item = HzQuoteFormat::QuoteFormat::formatVolume(volume);
    if(item.strValue != DEFAULT_STR_PRICE)
        item.strValue += QStringLiteral("手");
    QRectF rc = m_volumeRc;
    rc.setLeft(0);
    rc.setTop(m_volumeRc.top()+20);
    rc.setRight(m_volumeRc.left());
    dc.drawText(rc,Qt::AlignHCenter|Qt::AlignTop,item.strValue);
}

void BiddingChart::CalcRc(QRect rc)
{
    rc.setTop(rc.top()+BiddWndToBottomMargin);
    rc.setBottom(rc.bottom()-BiddWndToBottomMargin);
    rc.setLeft(rc.left()+BiddWndAxisWidth);
    rc.setRight(rc.right()-BiddWndAxisWidth);
    m_chartRc = rc;
    m_chartRc.setBottom(rc.bottom()-rc.height()/3); //成交量和时间轴区域为主图区域的1/3
    m_timeRc = m_chartRc;
    m_timeRc.setTop(m_timeRc.bottom());
    m_timeRc.setBottom(m_timeRc.top()+BiddWndTimeHight);
    m_volumeRc = rc;
    m_volumeRc.setTop(m_timeRc.bottom());

    //主图区域网格
    m_chartGridLines.clear();
    QPointF pt1,pt2;
    int height = m_chartRc.height()/6;
    pt1.setX(m_chartRc.left());
    pt2.setX(m_chartRc.right());
    m_chartGridLines.clear();
    for(int i =0;i < 6;i++)
    {
        pt1.setY(height*i+m_chartRc.top());
        pt2.setY(pt1.y());
        m_chartGridLines.push_back(QLineF(pt1,pt2));
    }
    m_chartGridLines.push_back(QLineF(m_chartRc.bottomLeft(),m_chartRc.bottomRight()));

    m_verPtxs.clear();
    int width = m_chartRc.width()/4;
    for(int i=1;i< 4;i++)
    {
        m_verPtxs.push_back(m_chartRc.left()+i*width);
    }
}

BiddingCanvas::BiddingCanvas(QWidget *parent)
    :BaseWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    m_pTitle = new BiddingTitle(this);
    m_pTitle->setFixedHeight(46);
    m_pTitle->setObjectName("bidtitle");
    m_pChart = new BiddingChart(this);
    pLayout->addWidget(m_pTitle);
    pLayout->addSpacing(1);
    pLayout->addWidget(m_pChart);
    this->setLayout(pLayout);
    m_pProvider = new BiddingProvider(this,this);
}

BiddingCanvas::~BiddingCanvas()
{

}

void BiddingCanvas::afterActive()
{
    if(!m_stockCode.isEmpty())
        m_pProvider->subData(m_stockCode);
}

void BiddingCanvas::afterDeactive()
{
    m_pProvider->cancle();
}

void BiddingCanvas::onDataRefresh(BiddingData *pData)
{
    if(pData && pData->data.isEmpty() == false)
        m_pTitle->updateData(pData->data[pData->data.size()-1]);
    else
        m_pTitle->updateData(BiddingItem());
    m_pChart->updateData(pData);
}

BiddingStyle::BiddingStyle(QWidget *parent)
    :BaseWidget(parent)
{

}
