#include "risefallbarchart.h"

#include <QEvent>
#include <QPainter>

QColor riseColor = QColor("#f22f2f");
QColor plainColor = QColor("#adadad");
QColor fallColor = QColor("#0aaa38");

QColor textColor = QColor("#adadad");
QColor gridColor = QColor("#212121");

BaseRiseFallChart::BaseRiseFallChart(QObject *parent)
    :QObject(parent),m_paintAction(DRAW)
{
    m_txtfont.setFamily("SimSun");
    m_txtfont.setPixelSize(12);
}

void BaseRiseFallChart::initial(QWidget *wgRect)
{
    m_wgRect = wgRect;
    wgRect->installEventFilter(this);
}

void BaseRiseFallChart::setData(const HZData::StockRiseFallInfo &risefallInfo)
{
    calData(risefallInfo);
    draw();
}

void BaseRiseFallChart::draw()
{
    m_paintAction =  DRAW;
    m_wgRect->update();
}
void BaseRiseFallChart::objDraw()
{
    m_paintAction = OBJDRAW;
    m_wgRect->update();
}

void BaseRiseFallChart::reDraw()
{
    m_paintAction = REDRAW;
    m_wgRect->update();
}

bool BaseRiseFallChart::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Paint && obj == m_wgRect)
        paintOnWidget(m_wgRect);

    return QObject::eventFilter(obj,event);
}

void BaseRiseFallChart::paintOnWidget(QWidget *wg)
{
    if(m_graphSize  != wg->size()){
        m_paintAction = DRAW;
        m_graphSize = wg->size();
    }

    if( m_paintAction == DRAW){
        m_pixmap = QPixmap(m_graphSize);
        m_pixmap.fill(Qt::transparent);
        drawPixmap(&m_pixmap);
        m_paintAction = REDRAW;
    }

    if(m_paintAction != NODRAW){
        QPainter pt(wg);
        pt.drawPixmap(0,0,m_pixmap);
        pt.end();
    }
}


///RiseFallBarChart
RiseFallBarChart::RiseFallBarChart(QObject *parent)
    : BaseRiseFallChart(parent)
{
    initData();

}

void RiseFallBarChart::drawPixmap(QPixmap *pix)
{
    QPainter pt(pix);

    QFontMetrics fontMetrics(m_txtfont);
    //计算横向刻度
    int offset_x = m_wgRect->width()*float(25)/float(609);
    int offset_y = m_graphSize.height()*float(20)/float(315)+ fontMetrics.height() + CHART_SPACING+1;

    //先计算纵向范围及值
    int maxValue = m_risefallBarData.maxValue;
    //图像最大空间
    int graphHeight = m_graphSize.height() - fontMetrics.height() - 5  - offset_y;
    //刻度区空间
    int scaleHeight  = m_graphSize.height() - fontMetrics.height() -CHART_SPACING - offset_y;
    //计算整个刻度区对应的值
    int scaleMaxValue = maxValue*float(scaleHeight)/float(graphHeight);
    //计算垂直刻度
    YScaleUnit YscaleUnit = getYScaleUnit(scaleMaxValue,scaleHeight);

    //qDebug()<<"maxYvalue:"<<YscaleUnit.max<<" scale height:"<<scaleHeight
    //           <<"max vlaue:"<<maxValue<<" max graphheight:"<<graphHeight;


    RiseFallBarSet   barSet = m_risefallBarData.barSet;
    int graphWidth = m_graphSize.width() - CHART_VERT_SCALE_WIDTH-offset_x*2;
    XScaleUnit XscaleUnit   = getXScaleUnit(barSet.size(),graphWidth,CHART_BAR_WIDTH);

    //移到左下角
    pt.translate(offset_x,m_graphSize.height() - offset_y);

    //绘制垂直坐标
    drawVertScale(&pt,YscaleUnit,fontMetrics.height(),graphWidth);

    //绘制条形图和水平坐标
    drawBar(&pt,XscaleUnit,barSet,YscaleUnit.max,scaleHeight,fontMetrics.height());

    pt.end();
}

void RiseFallBarChart::drawBar(QPainter *pt,const XScaleUnit &scaleUnit,const RiseFallBarSet &barSet,int maxValue,int totalHeihgt,int txtHeight)
{
    //步长
    float length = scaleUnit.width + scaleUnit.distance;
    float x,height;
    QLinearGradient gradient;
    QColor                color;
    for(int i=0;i<barSet.size();i++){
        x = length*i;
        height = qRound(-totalHeihgt*float(barSet[i].value)/float(maxValue));

        color = barSet[i].color;
        color.setAlphaF(barSet[i].minAlpha);
        gradient.setColorAt(0,color);
        color.setAlphaF(barSet[i].maxAlpha);
        gradient.setColorAt(1,color);

        gradient.setStart(QPoint(x,0));
        gradient.setFinalStop(QPoint(x,height));

        //绘制条形
        pt->setPen(Qt::NoPen);
        pt->setBrush(gradient);
        pt->drawRect(x,0,scaleUnit.width, height);

        //绘制顶部值
        pt->setPen(barSet[i].color);
        pt->drawText(QRect(x,height-5,scaleUnit.width,-txtHeight),Qt::AlignCenter
                     ,QString::number(barSet[i].value));

        //绘制底部刻度
        pt->setPen(plainColor);
        pt->drawText(QRect(x, CHART_SPACING + txtHeight-1,scaleUnit.width,-txtHeight),Qt::AlignCenter
                     , barSet[i].label);
    }
}

void RiseFallBarChart::drawVertScale(QPainter *pt,YScaleUnit YscaleUnit,int txtHeight,int graphWidth)
{
    int   halfTxtHeight = txtHeight/2;

    float y;
    for(int i=0;i<=YscaleUnit.num;i++){
        pt->setPen(gridColor);
        y = -YscaleUnit.distance*i;
        pt->drawLine(0,y,graphWidth,y);

        pt->setPen(plainColor);
        pt->drawText(graphWidth+CHART_SPACING,-YscaleUnit.distance*i+halfTxtHeight
                     ,QString::number(YscaleUnit.dvalue*i));
    }
}

RiseFallBarChart::XScaleUnit RiseFallBarChart::getXScaleUnit(int size,int totalWidth,int barWidth,int minDis)
{
    float width  =  barWidth;
    float distance = float(totalWidth - barWidth*size)/float(size-1);

    //至少保证一个像素间距
    if(distance<minDis){
        width = (totalWidth - size +1)/size;
        distance = float(totalWidth - width*size)/float(size-1);
    }
    return XScaleUnit(distance,width);
}

RiseFallBarChart::YScaleUnit RiseFallBarChart::getYScaleUnit(int max,int totalHeight,int height)
{
    YScaleUnit unit;
    if(max == 0){
        unit.num = 0;
        return unit;
    }
    int ticks = qRound(float(totalHeight)/float(height));

    //计算一个合适的最大值
    float corStep = float(max- 0)/float(ticks);
    //步长的数量级
    int temp  = ceil(log10(corStep));

    //步长规范到 0-1
    float newCorStep = corStep/pow(10,temp);

    //向上一位取整
    float temp1 = pow(10,temp-1);

    unit.dvalue   = ceil(newCorStep*temp1)*10;

    unit.num      =  ceil(float(max)/unit.dvalue);
    //修正最大值
    unit.max      =  unit.num*unit.dvalue;
    unit.distance= float(unit.dvalue)/float(unit.max)*totalHeight;
    return unit;
}


void RiseFallBarChart::calData(const HZData::StockRiseFallInfo &risefallInfo)
{
    //Barset
    int max1 = 0;
    int max2 = 0;
    m_risefallBarData.barSet[0].value = risefallInfo.limit_up_num;
    max1 = qMax<int>(max1,risefallInfo.limit_up_num);
    m_risefallBarData.barSet[1].value = risefallInfo.greater_seven;
    max1 = qMax<int>(max1,risefallInfo.greater_seven);
    m_risefallBarData.barSet[2].value = risefallInfo.up_five_seven;
    max1 = qMax<int>(max1,risefallInfo.up_five_seven);
    m_risefallBarData.barSet[3].value = risefallInfo.up_two_five;
    max1 = qMax<int>(max1,risefallInfo.up_two_five);
    m_risefallBarData.barSet[4].value = risefallInfo.up_zero_two;
    max1 = qMax<int>(max1,risefallInfo.up_zero_two);
    for(int i=0;i<5;i++)
        m_risefallBarData.barSet[i].maxAlpha = m_risefallBarData.barSet[i].minAlpha
                + 0.8*float(m_risefallBarData.barSet[i].value)/float(max1);

    m_risefallBarData.barSet[5].value = risefallInfo.flat_num;

    m_risefallBarData.barSet[6].value = risefallInfo.down_zero_two;
    max2 = qMax<int>(max2,risefallInfo.down_zero_two);
    m_risefallBarData.barSet[7].value = risefallInfo.down_two_five;
    max2 = qMax<int>(max2,risefallInfo.down_two_five);
    m_risefallBarData.barSet[8].value = risefallInfo.down_five_seven;
    max2 = qMax<int>(max2,risefallInfo.down_five_seven);
    m_risefallBarData.barSet[9].value = risefallInfo.less_seven;
    max2 = qMax<int>(max2,risefallInfo.less_seven);
    m_risefallBarData.barSet[10].value = risefallInfo.limit_down_num;
    max2 = qMax<int>(max2,risefallInfo.limit_down_num);
    for(int i=6;i<11;i++)
        m_risefallBarData.barSet[i].maxAlpha = m_risefallBarData.barSet[i].minAlpha +
                0.8*float(m_risefallBarData.barSet[i].value)/float(max2);

    m_risefallBarData.maxValue = qMax<int>(std::max(max1,max2),risefallInfo.flat_num);
}

void RiseFallBarChart::initData()
{
    RiseFallBarSet barset(11);
    //label
    QStringList barList={QStringLiteral("涨停"),">7","5-7","2-5","0-2","0","2-0","5-2","7-5",">7",QStringLiteral("跌停")};

    //bar set
    for(int i=0;i<barList.size();i++){
        barset[i].label = barList[i];
        if(i<5)
            barset[i].color = riseColor;
        else if(i == 5)
            barset[i].color = plainColor;
        else
            barset[i].color = fallColor;
    }

    m_risefallBarData.barSet = std::move(barset);
}




///RiseFallLegendChart-----------
RiseFallLegendChart::RiseFallLegendChart(QObject *parent)
    :BaseRiseFallChart(parent)
{
    initData();
}

void RiseFallLegendChart::drawPixmap(QPixmap *pix)
{
    QPainter pt(pix);
    QFontMetrics fontMetrics(m_txtfont);

#define Graph_Origin_Height 315
#define Graph_Origin_Width 609

    pt.setFont(m_txtfont);
    int offset_y = m_wgRect->height()*float(16)/float(Graph_Origin_Height);
    int rectY = (m_graphSize.height() - LEGEND_RECT_WIDTH-1)/2+offset_y;
    int textY = (m_graphSize.height() + fontMetrics.height()-1)/2+offset_y;
    //空白起始宽度
    int x = m_wgRect->width()*float(25)/float(Graph_Origin_Width);
    for(int i=0;i<m_riseFallLegendSet.size();i++){
        //rect
        pt.setPen(Qt::NoPen);
        pt.setBrush(m_riseFallLegendSet[i].color);
        pt.drawRect(x,rectY,LEGEND_RECT_WIDTH,LEGEND_RECT_WIDTH);

        //text
        x  += LEGEND_RECT_WIDTH +m_wgRect->width()*6.f/float(Graph_Origin_Width);
        pt.setPen(plainColor);
        pt.drawText(x ,textY,m_riseFallLegendSet[i].label);

        //value
        x  += fontMetrics.width(m_riseFallLegendSet[i].label)-2;
        pt.setPen(m_riseFallLegendSet[i].color);
        QString text = QString::number(m_riseFallLegendSet[i].value);
        pt.drawText(x ,textY,text);
        x  += fontMetrics.width(text) + m_wgRect->width()*12.f/float(Graph_Origin_Width);;
    }
    pt.end();
}

void RiseFallLegendChart::calData(const HZData::StockRiseFallInfo &risefallInfo)
{
    //legend
    m_riseFallLegendSet[0].value = risefallInfo.up_num;
    m_riseFallLegendSet[1].value = risefallInfo.down_num;
    m_riseFallLegendSet[2].value = risefallInfo.flat_num;
    m_riseFallLegendSet[3].value = risefallInfo.limit_up_num;
    m_riseFallLegendSet[4].value = risefallInfo.limit_down_num;
    m_riseFallLegendSet[5].value = risefallInfo.stop_num;

}

void RiseFallLegendChart::initData()
{
    m_riseFallLegendSet.resize(6);
    QStringList legendList = {QStringLiteral("上涨："),QStringLiteral("下跌："),
                              QStringLiteral("平家："),QStringLiteral("涨停："),
                              QStringLiteral("跌停："),QStringLiteral("停牌：")};
    //legendset
    for(int i=0;i<legendList.size();i++)
        m_riseFallLegendSet[i].label = legendList[i];

    m_riseFallLegendSet[0].color = riseColor;
    m_riseFallLegendSet[1].color = fallColor;
    m_riseFallLegendSet[2].color = plainColor;
    m_riseFallLegendSet[3].color = riseColor;
    m_riseFallLegendSet[4].color = fallColor;
    m_riseFallLegendSet[5].color = plainColor;
}



