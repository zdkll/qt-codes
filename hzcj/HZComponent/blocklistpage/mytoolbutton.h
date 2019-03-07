#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H
#include <QWidget>
#include <QToolButton>

class MyToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit MyToolButton(QWidget *parent = nullptr);

};

#endif // MYTOOLBUTTON_H
