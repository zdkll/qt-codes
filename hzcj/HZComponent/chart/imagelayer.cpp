#include "imagelayer.h"
#include <qpainter.h>

ImageLayer::ImageLayer(QWidget* wnd):m_wnd(wnd),m_index(0)
{
}

ImageLayer::~ImageLayer()
{
    if(m_layVec.empty() == false)
    {
        for(int i=0;i < m_layVec.size();i++)
        {
            delete m_layVec[i].pix;
            m_layVec[i].pix = nullptr;
        }
    }
    m_layVec.clear();
    m_layMap.clear();
}

ImgLayerInfo ImageLayer::createLayout(QRect rc,QColor fillColor)
{
    int type = m_index++;
    ImgLayerInfo info;
    QPixmap* pix = new QPixmap(rc.width(),rc.height());
    pix->fill(fillColor);
    info.pix = pix;
    info.rc = rc;
    info.type = type;
    info.fillColor = fillColor;
    m_layMap[type] = info;
    m_layVec.push_back(info);
    m_layIndeMap[type] = m_layVec.size()-1;
    return info;
}

ImgLayerInfo ImageLayer::getLayout(int type)
{
    ImgLayerInfo info;
    auto it = m_layMap.find(type);
    if(it != m_layMap.end())
    {
        info = it->second;
    }
    return info;
}

void ImageLayer::remove(int type)
{
    m_layIndeMap.clear();
    for(auto it = m_layVec.begin();it != m_layVec.end();it++)
    {
        if(it->type == type)
        {
            auto iter = m_layMap.find(type);
            if (iter != m_layMap.end())
			{
				delete iter->second.pix;
                m_layMap.erase(iter);
			}
			it->pix = nullptr;
            m_layVec.erase(it);
            break;
        }
    }
    for(int i=0;i < m_layVec.size();i++)
    {
        m_layIndeMap[ m_layVec[i].type] = i;
    }
}

void ImageLayer::clear(int type)
{
    auto iter = m_layMap.find(type);
    if(iter != m_layMap.end())
    {
        if(iter->second.pix != nullptr)
        {
            delete iter->second.pix;
            iter->second.pix = new QPixmap(iter->second.rc.width(),iter->second.rc.height());
            iter->second.pix->fill(iter->second.fillColor);
            m_layVec[m_layIndeMap[type]] = iter->second;
        }
    }
}

void ImageLayer::hide(int type, bool bshow)
{
    auto iter = m_layMap.find(type);
    if(iter != m_layMap.end())
    {
        if(iter->second.pix != nullptr)
        {
            iter->second.bHide=!bshow;
            m_layVec[m_layIndeMap[type]] = iter->second;
        }
    }
}

void ImageLayer::hide(QRect rc, int type, bool bshow)
{
    auto iter = m_layMap.find(type);
    if(iter != m_layMap.end())
    {
        if(iter->second.pix != nullptr)
        {
            iter->second.bHide=!bshow;
            if(!iter->second.bHide)
            {
                QPixmap* pix = new QPixmap(rc.width(),rc.height());
                pix->fill(iter->second.fillColor);

                //重置大小就清楚绘制的内容
                //QPainter p(pix);
                //p.drawPixmap(QPoint(0,0),*(iter->second.pix));
                delete iter->second.pix;
                iter->second.pix = pix;
                iter->second.rc = rc;
            }
            m_layVec[m_layIndeMap[type]] = iter->second;
        }
    }
}

void ImageLayer::resize(QRect rc, int type)
{
    auto iter = m_layMap.find(type);
    if(iter != m_layMap.end())
    {
        if(iter->second.pix != nullptr)
        {
            QPixmap* pix = new QPixmap(rc.width(),rc.height());
            pix->fill(iter->second.fillColor);

            //重置大小就清楚绘制的内容
            //QPainter p(pix);
            //p.drawPixmap(QPoint(0,0),*(iter->second.pix));
            delete iter->second.pix;
            iter->second.pix = pix;
            iter->second.rc = rc;
            m_layVec[m_layIndeMap[type]] = iter->second;
        }
    }
}

void ImageLayer::repaint()
{
    if(m_wnd && m_layVec.empty() == false)
    {
        QPainter painter(m_wnd);
        for(int i=0;i < m_layVec.size();i++)
        {
            if(m_layVec[i].bHide == false)
                painter.drawPixmap(m_layVec[i].rc.left(),m_layVec[i].rc.top(),*(m_layVec[i].pix));
        }
    }
}

QWidget *ImageLayer::getWnd()
{
    return m_wnd;
}

void ImageLayer::clearAll()
{
    for(auto iter = m_layMap.begin();iter!= m_layMap.end();iter++)
    {
        if(iter->second.pix != nullptr)
        {
            delete iter->second.pix;
            iter->second.pix = new QPixmap(iter->second.rc.width(),iter->second.rc.height());
            iter->second.pix->fill(iter->second.fillColor);
            m_layVec[m_layIndeMap[iter->first]] = iter->second;
        }
    }
}

