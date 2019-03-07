#include "hzdirhelper.h"
#include <QDir>
#include <QJsonDocument>

bool HzDirHelper::isDirExist(const QString& fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    return false;
}

bool HzDirHelper::createDir(const QString &fullPath)
{
    bool bRet = HzDirHelper::isDirExist(fullPath);
    if(!bRet)
    {
        QDir dir(fullPath);
        bRet = dir.mkpath(fullPath);
    }
    return bRet;
}

bool HzDirHelper::readJson(QJsonDocument &doc, const QString &path)
{
    bool bRet = false;
    if(!path.isEmpty() )
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray ba = file.readAll();
            QJsonParseError e;
            doc = QJsonDocument::fromJson(ba, &e);
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                bRet = true;
            }
            file.close();
        }
    }
    return bRet;
}

bool HzDirHelper::saveJson(QJsonDocument &doc, const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        return false;
     file.write(doc.toJson());
    file.close();
    return true;
}
