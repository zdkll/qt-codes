#include "popupwindow.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>


PopupWindow::PopupWindow(QWidget *parent) : QDialog(parent),m_bMouse(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint );

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setMargin(1);
    gridLayout->setSpacing(0);

    pTitleWnd_ = new TitleBar(this,"");
    pTitleWnd_->setFixedHeight(30);

    pMidWidget_ = new QWidget(this);
    pMidWidget_->setObjectName("contentwidget");
    gridLayout->addWidget(pTitleWnd_);
    gridLayout->addWidget(pMidWidget_);
    setLayout(gridLayout);
    connect(pTitleWnd_, &TitleBar::mainWndMaximized, this, &PopupWindow::SltMainWndMaximized);
}

void PopupWindow::SetTitleWnd(const QString &str)
{
    pTitleWnd_->SetTitleWnd(str);
}

void PopupWindow::CloseParent()
{
    this->close();
}

void PopupWindow::mousePressEvent(QMouseEvent *event)
{
    if((pTitleWnd_->rect().contains(event->pos())) && (event->button() == Qt::LeftButton))
    {
        m_bMouse = true;
        m_StartPt = event->pos();
    }
    QDialog::mousePressEvent(event);
}

void PopupWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMouse)
    {
        QPoint relaPos(event->globalPos() - m_StartPt);
        move(relaPos);
    }
   QDialog::mouseMoveEvent(event);
}

void PopupWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMouse = false;
    QDialog::mouseReleaseEvent(event);
}

void PopupWindow::SltMainWndMaximized()
{
   // this->OnShowMaximized();
}

