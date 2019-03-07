#ifndef HZDIRHELPER_H
#define HZDIRHELPER_H
#include "hzcore_global.h"
class QJsonDocument;
class HZCORESHARED_EXPORT HzDirHelper
{
public:
    static bool isDirExist(const QString& fullPath);

    static bool createDir(const QString& fullPath);

    static bool readJson(QJsonDocument& doc,const QString& path);

    static bool saveJson(QJsonDocument& doc,const QString& path);
};

#endif // HZDIRHELPER_H
