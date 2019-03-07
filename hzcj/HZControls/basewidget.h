#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "hzcontrols_global.h"
#include "hzobject.h"
#include <QWidget>
#include <QDialog>

class HZCONTROLSSHARED_EXPORT BaseWidget : public QWidget,public HZObject
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0,QString instName="");
    virtual ~BaseWidget();
signals:

public slots:

protected:

    virtual void paintEvent(QPaintEvent *e);
};



//Dialog
class HZCONTROLSSHARED_EXPORT BaseDialog : public QDialog,public HZObject
{
    Q_OBJECT
public:
    explicit BaseDialog(QWidget *parent = 0,QString instName="");
    virtual ~BaseDialog();
signals:

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void paintEvent(QPaintEvent *e);
};

#endif // BASEWIDGET_H
