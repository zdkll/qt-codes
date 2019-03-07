#include "testlayermanager.h"

TestLayerManager::TestLayerManager(QWidget *wnd):
    QObject(wnd),
    m_pWnd(wnd)
{

}

TestLayerManager::~TestLayerManager()
{
    foreach (LayerInfo info, m_layerVec) {
            if(!info.pix)
            {
                delete info.pix;
                info.pix = nullptr;
            }
    }
}

int TestLayerManager::createLayer(QRect rc, QColor fillColor)
{
    int index = m_nIndex++;

    LayerInfo info;
    QPixmap *pix = new QPixmap(rc.width(), rc.height());
    pix->fill(fillColor);
    info.pix = pix;
    info.rc = rc;
    info.nIndex =  index;
    m_layerMap[index] = info;
    m_layerVec.append(info);

    return index;
}
