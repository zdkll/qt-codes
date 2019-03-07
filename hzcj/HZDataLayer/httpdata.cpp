#include "httpdata.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <hzdirhelper.h>
#include <hzcfg.h>
#include <hzlog.h>
#include <QMap>
#include "httpclient.h"
#include "replytimeout.h"
#include "HChineseConvertToPY.h"


using namespace HZData;

Httpdata::Httpdata():
    m_httpClient(new HttpClient(this))
{
    m_strAppPath = qApp->applicationDirPath() + "/cfg/data/";
    m_funCount.finishedFun = std::bind(&Httpdata::onCountDataArrived,this,std::placeholders::_1,std::placeholders::_2);
    m_funPlate.finishedFun = std::bind(&Httpdata::onPlateDataArrived,this,std::placeholders::_1,std::placeholders::_2);
    m_funIndex.finishedFun = std::bind(&Httpdata::onIndexDataArrived,this,std::placeholders::_1,std::placeholders::_2);
    m_funStock.finishedFun = std::bind(&Httpdata::onStockDataArrived,this,std::placeholders::_1,std::placeholders::_2);


    //赋值重要指数
    m_impStockListVec= {"sh000001","sz399001","sz399005"
                        ,"sz399006","sh000300","sh000016"
                        ,"sh000132","sh000003","sh000012"
                        ,"sh000013","sz399106"
                        ,"sz399004","sz399007","sz399008"
                        ,"sz399100","sz399550","sh000903"};
    for(int i=0;i < m_impStockListVec.size();i++)
    {
        m_impStocksMap[m_impStockListVec[i]] = m_impStockListVec[i];
    }
}

Httpdata::~Httpdata()
{
    m_httpClient->cancle(m_reqIndexId);
    m_httpClient->cancle(m_reqPlaterId);
    m_httpClient->cancle(m_reqCountId);
    m_httpClient->cancle(m_reqstockId);

}

int Httpdata::getCountByFiter(int filter,QString bkCode)
{
    if(filterCountMap_.isEmpty())
        loadCountData();
    int count = -1;
    auto it = filterCountMap_.find(filter);
    if(it != filterCountMap_.end())
    {
        if(bkCode.isEmpty())
            count = it.value();
        else
        {
            QString key = bkCode;
            //key.append(QString::number(filter));
            auto iter = bkListCountMap_.find(key);
            if(iter != bkListCountMap_.end())
            {
                count = iter.value();
            }
        }
    }
    return count;
}

int Httpdata::getfilterByBkCode(const QString &bkCode)
{
    if(bkCodefilterMap_.isEmpty())
        loadCountData();
    int filter = -1;
    auto it = bkCodefilterMap_.find(bkCode);
    if(it != bkCodefilterMap_.end())
    {
        filter = it.value();
    }
    return filter;
}

bool Httpdata::isHasObjFromFilter(const QString &obj, int filter)
{
    QMap<QString,HZData::StockTable> out;
    getListMapByFilter(out,filter);
    bool bRet = false;
    auto it = out.find(obj);
    if(it != out.end())
        bRet = true;
    return bRet;
}

void Httpdata::getListMapByFilter(QMap<QString,HZData::StockTable>& out,int filter,QString bkCode)
{
    auto it = filterListMap_.find(filter);
    if(it == filterListMap_.end())
    {
        preLoade(filter);
        it  = filterListMap_.find(filter);
    }
    if(bkCode.isEmpty())
    {
        if(it != filterListMap_.end())
        {
            out = filterListMap_[filter].stockMap;
        }
    }else
    {
        auto iter =  bkMap_.find(bkCode);
        if(iter != bkMap_.end())
        {
            out = iter.value().stockMap;
        }
    }
}

void Httpdata::getListVecByFilter(QVector<HZData::StockTable>& out,int filter, QString bkCode)
{
    auto it = filterListMap_.find(filter);
    if(it == filterListMap_.end())
    {
        preLoade(filter);
        it  = filterListMap_.find(filter);
    }
    if(bkCode.isEmpty())
    {
        if(it != filterListMap_.end())
        {
            out = filterListMap_[filter].stockVec;
        }
    }else
    {
        auto iter =  bkMap_.find(bkCode);
        if(iter != bkMap_.end())
        {
            out = iter.value().stockVec;
        }
    }
}

int Httpdata::getFilterByCode(const QString &code)
{
    int ret = getfilterByBkCode(code);//板块中查找
    if(ret == -1)
    {
        QMap<QString,HZData::StockTable> out;
        getListMapByFilter(out,HZData::MarketSZ);
        auto it = out.find(code);
        if(it != out.end()) //深证市场查找
        {
            ret = HZData::MarketSZ;
        }else//上证市场查找
        {
            getListMapByFilter(out,HZData::MarketSH);
            it = out.find(code);
            if(it != out.end())
            {
                ret = HZData::MarketSH;
            }else  //指数中查找
            {
                getListMapByFilter(out,HZData::IndexInner);
                it = out.find(code);
                if(it != out.end())
                {
                    ret = HZData::IndexInner;
                }
            }
        }
    }
    return ret;
}

QString Httpdata::getCodeNameByCode(const QString& code)
{
    QString objName = "";
    QMap<QString,HZData::StockTable> out;
    getListMapByFilter(out,HZData::MarketSZ);
    auto it = out.find(code);
    if(it != out.end()) //深证市场查找
    {
        objName = it.value().stock_name;
    }else
    {
        getListMapByFilter(out,HZData::MarketSH); //上证市场查找
        it = out.find(code);
        if(it != out.end())
        {
            objName = it.value().stock_name;
        }else
        {
            out.clear();
            getListMapByFilter(out,HZData::BlockWhole);
            it = out.find(code);
            if(it != out.end()) //板块中查找
            {
                objName = it.value().stock_name;
            }else
            {
                out.clear();
                getListMapByFilter(out,HZData::IndexInner); //指数中查找
                it = out.find(code);
                if(it != out.end())
                {
                    objName = it.value().stock_name;
                }
            }
        }
    }
    return objName;
}

QString Httpdata::getCodeNameByStocks(const QString &code)
{
    QString objName = "";
    QMap<QString,HZData::StockTable> out;
    getListMapByFilter(out,HZData::MarketSZ);
    auto it = out.find(code);
    if(it != out.end()) //深证市场查找
    {
        objName = it.value().stock_name;
    }else
    {
        getListMapByFilter(out,HZData::MarketSH); //上证市场查找
        it = out.find(code);
        if(it != out.end())
        {
            objName = it.value().stock_name;
        }
    }
    return objName;
}

QString Httpdata::getCodeNameByCodeFromBk(const QString &code)
{
    QString objName = "";
    QMap<QString,HZData::StockTable> out;
    getListMapByFilter(out,HZData::BlockWhole);
    auto it = out.find(code);
    if(it != out.end()) //板块中查找
    {
        objName = it.value().stock_name;
    }
    return objName;
}

QString Httpdata::getCodeNameByCodeFromIndex(const QString &code)
{
    QString objName = "";
    QMap<QString,HZData::StockTable> out;
    getListMapByFilter(out,HZData::IndexInner); //指数中查找
    auto it = out.find(code);
    if(it != out.end()) //指数中查找
    {
        objName = it.value().stock_name;
    }
    return objName;
}
#include <QTextCodec>
QVector<BlockFormatDataItem> Httpdata::getFormatDataFromBkList(int filter)
{
    QVector<BlockFormatDataItem> retVec;
    auto it = m_blockFormatDataMap.find(filter);
    if(it == m_blockFormatDataMap.end())
    {
        QVector<HZData::StockTable> out;
        QVector<HZData::StockTable> numList;
        getListVecByFilter(out,filter);
        QMap<QString,QVector<HZData::StockTable>> retmap;
        if(!out.isEmpty())
        {
            std::string in,ot;
            QString otStr;
            char* pCh = nullptr;
            QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
            QTextCodec::setCodecForLocale(utf8);
//            QTextCodec::setCodecForCStrings(utf8);
            QTextCodec* gbk = QTextCodec::codecForName("gbk");
            for(int i=0;i < out.size();i++)
            {
                //utf8 -> gbk
                //1. utf8 -> unicode
                QString strUnicode= utf8->toUnicode(out[i].stock_name.toLocal8Bit().data());
                //2. unicode -> gbk, 得到QByteArray
                in = gbk->fromUnicode(strUnicode).data();
                if(HChineseConvertToPY::standingInitial(in.data(),pCh)!=0)
                {
                    otStr = pCh;
                    otStr = otStr.toUpper();
                    otStr = otStr.mid(0,1);
                    bool bNumber = false;
                    otStr.toInt(&bNumber);
                    if(bNumber)
                    {
                        numList.push_back(out[i]);
                    }else
                    {
                        auto iter = retmap.find(otStr);
                        if(iter != retmap.end())
                        {
                            iter.value().push_back(out[i]);
                        }else
                        {
                            QVector<HZData::StockTable> vec;
                            vec.push_back(out[i]);
                            retmap[otStr] = vec;
                        }
                    }
                }
            }
            int begin = QChar('A').unicode();
            int end = QChar('Z').unicode()+1;
            QString key = "";
            do
            {
                BlockFormatDataItem item;
                if(begin > end)
                    break;
                key = QChar(begin);
                auto it = retmap.find(QString((char)begin));
                if(it != retmap.end())
                {
                    item.dataList.append(it.value());
                }
                begin++;
                if(begin < end)
                {
                    key += QChar(begin);
                    it = retmap.find(QString((char)begin));
                    if(it != retmap.end())
                    {
                        item.dataList.append(it.value());
                    }
                }
                item.firstChar = key;
                if(!item.dataList.isEmpty())
                    retVec.push_back(item);
                begin++;
            }while(true);

            //处理数字部分
            if(!numList.isEmpty())
            {
                BlockFormatDataItem item;
                item.firstChar = "#";
                item.dataList.append(numList);
                retVec.push_back(item);
            }
            if(!retVec.isEmpty())
            {
                m_blockFormatDataMap[filter] = retVec;
            }
        }
    }else
    {
        return it.value();
    }
    return retVec;
}

QMap<QString,QString> Httpdata::getImpIndexStockMap()
{
    return  m_impStocksMap;
}

QVector<QString> Httpdata::getImpIndexStockVec()
{
    return m_impStockListVec;
}

QString Httpdata::filterCodeToName(int filter)
{
    FilterFlag type = (FilterFlag)filter;
    QString str = "";
    switch (type) {
    case HZData::MarketAll:
        str = QStringLiteral("全部市场");
        break;
    case HZData::MarketSZ:
        str = QStringLiteral("深证市场");
        break;
    case HZData::MarketSH:
        str = QStringLiteral("上证市场");
        break;
    case HZData::BlockIndustry:
        str = QStringLiteral("行业板块");
        break;
    case HZData::BlockConcept:
        str = QStringLiteral("概念板块");
        break;
    case HZData::BlockRegion:
        str = QStringLiteral("地域板块");
        break;
    case HZData::BlockWhole:
        str = QStringLiteral("全部板块");
        break;
    case HZData::IndexSH:
        str = QStringLiteral("上证系列指数");
        break;
    case IndexSZ:
        str = QStringLiteral("深圳系列指数");
        break;
    case HZData::IndexInner:
        str = QStringLiteral("全部指数");
        break;
//    case IndexOutter:
//        str = QStringLiteral("国外指数");
//        break;
    case ZXStocks:
        str = QStringLiteral("自选股");
        break;
    default:
        break;
    }
    return str;

}


void Httpdata::init()
{
    loadFromHttp();
}

void Httpdata::getBkListByStock(QVector<FilterDetail> &out, const QString &stock)
{
    auto it =  bkSearchCache_.find(stock);
    if(it != bkSearchCache_.end())
    {
         out = it.value();
         return;
    }
    if(bkVec_.isEmpty())
        loadPlateData();
    for(int i=0;i < bkVec_.size();i++)
    {
        auto it = bkVec_[i].stockMap.find(stock);
        if(it != bkVec_[i].stockMap.end())
            out.push_back(bkVec_[i]);
    }
    bkSearchCache_[stock] = out;
}

void Httpdata::loadFromHttp()
{
    loadCountData();
    loadPlateData();
    loadIndexData();
    loadStockData();
    readIndexStockChildList();
}

void Httpdata::readCountData()
{
    QString strPath = m_strAppPath + "countinfo.json";
    if(strPath.isEmpty()) return;
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc, strPath))
    {
        if(doc.isObject())
        {
            auto obj = doc.object();
            if(obj.contains("indexinfo"))
            { 
                int filter = -1;
                int count = -1;
                int innerCount = -1;
                auto indexInfo = obj["indexinfo"];
                if(indexInfo.isArray())
                {
                    filterCountMap_.clear();
                    auto jsonArray = indexInfo.toArray();
                    for(int i = 0; i < jsonArray.size(); i++)
                    {
                        if(jsonArray[i].isObject())
                        {
                            auto countInfoObj = jsonArray[i].toObject();
                            if(countInfoObj.contains("filterflag"))
                            {
                                filter = countInfoObj["filterflag"].toInt();
                                if(countInfoObj.contains("count"))
                                {
                                    count = countInfoObj["count"].toInt();
                                    filterCountMap_[filter] = count;
                                    if(filter == HZData::IndexSH || filter == IndexSZ)
                                        innerCount += count;
                                }
                            }
                        }
                    }
                    //添加全部指数
                     if(innerCount != -1)
                     {
                         filterCountMap_[HZData::IndexInner] = innerCount;
                     }
                }

            }

            if(obj.contains("plateinfo"))
            {
                int filter = -1;
                int count = -1;
                int totalCount = -1;
                auto plateInfo = obj["plateinfo"];
                if(plateInfo.isObject())
                {
                    auto  value = plateInfo.toObject();
                    if(value.contains("bkCategory"))
                    {
                        auto bkCategoryArray = value["bkCategory"];
                        if(bkCategoryArray.isArray())
                        {
                            auto jsonArray = bkCategoryArray.toArray();
                            for(int i = 0; i < jsonArray.size(); i++)
                            {
                                if(jsonArray[i].isObject())
                                {
                                    auto countInfoObj = jsonArray[i].toObject();
                                    if(countInfoObj.contains("filterflag"))
                                    {
                                        filter = countInfoObj["filterflag"].toInt();
                                        if(countInfoObj.contains("count"))
                                        {
                                            count = countInfoObj["count"].toInt();
                                            totalCount += count;
                                            filterCountMap_[filter] = count;
                                        }
                                    }
                                }
                            }
                            if(totalCount != -1)
                            {
                                filterCountMap_[(int)HZData::BlockWhole] = totalCount;
                            }
                        }
                    }

                    if(value.contains("bkInfo"))
                    {
                        int filter = -1;
                        int count = -1;
                        auto bkInfoArray = value["bkInfo"];
                        if(bkInfoArray.isArray())
                        {
                            auto jsonArray = bkInfoArray.toArray();
                            for(int i = 0; i < jsonArray.size(); i++)
                            {
                                if(jsonArray[i].isObject())
                                {
                                    auto countInfoObj = jsonArray[i].toObject();
                                    if(countInfoObj.contains("filterflag"))
                                    {
                                        filter = countInfoObj["filterflag"].toInt();
                                        if(countInfoObj.contains("count"))
                                        {
                                            count = countInfoObj["count"].toInt();
                                            if(countInfoObj.contains("code"))
                                            {
                                                QString code = countInfoObj["code"].toString();
                                                bkCodefilterMap_[code] = filter;
                                                //code.append(QString::number(filter));
                                                bkListCountMap_[code] = count;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if(obj.contains("stockinfo"))
            {
                int filter = -1;
                int count = -1;
                int totalCount = -1;
                auto stockInfo = obj["stockinfo"];
                if(stockInfo.isArray())
                {
                    auto jsonArray = stockInfo.toArray();
                    for(int i = 0; i < jsonArray.size(); i++)
                    {
                        if(jsonArray[i].isObject())
                        {
                            auto countInfoObj = jsonArray[i].toObject();
                            if(countInfoObj.contains("filterflag"))
                            {
                                filter = countInfoObj["filterflag"].toInt();
                                if(countInfoObj.contains("count"))
                                {
                                    count = countInfoObj["count"].toInt();
                                    totalCount += count;
                                    filterCountMap_[filter] = count;
                                }
                            }
                        }
                    }
                    if(totalCount != -1)
                    {
                        filterCountMap_[(int)HZData::MarketAll] = totalCount;
                    }
                }
            }
        }
    }
}

void Httpdata::readPlateData()
{
    QString strPath = m_strAppPath + "plateinfo.json";
    if(strPath.isEmpty()) return;
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc, strPath))
    {
        if(doc.isObject())
        {
            auto obj = doc.object();
            //1.2 bkCategory
            if(obj.contains("bkCategory"))
            {
                FilterDetail totalBk;
                totalBk.filterflag = HZData::BlockWhole;
                totalBk.stock_name = QStringLiteral("全部板块");
                auto bkCategoryArray = obj["bkCategory"];
                if(bkCategoryArray.isArray())
                {
                    auto jsonArray = bkCategoryArray.toArray();
                    for(int i = 0; i < jsonArray.size(); i++)
                    {
                        if(jsonArray[i].isObject())
                        {
                            auto bkCategoryObj = jsonArray[i].toObject();
                            FilterDetail bkCateGory;
                            if(bkCategoryObj.contains("filterflag"))
                            {
                                bkCateGory.filterflag = bkCategoryObj["filterflag"].toInt();
                                if(bkCategoryObj.contains("name") && bkCategoryObj["name"].isString())
                                {
                                    bkCateGory.stock_name = bkCategoryObj["name"].toString();
                                }
                                if(bkCategoryObj.contains("stocks") && bkCategoryObj["stocks"].isArray())
                                {
                                    auto stocksArray = bkCategoryObj["stocks"].toArray();
                                    for(int i = 0; i < stocksArray.size(); i++)
                                    {
                                        if(stocksArray[i].isObject())
                                        {
                                            HZData::StockTable stock;
                                            auto stockObj = stocksArray[i].toObject();
                                            if(stockObj.contains("code") && stockObj["code"].isString())
                                            {
                                                stock.stock_code = stockObj["code"].toString();
                                            }
                                            if(stockObj.contains("name") && stockObj["name"].isString())
                                            {
                                                stock.stock_name = stockObj["name"].toString();
                                            }
                                            totalBk.stockVec.push_back(stock);
                                            totalBk.stockMap[stock.stock_code] = stock;
                                            bkCateGory.stockVec.push_back(stock);
                                            bkCateGory.stockMap[stock.stock_code] = stock;
                                        }
                                    }
                                }
                                filterListMap_.insert(bkCateGory.filterflag,bkCateGory);
                            }
                        }
                    }
                    if(!totalBk.stockMap.isEmpty())
                        filterListMap_.insert(HZData::BlockWhole,totalBk);
                }
            }

            //1.1 bkInfo
            if(obj.contains("bkInfo"))
            {
                auto bkInfoArray = obj["bkInfo"];
                if(bkInfoArray.isArray())
                {
                    auto jsonArray = bkInfoArray.toArray();
                    int size = jsonArray.size();
                    for(int i = 0; i < jsonArray.size(); i++)
                    {

                        if(jsonArray[i].isObject())
                        {
                            auto &bkDetailObj = jsonArray[i].toObject();
                            FilterDetail detail;
                            if(bkDetailObj.contains("filterflag"))
                            {
                                detail.filterflag = bkDetailObj["filterflag"].toInt();
                                if(bkDetailObj.contains("name") && bkDetailObj["name"].isString())
                                {
                                    detail.stock_name = bkDetailObj["name"].toString();
                                }
                                if(bkDetailObj.contains("code") && bkDetailObj["code"].isString())
                                {
                                    detail.stock_code = bkDetailObj["code"].toString();
                                }
                                if(bkDetailObj.contains("stocks") && bkDetailObj["stocks"].isArray())
                                {
                                    auto stocksArray = bkDetailObj["stocks"].toArray();
                                    for(int i = 0; i < stocksArray.size(); i++)
                                    {
                                        if(stocksArray[i].isObject())
                                        {
                                            HZData::StockTable stock;
                                            auto &stockObj = stocksArray[i].toObject();
                                            if(stockObj.contains("code") && stockObj["code"].isString())
                                            {
                                                stock.stock_code = stockObj["code"].toString();
                                            }
                                            if(stockObj.contains("name") && stockObj["name"].isString())
                                            {
                                                stock.stock_name = stockObj["name"].toString();
                                            }
                                            detail.stockVec.push_back(stock);
                                            detail.stockMap.insert(stock.stock_code, stock);
                                        }
                                    }
                                }
                                bkMap_[detail.stock_code] = detail;
                                bkVec_.push_back(detail);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Httpdata::readIndexData()
{
    QString strPath = m_strAppPath + "indexinfo.json";
    if(strPath.isEmpty()) return;
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc, strPath))
    {
        FilterDetail innerIndex;
        innerIndex.filterflag = HZData::IndexInner;
        innerIndex.stock_name = QString(QStringLiteral("国内指数"));
        auto jsonArray = doc.array();
        //2. indexinfo
        for(int i = 0; i < jsonArray.size(); i++)
        {
            if(jsonArray[i].isObject())
            {
                auto bkCategoryObj = jsonArray[i].toObject();
                FilterDetail category;
                if(bkCategoryObj.contains("filterflag"))
                {
                    category.filterflag = bkCategoryObj["filterflag"].toInt();
                    if(bkCategoryObj.contains("name") && bkCategoryObj["name"].isString())
                    {
                        category.stock_name = bkCategoryObj["name"].toString();
                    }

                    if(bkCategoryObj.contains("stocks") && bkCategoryObj["stocks"].isArray())
                    {
                        auto stocksArray = bkCategoryObj["stocks"].toArray();
                        for(int i = 0; i < stocksArray.size(); i++)
                        {
                            if(stocksArray[i].isObject())
                            {
                                HZData::StockTable stock;
                                auto stockObj = stocksArray[i].toObject();
                                if(stockObj.contains("code") && stockObj["code"].isString())
                                {
                                    stock.stock_code = stockObj["code"].toString();
                                }
                                if(stockObj.contains("name") && stockObj["name"].isString())
                                {
                                    stock.stock_name = stockObj["name"].toString();
                                }
                                if(category.filterflag == HZData::IndexSH || category.filterflag == IndexSZ)
                                {
                                    innerIndex.stockVec.push_back(stock);
                                    innerIndex.stockMap[stock.stock_code] = stock;
                                }
                                category.stockVec.push_back(stock);
                                category.stockMap[stock.stock_code] = stock;
                            }
                        }
                    }
                    filterListMap_.insert(category.filterflag, category);
                }
            }
        }
        if(!innerIndex.stockMap.isEmpty())
            filterListMap_.insert(HZData::IndexInner,innerIndex);
    }
}

void Httpdata::readStockData()
{
    QString strPath = m_strAppPath + "stockinfo.json";
    if(strPath.isEmpty()) return;
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc, strPath))
    {
        FilterDetail marketAll;
        marketAll.filterflag = HZData::MarketAll;
        marketAll.stock_name = QString(QStringLiteral("沪深A股"));
        auto jsonArray = doc.array();
        //3. stockinfo
        for(int i = 0; i < jsonArray.size(); i++)
        {
            if(jsonArray[i].isObject())
            {
                auto bkCategoryObj = jsonArray[i].toObject();
                FilterDetail category;
                if(bkCategoryObj.contains("filterflag"))
                {
                    category.filterflag = bkCategoryObj["filterflag"].toInt();
                    if(bkCategoryObj.contains("name") && bkCategoryObj["name"].isString())
                    {
                        category.stock_name = bkCategoryObj["name"].toString();
                    }
                    if(bkCategoryObj.contains("stocks") && bkCategoryObj["stocks"].isArray())
                    {
                        auto stocksArray = bkCategoryObj["stocks"].toArray();
                        for(int i = 0; i < stocksArray.size(); i++)
                        {
                            if(stocksArray[i].isObject())
                            {
                                HZData::StockTable stock;
                                auto stockObj = stocksArray[i].toObject();
                                if(stockObj.contains("code") && stockObj["code"].isString())
                                {
                                    stock.stock_code = stockObj["code"].toString();
                                }
                                if(stockObj.contains("name") && stockObj["name"].isString())
                                {
                                    stock.stock_name = stockObj["name"].toString();
                                }
                                marketAll.stockVec.push_back(stock);
                                marketAll.stockMap[stock.stock_code] = stock;

                                category.stockVec.push_back(stock);
                                category.stockMap[stock.stock_code] = stock;
                            }
                        }
                    }
                    filterListMap_.insert(category.filterflag, category);
                }
            }
        }
        if(!marketAll.stockMap.isEmpty())
            filterListMap_.insert(HZData::MarketAll,marketAll);
    }
}

void Httpdata::readIndexStockChildList()
{
    QString strPath = m_strAppPath + "indexStockChildList.json";
    if(strPath.isEmpty()) return;
    QJsonDocument doc;
    if(HzDirHelper::readJson(doc, strPath) && doc.isObject())
    {
       QMap<QString, QVariant> docMap =doc.toVariant().toMap();
       if(docMap.contains("index_info"))
       {
           QMap<QString, QVariant> docData = docMap["index_info"].toMap();
           for(auto it= docData.begin();it != docData.end();it++)
           {
               QMap<QString,QVariant> docItem = it.value().toMap();
               if(docItem.contains("list"))
               {
                   QVariantList fLst = docData["list"].toList();
                   foreach(const QVariant &f, fLst){
                       FilterDetail item;
                       item.filterflag = HZData::IndexStocks;
                       QMap<QString, QVariant> fMap = f.toMap();
                       if(fMap.contains("name"))
                           item.stock_name = fMap["name"].toString();
                       if(fMap.contains("code"))
                           item.stock_code = fMap["code"].toString();
                       if(fMap.contains("num"))
                       {
                           int num = fMap["num"].toInt();
                           if(num != 0)
                           {
                               item.stockVec.resize(num);
                           }
                       }
                       QVariantList itemLst = fMap["stock_list"].toList();
                       foreach(const QVariant &f, itemLst){
                           HZData::StockTable stockTab;
                           QMap<QString, QVariant> fMap = f.toMap();
                           if(fMap.contains("name"))
                               stockTab.stock_name = fMap["name"].toString();
                           if(fMap.contains("code"))
                               stockTab.stock_code = fMap["code"].toString();
                           item.stockVec.push_back(stockTab);
                           item.stockMap[stockTab.stock_code] = stockTab;
                       }
                       indexStockChildCodeMap[item.stock_code] = item;
                   }
               }
           }
       }
    }
}

void Httpdata::onCountDataArrived(qint64 id, const QByteArray &array)
{
    qDebug() << id;
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(array, &e);
    //    QString strAppPath = qApp->applicationDirPath() + "/";
    QString strCountInfoPath = m_strAppPath + "countinfo.json";
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        m_bFlgCount = true;
        HzDirHelper::saveJson(doc, strCountInfoPath);
    }
}

void Httpdata::onPlateDataArrived(qint64 id, const QByteArray &array)
{
    qDebug() << id;
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(array, &e);
    //    QString strAppPath = qApp->applicationDirPath() + "/";
    QString strPlateInfoPath = m_strAppPath + "plateinfo.json";
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        m_bFlgPlate = true;
        HzDirHelper::saveJson(doc, strPlateInfoPath);
    }
}

void Httpdata::onIndexDataArrived(qint64 id, const QByteArray &array)
{
    qDebug() << id;
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(array, &e);
    //    QString strAppPath = qApp->applicationDirPath() + "/";
    QString strIndexInfoPath = m_strAppPath + "indexinfo.json";
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        m_bFlgIndex = true;
        HzDirHelper::saveJson(doc, strIndexInfoPath);
    }
}

void Httpdata::onStockDataArrived(qint64 id, const QByteArray &array)
{
    qDebug() << id;
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(array, &e);
    //    QString strAppPath = qApp->applicationDirPath() + "/";
    QString strStockInfoPath = m_strAppPath + "stockinfo.json";
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        m_bFlgStock = true;
        HzDirHelper::saveJson(doc, strStockInfoPath);
    }
}

void Httpdata::onIndexStockChildList(qint64 id, const QByteArray &array)
{
    qDebug() << id;
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(array, &e);
    QString strStockInfoPath = m_strAppPath + "indexStockChildList.json";
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        m_bFlgIndexStockList = true;
        HzDirHelper::saveJson(doc, strStockInfoPath);
    }
}

void Httpdata::preLoade(int filter)
{
   FilterFlag flag = (FilterFlag)filter;
   switch (flag) {
   case HZData::MarketAll:
   case HZData::MarketSZ:
   case HZData::MarketSH:
       loadStockData();
       break;
   case HZData::BlockIndustry:
   case HZData::BlockConcept:
   case HZData::BlockRegion:
   case HZData::BlockWhole:
       loadPlateData();
       break;
   case HZData::IndexSH:
   case IndexSZ:
   case HZData::IndexInner:
   //case IndexOutter:
       loadIndexData();
       break;
   default:
       break;
   }
}

void Httpdata::loadCountData()
{
    if(!m_bFlgCount)
    {
        QByteArray array;
        QNetworkReply::NetworkError code;
        m_httpClient->cancle(m_reqCountId);
        m_httpClient->syncGet(Hzcfg::getInstance()->serverInfo().counturl, array, code, 3000);
        if(code == QNetworkReply::NoError)
        {
            QJsonDocument doc;
            QJsonParseError e;
            doc = QJsonDocument::fromJson(array, &e);
            QString strCountInfoPath = m_strAppPath + "countinfo.json";
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                m_bFlgCount = true;
                HzDirHelper::saveJson(doc, strCountInfoPath);
            }
        }
    }
    readCountData();

//    if(m_bFlgCount)
//    {
//        readCountData();
//    }
//    else
//    {
//        QByteArray array;
//        QNetworkReply::NetworkError code;
//        m_httpPlate->syncGet(Hzcfg::getInstance()->serverInfo().counturl, array, code, 3000);
//        if(code == QNetworkReply::NoError)
//        {
//            QJsonDocument doc;
//            QJsonParseError e;
//            doc = QJsonDocument::fromJson(array, &e);
//            QString strCountInfoPath = m_strAppPath + "countinfo.json";
//            if(e.error == QJsonParseError::NoError && !doc.isNull())
//            {
//                m_bFlgCount = true;
//                HzDirHelper::saveJson(doc, strCountInfoPath);
//                readCountData();
//            }
//        }
//        else
//        {
//            readCountData();
//        }

//    }
}

void Httpdata::loadPlateData()
{
    if(!m_bFlgPlate)
    {
        QByteArray array;
        QNetworkReply::NetworkError code;
        m_httpClient->cancle(m_reqPlaterId);
        m_httpClient->syncGet(Hzcfg::getInstance()->serverInfo().plateurl, array, code, 3000);
        if(code == QNetworkReply::NoError)
        {
            QJsonDocument doc;
            QJsonParseError e;
            doc = QJsonDocument::fromJson(array, &e);
            QString strPlateInfoPath = m_strAppPath + "plateinfo.json";
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                m_bFlgPlate = true;
                HzDirHelper::saveJson(doc, strPlateInfoPath);
            }
        }
    }
    readPlateData();
}

void Httpdata::loadIndexData()
{
    if(!m_bFlgIndex)
    {
        QByteArray array;
        QNetworkReply::NetworkError code;
        m_httpClient->cancle(m_reqIndexId);
        m_httpClient->syncGet(Hzcfg::getInstance()->serverInfo().indexurl, array, code, 3000);
        if(code == QNetworkReply::NoError)
        {
            QJsonDocument doc;
            QJsonParseError e;
            doc = QJsonDocument::fromJson(array, &e);
            QString strIndexInfoPath = m_strAppPath + "indexinfo.json";
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                m_bFlgIndex = true;
                HzDirHelper::saveJson(doc, strIndexInfoPath);
            }
        }
    }
    readIndexData();
}

void Httpdata::loadStockData()
{
    if(!m_bFlgStock)
    {
        QByteArray array;
        QNetworkReply::NetworkError code;
        m_httpClient->cancle(m_reqstockId);
        m_httpClient->syncGet(Hzcfg::getInstance()->serverInfo().stockurl, array, code, 3000);
        if(code == QNetworkReply::NoError)
        {
            QJsonDocument doc;
            QJsonParseError e;
            doc = QJsonDocument::fromJson(array, &e);
            QString strStockInfoPath = m_strAppPath + "stockinfo.json";
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                m_bFlgStock = true;
                HzDirHelper::saveJson(doc, strStockInfoPath);
            }
        }
    }
    readStockData();
}

void Httpdata::loadIndexStockChildList()
{
    if(!m_bFlgIndexStockList)
    {
        QByteArray array;
        QNetworkReply::NetworkError code;
        m_httpClient->cancle(m_reqIndexStockChildList);
        m_httpClient->syncGet(Hzcfg::getInstance()->serverInfo().indexStockChildListUrl, array, code, 3000);
        if(code == QNetworkReply::NoError)
        {
            QJsonDocument doc;
            QJsonParseError e;
            doc = QJsonDocument::fromJson(array, &e);
            QString strIndexInfoPath = m_strAppPath + "indexStockChildList.json";
            if(e.error == QJsonParseError::NoError && !doc.isNull())
            {
                m_bFlgIndexStockList = true;
                HzDirHelper::saveJson(doc, strIndexInfoPath);
            }
        }
    }
    readIndexStockChildList();
}


void Httpdata::finished()
{
    //    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //    if(200 == statusCode && m_reply->error() == QNetworkReply::NoError)
    //    {
    //        readJson(m_reply->readAll());
    //        m_bisReady = true;
    //        emit initReady(m_bisReady);
    //    }
    //    else {
    //        m_bisReady = false;
    //        emit initReady(m_bisReady);
    //    }

    //    qDebug() << "finished-------------------------";
    //    m_reply->deleteLater();
}



