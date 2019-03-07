#ifndef HZAPPLICATION_H
#define HZAPPLICATION_H
/********************************************************************
created:
author: min.cheng
purpose: 用于程序初始化，代替在Main函数中处理代码
*********************************************************************/
#include <QApplication>
#include <memory>
#include <QObject>

class HZApplication:public QObject
{
    Q_OBJECT
public:
    HZApplication(QObject *parent=Q_NULLPTR);

    ~HZApplication();

    int initInstance(int argc, char *argv[]);
protected slots:
    void onExitInstance();
protected:
    int onInitInstance(QApplication& app);
    QApplication * m_app;
};

#endif // HZAPPLICATION_H
