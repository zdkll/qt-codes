#ifndef CHARTFACTORY_H
#define CHARTFACTORY_H
/********************************************************************
created:
author: min.cheng
purpose: 生产工厂
*********************************************************************/
#include <QObject>
#include "Ioc.hpp"

class ChartFactory:public QObject
{
    Q_OBJECT
public:
    //Ioc容器，用于对象创建和注册
    static IocContainer* getIoc();
private:
    ChartFactory();
    IocContainer m_ioc;
};

#endif // CHARTFACTORY_H
