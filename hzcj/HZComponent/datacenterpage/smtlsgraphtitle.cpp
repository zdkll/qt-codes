#include "smtlsgraphtitle.h"
#include <qfontmetrics.h>

SmtLsGraphTitle::SmtLsGraphTitle(QWidget *parent):
    CurveGraphStyle(parent),
    m_imageLayer(new ImageLayer(this))
{
   m_layerId = m_imageLayer->createLayout(getRc()).type;
}

SmtLsGraphTitle::~SmtLsGraphTitle()
{

}

void SmtLsGraphTitle::updateData(SmtTiletDataItem data)
{
    m_data = data;
    drawData();
    this->update();
}

QRect SmtLsGraphTitle::getRc()
{
    return QRect(0,0,this->geometry().width(),this->geometry().height());
}

void SmtLsGraphTitle::paintEvent(QPaintEvent *event)
{
    BaseWidget::paintEvent(event);
    m_imageLayer->repaint();
}

void SmtLsGraphTitle::resizeEvent(QResizeEvent *event)
{
    m_imageLayer->resize(getRc(),m_layerId);
    drawData();
}

void SmtLsGraphTitle::setDcClr(HzQuoteFormat::QuoteItem item,QPainter& dc)
{
    QPen pen;
    pen.setColor(m_equalClr);
    if(item.dirc == HzQuoteFormat::quote_up)
    {
        pen.setColor(m_upClr);
    }else if(item.dirc == HzQuoteFormat::quote_down)
    {
        pen.setColor(m_downClr);
    }
    dc.setPen(pen);
}

void SmtLsGraphTitle::drawData()
{
    m_imageLayer->clear(m_layerId);
    QPainter dc(m_imageLayer->getLayout(m_layerId).pix);
    QRect rc = getRc();
    rc.setLeft(rc.left()+30);
    rc.setRight(rc.right());
    int midPos = rc.top()+rc.height()/2;
    int width = rc.width()/3;

    //绘制文本
    QRect textRc = rc;
    textRc.setBottom(midPos);
    textRc.setRight(textRc.left()+width);
    QFont font;
    font.setFamily("SimSun");
    font.setPixelSize(14);
    dc.setFont(font);
    QPen pen;
    pen.setColor(m_titleTextClr);
    dc.setPen(pen);
    dc.drawText(textRc,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("两融余额"));
    textRc.setLeft(textRc.right());
    textRc.setRight(textRc.left()+width);
    dc.drawText(textRc,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("融资余额"));
    textRc.setLeft(textRc.right());
    textRc.setRight(textRc.left()+width);
    dc.drawText(textRc,Qt::AlignLeft|Qt::AlignVCenter,QStringLiteral("融券余额"));

    //绘制数据
    textRc = rc;
    textRc.setTop(midPos);
    font.setFamily("Arial");
    font.setPixelSize(16);
    font.setBold(true);
    dc.setFont(font);

    //两市余额
    QRect lsyeRc = textRc;
    lsyeRc.setRight(lsyeRc.left()+width);
    HzQuoteFormat::QuoteItem item = HzQuoteFormat::QuoteFormat::formatPrice(m_data.lsye,m_data.preLsye);
    setDcClr(item,dc);
    QString str = HzQuoteFormat::StringUnitFormat::toFormatString(m_data.lsye);
    QFontMetrics fm(font);
    int strWidth = fm.boundingRect(str).width()+5;
    lsyeRc.setRight(lsyeRc.left()+strWidth);
    dc.drawText(lsyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);

    //融资余额
    QRect rzyeRc = textRc;
    rzyeRc.setLeft(textRc.left()+width);
    item = HzQuoteFormat::QuoteFormat::formatPrice(m_data.rzye,m_data.preRzye);
    setDcClr(item,dc);
    str = HzQuoteFormat::StringUnitFormat::toFormatString(m_data.rzye);
    strWidth = fm.boundingRect(str).width()+5;
    rzyeRc.setRight(rzyeRc.left()+strWidth);
    dc.drawText(rzyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);

    //融券余额
    QRect rqyeRc = textRc;
    rqyeRc.setLeft(textRc.left()+width*2);
    item = HzQuoteFormat::QuoteFormat::formatPrice(m_data.rqye,m_data.preRqye);
    setDcClr(item,dc);
    str = HzQuoteFormat::StringUnitFormat::toFormatString(m_data.rqye);
    strWidth = fm.boundingRect(str).width()+5;
    rqyeRc.setRight(rqyeRc.left()+strWidth);
    dc.drawText(rqyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);

    font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPixelSize(13);
    dc.setFont(font);

    //两市余额
    item = HzQuoteFormat::QuoteFormat::formatChange(m_data.lsye,m_data.preLsye);
    setDcClr(item,dc);
    str = item.strValue;
    if(m_data.lsye != DBL_MAX && m_data.preLsye != DBL_MAX)
    {
        str = HzQuoteFormat::StringUnitFormat::toFormatString(m_data.lsye-m_data.preLsye);
    }
    str += " ";
    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_data.lsye,m_data.preLsye);
    str += item.strValue;
    lsyeRc.setLeft(lsyeRc.right()+5);
    lsyeRc.setRight(lsyeRc.left()+100);
    dc.drawText(lsyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);


    //融资余额
    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_data.rzye,m_data.preRzye);
    setDcClr(item,dc);
    str = item.strValue;
    rzyeRc.setLeft(rzyeRc.right()+5);
    rzyeRc.setRight(rzyeRc.left()+100);
    dc.drawText(rzyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);

    //融券余额
    item = HzQuoteFormat::QuoteFormat::formatChangeRate(m_data.rqye,m_data.preRqye);
    setDcClr(item,dc);
    str = item.strValue;
    rqyeRc.setLeft(rqyeRc.right()+5);
    rqyeRc.setRight(rqyeRc.left()+100);
    dc.drawText(rqyeRc,Qt::AlignLeft|Qt::AlignVCenter,str);
}
