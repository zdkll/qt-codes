#ifndef TESTYAXIS_H
#define TESTYAXIS_H
#include "testbasechart.h"


class TestYaxis:public QObject
{
    Q_OBJECT
public:
    explicit TestYaxis(TestBaseChart* pchart, QObject *parent = nullptr);
};

#endif // TESTYAXIS_H
