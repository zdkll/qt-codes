#ifndef CBASEMAINWIDGET_H
#define CBASEMAINWIDGET_H

#include "hzcontrols_global.h"
#include "basewidget.h"

class HZCONTROLSSHARED_EXPORT CBaseMainWidget  :  public BaseWidget
{
    Q_OBJECT
public:

    enum ECursorStyle
    {
        eCursorNormal = 0,   //普通鼠标
        eCursorLeft,         //左边拉伸
        eCursorRight,        //右边拉伸
        eCursorTop,          //上边拉伸
        eCursorBottom,       //下边拉伸
        eCursorLeftTop,      //左上拉伸
        eCursorRightBottom,  //右下拉伸
        eCursorRightTop,     //右上拉伸
        eCursorLeftBottom    //坐下拉伸
    };

    const int StretchableEdge = 3;

    explicit CBaseMainWidget(QWidget *parent = 0,QString instName="", QRect &clickableRect = QRect());
    virtual ~CBaseMainWidget();
    bool IsMaximized() { return m_IsMaximized; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

public slots:
    void OnShowMaximized();

private:
    void switchNormalMaximize();
    bool isPosInClickableRect(const QPoint &point);
    int setCursorStyle(const QPoint& point);
    void resizeWidget(const QPoint& point);
    void setWidgetGeometry(const QRect& rect);

private:
    QRect m_ClickableRect;       //可点击区域
    QPoint m_MovePosition;
    QRect m_NormalLocation;
    bool m_IsDraggable;          //可拖拽
    bool m_IsStretchable;        //可拉伸
    bool m_IsMaximized;
    ECursorStyle m_CursorStyle;  //鼠标样式
};

#endif // CBASEMAINWIDGET_H
