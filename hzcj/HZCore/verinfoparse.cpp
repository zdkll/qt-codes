#include "verinfoparse.h"

#define UpdateType_Optional     "Optional"
#define UpdateType_Required    "Required"

//当前版本信息
#include "hzcjinfo.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QFile>
#include <QDebug>

VerInfoParse::VerInfoParse()
    :m_updateType(NoUpdate)
{
    m_Path = QApplication::applicationDirPath();
}

VerInfoParse::VerInfoParse(const QString& fileName)
    :m_updateType(NoUpdate)
{
    m_Path = QApplication::applicationDirPath();
    setVersionFile(fileName);
}

void VerInfoParse::setVersionFile(const QString &fileName)
{
    //读取json文件保存信息
    if(!parseJson(fileName))
        return;

    //判断是否更新
    m_updateType = NoUpdate;

    //版本号对比
    if(m_rmVerInfo.updateVersion != PRODUCT_VERSION){
        if(m_rmVerInfo.updateType == UpdateType_Optional)
            m_updateType = OptionalUpdate;
        if(m_rmVerInfo.updateType == UpdateType_Required)
            m_updateType = RequredUpdate;
    }
}

bool  VerInfoParse::parseJson(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QJsonDocument doc = QJsonDocument::fromJson( file.readAll());
    file.close();
    if(doc.isNull())
        return false;
    if(!doc.isObject())
        return false;

    QJsonObject obj = doc.object();

    //读取基本信息
    m_rmVerInfo.updateVersion =  obj.value(VerJson_Key_UpdateVersion).toString();
    m_rmVerInfo.updateType     =  obj.value(VerJson_Key_UpdateType).toString();
    m_rmVerInfo.channel           =  obj.value(VerJson_Key_Channel).toInt();
    m_rmVerInfo.partialFrom      =  obj.value(VerJson_Key_Partial_From).toString();
    m_rmVerInfo.releaseTime     =  obj.value(VerJson_Key_Release_Time).toString();
    m_rmVerInfo.desc                =  obj.value(VerJson_Key_Desc).toString();
    m_rmVerInfo.detialsUrl         =  obj.value(VerJson_Key_Details_Url).toString();

    //读取包信息
    QJsonArray jarray = obj.value(VerJson_Key_Patch).toArray();
    if(jarray.size() !=2)
        return false;

    m_rmVerInfo.completePatch.type = jarray[0].toObject().value(Patch_Key_Type).toString();
    m_rmVerInfo.completePatch.size = jarray[0].toObject().value(Patch_Key_Size).toString().toLongLong();
    m_rmVerInfo.completePatch.url = jarray[0].toObject().value(Patch_Key_Url).toString();

    m_rmVerInfo.optionalPatch.type = jarray[1].toObject().value(Patch_Key_Type).toString();
    m_rmVerInfo.optionalPatch.size = jarray[1].toObject().value(Patch_Key_Size).toString().toLongLong();
    m_rmVerInfo.optionalPatch.url = jarray[1].toObject().value(Patch_Key_Url).toString();

    return true;
}

