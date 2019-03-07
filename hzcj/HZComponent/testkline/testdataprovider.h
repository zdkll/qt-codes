#ifndef TESTDATAPROVIDER_H
#define TESTDATAPROVIDER_H
#include <QObject>


class TestDataProvider:public QObject
{
    Q_OBJECT
public:
    explicit TestDataProvider(QObject *parent = nullptr);
};

#endif // TESTDATAPROVIDER_H
