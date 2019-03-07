#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "linesdrawer.h"
#include "drawlinepanel.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //设置物理坐标范围

protected slots:
    void startCommand(const DrawLine::Command& cmd);

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::Widget *ui;

    LinesDrawer      *m_linesDrawer;
    DrawLinePanel  *m_drawLinePanel;
};

#endif // WIDGET_H
