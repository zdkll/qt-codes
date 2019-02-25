#ifndef PAGES_H
#define PAGES_H

#include <QWidget>
#include <QStackedWidget>

class BasePage : public QWidget
{
public:
    BasePage(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);
    QColor m_color;
};


//
class HomePage : public BasePage
{
public:
    HomePage(QWidget *parent = 0);
};

class StockPage : public BasePage
{
public:
    StockPage(QWidget *parent = 0);

};



class DataPage : public BasePage
{
public:
    DataPage(QWidget *parent = 0);

};


#endif // PAGES_H
