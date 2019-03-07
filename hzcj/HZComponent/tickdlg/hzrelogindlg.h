#ifndef HZRELOGINDLG_H
#define HZRELOGINDLG_H

#include <basewidget.h>
#include <datamodeldef.h>

#include <QDialog>
#include <QFrame>
#include <QMouseEvent>

#include "hzcomponent_global.h"

class HZObject;
class QPushButton;
class HZCOMPONENTSHARED_EXPORT HZReloginDlg : public QDialog, public HZObject
{
    Q_OBJECT
public:
    HZReloginDlg(QWidget *parent = 0);
    void startKick(const HZData::UserLoginKickInfo &kickInfo);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public slots:
    void slotBtnClicked();

private:
    QPushButton *mBtnReLogin, *mBtnClose;
    QFrame *mTitleFrame;
    bool mDragWindow;
    QPoint mMousePoint;
    QFont mTitleFont;
    void loadSkin();
};

#endif // HZRELOGINDLG_H
