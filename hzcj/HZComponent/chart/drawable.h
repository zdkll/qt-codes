#ifndef DRAWABLE_H
#define DRAWABLE_H
/********************************************************************
created: 2018/4/4
author: min.cheng
purpose: 可绘制区域
*********************************************************************/
#include <qevent.h>
#include <qpoint.h>
#include <qobject.h>
#include <qcolor.h>
#include <QPainter>
#include <QWidget>

class ImageLayer;
class  Drawable: public QObject
{
    Q_OBJECT
public:
    Drawable(ImageLayer* layer);
    ~Drawable();

    //设置区域背景
    void setBkClr(QColor bkClr);

    //设置获取区域位置 wndDrawArea表示整個窗口用于自绘的区域
    void setRc(const QRect& rc,const QRect& wndDrawArea);
    const QRect& getRc() const;

    //绘图区域创建 成功后会触发onCreate
    void createEx();

    //绘图区域设置显示与否，成功会触发onShow事件
    void showEx(bool bShow);

    //重置大小 成功会触发onResize()事件
    void resize();

    //获取图层ID
    int type();

    //判断绘图区域是否显示
    bool isShowEx();

    //重绘接口，子类通过重写来实现图层重绘
    virtual void redraw();

    //createEx()调用后，自动调用
    virtual void onCreate();

    //showEx()调用后，自动调用
    virtual void onShow(bool bshow);

    //resize()调用后，自动调用
    virtual void onResize();

    QWidget* getCanvasWnd();
signals:

public slots:

protected:
    bool m_show;
    QRect m_rc;
    QRect m_wndDrawRc;
    QColor m_bkClr;
    ImageLayer* m_imageLayer;
    int m_type;
};

#endif // DRAWABLE_H
