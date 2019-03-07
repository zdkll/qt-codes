#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "hzcontrols_global.h"
#include "titlebar.h"
#include "QMessageBox"
#include <QMouseEvent>

class TitleBar;
class HZCONTROLSSHARED_EXPORT PopupWindow : public QDialog
{
    Q_OBJECT
public:
    explicit PopupWindow(QWidget *parent = 0);
    void SetTitleWnd(const QString &str);
signals:

public slots:
    void CloseParent();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    void SltMainWndMaximized();
    QWidget            *pMidWidget_;
private:
    TitleBar    *pTitleWnd_;
    bool    m_bMouse;
    QPoint  m_StartPt;
};

#endif // POPUPWINDOW_H
