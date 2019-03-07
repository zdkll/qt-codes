#include "minlinehisdlg.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDateTime>

#include "titlebar.h"

MinLineHisWidget::MinLineHisWidget(QWidget *parent, QString instname):
    PopupWindow(parent,instname),
    m_pMline(new MinHisCanvas)
{
    createWg();
}

void MinLineHisWidget::setObject(const QString& strCode, const QString& strName,qint64 nTime,qint64 reqTime)
{

    QDateTime nt;
    nt.setTime_t(nTime);
    QString strDate = nt.toString("yyyy-MM-dd");
    m_pTitleBar->setWinTitle(strName + " " + strDate);
    m_pMline->switchStock(strCode,reqTime);
}


void MinLineHisWidget::createWg()
{

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    m_pTitleBar->setControlFlags(WindowControlBox::WindowCloseButton);
    m_pTitleBar->setResizable(false);
    m_pTitleBar->setObjectName("minHistitle");

    QWidget *widget = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_pMline);
    m_pMline->createWg();

    //m_pMline->switchStockEx("sh000001");
    widget->setObjectName("centerwidget");
    widget->setFixedSize(518, 314);

    widget->setLayout(mainLayout);
    setWidget(widget);

    setFixedSize(sizeHint().width(),sizeHint().height());


    showMsg();
}

void MinLineHisWidget::showMsg()
{
    QDesktopWidget *deskTop = QApplication::desktop();
    QRect destRect = deskTop->availableGeometry();
    QPoint point;
    point.setX(destRect.width()- rect().width());
    point.setY(destRect.height() - rect().height());

    move(point);
}

