#ifndef TESTKLINEDATAPROVIDER_H
#define TESTKLINEDATAPROVIDER_H
#include "testdataprovider.h"


class TestKlineDataProvider:public TestDataProvider
{
    Q_OBJECT
public:
    explicit TestKlineDataProvider(QObject* parent = nullptr);
};

#endif // TESTKLINEDATAPROVIDER_H
