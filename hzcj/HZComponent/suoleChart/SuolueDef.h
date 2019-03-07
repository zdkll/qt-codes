#ifndef SUOLUEDEF_H
#define SUOLUEDEF_H
#include <QObject>
#include <QDebug>
#include "Any.hpp"
/*
配置页面信息
*/
struct PageInfo{
    QString className;
    Any any;
    PageInfo(const QString &strClass, Any anyVal):
        className(strClass),
        any(anyVal)
    {

    }
};

//分时数据结构
typedef struct{
    QStringList  strName;
    QStringList  strCode;
}FSLIST;


//板块数据结构
typedef struct{
    QString  strName;
    int          nFild;
}BKDATA;
#endif // SUOLUEDEF_H
