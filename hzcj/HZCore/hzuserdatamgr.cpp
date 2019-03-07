#include "hzuserdatamgr.h"
#include "hzdirhelper.h"
#include <QStandardPaths>
#include <QDir>

#define SHHZCJ_DIR    "shhzcjEx"
#define HZ_USER_DIR "/Hzcj Files"
#define ALL_USER_DIR "/All Users"
#define TOURISTS_DIR "tourists"

#include <QDebug>
HzUserDataMgr::HzUserDataMgr(QObject *parent) : QObject(parent)
{
    rootDir_ = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(rootDir_);
    dir.cdUp();
    rootDir_ = dir.absoluteFilePath(SHHZCJ_DIR);
    dir.setPath(rootDir_);
    if(!dir.exists())
        dir.mkdir(rootDir_);

    allUserDir_ = rootDir_;
    allUserDir_.append(ALL_USER_DIR);
    allUserDir_.append("/");

    //检测公共用户目录是否存在，否则创建
    HzDirHelper::createDir(allUserDir_);
}

HzUserDataMgr *HzUserDataMgr::getInstance()
{
    static HzUserDataMgr mgr;
    return &mgr;
}

void HzUserDataMgr::switchUser(const QString &userId,bool isTourists)
{
    //切换用户前
    currUserDir_ = rootDir_;
    currUserDir_.append("/");
    if(isTourists){
        currUserDir_.append(TOURISTS_DIR);
    }else
    {
        currUserDir_.append(userId);
    }
    HzDirHelper::createDir(currUserDir_);
}

QString HzUserDataMgr::getCurrUserDir()
{
    return currUserDir_;
}


