#ifndef HZUSERDATAMGR_H
#define HZUSERDATAMGR_H
/********************************************************************
created:
author: min.cheng
purpose:用户数据获取，管理功能  其中包括公共用户数据目录，在初始执行的时候会创建公共用户数据目录
*********************************************************************/

#include <QObject>
#include <memory>
#include "hzcore_global.h"

class HZCORESHARED_EXPORT HzUserDataMgr : public QObject
{
    Q_OBJECT
public:
    //单例对象获取
    static HzUserDataMgr* getInstance();

    //切换用户后调用，用于用户数据目录切换
    void switchUser(const QString& userId,bool isTtourists);

    //获取当前用户目录
    QString getCurrUserDir();

    //获取当前用户目录
    QString getAllUsersDir(){return allUserDir_;}

signals:

public slots:

private:
    explicit HzUserDataMgr(QObject *parent = 0);
private:
    //用户数据根目录
    QString rootDir_;
    //所有用户目录
    QString allUserDir_;
    //d
    QString currUserDir_;

};

#endif // HZUSERDATAMGR_H
