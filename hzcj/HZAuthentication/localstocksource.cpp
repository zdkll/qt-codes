#include "localstocksource.h"

#define OPTION_STOCKS_FILE   "optionStocks.json"
#define HISTORY_STOCKS_FILE "historyStocks.json"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

#include "hzdirhelper.h"
#include "hzuserdatamgr.h"

LocalStockSource *LocalStockSource::m_zxStockSource = 0;
LocalStockSource *LocalStockSource::m_histStockSource = 0;

LocalStockSource::LocalStockSource(const QString &fileName)
    :m_fileName(fileName),m_isNeedSaved(false)
{

}

LocalStockSource::~LocalStockSource()
{

}

void LocalStockSource::init()
{
    LocalStockSource::globalZXInstance()->load();

    LocalStockSource::globalHistInstance()->load();
}

void LocalStockSource::saveStocks()
{
    if(LocalStockSource::globalZXInstance()->isNeedSaved())
        LocalStockSource::globalZXInstance()->save();
    if(LocalStockSource::globalHistInstance()->isNeedSaved())
        LocalStockSource::globalHistInstance()->save();
}

LocalStockSource *LocalStockSource::globalZXInstance()
{
    if(!m_zxStockSource){
        m_zxStockSource = new LocalStockSource(OPTION_STOCKS_FILE);
    }
    return m_zxStockSource;
}

LocalStockSource *LocalStockSource::globalHistInstance()
{
    if(!m_histStockSource){
        m_histStockSource = new LocalStockSource(HISTORY_STOCKS_FILE);
    }
    return m_histStockSource;
}

void LocalStockSource::unInit()
{
    if(m_zxStockSource){
        if(m_zxStockSource->isNeedSaved())
            m_zxStockSource->save();
        delete m_zxStockSource; m_zxStockSource = NULL;
    }
    if(m_histStockSource){
        if(m_histStockSource->isNeedSaved())
            m_histStockSource->save();
        delete m_histStockSource;m_histStockSource = NULL;
    }
}

bool LocalStockSource::updateStock(const QStringList &zxstocks, QString *errString)
{
    //自选股不能超过一百个限制
    if(m_fileName == OPTION_STOCKS_FILE && zxstocks.size() >=100){
        if(errString)
            *errString = QStringLiteral("自选股已经有100个，无法继续添加。");
        return false;
    }
    m_stocks.clear();
    m_stocks = zxstocks.toVector();
    m_isNeedSaved = true;
    return true;
}

bool LocalStockSource::addStock(const QString &code,QString *errString)
{
    int index = m_stocks.indexOf(code);
    if(index != -1)
        m_stocks.remove(index);
    //自选股不能超过一百个限制
    if(m_fileName == OPTION_STOCKS_FILE && m_stocks.size() >=100){
        if(errString)
            *errString = QStringLiteral("自选股已经有100个，无法继续添加。");
        return false;
    }
    m_stocks.prepend(code);
    if(m_stocks.size() >100){
        m_stocks.removeLast();
    }
    m_isNeedSaved = true;
    return true;
}

bool LocalStockSource::deleteStock(const QString &code,QString *errString)
{
    int pos = m_stocks.indexOf(code);
    if(pos != -1){
        m_stocks.remove(pos);
        m_isNeedSaved = true;
        return true;
    }
    if(errString)
        *errString = QStringLiteral("自选股不存在");
    return false;
}

void LocalStockSource::load()
{
    m_stocks.clear();
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc,getFilePath()))
    {
        auto obj = doc.object();
        auto  jsonArray = obj["optionStocks"].toArray();
        m_stocks.resize( jsonArray.size());
        for(int i=0;i < jsonArray.size();i++)
            m_stocks[i] = jsonArray[i].toObject().value("code").toString();
    }
    m_isNeedSaved = false;
}

void LocalStockSource::save()
{
    QJsonDocument doc;
    QJsonArray array;
    for(int i=0;i < m_stocks.size();i++)
    {
        QJsonObject obj;
        obj.insert("code",m_stocks[i]);
        array.append(obj);
    }
    QJsonObject obj;
    obj.insert("optionStocks",array);
    doc.setObject(obj);

    HzDirHelper::saveJson(doc,getFilePath());
    m_isNeedSaved = false;
}

QString LocalStockSource::getFilePath()
{
    QString path = HzUserDataMgr::getInstance()->getCurrUserDir();
    path.append("/");
    path.append(m_fileName);

    return path;
}
