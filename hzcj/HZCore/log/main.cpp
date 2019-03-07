#include <iostream>
#include <QApplication>

#include "singleton.h"
#include "hzlog.h"

#include <QDebug>
#include <hzcfg.h>

#include <tchar.h>

//precompiler inc
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


int main(int argc, char ** argv)
{
    QApplication app(argc,argv);

    qDebug() << "path:" << Hzcfg::getInstance()->getLogCfgPath();
    Hzcfg::getInstance()->loadCfg();

    gHZLogIns.initialization();

    //test 1
    Log("KBLOG","WARN","hello info tickcount %d %s ", 6, "556");

    //test 2
    QString str = QStringLiteral("中文");
    Log("KBLOG","WARN","hello info tickcount %d %s ", 6, str.toStdString().c_str());

    //test3
    str = ("中文");
    Log("KBLOG","WARN","hello info tickcount %d %s ", 6, str.toStdString().c_str());

    Log("LogInOG","WARN","login test ");


    return app.exec();
}
