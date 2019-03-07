#ifndef TitleBar_H
#define TitleBar_H

#include "hzcontrols_global.h"
#include "basewidget.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
namespace Ui {
    class titlebar;
}

class QHBoxLayout;
class WindowControlBox;
class HZCONTROLSSHARED_EXPORT TitleBar : public BaseWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0,QString instName="",BaseWidget* midWidget = nullptr);

    void SetTitleWnd(const QString& str);

signals:
    void mainWndMinimized();
    void mainWndMaximized();
    void mainWndClose();
public slots:
    void OnMinimized();
    void OnMaximinzed();
    void OnClose();
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
    WindowControlBox* controlBox_;
    BaseWidget* midWidget_;
    bool         m_bPressed;
    QPoint       m_startMovePos;
    Ui::titlebar*  ui;
};

#endif // TitleBar_H
