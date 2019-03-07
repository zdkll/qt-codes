#include "fundflowpiechart.h"

#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QToolTip>
#include <QMouseEvent>

#include "quoteformat.h"

#define PIE_PI   3.14159f

FundFlowPieChart::FundFlowPieChart(QWidget *parent)
    :FundBaseChart(parent),m_data(0)
    ,m_pieCharFont("Arial"),m_showTip(false)
    ,m_titleFont("SimSun")
    ,m_titleNumFont("Arial")
{
    this->setMouseTracking(true);

    m_pieCharFont.setPixelSize(11);
    m_titleFont.setPixelSize(14);
    m_titleNumFont.setPixelSize(13);
    m_titleNumFont.setBold(true);
    m_titleNumFont.setStyleStrategy(QFont::NoAntialias);

    m_pieItems[0].label = QStringLiteral("超大单流入");
    m_pieItems[1].label = QStringLiteral("超大单流出");
    m_pieItems[2].label = QStringLiteral("大单流入");
    m_pieItems[3].label = QStringLiteral("大单流出");
    m_pieItems[4].label = QStringLiteral("中单流入");
    m_pieItems[5].label = QStringLiteral("中单流出");
    m_pieItems[6].label = QStringLiteral("小单流入");
    m_pieItems[7].label = QStringLiteral("小单流出");

    //圆心位置
    m_pieCenter = QPoint(55,100);
}



void FundFlowPieChart::setData(FundInOutData *data)
{
    m_data = data;

    updateData(data);

    draw();
}

void FundFlowPieChart::drawPixmap(QPixmap *pix)
{
    pix->fill(Qt::transparent);
    QPainter pt(pix);

    //绘制Legend
    drawLeaderNetput(&pt);

    pt.translate(15,60);

    //绘制饼状图
    drawInOut(&pt);

    pt.end();
}

void FundFlowPieChart::drawLeaderNetput(QPainter *pt)
{
    pt->setFont(m_titleFont);
    pt->setPen(titleColor());

    int txtHeight = QFontMetrics(m_titleFont).height();

    pt->drawText(15,11+txtHeight,QStringLiteral("主力净流向"));

    pt->setFont(m_titleNumFont);

    QColor color = titleColor();
    QString preffix ;
    if(m_data->leaderNetput>0)
        color = riseColor();
    else if(m_data->leaderNetput<0){
        preffix = "-";
        color = fallColor();
    }
    long long leaderNetput = m_data->leaderNetput;
    QString text = preffix +HzQuoteFormat::StringUnitFormat::toFormatString(abs(leaderNetput));
    pt->setPen(color);
    pt->drawText(15,16+2*txtHeight,text);
}

void FundFlowPieChart::drawInOut(QPainter *pt)
{
    //总的资金流向
    if(m_data->totalInOut == 0){
        m_showTip = false;
        return;
    }
    m_showTip = true;

    int   startAngle = 0,spanAngle;
    QRect rect(0,0,80,80);
    pt->setPen(lineColor());

    pt->setRenderHint(QPainter::Antialiasing);
    int  ratio;
    int txtAngle;
    for(int i=0;i<8;i++){
        spanAngle = m_pieItems[i].ratio*360*16;
        pt->setBrush(m_pieItems[i].color);
        pt->drawPie(rect,startAngle,spanAngle);
        startAngle += spanAngle;
    }

    //绘制标识
    startAngle = 0;
    //移到圆心
    pt->translate(40,40);
    float x,y;
    pt->setPen(percentColor());
    pt->setFont(m_pieCharFont);
    QFontMetrics fontMetrics(m_pieCharFont);
    int  txtwidth;
    int  txtHeight = fontMetrics.height();
    QString text;
    for(int i=0;i<8;i++){
        //百分比大于20才绘制
        spanAngle = m_pieItems[i].ratio*360;
        ratio = m_pieItems[i].ratio*100;
        if(ratio>=12){
            text = QString::number(ratio)+"%";
            txtwidth = fontMetrics.width(text);

            txtAngle = startAngle + spanAngle/2;
            x =  15*std::cos(txtAngle/180.f*PIE_PI);
            y = -15*std::sin(txtAngle/180.f*PIE_PI);

            x += (x-15.f)/30.f*txtwidth;
            y +=(y+15.f)/30.f*txtHeight;

            pt->drawText(x,y,text);
        }
        startAngle += spanAngle;
    }
}


void FundFlowPieChart::showTip(const QPoint &pt)
{
    //计算角度
    QPointF disPt = pt - m_pieCenter;
    double length = sqrt(pow(disPt.x(),2)+pow(disPt.y(),2));
    double angle = 180.f*std::acos(disPt.x()/length)/PIE_PI;
    angle = disPt.y()<=0?angle:360.f-angle;
    //获取对应的成分
    int i  = 0;
    int start_angle = 0;
    int end_angle = 0;
    for(;i<8;i++){
        end_angle = start_angle+m_pieItems[i].ratio*360;
        if(angle>=start_angle && angle<end_angle)
            break;
        start_angle += m_pieItems[i].ratio*360;
    }
    i = i==8?7:i;

    //显示Info窗口
    this->setToolTip(QString("%1:%2").arg(m_pieItems[i].label).arg(m_pieItems[i].value));
}

void FundFlowPieChart::mouseMoveEvent(QMouseEvent *e)
{
    QPoint disPt = e->pos() - m_pieCenter;

    int length = sqrt(pow(disPt.x(),2)+pow(disPt.y(),2));
    if(length<=40 && m_showTip){
        showTip(e->pos());
    }else{
        this->setToolTip("");
    }
    return QWidget::mouseMoveEvent(e);
}

void FundFlowPieChart::updateData(FundInOutData *data)
{
    long long max = data->totalInOut;
    max = max>0?max:1;

    for(int i=0;i<4;i++){
        //流入
        m_pieItems[i*2].ratio  = double(data->fundOrders[i].input)/double(max);
        m_pieItems[i*2].value = HzQuoteFormat::StringUnitFormat::toFormatString(data->fundOrders[i].input);
        m_pieItems[i*2].color =  inColor();
        //流出
        m_pieItems[i*2+1].ratio = double(data->fundOrders[i].output)/double(max);
        m_pieItems[i*2+1].value= HzQuoteFormat::StringUnitFormat::toFormatString(data->fundOrders[i].output);
        m_pieItems[i*2+1].color = outColor();
    }
}


