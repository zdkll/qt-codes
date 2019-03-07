#include "fundflowbarchart.h"

#define  TableView_Row_Height  22

#include <QPainter>
#include <QColor>
#include <QDebug>

#include "quoteformat.h"

FundFlowBarChart::FundFlowBarChart(QWidget *parent)
    :FundBaseChart(parent),m_data(0)
    ,m_charFont("SimSun"),m_digitalFont("Arial")
{
    m_charFont.setPixelSize(12);
    m_digitalFont.setBold(true);
    m_digitalFont.setStyleStrategy(QFont::NoAntialias);

    m_digitalFont.setPixelSize(13);

    initData();
}

void FundFlowBarChart::setData(FundInOutData *data)
{
    m_data = data;
    updateData(m_data);

    draw();
}


void FundFlowBarChart::drawPixmap(QPixmap *pix)
{
    pix->fill(Qt::transparent);
    QPainter pt(pix);

    int txtCharHeight = QFontMetrics(m_charFont).height();
    int txtDigitalHeight = QFontMetrics(m_digitalFont).height();

    //绘制Title
    pt.setFont(m_charFont);
    pt.setPen(textColor());
    pt.drawText(41,11+txtCharHeight,QString(QStringLiteral("资金流向(单位：%1)")).arg(m_barItems[0].unit));

    //绘制Bar
    pt.save();
    int barGraphWidth  = m_size.width()-56;//41+15
    int barGraphHeight = m_size.height()/2 -txtCharHeight*2-31-txtDigitalHeight;
    pt.translate(41,m_size.height()/2 -txtCharHeight-10);   //移到坐标原点
    drawBar(&pt,barGraphWidth,barGraphHeight,5);
    pt.restore();

    //绘制图表
    pt.save();
    int tableWidth = m_size.width()-56;
    int tableHeight = 4*TableView_Row_Height + 5;
    pt.translate(41,m_size.height()/2 + 5);
    drawTableView(&pt,tableWidth,tableHeight);
    pt.restore();

    pt.end();
}

void FundFlowBarChart::drawBar(QPainter *pt,int width,int height ,int space)
{
    pt->setPen(lineColor());
    pt->drawLine(0,0,width,0);

    //计算间隔
    int  offset = 5;
    int  barWidth = 40,barHeight;
    float  distance = (width - 2*offset -barWidth*3)/2.f;
    QFontMetrics charMetrics(m_charFont);
    QFontMetrics digitalMetrics(m_digitalFont);

    m_data->maxTotalFund = m_data->maxTotalFund<1?1:m_data->maxTotalFund;

    QString text;
    int x,txtWidth;
    for(int i=0;i<3;i++){
        text = m_barItems[i].label;
        barHeight = m_barItems[i].ratio*height;
        x = offset+i*(distance+barWidth);

        pt->setPen(Qt::NoPen);
        pt->setBrush(m_barItems[i].color);
        pt->drawRect(x,0,barWidth,-barHeight);

        //值
        txtWidth = digitalMetrics.width(m_barItems[i].value);
        pt->setPen(m_barItems[i].color);
        pt->setFont(m_digitalFont);
        pt->drawText(x-(txtWidth-barWidth)/2,-barHeight-space-digitalMetrics.height()
                     ,txtWidth,digitalMetrics.height(),Qt::AlignCenter,m_barItems[i].value);

        //label
        txtWidth = charMetrics.width(m_barItems[i].label);
        pt->setPen(textColor());
        pt->setFont(m_charFont);
        pt->drawText(x-(txtWidth-barWidth)/2,5
                     ,txtWidth,charMetrics.height(),Qt::AlignCenter,m_barItems[i].label);
    }
}

void FundFlowBarChart::drawTableView(QPainter *pt,int width,int height)
{
    QFontMetrics charMetrics(m_charFont);
//    QFontMetrics digitalMetrics(m_digitalFont);

    pt->setPen(lineColor());
    pt->drawRect(0,0,width,height);

    //绘制表格
   // pt->drawLine(width/2,0,width/2,height);
    int lineWidth = 1;
    int y;
    for(int i=0;i<3;i++){
        y=(i+1)*(lineWidth+TableView_Row_Height);
        pt->drawLine(0,y,width,y);
    }

    //绘制表头
    pt->setPen(textColor());
    pt->setFont(m_charFont);

    for(int i=0;i<4;i++){
        y=(i+1)*(lineWidth+TableView_Row_Height);
        pt->drawText(-charMetrics.width(m_tableItems[i].label)-5,y,m_tableItems[i].label);
    }

    //绘制表格内容和数值
    pt->setFont(m_digitalFont);
    int rectWidth;
    for(int i=0;i<4;i++){
        pt->setPen(m_tableItems[i].color);
        y= lineWidth + i*(lineWidth+TableView_Row_Height)+1;
        pt->drawText(QRect(0,y,width/2-5,TableView_Row_Height) ,Qt::AlignRight|Qt::AlignVCenter
                     ,m_tableItems[i].value);
        pt->setBrush(m_tableItems[i].color);

        rectWidth = m_tableItems[i].ratio*(width/2-1);
        if(rectWidth<1) continue;
        pt->drawRect(QRect(width/2,y+2,rectWidth,TableView_Row_Height-6)  );
    }
}

void  FundFlowBarChart::initData()
{
    m_barItems[0].label = QStringLiteral("净流入");
    m_barItems[1].label = QStringLiteral("流入");
    m_barItems[2].label = QStringLiteral("流出");

    m_tableItems[0].label = QStringLiteral("超大单");
    m_tableItems[1].label = QStringLiteral("大单");
    m_tableItems[2].label = QStringLiteral("中单");
    m_tableItems[3].label = QStringLiteral("小单");
}

void  FundFlowBarChart::updateData(FundInOutData *data)
{
    long long max = data->maxTotalFund>0?data->maxTotalFund:1;

    //bar=========================================
    //计算单位
    HzQuoteFormat::UnitType  type = HzQuoteFormat::calUnitByValue(max);
    QString unit_str = HzQuoteFormat::unitString(type);

    //净流入
    m_barItems[0].unit   = unit_str ;
    m_barItems[0].value = HzQuoteFormat::formatByUnitType(data->totalNetput,type);
    m_barItems[0].ratio  = double(abs(data->totalNetput))/double(max);
    m_barItems[0].color  = data->totalNetput>0?inColor():data->totalNetput<0?outColor():textColor();

    //流入
    m_barItems[1].unit   = unit_str;
    m_barItems[1].value = HzQuoteFormat::formatByUnitType(data->totalInput,type);
    m_barItems[1].ratio  = double(data->totalInput)/double(max);
    m_barItems[1].color  = data->totalInput>0?inColor():textColor();

    //流出
    m_barItems[2].unit   = unit_str;
    m_barItems[2].value = "-"+ HzQuoteFormat::formatByUnitType(data->totalOutput,type);
    m_barItems[2].ratio  = double(data->totalOutput)/double(max);
    m_barItems[2].color = data->totalOutput>0?outColor():textColor();

    //tableview=========================================================
    max = data->maxNetput>0?data->maxNetput:1;
    type = HzQuoteFormat::calUnitByValue(max);
    unit_str = HzQuoteFormat::unitString(type);

    long long value;
    for(int i=0;i<4;i++){
        value = data->fundOrders[i].netput;
        m_tableItems[i].unit = unit_str;
        calFunItem(m_tableItems[i], value,max,type);
    }
}

void FundFlowBarChart::calFunItem(FundItem &item,long long value,long long max,HzQuoteFormat::UnitType type)
{
    item.value = HzQuoteFormat::formatByUnitType(value,type)+HzQuoteFormat::unitString(type);
    item.value.chop(1);
    item.ratio  = double(abs(value))/double(max);

    item.color  = value>0?inColor():value<0?outColor():textColor();
}

