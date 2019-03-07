#include "testdrawable.h"
#include "testlayermanager.h"

TestDrawable::TestDrawable(QObject *parent, TestLayerManager *layer):
    QObject(parent),
    m_layerManager(layer)
{

}

void TestDrawable::createEx()
{
    m_nIndex = m_layerManager->createLayer(m_wholeRc);

    onCreate();
}

void TestDrawable::onCreate()
{

}

void TestDrawable::setRc(QRect &rc, QRect &wholeRc)
{
    m_rc = rc;
    m_wholeRc = wholeRc;
}

QRect &TestDrawable::getRc()
{
    return m_rc;
}
