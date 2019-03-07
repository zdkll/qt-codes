#include "hzline.h"

#include <QStyleOption>
#include <QPainter>

#include "micro.h"

HZLine::HZLine(QWidget *parent )
    :m_orientation(Qt::Horizontal),QFrame(parent)
{
    createLine();
}

HZLine::HZLine(Qt::Orientation orientation,QWidget *parent)
    :m_orientation(orientation),QFrame(parent)
{
    createLine();
}

//void HZLine::paintEvent(QPaintEvent *)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Frame,&opt,&p,this);
//}

void HZLine::createLine()
{
    m_hzLineWidth = DEFAULT_HZLINE_WIDTH;

    QFrame::Shape shape = m_orientation==Qt::Horizontal?QFrame::HLine:QFrame::VLine;
    this->setFrameShape(shape);
    //默认Plain样式
    this->setFrameShadow(QFrame::Plain);

    //根据方向设置边框大小
    this->setLineWidth(m_hzLineWidth);
    if(m_orientation == Qt::Horizontal)
    {
        this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        this->setFixedHeight(m_hzLineWidth);
    }else
    {
        this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
        this->setFixedWidth(m_hzLineWidth);
    }
}

void HZLine::setHZlineWidth(const int &lineWidth)
{
    m_hzLineWidth  = lineWidth;
    if(m_orientation == Qt::Horizontal) {
        this->setFixedHeight(m_hzLineWidth);
    }else {
        this->setFixedWidth(m_hzLineWidth);
    }
}
