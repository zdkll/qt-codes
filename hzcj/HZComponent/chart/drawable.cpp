#include "drawable.h"
#include <assert.h>
#include "urlparse.h"
#include "imagelayer.h"

Drawable::Drawable(ImageLayer* layer)
    :m_imageLayer(layer)
{
    m_show = true;
    m_type = -1;
}

Drawable::~Drawable()
{
    if(m_imageLayer)
    {
        m_imageLayer->remove(m_type);
    }
}

void Drawable::setBkClr(QColor bkClr)
{
    m_bkClr = bkClr;
}

void Drawable::setRc(const QRect &rc,const QRect& wndDrawArea)
{
    m_rc = rc;
    m_wndDrawRc = wndDrawArea;
}

const QRect &Drawable::getRc() const
{
    return m_rc;
}

void Drawable::resize()
{
    if(m_type != -1)
        m_imageLayer->resize(m_wndDrawRc,m_type);
    onResize();
}

void Drawable::redraw()
{

}

void Drawable::createEx()
{
    auto info = m_imageLayer->createLayout(m_wndDrawRc);
    m_type = info.type;
    onCreate();
}

void Drawable::showEx(bool bShow)
{
    m_imageLayer->hide(m_type,bShow);
    onShow(bShow);
    m_show = bShow;
}

int Drawable::type()
{
    return m_type;
}

bool Drawable::isShowEx()
{
    return m_show;
}

void Drawable::onResize()
{

}

QWidget *Drawable::getCanvasWnd()
{
    if(m_imageLayer)
        return m_imageLayer->getWnd();
}

void Drawable::onCreate()
{

}

void Drawable::onShow(bool bshow)
{

}
