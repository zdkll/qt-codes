#include "curvecanvas.h"
#include "curvedataprovider.h"

CurveCanvas::CurveCanvas(QWidget *parent):
    CurveGraphStyle(parent),
    m_imageLayer(new ImageLayer(this))
{
}

CurveCanvas::~CurveCanvas()
{
    if(m_imageLayer)
    {
        delete m_imageLayer;
        m_imageLayer = nullptr;
    }
}

void CurveCanvas::createWg()
{
}


void CurveCanvas::setAxisWidth(int left, int right)
{
    m_leftAxisWidth = left;
    m_rightAxisWidth = right;
}

void CurveCanvas::paintEvent(QPaintEvent *event)
{
    m_imageLayer->repaint();
}

QRectF CurveCanvas::getRc()
{
    return QRectF(0,0,this->geometry().width(),this->geometry().height());
}

void CurveCanvas::dataUpdate()
{
    for(int i=0;i < m_veObserverList.size();i++)
    {
        m_veObserverList[i]->updateData();
    }
    this->update();
}
