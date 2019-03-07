/********************************************************************
created:2018/04/20
author: min.cheng dk.zhang
purpose:通用的标题栏控件，包括标题名称和右侧最小化最大化和关闭按钮
*********************************************************************/
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "hzcontrols_global.h"
#include "basewidget.h"
#include "windowcontrolbox.h"

class QLabel;
class HZCONTROLSSHARED_EXPORT TitleBar : public BaseWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0,QString instName="");
    ~TitleBar();

    virtual void createTitleBar();

    WindowControlBox *windowControlBox(){return m_controlBox;}

    void setControlFlag(const WindowControlBox::ControlFlag &controlFlag,bool enable = true);
    void setControlFlags(const WindowControlBox::ControlFlags &controlFlags);

    void setResizable(const bool &enable){m_isResizable = enable;}
public slots:
    void setWinTitle(const QString& str);

signals:
    void mainWndMinimized();
    void mainWndMaximized();
    void mainWndClose();

protected:
    // 双击标题栏进行界面的最大化/还原
//    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    // 进行鼠界面的拖动
    // virtual void mousePressEvent(QMouseEvent *event);

    // 设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);

protected:
    // 最大化/还原
    void onMinimized();
    void onMaximinzed();
    void onClose();

protected:
    QLabel                     *m_pIconLabel;
    QLabel                     *m_pTitleLabel;
    WindowControlBox *m_controlBox;
    bool                          m_isResizable;
};
#endif // TITLEBAR_H
