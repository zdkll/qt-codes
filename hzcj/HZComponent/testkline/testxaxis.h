#ifndef TESTXAIS_H
#define TESTXAIS_H
#include <QObject>
#include "testbasechart.h"


class TestXaxis:public QObject
{
public:
    explicit TestXaxis(TestBaseChart* pchart, QObject* parent = nullptr);
};

#endif // TESTXAIS_H
