#ifndef SUOLUEFACTORY_H
#define SUOLUEFACTORY_H
/********************************************************************
created:
author: min.cheng
purpose: 缩略图构造工厂
*********************************************************************/
#include <QObject>
#include "Ioc.hpp"
#include "suoluebase.h"

class SuolueFactory :public QObject
{
    Q_OBJECT
public:
    //Ioc容器，用于对象创建和注册
    static IocContainer* getIoc();
private:
    SuolueFactory();
private:
    IocContainer m_ioc;
};

#endif // SUOLUEFACTORY_H
