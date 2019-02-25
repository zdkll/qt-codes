#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

#include "pagehelper.h"
#include "publicdef.h"

class PageHelper;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void slotButtonClicked(int id);

private:
    Ui::Widget *ui;
    PageHelper *m_pageHelper;
    std::unordered_map<PageIdx,int> m_pageIdxMap;
};

#endif // WIDGET_H
