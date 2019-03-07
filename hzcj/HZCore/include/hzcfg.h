#ifndef HZCFG_H
#define HZCFG_H
/********************************************************************
created:  2018-4-17
author: min.cheng
purpose: 简单的配置读取类
*********************************************************************/

#include "hzcore_global.h"
#include "cfgdef.hpp"
#include <map>
#include <vector>
#include <QVariantMap>

class QJsonDocument;
class HZCORESHARED_EXPORT Hzcfg
{
public:
    static Hzcfg* getInstance();

    /**
     * @brief 加载配置，一般在登陆成功后加载配置 （不同用户数据不一样）
     */
    void loadCfg();

    /**
     * @brief 存储配置  一般在退出或切换用户是存储
     */
    void saveCfg();

    /**
     * @brief 获取指标数据
     */
    const InidItemList& getIndiList() const;
    const IndiItemMap& getIndiMap() const;
    const IndiIndexMap& getIndiIndexMap() const;
    void setIndiInfo(const IndiItemInfo& info);

    /**
     * @brief 获取服务器信息
     */
    const ServerInfo&   serverInfo()const {return m_serverInfo;}

    //log cfg
    QString getLogCfgPath();
    void readLogCfg();
    LogSTCateoriesHash getLogCfgData();

    //shortcut cfg path
    QString getShortCutCfgPath();

private:
    QString getApplicationPath();
    QString getExePath();
    QString getIndiInfoPath();

    void readIndiInfo();
    void writeIndiInfo();

    //server
    QString getServerInfoFile();
    bool      readServer();

private:
    Hzcfg();
    QString path_;
    InidItemList inidItemList_;
    IndiItemMap indiItemMap_;
    IndiIndexMap indiIndexMap_;

    QVariantMap    m_serverInfoMap;
    ServerInfo         m_serverInfo;

    LogSTCateoriesHash mLogCfgs;
};

#endif // HZCFG_H
