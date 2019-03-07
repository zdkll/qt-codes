#ifndef TESTCHARTFACTORY_H
#define TESTCHARTFACTORY_H
#include <QObject>
#include "Ioc.hpp"

class testChartFactory:public QObject
{
    Q_OBJECT

public :
    static IocContainer* getIoc();

private:
    testChartFactory();
    IocContainer m_ioc;
};

#endif // TESTCHARTFACTORY_H
