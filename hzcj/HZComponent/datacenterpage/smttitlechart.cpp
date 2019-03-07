#include "smttitlechart.h"
#include <QPainter>

SmtTitleStaticChart::SmtTitleStaticChart(ImageLayer *imagLayer, const CurveDataList &data, CurveChartMargin margin):
    CurveChart(imagLayer,data,margin)
{

}

void SmtTitleStaticChart::resize(const QRectF &rc, const QRectF &wndRc)
{
    CurveChart::resize(rc,wndRc);
    draw();
}

void SmtTitleStaticChart::draw()
{
    m_imageLayer->clear(m_layerId);
    QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
    int midPos = m_rc.top()+m_rc.height()/2;
    QRectF rc = m_rc;
    rc.setLeft(m_wndRc.left()+30);

    //绘制融资余额相关 
    QRectF rzRc = rc;
    rzRc.setTop(midPos-5);
    rzRc.setBottom(midPos+5);
    rzRc.setRight(rc.left()+10);
    dc.fillRect(rzRc,m_style->getMinLineClr());

    QRectF rqRc = rzRc;
    rqRc.setLeft(rzRc.left()+176);
    rqRc.setRight(rqRc.left()+10);
    dc.fillRect(rqRc,m_style->getMinAvgLineClr());

    QPen pen;
    pen.setColor(m_style->getTitleTextClr());
    QFont font;
    font.setFamily("SimSun");
    font.setPixelSize(12);
    dc.setPen(pen);
    dc.setFont(font);

    rzRc.setLeft(rzRc.right()+10);
    rzRc.setRight(rzRc.left()+100);
    rzRc.setTop(rc.top());
    rzRc.setBottom(rc.bottom());
    dc.drawText(rzRc,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("融资余额（元）"));

    rqRc.setLeft(rqRc.right()+10);
    rqRc.setRight(rqRc.left()+100);
    rqRc.setTop(rc.top());
    rqRc.setBottom(rc.bottom());
    dc.drawText(rqRc,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("融券余额（元）"));
}
