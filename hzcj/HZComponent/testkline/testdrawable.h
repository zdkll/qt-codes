#ifndef TESTDRAWABLE_H
#define TESTDRAWABLE_H
#include <QObject>
#include <QRect>

class TestLayerManager;
class TestDrawable:public QObject
{
    Q_OBJECT
public:
    explicit TestDrawable(QObject *parent = nullptr, TestLayerManager *layer = nullptr);

    //创建时候调用
    virtual void createEx();

    //createEx()调用后，自动调用
    virtual void onCreate();


    //设置区域位置
    void  setRc(QRect &rc,  QRect& wholeRc);
    QRect& getRc() ;


protected:
    TestLayerManager* m_layerManager;
    QRect m_rc;     //绘制区域
    QRect m_wholeRc;   //所有区域
    int m_nIndex = -1; //图层下标

};

#endif // TESTDRAWABLE_H
