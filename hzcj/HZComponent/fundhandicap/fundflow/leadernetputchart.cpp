#include "leadernetputchart.h"

#include <QPainter>
#include "quoteformat.h"

LeaderNetputChart::LeaderNetputChart(QWidget *parent)
    :FundBaseChart(parent),m_charFont("SimSun")
    ,m_digitalFont("Arial")
{
    m_data[0].label = QStringLiteral("净流入");
    m_data[1].label = QStringLiteral("3日净流入");
    m_data[2].label = QStringLiteral("5日净流入");
    m_data[3].label = QStringLiteral("10日净流入");
    m_charFont.setPixelSize(12);
    m_digitalFont.setBold(true);
    m_digitalFont.setStyleStrategy(QFont::NoAntialias);
    m_digitalFont.setPixelSize(13);
}

void LeaderNetputChart::setData(const QVector<long long> &netputs)
{
    for(int i=0;i<4;i++)
        m_data[i].netput = netputs[i];

    draw();
}

void LeaderNetputChart::clearData()
{
    for(int i=0;i<4;i++)
        m_data[i].netput = INVALID_INT64;

    draw();
}

void LeaderNetputChart::drawPixmap(QPixmap *pix)
{
    pix->fill(Qt::transparent);

    //绘制分割线
    QPainter pt(pix);
    paintVertLine(&pt);
}

void LeaderNetputChart::paintVertLine(QPainter *pt)
{
    float space =  float(m_size.width())/4.0f;
    pt->setPen(Qt::black);
    for(int i=1;i<4;i++)
        pt->drawLine(space*i,5,space*i,m_size.height()-5);

    int height = m_size.height() - 5*2;
    //绘制条目
    pt->setPen(textColor());
    pt->setFont(m_charFont);
    for(int i=0;i<4;i++){
        pt->drawText(space*i,5,space,height,Qt::AlignHCenter|Qt::AlignTop,m_data[i].label);
    }

    //绘制数据
    QString text;
    QColor color = textColor();
    pt->setFont(m_digitalFont);
    for(int i=0;i<4;i++){
        text = HzQuoteFormat::StringUnitFormat::toFormatString(m_data[i].netput);
        if(m_data[i].netput != INVALID_INT64)
            if(m_data[i].netput>0)
                color = riseColor() ;
            else if(m_data[i].netput<0)
                color = fallColor();
        pt->setPen(color);
        pt->drawText(space*i,5,space,height,Qt::AlignHCenter|Qt::AlignBottom,text);
    }
}
