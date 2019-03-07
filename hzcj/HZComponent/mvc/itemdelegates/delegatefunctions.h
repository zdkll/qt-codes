/********************************************************************
created:2018/06/20
author: dk.zhang
purpose: 行情字段格式化函数
*********************************************************************/
#ifndef DELEGATEFUNCTIONS_H
#define DELEGATEFUNCTIONS_H

#include <QtCore>

#include "datamodeldef.h"
namespace DelegateFunctions {
//ulonglong 格式化显示
QString   toFormatString(const qulonglong &in);
//longlong 格式化
QString   toFormatString(const qlonglong &in);
//double   格式化
QString   toFormatString(const double &in);

//为处理 /100 精度问题，单独处理股本转手数问题
QString   stockToFormatString(const qulonglong &in);

QString   stockToFormatString(const qlonglong &in);

}


#endif // DELEGATEFUNCTIONS_H
