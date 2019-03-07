#ifndef TESTLAYERMANAGER_H
#define TESTLAYERMANAGER_H
#include <QWidget>
#include <QMap>


struct LayerInfo
{
    LayerInfo():pix(nullptr){}
    bool bHide = false;
    int nIndex;
    QRect rc;
    QPixmap *pix;
    QColor fillColor;
};


class TestLayerManager:public QObject
{
public:
    explicit TestLayerManager(QWidget *wnd);

    ~TestLayerManager();


    //创建图层, 返回对应索引
    int createLayer(QRect rc, QColor fillColor = Qt::transparent);

private:
    QWidget *m_pWnd;
    int m_nIndex = 0;
    QMap<int, LayerInfo> m_layerMap;
//    QMap<int, int> m_layerIndexMap;
    QVector<LayerInfo> m_layerVec;
};

#endif // TESTLAYERMANAGER_H
