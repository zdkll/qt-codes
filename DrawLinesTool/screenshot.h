#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QEvent>

#include "screen.h"
#include "screenshottoolbar.h"

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    ScreenShot(QWidget *parent = 0);
    ~ScreenShot();

    void startScreenShot();

signals:
    void finishedScreenShot(bool success);

protected slots:
    void slotToolBarCmd(int );

protected:
    bool eventFilter(QObject *watched, QEvent *event);

    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void endScreenShot(bool success);
    void hideToolBar(){m_screenShotToolBar->hide();}
    void showToolBar(const QPoint& pt);

    QPoint  m_pressedPt,m_curPos;
    bool      m_isPressed;

    QPixmap     *m_fullScreen,*m_bgScreen;
    ScreenRect *m_screenRect;
    ScreenShotToolBar  *m_screenShotToolBar;
};

#endif // SCREENSHOT_H
