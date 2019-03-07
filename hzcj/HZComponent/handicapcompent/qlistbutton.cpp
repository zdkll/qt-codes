#include "QListButton.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QDateTime>
#include <QDebug>


QListButton::QListButton(QWidget *parent):
    QPushButton(parent)
{
    m_strText = "XXXX-XX-XX";

    QListButton::setFixedSize(114,20);                      //调整控件尺寸
    this->setCheckable(true);
    this->setMouseTracking(true);

    m_pix = QPixmap(":/skin/default/img/tableview/down02.png");

}

void QListButton::updateText(const QString &strText, bool bChecked)
{
    m_strText = strText;

    if(!bChecked)
    {
        m_pix = QPixmap(":/skin/default/img/tableview/down02.png");
    }
    else
    {
        m_pix = QPixmap(":/skin/default/img/tableview/down.png");
    }

    this->update();
}

void QListButton::paintEvent(QPaintEvent *event)
{
      QPushButton::paintEvent(event);

      QPainter painter(this);

      QPixmap pix(":/skin/default/img/infopage/titleicon.png");

//      QPixmap pix2(":/skin/default/img/tableview/down02.png");


      QRect rect = this->rect();

      QRect rc;

      rc.setRect(5, 5, pix.width(), pix.height());

      QFont font;

      font.setFamily("Arial");
      font.setPixelSize(12);

      painter.setFont(font);

      painter.drawPixmap(rc, pix);

      painter.setPen(this->textColor());

      painter.drawText(rect, Qt::AlignCenter, m_strText);

      rc.setRect(rect.width() - 5 - m_pix.width(), 8, m_pix.width(), m_pix.height());

      painter.drawPixmap(rc, m_pix);

}




