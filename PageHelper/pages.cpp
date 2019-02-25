#include "pages.h"

#include <QPainter>

BasePage::BasePage(QWidget *parent)
    :QWidget(parent)
{

}

void BasePage::paintEvent(QPaintEvent *e)
{
    QPainter pt(this);
    pt.fillRect(this->rect(),m_color);
    pt.end();
}


//HomePage
HomePage::HomePage(QWidget *parent)
    :BasePage(parent)
{
    m_color = Qt::red;
}


//StockPage
StockPage::StockPage(QWidget *parent)
    :BasePage(parent)
{

    m_color = Qt::green;
}


//DataPage
DataPage::DataPage(QWidget *parent)
    :BasePage(parent)
{
    m_color = Qt::blue;
}
