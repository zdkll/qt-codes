#include "leaderincdecchart.h"

#define  Graph_Margin_Vertical      5
#define  Graph_Margin_Horizontal  14

#define  Graph_Spacing                  5

#include <QDate>
#include <QPainter>
#include <QDebug>

#include "quoteformat.h"

bool oppositeSigns(long long in1,long long in2);


LeaderIncDecChart::LeaderIncDecChart(QWidget *parent)
    :FundBaseChart(parent)
    ,m_font("Arial"),m_interData(NULL)
    ,m_d(NULL)
{
    m_font.setPixelSize(11);
    m_font.setStyleStrategy(QFont::NoAntialias);
}

LeaderIncDecChart::~LeaderIncDecChart()
{
    if(m_d){
        delete m_d;
        m_d = NULL;
    }
}

QString LeaderIncDecChart::setData(const HZData::IncrDecPostionInfo& data)
{
    if(m_d ==  NULL)
        m_d = new IncrDecPosData;

    //统计最大值正数，最小值负值
    INT64 max =0,min = 0;

    int size = data.size();
    for(int i=0;i<size;i++){
        if(data[i].money>max){
            max = data[i].money;
            m_d->max_index = i;
        }
        if(data[i].money<min){
            min = data[i].money;
            m_d->min_index = i;
        }
    }

    m_d->incDecPos = std::move(data);
    m_d->min  = min;
    m_d->max = max;

    long long  maxValue = std::max(abs(max),abs(min));
    HzQuoteFormat::UnitType unitType = HzQuoteFormat::calUnitByValue(maxValue);
    m_d->uintType = unitType;

    draw();

    return HzQuoteFormat::unitString(unitType);
}


void LeaderIncDecChart::clearData()
{
    if(m_d){
        delete m_d;m_d = NULL;
    }
}

void LeaderIncDecChart::drawPixmap(QPixmap *pixmap)
{
    pixmap->fill(Qt::transparent);

    int txtHeight      = QFontMetrics(m_font).height();
    int graphHeight = m_size.height()/2 - Graph_Margin_Vertical  - txtHeight - Graph_Spacing;
    int graphWidth  = m_size.width()-Graph_Margin_Horizontal*2;

    QPainter pt(pixmap);

    //绘制坐标线
    pt.translate(Graph_Margin_Horizontal,m_size.height()/2);

    pt.setPen(midColor());
    pt.drawLine(0,0,graphWidth,0);

    if(m_d == NULL) return;

    if(m_d->incDecPos.size()<2)  return;

    //绘制-移到坐标原点
    //填充渐变颜色
    paintGrap(&pt,graphWidth,graphHeight);
    //绘制线
    pt.drawPixmap(0,-graphHeight,createLinePix(graphWidth,graphHeight));

    //绘制时间和金额
    paintDate(&pt,graphWidth,graphHeight);

    pt.end();
}

void LeaderIncDecChart::paintGrap(QPainter *pt,int width,int height)
{
    int num = m_d->incDecPos.size();

    long long max = std::max(abs(m_d->max),abs(m_d->min));
    if(max<1)
        return;

    float x_space = num ==1?width:float(width)/float(num-1);

    //计算填充剪切Path
    QPainterPath clip_path;
    clip_path.moveTo(0,0);
    float x,y;
    for(int i=0;i<num;i++){
        //坐标
        x = qRound(i*x_space);
        y = -qRound(height*double(m_d->incDecPos[i].money)/double(max));
        clip_path.lineTo(x,y);
    }
    clip_path.lineTo(width,0);
    clip_path.closeSubpath();

    //绘制上半部分
    pt->setPen(Qt::NoPen);
    QLinearGradient up_gradient;
    QColor color_rise = riseMaxColor();
    color_rise.setAlphaF(0.1);
    up_gradient.setColorAt(0,color_rise);//
    color_rise.setAlphaF(0.3);
    up_gradient.setColorAt(1,color_rise);
    up_gradient.setStart(0,0);
    up_gradient.setFinalStop(0,-double(abs(m_d->max))/double(max)*height );

    QPainterPath rect_path1;
    rect_path1.addRect(0,0,width,-double(abs(m_d->max))/double(max)*height);

    pt->fillPath(clip_path.intersected(rect_path1),up_gradient);

    //绘制下半部分
    QLinearGradient down_gradient;
    QColor color_fall = fallMaxColor();
    color_fall.setAlphaF(0.1);
    down_gradient.setColorAt(0,color_fall);
    color_fall.setAlphaF(0.3);
    down_gradient.setColorAt(1,color_fall);

    down_gradient.setStart(0,0);
    down_gradient.setFinalStop(0,double(abs(m_d->min))/double(max)*height);

    QPainterPath rect_path2;
    rect_path2.addRect(0,0,width,double(abs(m_d->min))/double(max)*height);

    pt->fillPath(clip_path.intersected(rect_path2),down_gradient);
}


QPixmap  LeaderIncDecChart::createLinePix(int width,int height)
{
    int num = m_d->incDecPos.size();

    long long max = std::max(abs(m_d->max),abs(m_d->min));

    QPixmap pix(width,2*height);
    pix.fill(Qt::transparent);

    QPixmap up_pix(width,height);
    up_pix.fill(riseColor());
    QPixmap down_pix(width,height );
    down_pix.fill(fallColor());

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing,true);
    p.translate(0,height);

    float x_space = num ==1?width:float(width)/float(num-1);
    QPoint points[30];
    for(int i=0;i<num;i++){
        //坐标
        points[i].setX(i*x_space);
        points[i].setY(-height*double(m_d->incDecPos[i].money)/double(max));
    }
    p.drawPolyline(points,num);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    p.drawPixmap(0,-height ,width,height,up_pix);
    p.drawPixmap(0,1,width,height,down_pix);
    p.end();

    return pix;
}


void LeaderIncDecChart::paintDate(QPainter *pt,int width,int height)
{
    int num = m_d->incDecPos.size();
    if(num<1)
        return;
    long long max = std::max(abs(m_d->max),abs(m_d->min));
    if(max<1)
        return;

    float x_space = num ==1?width:float(width)/float(num-1);

    QFontMetrics fontMetrics(m_font);
    int txtHeight = fontMetrics.height();
    QString text;
    pt->setFont(m_font);
    int y = 0;
    QString dateStr ;
    for(int i= num-3;i>=0;i -= 5){
        dateStr =  QString::number(m_d->incDecPos[i].date);
        text  = QString("%1-%2").arg(dateStr.mid(4,2)).arg(dateStr.mid(6));

        pt->setPen(textColor());
        pt->drawText(x_space*i-fontMetrics.width(text)/2,txtHeight,text);

        text =  HzQuoteFormat::formatByUnitType(m_d->incDecPos[i].money,m_d->uintType,1);
        if(m_d->incDecPos[i].money>=0){
            y = -height - Graph_Spacing;
            pt->setPen(riseColor());
            text = "+"+text;
        }else{
            y = height +Graph_Spacing ;
            pt->setPen(fallColor());
        }
        pt->drawText(x_space*i-fontMetrics.width(text)/2,y,text);
    }
}

bool oppositeSigns(long long in1,long long in2)
{
    if((in1>0 && in2<0) || (in1<0 && in2>0))
        return true;
    return false;
}
