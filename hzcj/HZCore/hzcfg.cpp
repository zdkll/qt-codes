#include "hzcfg.h"
#include "hzdirhelper.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QDir>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include "hzlog.h"

Hzcfg *Hzcfg::getInstance()
{
    static Hzcfg instance;
    return &instance;
}

void Hzcfg::loadCfg()
{
    //load cfg
    readLogCfg();
    readIndiInfo();
    readServer();
}

void Hzcfg::saveCfg()
{
    writeIndiInfo();
}

const InidItemList &Hzcfg::getIndiList() const
{
    return inidItemList_;
}

const IndiItemMap &Hzcfg::getIndiMap() const
{
    return indiItemMap_;
}

const IndiIndexMap &Hzcfg::getIndiIndexMap() const
{
    return indiIndexMap_;
}

void Hzcfg::setIndiInfo(const IndiItemInfo &info)
{
    for(unsigned int i =0;i < inidItemList_.size();i++)
    {
        if(inidItemList_[i].name == info.name)
        {
            inidItemList_[i] = info;
            break;
        }
    }
    for(auto it = indiIndexMap_.begin();it != indiIndexMap_.end();it++)
    {
        if(it->second.name == info.name)
        {
            it->second = info;
            break;
        }
    }
    auto it = indiItemMap_.find(info.name);
    if(it != indiItemMap_.end())
        it->second = info;
}

QString Hzcfg::getIndiInfoPath()
{
    QString path = getApplicationPath() + "/";
    path += "cfg/indiinfo.json";
    return path;
}

QString Hzcfg::getServerInfoFile()
{
    QString path = getApplicationPath() + "/";
    path += "cfg/server.json";
    return path;
}

QString Hzcfg::getApplicationPath()
{
    return qApp->applicationDirPath() ;
}

QString Hzcfg::getExePath()
{
    return qApp->applicationFilePath();
}

QString Hzcfg::getLogCfgPath()
{
    QString path = getApplicationPath() + "/";
    path += "cfg/log.cfg";
    return path;
}

void Hzcfg::readLogCfg()
{
    QString path = getLogCfgPath();
    if(path.isEmpty()) return;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))return;
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if(!doc.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        mLogCfgs.clear();
        QVariantMap cgMap = doc.toVariant().toMap()["logCfg"].toMap()["categories"].toMap();
        QMapIterator<QString, QVariant> it(cgMap);
        while(it.hasNext()){
            it.next();
            QList<STCategories> appends;
            for(QVariant &var : it.value().toList()) {
                STCategories stcg;
                QVariantMap propertyMap = var.toMap();
                stcg.name       = propertyMap["name"].toString();
                stcg.priority   = propertyMap["priority"].toString();
                stcg.appender   = propertyMap["appender"].toString();
                stcg.fileName   = propertyMap["fileName"].toString();
                stcg.layout     = propertyMap["layout"].toString();
                stcg.conversionPattern = propertyMap["ConversionPattern"].toString();
                appends << stcg;
            }
            mLogCfgs[it.key()] = appends;
        }
    }

    file.close();

    //日志初始化
    gHZLogIns.initialization();
}

LogSTCateoriesHash Hzcfg::getLogCfgData()
{
    return mLogCfgs;
}

QString Hzcfg::getShortCutCfgPath()
{
    QString path = getApplicationPath() + "/";
    path += "cfg/shortcut/shortcut.json";
    return path;
}

void Hzcfg::readIndiInfo()
{
    HZ_AUTO_ELAPSED_TIMER;

    QJsonDocument doc;
    if(HzDirHelper::readJson(doc,getIndiInfoPath()))
    {
        if(doc.isObject())
        {
            auto obj = doc.object();
            if(obj.contains("Indi"))
            {
                auto  value = obj["Indi"];
                if(value.isArray())
                {
                    auto jsonArray = value.toArray();
                    for(int i=0;i < jsonArray.size();i++)
                    {
                        if(jsonArray[i].isObject())
                        {
                            auto indiObj = jsonArray[i].toObject();
                            IndiItemInfo info;
                            info.id = i;
                            if(indiObj.contains("name") && indiObj["name"].isString())
                            {
                                info.name = indiObj["name"].toString().toStdString();
                                if(indiObj.contains("script") && indiObj["script"].isString())
                                {
                                    info.script = indiObj["script"].toString().toStdString();
                                }
                                if(indiObj.contains("screenname") && indiObj["screenname"].isString())
                                {
                                    info.screenname = indiObj["screenname"].toString().toStdString();
                                }
                                if(indiObj.contains("bLocalCalc") && indiObj["bLocalCalc"].isBool())
                                {
                                    info.bLocalCalc = indiObj["bLocalCalc"].toBool();
                                }
                                if(indiObj.contains("drawPan"))
                                {
                                    info.drawArea = indiObj["drawPan"].toInt();
                                }
                                if(indiObj.contains("param") && indiObj["param"].isArray())
                                {
                                    auto paramArray = indiObj["param"].toArray();

                                    for(int i=0;i < paramArray.size();i++)
                                    {
                                        if(paramArray[i].isObject())
                                        {
                                            IndiParam param;
                                            auto paramObj = paramArray[i].toObject();
                                            if(paramObj.contains("name") && paramObj["name"].isString())
                                            {
                                                param.name = paramObj["name"].toString().toStdString();
                                                if(paramObj.contains("value"))
                                                {
                                                    param.value = paramObj["value"].toInt();
                                                }
                                                if(paramObj.contains("minvalue"))
                                                {
                                                    param.minValue = paramObj["minvalue"].toInt();
                                                }
                                                if(paramObj.contains("maxvalue"))
                                                {
                                                    param.maxValue = paramObj["maxvalue"].toInt();
                                                }
                                                if(paramObj.contains("des") && paramObj["des"].isString())
                                                {
                                                    param.des = paramObj["des"].toString().toStdString();
                                                }
                                                info.params.push_back(param);
                                                info.paramMap[param.name] = param;
                                            }
                                        }
                                    }
                                }
                                if(indiObj.contains("result") && indiObj["result"].isArray())
                                {
                                    auto resultArray = indiObj["result"].toArray();
                                    for(int i=0;i < resultArray.size();i++)
                                    {
                                        IndiResult result;
                                        auto resultObj = resultArray[i].toObject();
                                        if(resultObj.contains("screenname") && resultObj["screenname"].isString())
                                        {
                                            result.screenname = resultObj["screenname"].toString().toStdString();
                                            if(resultObj.contains("isVisible"))
                                            {
                                                result.isVisible = resultObj["isVisible"].toInt();
                                            }
                                            if(resultObj.contains("color") && resultObj["color"].isString())
                                            {
                                                QString clr = resultObj["color"].toString();
                                                result.clrMap[0]  = QColor(clr);

                                            }
                                            if(resultObj.contains("linewidth"))
                                            {
                                                result.linewidth = resultObj["linewidth"].toInt();
                                            }
                                            if(resultObj.contains("dsc") && resultObj["dsc"].isString())
                                            {
                                                result.dsc = resultObj["dsc"].toString().toStdString();
                                            }
                                            if(resultObj.contains("drawType"))
                                            {
                                                result.drawType = resultObj["drawType"].toInt();
                                            }
                                            if(resultObj.contains("icon") && resultObj["icon"].isString())
                                            {
                                                result.icon = resultObj["icon"].toString().toStdString();
                                            }
                                            if(resultObj.contains("bShowName") && resultObj["bShowName"].isBool())
                                            {
                                                result.bShowName = resultObj["bShowName"].toBool();
                                            }
                                            info.results.push_back(result);
                                            info.resultMap[result.screenname] = result;
                                        }
                                    }
                                }
                                inidItemList_.push_back(info);
                                indiItemMap_[info.name] = info;
                                indiIndexMap_[info.id] = info;
                            }
                        }
                    }
                }
            }
        }
    }
}

#include <QDebug>
bool Hzcfg::readServer()
{
    HZ_AUTO_ELAPSED_TIMER;

    QString fileName = getServerInfoFile();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(doc.isNull())
        return false;

    QJsonObject rootObj = doc.object();

    m_serverInfo.updateInfoUrl = rootObj.value(Server_Key_Update_Version).toString();
    QString Static_Stock_Url = rootObj.value("Static_Stock_Url").toString();

    m_serverInfo.counturl = Static_Stock_Url+"/getfile/content-with-count";
    m_serverInfo.plateurl = Static_Stock_Url+"/getfile/plateinfo";
    m_serverInfo.indexurl = Static_Stock_Url+"/getfile/indexinfo";
    m_serverInfo.indexStockChildListUrl = Static_Stock_Url+"/getfile/indexstocks";
    m_serverInfo.stockurl = Static_Stock_Url+"/getfile/stockinfo";
    m_serverInfo.authurl  = rootObj.value(Server_Key_AuthUrl).toString();
    m_serverInfo.apiurl  = rootObj.value(Server_Key_ApiUrl).toString();
    m_serverInfo.infoSysUrl = rootObj.value(Server_Key_InfoSys).toString();
    m_serverInfo.infoStsH5Url = rootObj.value(Server_Key_InfoSys_H5).toString();
    m_serverInfo.infoQuoteUrl = rootObj.value(Server_Key_QuoteSys).toString();
    m_serverInfo.dmpUploadUrl =  rootObj.value(Server_Key_DumpUrl).toString();
    m_serverInfo.webproducturl = rootObj.value(Server_Key_Web_Product).toString();
    qDebug() << Q_FUNC_INFO << m_serverInfo.infoStsH5Url;

    QJsonObject dataSeverObj = rootObj.value(Server_Key_DataServer).toObject();
    m_serverInfo.quoteSrv.ip = dataSeverObj.value(Server_Key_Sever_IP).toString();
    m_serverInfo.quoteSrv.port = dataSeverObj.value(Server_Key_Sever_Port).toInt();
    return true;
}

void Hzcfg::writeIndiInfo()
{

}

Hzcfg::Hzcfg()
{

}
