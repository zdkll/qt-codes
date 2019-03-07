#include "basewidget.h"

#include <QPainter>
#include <QStyleOption>


BaseWidget::BaseWidget(QWidget *parent,QString instName)
    : QWidget(parent),HZObject(instName)
{

}

BaseWidget::~BaseWidget()
{


}

void BaseWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(e);
}




//BaseDialog----
BaseDialog::BaseDialog(QWidget *parent,QString instName)
    : QDialog(parent),HZObject(instName)
{

}

BaseDialog::~BaseDialog()
{

}

void BaseDialog::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void BaseDialog::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}

void BaseDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void BaseDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
}


void BaseDialog::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QDialog::paintEvent(e);
}

