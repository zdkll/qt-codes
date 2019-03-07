/********************************************************************
created:2019/05/09
author: dk.zhang
purpose: 下载并解析服务器版本信息，并获取本地版本信息，判断是否更新等
*********************************************************************/
#ifndef VERINFOPARSE_H
#define VERINFOPARSE_H

//版本信息字段
#define  VerJson_Key_UpdateVersion   "UpdateVersion"
#define  VerJson_Key_UpdateType       "UpdateType"
#define  VerJson_Key_Channel            "Channel"
#define  VerJson_Key_Partial_From     "Partial_From"
#define  VerJson_Key_Desc                 "Desc"
#define  VerJson_Key_Details_Url        "Details_Url"
#define  VerJson_Key_Patch                 "Patch"

#define  VerJson_Key_Release_Time  "Release_Time"

//全包和补丁信息
#define  Patch_Key_Type                "type"
#define  Patch_Key_Size                 "size"
#define  Patch_Key_Url                   "url"
#define  Patch_Key_Hashfunc         "hashfunc"
#define  Patch_Key_Hashvalue        "hashvalue"

#include <QtCore>
#include "hzcore_global.h"



//包信息
struct Patch
{
    QString type;
    long long size;
    QString url;
};

//版本信息
struct VerInfo
{
    QString updateVersion;
    QString updateType;
    int         channel;
    QString partialFrom;
    QString releaseTime;

    QString desc;
    QString detialsUrl;

    //两个包
    Patch    completePatch;
    Patch    optionalPatch;
};

class HZCORESHARED_EXPORT VerInfoParse
{
public:
    enum   UpdateType
    {
        NoUpdate = 0,
        OptionalUpdate = 1,
        RequredUpdate = 2
    };

    VerInfoParse();
    VerInfoParse(const QString& fileName);

    void setVersionFile(const QString &fileName);

    const VerInfo &versionInfo() {return m_rmVerInfo;}

    //是否需要更新
    const UpdateType &updateType() {return m_updateType;}

private:
    bool  parseJson(const QString &fileName);

private:
    QString        m_Path;
    UpdateType m_updateType;
    //服务器版本
    VerInfo        m_rmVerInfo;
};

#endif // VERINFOPARSE_H
