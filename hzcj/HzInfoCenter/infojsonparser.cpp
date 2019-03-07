#include "infojsonparser.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDateTime>
#include <algorithm>
#include "httpdata.h"
InfoJsonParser::InfoJsonParser()
{

}

INT64 InfoJsonParser::yyyymmddToTime(const QString &date)
{
    QDateTime time = QDateTime::fromString(date,"yyyyMMdd");
    if(time.isValid())
    {
        return time.toTime_t();
    }
    return 0;
}

QString InfoJsonParser::timeToYyyymmdd(INT64 time)
{
    return QDateTime::fromTime_t(time).toString("yyyyMMdd");
}

InfoRet InfoJsonParser::hostNew(const QByteArray& byteArray,HostNews &data,int& nextpage)
{
#if 0
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("nextpage"))
                {
                    nextpage = obj.value("nextpage").toInt();
                }else
                {
                    nextpage = -1;
                }
                if(obj.contains("list"))
                {
                    HostNewItem newItem;
                    QJsonArray list = obj.value("list").toArray();
                    for(int i=0;i < list.size();i++)
                    {
                        QJsonObject item = list[i].toObject();
                        if(item.contains("id") && item.value("id").isString())
                        {
                            newItem.id = item.value("id").toString();
                        }
                        if(item.contains("title") && item.value("title").isString())
                        {
                            newItem.title = item.value("title").toString();
                        }
                        if(item.contains("tag") && item.value("tag").isString())
                        {
                            newItem.tag = item.value("tag").toString();
                            newItem.type = converTagType(newItem.tag);
                        }
                        if(item.contains("stockcode") && item.value("stockcode").isArray())
                        {
                            QJsonArray array = item.value("stockcode").toArray();
                            for(int j=0;j < array.size();j++)
                            {
                                if(array[j].isString())
                                    newItem.stockCodes.push_back(array[j].toString());
                            }
                        }
                        if(item.contains("source") && item.value("source").isString())
                        {
                            newItem.source = item.value("source").toString();
                        }
                        if(item.contains("time") && item.value("time").isString())
                        {
                            QString time = item.value("time").toString();
                            newItem.time = time.toLongLong();
                        }
                        if(item.contains("desc") && item.value("desc").isString())
                        {
                            newItem.desc = item.value("desc").toString();
                        }
                        if(item.contains("url") && item.value("url").isString())
                        {
                            newItem.url = item.value("url").toString();
                        }
                        data.push_back(newItem);
                    }
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }

    return ret;
#else
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code    = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        nextpage    = doc.toVariant().toMap()["data"].toMap()["nextpage"].toInt();
        QVariantList fstLst  = doc.toVariant().toMap()["data"].toMap()["list"].toList();
        foreach(const QVariant &var, fstLst) {
            HostNewItem it;
            it.id       = var.toMap()["id"].toString();
            it.title   = var.toMap()["title"].toString();
            it.desc = var.toMap()["desc"].toString();
            it.tag = var.toMap()["tag"].toString();
            it.source    = var.toMap()["source"].toString();
            it.time    = var.toMap()["time"].toUInt();
            it.url      = var.toMap()["url"].toString();
            QVector<QString> tmpStocks;
            QVariantList stocks = var.toMap()["related_stockcode"].toList();
            foreach(const QVariant &var,stocks) tmpStocks.append(var.toString());
            it.stockCodes = tmpStocks;
            data.append(it);
        }
    } else {
        ret.message = QStringLiteral("josn格式错误");
    }

    return ret;
#endif
}

InfoRet InfoJsonParser::hostHostNewDetail(const QByteArray& byteArray, HostNewDetail &data)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("title") && obj.value("title").isString())
                {
                    data.title = obj.value("title").toString();
                }
                if(obj.contains("content") && obj.value("content").isString())
                {
                    data.content = obj.value("content").toString();
                }
                if(obj.contains("tag") && obj.value("tag").isString())
                {
                    data.tag = obj.value("tag").toString();
                    data.type = converTagType(data.tag);
                }
                if(obj.contains("stockcode") && obj.value("stockcode").isArray())
                {
                    QJsonArray array = obj.value("stockcode").toArray();
                    for(int j=0;j < array.size();j++)
                    {
                        if(array[j].isString())
                            data.stockCodes.push_back(array[j].toString());
                    }
                }
                if(obj.contains("source") && obj.value("source").isString())
                {
                    data.source = obj.value("source").toString();
                }
                if(obj.contains("time") && obj.value("time").isString())
                {
                    QString time = obj.value("time").toString();
                    data.time = time.toLongLong();
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::flashNew(const QByteArray& byteArray, FlashNews &data,int& nextpage)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("nextpage"))
                {
                    nextpage = obj.value("nextpage").toInt();
                }else
                {
                    nextpage = -1;
                }
                if(obj.contains("list"))
                {
                    FlashNewItem newItem;
                    QJsonArray list = obj.value("list").toArray();
                    for(int i=0;i < list.size();i++)
                    {
                        QJsonObject item = list[i].toObject();
                        if(item.contains("id") && item.value("id").isString())
                        {
                            newItem.id = item.value("id").toString();
                        }
                        if(item.contains("title") && item.value("title").isString())
                        {
                            newItem.title = item.value("title").toString();
                        }
                        if(item.contains("time") && item.value("time").isString())
                        {
                            QString time = item.value("time").toString();
                            newItem.time = time.toLongLong();
                        }
                        if(item.contains("url") && item.value("url").isString()) {
                            newItem.url = item.value("url").toString();
                        }
                        data.push_back(newItem);
                    }
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::flashNewDetail(const QByteArray& byteArray, FlashNewDetail &data)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("title") && obj.value("title").isString())
                {
                    data.title = obj.value("title").toString();
                }
                if(obj.contains("content") && obj.value("content").isString())
                {
                    data.content = obj.value("content").toString();
                }
                if(obj.contains("source") && obj.value("source").isString())
                {
                    data.source = obj.value("source").toString();
                }
                if(obj.contains("time") && obj.value("time").isString())
                {
                    QString time = obj.value("time").toString();
                    data.time = time.toLongLong();
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}


InfoRet InfoJsonParser::financeInfo(const QByteArray &byteArray, FinanceInfo &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        FinanceTaking taking;     //营业收入信息
        FinanceKeyindex keyIndex; //关键指标
        FinanceProfit profit;     //利润表
        FinanceBalanceSheet balanceSheet;//（资产负债）
        FinanceCashFlow cashFlow;//（现金流量）

        //营业收入
        QVector<FinanceTaking::FinanceTakingItem> ftis;
        QVariantMap fstMap  = doc.toVariant().toMap()["data"].toMap()["taking"].toMap();
        QVariantList fstLst = fstMap["chartData"].toList();
        foreach(const QVariant &var, fstLst) {
            QVector<double> jds;
            FinanceTaking::FinanceTakingItem it;
            jds.append(var.toMap()["one"].toDouble());
            jds.append(var.toMap()["two"].toDouble());
            jds.append(var.toMap()["three"].toDouble());
            jds.append(var.toMap()["four"].toDouble());
            it.year             =  var.toMap()["year"].toString();
            it.yearOnYearGrowth = var.toMap()["yearOnYearGrowth"].toDouble();
            it.quarterVec       = jds;
            ftis.append(it);
            taking.dataList.append(ftis);
        }
        taking.overview     = fstMap["overview"].toString();
        taking.dataList     = ftis;

        //关键指标
        QVector<FinanceKeyindex::EarningsPerShareChart> epcs;
        QVariantMap scdMap = doc.toVariant().toMap()["data"].toMap()["keyIndex"].toMap();
        foreach(const QVariant &var,scdMap["earningsPerShareChart"].toList()){
            FinanceKeyindex::EarningsPerShareChart epc;
            epc.earnings   = var.toMap()["earnings"].toDouble();
            epc.year       = var.toMap()["year"].toString();
            epcs.append(epc);
        }

        keyIndex.earningsPerShareChart  = epcs;
        if(scdMap.contains("title"))                    keyIndex.title                 = scdMap["title"].toString();
        if(scdMap.contains("earningsPerShare"))         keyIndex.earningsPerShare      = scdMap["earningsPerShare"].toDouble();
        if(scdMap.contains("netAssetValuePreShare"))    keyIndex.netAssetValuePreShare = scdMap["netAssetValuePreShare"].toDouble();
        if(scdMap.contains("roe"))                      keyIndex.roe                   = scdMap["roe"].toDouble();
        if(scdMap.contains("epsNum"))                   keyIndex.epsNum                = scdMap["epsNum"].toString();
        if(scdMap.contains("eps"))                      keyIndex.eps                   = scdMap["eps"].toString();

        //利润表
        QVector<FinanceProfit::ProfitChartData> pfcds;
        QVariantMap thrMap = doc.toVariant().toMap()["data"].toMap()["profit"].toMap();
        foreach(const QVariant &var, thrMap["chartData"].toList()){
            FinanceProfit::ProfitChartData pfcd;
            pfcd.year               = var.toMap()["year"].toString();
            pfcd.retainedProfit     = var.toMap()["retainedProfit"].toDouble();
            pfcd.retainedProfitsUp  = var.toMap()["retainedProfitsUp"].toDouble();
            pfcds.append(pfcd);
        }
        if(thrMap.contains("overview"))         profit.overview         = thrMap["overview"].toString();
        if(thrMap.contains("title"))            profit.title            = thrMap["title"].toString();
        if(thrMap.contains("grossRevenus"))     profit.grossRevenus     = thrMap["grossRevenus"].toDouble();
        if(thrMap.contains("operatingProfit"))  profit.operatingProfit  = thrMap["operatingProfit"].toDouble();
        if(thrMap.contains("retainedProfits"))  profit.retainedProfits  = thrMap["retainedProfits"].toDouble();
        if(thrMap.contains("grossRevenUp"))     profit.grossRevenUp     = thrMap["grossRevenUp"].toDouble();
        if(thrMap.contains("operatinProfitUp")) profit.operatinProfitUp = thrMap["operatinProfitUp"].toDouble();
        if(thrMap.contains("retainedProfitsUp"))profit.retainedProfitsUp = thrMap["retainedProfitsUp"].toDouble();
        profit.chartData        = pfcds;

        //（资产负债）
        QVariantMap forMap = doc.toVariant().toMap()["data"].toMap()["balanceSheet"].toMap();
        if(forMap.contains("title"))            balanceSheet.title              = forMap["title"].toString();
        if(forMap.contains("totalAssets"))      balanceSheet.totalAssets        = forMap["totalAssets"].toDouble();
        if(forMap.contains("totalLiabilities")) balanceSheet.totalLiabilities   = forMap["totalLiabilities"].toDouble();
        if(forMap.contains("totalOwnersEquity")) balanceSheet.totalOwnersEquity  = forMap["totalOwnersEquity"].toDouble();
        if(forMap.contains("assetLiabilityRatio")) balanceSheet.assetLiabilityRatio= forMap["assetLiabilityRatio"].toDouble();

        //（现金流量）
        QVariantMap fivMap = doc.toVariant().toMap()["data"].toMap()["cashFlow"].toMap();
        if(fivMap.contains("title"))                cashFlow.title              = fivMap["title"].toString();
        if(fivMap.contains("operatingCashFlow"))    cashFlow.operatingCashFlow  = fivMap["operatingCashFlow"].toDouble();
        if(fivMap.contains("netInvestmentCash"))    cashFlow.netInvestmentCash  = fivMap["netInvestmentCash"].toDouble();
        if(fivMap.contains("netAmountOfFinancingCash"))   cashFlow.netAmountOfFinancingCash = fivMap["netAmountOfFinancingCash"].toDouble();

        data.taking = taking;
        data.cashFlow = cashFlow;
        data.keyIndex = keyIndex;
        data.profit = profit;
        data.balanceSheet = balanceSheet;
    } else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::researchReportInfo(const QByteArray &byteArray, ResearchReports &data, int& nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code    = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariant baseVar = doc.toVariant().toMap()["data"];
        if(baseVar.toMap().contains("nextpage")) {
            nextpage = baseVar.toMap()["nextpage"].toInt();
        }else {
            nextpage = -1;
        }

        QVariantList fstLst  = baseVar.toMap()["list"].toList();
        foreach(const QVariant &var, fstLst) {
            ResearchReportItem it;
            it.id       = var.toMap()["id"].toString();
            it.iTitle   = var.toMap()["title"].toString();
            it.iDescription = var.toMap()["desc"].toString();
            it.iTime    = var.toMap()["time"].toUInt();
            it.iFrom    = var.toMap()["source"].toString();
            it.iTagName = var.toMap()["tag"].toString();
            it.type     = converTagType(it.iTagName);
            it.stockcode= var.toMap()["stockcode"].toString();
            it.url      = var.toMap()["url"].toString();
            QVector<QString> stocks;
            foreach(const QVariant &v, var.toMap()["relate_stockcode"].toList())stocks.append(v.toString());
            it.rerated_stockcode = stocks;
            data.append(it);
        }
    } else {
        ret.message = QStringLiteral("josn格式错误");
    }

    return ret;
}

InfoRet InfoJsonParser::researchReportDetailInfo(const QByteArray &byteArray, ResearchReportDetail &data)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isString())
            {
                QString str = obj.value("data").toString();
                QJsonDocument doc;
                QJsonParseError e;
                doc = QJsonDocument::fromJson(str.toUtf8(), &e);
                if(e.error == QJsonParseError::NoError && !doc.isNull())
                {
                    if(doc.isObject())
                    {
                        QJsonObject obj =  doc.object();
                        if(obj.contains("id"))
                            data.id = QString::number(obj.value("id").toDouble());
                        if(obj.contains("iContent") && obj.value("iContent").isString())
                            data.iContent = obj.value("iContent").toString();
                        if(obj.contains("iTitle") && obj.value("iTitle").isString())
                            data.iTitle = obj.value("iTitle").toString();
                        if(obj.contains("iNewsID") && obj.value("iNewsID").isString())
                            data.iNewsID = obj.value("iNewsID").toString();
                        if(obj.contains("iDescription") && obj.value("iDescription").isString())
                            data.iDescription = obj.value("iDescription").toString();
                        if(obj.contains("iTime"))
                            data.iTime = obj.value("iTime").toString().toDouble();
                        if(obj.contains("iFrom") && obj.value("iFrom").isString())
                            data.iFrom = obj.value("iFrom").toString();
                        if(obj.contains("iCode") && obj.value("iCode").isString())
                            data.iCode = obj.value("iCode").toString();
                        if(obj.contains("iThirdUrl") && obj.value("iThirdUrl").isString())
                            data.iThirdUrl = obj.value("iThirdUrl").toString();
                        if(obj.contains("iTagName") && obj.value("iTagName").isString())
                        {
                            data.iTagName = obj.value("iTagName").toString();
                            data.type = converTagType(data.iTagName);
                        }
                        if(obj.contains("iRelatedStock") && obj.value("iRelatedStock").isString())
                        {
                            QString str = obj.value("iRelatedStock").toString();
                            QJsonDocument doc;
                            QJsonParseError e;
                            doc = QJsonDocument::fromJson(str.toUtf8(), &e);
                            if(e.error == QJsonParseError::NoError && !doc.isNull())
                            {
                                if(doc.isArray())
                                {
                                    QJsonArray iRelatedStock = doc.array();
                                    for(int i =0;i < iRelatedStock.size();i++)
                                    {
                                        if(iRelatedStock[i].isString())
                                        {
                                            data.iRelatedStock.push_back(iRelatedStock[i].toString());
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::newsInfo(const QByteArray &byteArray, NewsInfo &data,int& nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantMap fMap =  doc.toVariant().toMap()["data"].toMap();
        nextpage = fMap.contains("nextpage")?fMap["nextpage"].toInt():-1;
        QVariantList fLst = fMap["list"].toList();
        foreach(const QVariant &f, fLst){
            NewInfoItem item;
            item.id = f.toMap()["id"].toString();
            item.stockcode = f.toMap()["stockcode"].toString();
            item.stockname = f.toMap()["stockname"].toString();
            item.title = f.toMap()["title"].toString();
            item.desc = f.toMap()["desc"].toString();
            item.source = f.toMap()["source"].toString();
            item.time = f.toMap()["time"].toUInt();
            item.tag  = f.toMap()["tag"].toString();
            item.type = converTagType(item.tag);
            item.url = f.toMap()["url"].toString();
            QVector<QString> related_stockcode;
            QVariantList sLst = f.toMap()["related_stockcode"].toList();
            foreach(const QVariant &var, sLst) {
                related_stockcode.append(var.toString());
            }
            item.related_stockcode = related_stockcode;
            data.push_back(item);
        }
    }else {
        ret.message = QStringLiteral("josn格式错误");
    }

    qDebug() << Q_FUNC_INFO << "data cnt:" << data.count() << nextpage;
    return ret;
}

InfoRet InfoJsonParser::newInfoDetail(const QByteArray &byteArray, NewInfoDetail &data)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("stockcode") && obj.value("stockcode").isString())
                {
                    data.stockcode = obj.value("stockcode").toString();
                }
                if(obj.contains("title") && obj.value("title").isString())
                {
                    data.title = obj.value("title").toString();
                }
                if(obj.contains("desc") && obj.value("desc").isString())
                {
                    data.desc = obj.value("desc").toString();
                }
                if(obj.contains("content") && obj.value("content").isString())
                {
                    data.content = obj.value("content").toString();
                }
                if(obj.contains("tag") && obj.value("tag").isString())
                {
                    data.tag = obj.value("tag").toString();
                    data.type = converTagType(data.tag);
                }
                if(obj.contains("source") && obj.value("source").isString())
                {
                    data.source = obj.value("source").toString();
                }
                if(obj.contains("time") && obj.value("time").isString())
                {
                    QString time = obj.value("time").toString();
                    data.time = time.toLongLong();
                }
                if(obj.contains("related_stockcode") && obj.value("related_stockcode").isArray())
                {
                    QJsonArray array = obj.value("related_stockcode").toArray();
                    for(int j=0;j < array.size();j++)
                    {
                        if(array[j].isString())
                            data.related_stockcode.push_back(array[j].toString());
                    }
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::anncsInfo(const QByteArray &byteArray, AnncsInfo &data, int &nextpage)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("nextpage"))
                {
                    nextpage = obj.value("nextpage").toInt();
                }else
                {
                    nextpage = -1;
                }
                if(obj.contains("list"))
                {
                    AnncInfoItem newItem;
                    QJsonArray list = obj.value("list").toArray();
                    for(int i=0;i < list.size();i++)
                    {
                        QJsonObject item = list[i].toObject();
                        if(item.contains("id") && item.value("id").isString())
                        {
                            newItem.id = item.value("id").toString();
                        }
                        if(item.contains("stockcode") && item.value("stockcode").isString())
                        {
                            newItem.stockcode = item.value("stockcode").toString();
                        }
                        if(item.contains("stockname") && item.value("stockname").isString())
                        {
                            newItem.stockname = item.value("stockname").toString();
                        }
                        if(item.contains("title") && item.value("title").isString())
                        {
                            newItem.title = item.value("title").toString();
                        }
                        if(item.contains("desc") && item.value("desc").isString())
                        {
                            newItem.desc = item.value("desc").toString();
                        }
                        if(item.contains("tag") && item.value("tag").isString())
                        {
                            newItem.tag = item.value("tag").toString();
                            newItem.type = converTagType(newItem.tag);
                        }
                        if(item.contains("source") && item.value("source").isString())
                        {
                            newItem.source = item.value("source").toString();
                        }
                        if(item.contains("time") && item.value("time").isString())
                        {
                            QString time = item.value("time").toString();
                            newItem.time = time.toLongLong();
                        }
                        if(item.contains("url") && item.value("url").isString())
                        {
                            newItem.url = item.value("url").toString();
                        }
                        data.push_back(newItem);
                    }
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::anncInfoDetail(const QByteArray &byteArray, AnncInfoDetail &data)
{
    QJsonDocument doc;
    QJsonParseError e;
    doc = QJsonDocument::fromJson(byteArray, &e);
    InfoRet ret;
    if(e.error == QJsonParseError::NoError && !doc.isNull())
    {
        QJsonObject obj = doc.object();
        if(obj.contains("code"))
        {
            ret.code = obj.value("code").toInt();
        }
        if(obj.contains("message") && obj.value("message").isString())
        {
            ret.message = obj.value("message").toString();
        }
        if(!ret.code)
        {
            if(obj.contains("data") && obj.value("data").isObject())
            {
                obj = obj.value("data").toObject();
                if(obj.contains("stockcode") && obj.value("stockcode").isString())
                {
                    data.stockcode = obj.value("stockcode").toString();
                }
                if(obj.contains("title") && obj.value("title").isString())
                {
                    data.title = obj.value("title").toString();
                }
                if(obj.contains("desc") && obj.value("desc").isString())
                {
                    data.desc = obj.value("desc").toString();
                }
                if(obj.contains("content") && obj.value("content").isString())
                {
                    data.content = obj.value("content").toString();
                }
                if(obj.contains("tag") && obj.value("tag").isString())
                {
                    data.tag = obj.value("tag").toString();
                    data.type = converTagType(data.tag);
                }
                if(obj.contains("source") && obj.value("source").isString())
                {
                    data.source = obj.value("source").toString();
                }
                if(obj.contains("time") && obj.value("time").isString())
                {
                    QString time = obj.value("time").toString();
                    data.time = time.toLongLong();
                }
                if(obj.contains("related_stockcode") && obj.value("related_stockcode").isArray())
                {
                    QJsonArray array = obj.value("related_stockcode").toArray();
                    for(int j=0;j < array.size();j++)
                    {
                        if(array[j].isString())
                            data.related_stockcode.push_back(array[j].toString());
                    }
                }
            }
        }

    }else {
        ret.message = QStringLiteral("josn格式错误");
    }
    return ret;
}

InfoRet InfoJsonParser::marketCalendarParse(const QByteArray &byteArray, MarketCalendar &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<MarketCalendar::FHPXData> FHPXDatas;
        QVector<MarketCalendar::TFPData> TFPDatas;
        QVector<MarketCalendar::XSJJData> XSJJDatas;
        QVector<MarketCalendar::ZFData> ZFDatas;
        QVector<MarketCalendar::YJPLData> YJPLDatas;
        QVariantList fLst = doc.toVariant().toMap()["data"].toMap()["fhpx"].toList();
        foreach (const QVariant &f, fLst) {
            MarketCalendar::FHPXData item;
            //stockcode	股票代码 stockname	股票名称 record_date	股权登记日 dividend_date	派息日 detail	详情
            item.stockcode  = f.toMap()["stockcode"].toString();
            item.stockname  = f.toMap()["stockname"].toString();
            item.record_date = f.toMap()["record_date"].toString();
            item.dividend_date = f.toMap()["dividend_date"].toString();
            item.detail     = f.toMap()["detail"].toString();
            FHPXDatas.push_back(item);
        };

        fLst = doc.toVariant().toMap()["data"].toMap()["tfp"].toList();
        foreach (const QVariant &f, fLst) {
            MarketCalendar::TFPData item;
            item.stockcode  = f.toMap()["stockcode"].toString();
            item.stockname  = f.toMap()["stockname"].toString();
            item.suspension_date = f.toMap()["suspension_date"].toString();
            item.resumption_date = f.toMap()["resumption_date"].toString();
            item.reason     = f.toMap()["reason"].toString();
            TFPDatas.push_back(item);
        }

        fLst = doc.toVariant().toMap()["data"].toMap()["xsjj"].toList();
        foreach (const QVariant &f, fLst) {
            MarketCalendar::XSJJData item;
            item.stockcode  = f.toMap()["stockcode"].toString();
            item.stockname  = f.toMap()["stockname"].toString();
            item.lifted     = f.toMap()["lifted"].toString();
            item.scale      = f.toMap()["scale"].toString();
            XSJJDatas.push_back(item);
        }

        fLst = doc.toVariant().toMap()["data"].toMap()["zf"].toList();
        foreach (const QVariant &f, fLst) {
            MarketCalendar::ZFData item;
            item.stockcode  = f.toMap()["stockcode"].toString();
            item.stockname  = f.toMap()["stockname"].toString();
            item.issue_price = f.toMap()["issue_price"].toString();
            item.issue_num  = f.toMap()["issue_num"].toString();
            item.issue_date = f.toMap()["issue_date"].toString();
            ZFDatas.push_back(item);
        }

        fLst = doc.toVariant().toMap()["data"].toMap()["yjpl"].toList();
        foreach (const QVariant &f, fLst) {
            MarketCalendar::YJPLData item;
            item.stockcode  = f.toMap()["stockcode"].toString();
            item.stockname  = f.toMap()["stockname"].toString();
            item.eps        = f.toMap()["eps"].toString();
            item.naps       = f.toMap()["naps"].toString();
            item.roe        = f.toMap()["roe"].toString();
            item.net_profit = f.toMap()["net_profit"].toString();
            YJPLDatas.push_back(item);
        }

        data.FHPXDatas   = FHPXDatas;
        data.TFPDatas    = TFPDatas;
        data.XSJJDatas   = XSJJDatas;
        data.ZFDatas     = ZFDatas;
        data.YJPLDatas   = YJPLDatas;
    }else {
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::marketCalendarTimesParse(const QByteArray &byte, MarketCalandarTimes &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<quint64> tmpDts;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst)tmpDts.prepend(yyyymmddToTime(f.toString()));
        data.datas = tmpDts;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::economicDataTimesParse(const QByteArray &byte, EconomicDataTimes &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<quint64> tmpDts;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst)tmpDts.prepend(yyyymmddToTime(f.toString()));
        data.datas = tmpDts;
    } else {
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::economicDataParse(const QByteArray &byte, EconomicData &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            EconomicData::EconomicDataItem record;
            record.time = f.toMap()["time"].toInt();
            record.name = f.toMap()["name"].toString();
            record.pre_value = f.toMap()["pre_value"].toString();
            record.exp_value = f.toMap()["exp_value"].toString();
            record.act_value = f.toMap()["act_value"].toString();
            record.region = f.toMap()["region"].toString();
            record.source = f.toMap()["source"].toString();
            data.datas.push_back(record);
        }
    } else {
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::majorMeetingParse(const QByteArray &byte, MajorMeeting &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<MajorMeeting::MajorMeetPrivate> vecs;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &var, fLst){
            MajorMeeting::MajorMeetPrivate item;
            item.id = var.toMap()["id"].toString();
            item.title = var.toMap()["title"].toString();
            item.url = var.toMap()["url"].toString();
            QVariantList sLst = var.toMap()["related_board"].toList();
            QVariantList tLst = var.toMap()["related_stock"].toList();
            foreach(const QVariant &f, sLst) item.relate_board.append(f.toString());
            foreach(const QVariant &f, tLst) {
                MajorMeeting::RelateStock stocks;
                stocks.stockcode = f.toMap()["stockcode"].toString();
                stocks.stockname = f.toMap()["stockname"].toString();
                stocks.isCustom = false;
                item.relate_stock.append(stocks);
                //item.relate_stock.append(qMakePair(f.toMap()["stockcode"].toString(), f.toMap()["stockname"].toString()));
            }
            vecs.push_back(item);
        }
        data.datas = vecs;
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::majorMeetingDetailParse(const QByteArray &byte, MajorMeetingDetail &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        data.title = doc.toVariant().toMap()["data"].toMap()["title"].toString();
        data.time = doc.toVariant().toMap()["data"].toMap()["time"].toInt();
        data.content = doc.toVariant().toMap()["data"].toMap()["content"].toString();
        QVariantList fLst = doc.toVariant().toMap()["data"].toMap()["related_board"].toList();
        QVariantList fMap = doc.toVariant().toMap()["data"].toMap()["related_stock"].toList();
        foreach(const QVariant &f, fLst) data.relate_board.append(f.toString());
        foreach(const QVariant &f, fMap) data.relate_stock.append(qMakePair(f.toMap()["stockcode"].toString(), f.toMap()["stockname"].toString()));
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::financialReportParse(const QByteArray &byte, FinancialReport &data)
{
    qDebug() << Q_FUNC_INFO << "byte:" << byte;
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            FinancialReport::FinancialItem record;
            record.code = f.toMap()["stockcode"].toString();
            record.name = f.toMap()["stockname"].toString();
            record.url  = f.toMap()["url"].toString();
            record.detail = f.toMap()["has_detail"].toBool();
            data.datas.push_back(record);
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }
    return ret;
}

InfoRet InfoJsonParser::financialReportDetailParse(const QByteArray &byte, FinancialReportDetail &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        data.title = doc.toVariant().toMap()["data"].toMap()["title"].toString();
        data.time = doc.toVariant().toMap()["data"].toMap()["time"].toInt();
        data.content = doc.toVariant().toMap()["data"].toMap()["content"].toString();
    }else {
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockLhbDatasParse(const QByteArray &byte, StockLhbDatas &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockLhbTypeItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("close_price"))
                            item.close_price = fMap["close_price"].toDouble();
                        if(fMap.contains("change_ratio"))
                            item.change_ratio = fMap["change_ratio"].toDouble();
                        if(fMap.contains("net_buy"))
                            item.net_buy = fMap["net_buy"].toDouble();
                        if(fMap.contains("buy"))
                            item.buy = fMap["buy"].toDouble();
                        if(fMap.contains("sell"))
                            item.sell = fMap["sell"].toDouble();
                        if(fMap.contains("related_plate")){
                            QVariantList varList = fMap["related_plate"].toList();
                            int i=0;
                            for(;i<varList.size() && i<3;i++){
                                item.block += varList[i].toMap()["platename"].toString()+",";
                            }
                            if(i>0)
                                item.block.chop(1);
                            if(varList.size()>3)
                                item.block += "...";
                        }
                        data.datas.push_back(item);
                    }
                }
                if(docData.contains("count"))
                {
                    QMap<QString, QVariant> countMap = docData["count"].toMap();
                    if(countMap.contains("sh"))
                        data.nShCOunt = countMap["sh"].toInt();
                    if(countMap.contains("sz"))
                        data.nSzCount = countMap["sz"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockLhbDetialParse(const QByteArray &byte, StockLhbDetialList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList dataList = docMap["data"].toList();
                foreach(const QVariant &fdata, dataList){
                    StockLhbDetial dataItem;
                    QMap<QString, QVariant> fdataMap = fdata.toMap();
                    if(fdataMap.contains("date"))
                    {
                        dataItem.date = yyyymmddToTime(fdataMap["date"].toString());
                    }
                    if(fdataMap.contains("close_price"))
                        dataItem.close_price = fdataMap["close_price"].toDouble();
                    if(fdataMap.contains("change_ratio"))
                        dataItem.change_ratio = fdataMap["change_ratio"].toDouble();
                    if(fdataMap.contains("detail"))
                    {
                        QVariantList detailList = fdataMap["detail"].toList();
                        foreach (const QVariant &f, detailList) {
                            StockLhbDetialListItem childItem;
                            QMap<QString, QVariant> fMap = f.toMap();
                            if(fMap.contains("net_buy"))
                                childItem.net_buy = fMap["net_buy"].toDouble();
                            if(fMap.contains("buy"))
                                childItem.buy = fMap["buy"].toDouble();
                            if(fMap.contains("sell"))
                                childItem.sell = fMap["sell"].toDouble();
                            if(fMap.contains("reason"))
                                childItem.reason = fMap["reason"].toString();
                            if(fMap.contains("top5_buy"))
                            {
                                QVariantList top5Map = fMap["top5_buy"].toList();
                                foreach (const QVariant &top, top5Map) {
                                    QMap<QString, QVariant> topMap = top.toMap();
                                    StockLhbDetialItem item;
                                    if(topMap.contains("id"))
                                        item.id = topMap["id"].toString();
                                    if(topMap.contains("name"))
                                        item.name = topMap["name"].toString();
                                    if(topMap.contains("buy"))
                                        item.buy = topMap["buy"].toDouble();
                                    if(topMap.contains("sell"))
                                        item.sell = topMap["sell"].toDouble();
                                    if(topMap.contains("ratio_3d"))
                                        item.ratio_3d = topMap["ratio_3d"].toDouble();
                                    childItem.top5_buy.push_back(item);
                                }
                            }
                            if(fMap.contains("top5_sell"))
                            {
                                QVariantList top5Map = fMap["top5_sell"].toList();
                                foreach (const QVariant &top, top5Map) {
                                    QMap<QString, QVariant> topMap = top.toMap();
                                    StockLhbDetialItem item;
                                    if(topMap.contains("id"))
                                        item.id = topMap["id"].toString();
                                    if(topMap.contains("name"))
                                        item.name = topMap["name"].toString();
                                    if(topMap.contains("buy"))
                                        item.buy = topMap["buy"].toDouble();
                                    if(topMap.contains("sell"))
                                        item.sell = topMap["sell"].toDouble();
                                    if(topMap.contains("ratio_3d"))
                                        item.ratio_3d = topMap["ratio_3d"].toDouble();
                                    childItem.top5_sell.push_back(item);
                                }
                            }
                            dataItem.data.push_back(childItem);
                        }
                    }
                    data.push_back(dataItem);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    std::sort(data.begin(),data.end(),[&](const StockLhbDetial& a,const StockLhbDetial& b){
        return a.date > b.date;
    });
    for(int i =0;i < data.size();i++)
    {
        qDebug() <<  data[i].date;
    }
    return ret;
}

InfoRet InfoJsonParser::stockYybDetailParse(const QByteArray &byte, StockYybDetail &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("name"))
                    data.name = docData["name"].toString();
                if(docData.contains("statistics"))
                {
                    QVariantList fLst = docData["statistics"].toList();
                    foreach(const QVariant &f, fLst){
                        StockYybDetailStatistics item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("label"))
                            item.label = fMap["label"].toString();
                        if(fMap.contains("count"))
                            item.count = fMap["count"].toInt();
                        if(fMap.contains("buy"))
                            item.buy = fMap["buy"].toDouble();
                        if(fMap.contains("sell"))
                            item.sell = fMap["sell"].toDouble();
                        if(fMap.contains("ratio"))
                            item.ratio = fMap["ratio"].toDouble();
                        data.statistics.push_back(item);
                    }
                }
                if(docData.contains("history"))
                {
                    QVariantList fLst = docData["history"].toList();
                    foreach(const QVariant &f, fLst){
                        StockYybDetaiHistory item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("type")){
                            QString dirc_str = fMap["type"].toString();
                            item.dirc = dirc_str==QStringLiteral("买入")?0:1;
                        }
                        if(fMap.contains("change_ratio"))
                            item.change_ratio = fMap["change_ratio"].toDouble();
                        if(fMap.contains("date"))
                            item.date = yyyymmddToTime(fMap["date"].toString());
                        if(fMap.contains("buy"))
                            item.buy = fMap["buy"].toDouble();
                        if(fMap.contains("sell"))
                            item.sell = fMap["sell"].toDouble();
                        data.history.push_back(item);
                    }
                }

            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }
    return ret;
}

InfoRet InfoJsonParser::stockYybDrbParse(const QByteArray &byteArray, StockYybDrbList &data, int &nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockYybDrbItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("id"))
                            item.id = fMap["id"].toString();
                        if(fMap.contains("name"))
                            item.name = fMap["name"].toString();
                        if(fMap.contains("stock_count"))
                            item.stockCount = fMap["stock_count"].toInt();
                        if(fMap.contains("net_buy"))
                            item.net_buy = fMap["net_buy"].toDouble();
                        if(fMap.contains("ratio_3d"))
                            item.ratio_3d = fMap["ratio_3d"].toDouble();
                        if(fMap.contains("stock_list"))
                        {
                            QVariantList stockLst = fMap["stock_list"].toList();
                            int size = 3;
                            for(int i=0;i < stockLst.size() && i < size;i++){
                                if(i == 0)
                                {
                                    item.stockCode = stockLst[i].toString();
                                }
                                item.stockName += Httpdata::instance()->getCodeNameByCode(stockLst[i].toString());;
                                if(i != stockLst.size()-1 && i != size-1)
                                    item.stockName += ",";
                                else if(i == size-1 && stockLst.size()>size)
                                    item.stockName += "…";
                            }
                        }
                        data.push_back(item);
                    }
                }
                if(docData.contains("nextpage"))
                {
                    nextpage = docData["nextpage"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockYybSlbParse(const QByteArray &byteArray, StockYybSlbList &data, int &nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockYybSlbItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("id"))
                            item.id = fMap["id"].toString();
                        if(fMap.contains("name"))
                            item.name = fMap["name"].toString();
                        if(fMap.contains("max_amount"))
                            item.max_amount = fMap["max_amount"].toDouble();
                        if(fMap.contains("max_amount_date"))
                            item.max_amount_date = yyyymmddToTime(fMap["max_amount_date"].toString());
                        if(fMap.contains("net_buy"))
                            item.net_buy = fMap["net_buy"].toDouble();
                        if(fMap.contains("net_buy_date"))
                            item.net_buy_date = yyyymmddToTime(fMap["net_buy_date"].toString());
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("stock_date"))
                            item.stock_date = yyyymmddToTime(fMap["stock_date"].toString());
                        data.push_back(item);
                    }
                }
                if(docData.contains("nextpage"))
                {
                    nextpage = docData["nextpage"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockYybHybParse(const QByteArray &byteArray, StockYybHybList &data, int &nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockYybHybItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("id"))
                            item.id = fMap["id"].toString();
                        if(fMap.contains("name"))
                            item.name = fMap["name"].toString();
                        if(fMap.contains("count_3m"))
                            item.count_3m = fMap["count_3m"].toInt();
                        if(fMap.contains("buy"))
                            item.buy = fMap["buy"].toDouble();
                        if(fMap.contains("sell"))
                            item.sell = fMap["sell"].toDouble();
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("stock_date"))
                            item.stock_date = yyyymmddToTime(fMap["stock_date"].toString());
                        item.stock_rate = fMap["change_ratio"].toDouble();
                        data.push_back(item);
                    }
                }
                if(docData.contains("nextpage"))
                {
                    nextpage = docData["nextpage"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockLhbDateListParse(const QByteArray &byteArray, StockLhbDateList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    data.push_back(yyyymmddToTime(f.toString()));
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockDrListParse(const QByteArray &byteArray, StockDrList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    StockDrItem item;
                    QMap<QString, QVariant> fMap = f.toMap();
                    if(fMap.contains("cqcxr"))
                    {
                        item.cqcxr = yyyymmddToTime(fMap["cqcxr"].toString());
                    }
                    if(fMap.contains("zgbl"))
                    {
                        item.zgbl = fMap["zgbl"].toDouble();
                        if(item.zgbl != 0)
                            item.percentageChange += item.zgbl/10;
                    }
                    if(fMap.contains("sgbl"))
                    {
                        item.sgbl = fMap["sgbl"].toDouble();
                        if(item.sgbl != 0)
                            item.percentageChange += item.sgbl/10;
                    }
                    if(fMap.contains("xjfhbl"))
                    {
                        item.xjfhbl = fMap["xjfhbl"].toDouble();
                        item.cashDividends = item.xjfhbl/10;
                    }
                    if(fMap.contains("pgj"))
                    {
                        item.pxgj = fMap["pgj"].toDouble();
                    }
                    if(fMap.contains("pgbl"))
                    {
                        item.pxgbl = fMap["pgbl"].toDouble();
                        if(item.pxgbl != 0)
                            item.percentageChange += item.pxgbl/10;
                    }
                    if(fMap.contains("multiply_forward"))
                    {
                        item.multiply_forward = fMap["multiply_forward"].toDouble()/10000;
                        qDebug() << "fMap[multiply_forward]: " << fMap["multiply_forward"] << "value: " << item.multiply_forward;
                    }
                    if(fMap.contains("add_forward"))
                    {
                        item.add_forward = fMap["add_forward"].toDouble()/10000;
                        qDebug() << "fMap[add_forward]: " << fMap["add_forward"] << "value: " << item.add_forward;
                    }
                    if(fMap.contains("multiply_backward"))
                    {
                        item.multiply_backward = fMap["multiply_backward"].toDouble()/10000;
                        qDebug() << "fMap[multiply_backward]: " << fMap["multiply_backward"] << "value: " << item.multiply_backward;
                    }
                    if(fMap.contains("add_backward"))
                    {
                        item.add_backward = fMap["add_backward"].toDouble()/10000;
                        qDebug() << "fMap[add_backward]: " << fMap["add_backward"] << "value: " << item.add_backward;
                    }
                    data.push_back(item);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockRzrqTopListParse(const QByteArray &byteArray, StockRzrqTopList &data, int &nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockRzrqTopItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("close_price"))
                            item.close_price = fMap["close_price"].toDouble();
                        if(fMap.contains("change_ratio"))
                            item.change_ratio = fMap["change_ratio"].toDouble();
                        //计算涨跌额
                        item.up_down_price = item.close_price*item.change_ratio/(100.f+item.change_ratio);
                        if(fMap.contains("rzmre"))
                            item.rzmre = fMap["rzmre"].toDouble();
                        if(fMap.contains("rzjmre"))
                            item.rzjmre = fMap["rzjmre"].toDouble();
                        if(fMap.contains("rzye"))
                            item.rzye = fMap["rzye"].toDouble();
                        if(fMap.contains("rzche"))
                            item.rzche = fMap["rzche"].toDouble();
                        if(fMap.contains("rqyl"))
                            item.rqyl = fMap["rqyl"].toLongLong();
                        if(fMap.contains("rqmcl"))
                            item.rqmcl = fMap["rqmcl"].toLongLong();
                        if(fMap.contains("rqchl"))
                            item.rqchl = fMap["rqchl"].toLongLong();
                        if(fMap.contains("rzrqye"))
                            item.rzrqye = fMap["rzrqye"].toDouble();
                        data.push_back(item);
                    }
                }
                if(docData.contains("nextpage"))
                {
                    nextpage = docData["nextpage"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockBoardRzrqTopListParse(const QByteArray &byteArray, StockBoardRzrqTopList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    StockBoardRzrqTopItem item;
                    QMap<QString, QVariant> fMap = f.toMap();
                    if(fMap.contains("boardcode"))
                        item.boardcode = fMap["boardcode"].toString();
                    if(fMap.contains("boardname"))
                        item.boardname = fMap["boardname"].toString();
                    if(fMap.contains("rzmre"))
                        item.rzmre = fMap["rzmre"].toDouble();
                    data.push_back(item);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockBoardRzrqLisParse(const QByteArray &byteArray, StockBoardRzrqList &data, int &nextpage)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QMap<QString, QVariant> docData = docMap["data"].toMap();
                if(docData.contains("list"))
                {
                    QVariantList fLst = docData["list"].toList();
                    foreach(const QVariant &f, fLst){
                        StockRzrqTopItem item;
                        QMap<QString, QVariant> fMap = f.toMap();
                        if(fMap.contains("stockcode"))
                            item.stockcode = fMap["stockcode"].toString();
                        if(fMap.contains("stockname"))
                            item.stockname = fMap["stockname"].toString();
                        if(fMap.contains("close_price"))
                            item.close_price = fMap["close_price"].toDouble();
                        if(fMap.contains("change_ratio"))
                            item.change_ratio = fMap["change_ratio"].toDouble();
                        item.up_down_price = item.close_price*item.change_ratio/(100.f+item.change_ratio);
                        if(fMap.contains("rzmre"))
                            item.rzmre = fMap["rzmre"].toDouble();
                        if(fMap.contains("rzjmre"))
                            item.rzjmre = fMap["rzjmre"].toDouble();
                        if(fMap.contains("rzye"))
                            item.rzye = fMap["rzye"].toDouble();
                        if(fMap.contains("rzche"))
                            item.rzche = fMap["rzche"].toDouble();
                        if(fMap.contains("rqyl"))
                            item.rqyl = fMap["rqyl"].toLongLong();
                        if(fMap.contains("rqmcl"))
                            item.rqmcl = fMap["rqmcl"].toLongLong();
                        if(fMap.contains("rqchl"))
                            item.rqchl = fMap["rqchl"].toLongLong();
                        if(fMap.contains("rzrqye"))
                            item.rzrqye = fMap["rzrqye"].toDouble();
                        data.dataList.push_back(item);
                    }
                }
                if(docData.contains("count"))
                {
                    data.count = docData["count"].toInt();
                }
                if(docData.contains("nextpage"))
                {
                    nextpage = docData["nextpage"].toInt();
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockRzrqDetailListParse(const QByteArray &byteArray, StockRzrqDetailList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    StockRzrqDetailItem item;
                    QMap<QString, QVariant> fMap = f.toMap();
                    if(fMap.contains("date"))
                        item.date = yyyymmddToTime(fMap["date"].toString());
                    if(fMap.contains("rzye"))
                    {
                        item.rzye = fMap["rzye"].toDouble();
                        if(item.rzye*1000 > data.valueMax*1000)
                            data.valueMax = item.rzye;
                        if(item.rzye*1000 < data.valueMin*1000)
                            data.valueMin = item.rzye;
                    }
                    if(fMap.contains("rqye"))
                    {
                        item.rqye = fMap["rqye"].toDouble();
                        if(item.rqye*1000 > data.valueMax*1000)
                            data.valueMax = item.rqye;
                        if(item.rqye*1000 < data.valueMin*1000)
                            data.valueMin = item.rqye;
                    }
                    if(fMap.contains("rzrqye"))
                        item.rzrqye = fMap["rzrqye"].toDouble();
                    if(fMap.contains("rzmre"))
                        item.rzmre = fMap["rzmre"].toDouble();
                    if(fMap.contains("rzjmre"))
                        item.rzjmre = fMap["rzjmre"].toDouble();
                    if(fMap.contains("rzche"))
                        item.rzche = fMap["rzche"].toDouble();
                    if(fMap.contains("rqyl"))
                        item.rqyl = fMap["rqyl"].toULongLong();
                    if(fMap.contains("rqmcl"))
                        item.rqmcl = fMap["rqmcl"].toULongLong();
                    if(fMap.contains("rqchl"))
                        item.rqchl = fMap["rqchl"].toLongLong();
                    data.dataList.push_back(item);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }
    if(!data.dataList.isEmpty())
    {
        std::sort(data.dataList.begin(),data.dataList.end(),[&](const StockRzrqDetailItem& a,const StockRzrqDetailItem& b){
            return a.date > b.date;
        });
    }
    return ret;
}

InfoRet InfoJsonParser::stockMarketRzrqListParse(const QByteArray &byteArray, StockMarketRzrqList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    StockMarketRzrqItem item;
                    QMap<QString, QVariant> fMap = f.toMap();
                    if(fMap.contains("date"))
                        item.date = yyyymmddToTime(fMap["date"].toString());
                    if(fMap.contains("rzrqye"))
                        item.rzrqye = fMap["rzrqye"].toDouble();
                    if(fMap.contains("rzye"))
                    {
                        item.rzye = fMap["rzye"].toDouble();
                        if(item.rzye*1000 > data.valueMax*1000)
                            data.valueMax = item.rzye;
                        if(item.rzye*1000 < data.valueMin*1000)
                            data.valueMin = item.rzye;
                    }
                    if(fMap.contains("rqye"))
                    {
                        item.rqye = fMap["rqye"].toDouble();
                        if(item.rqye*1000 > data.valueMax*1000)
                            data.valueMax = item.rqye;
                        if(item.rqye*1000 < data.valueMin)
                            data.valueMin = item.rqye;
                    }

                    data.dataList.push_back(item);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }
    return ret;
}

InfoRet InfoJsonParser::stockRzrqDateListParse(const QByteArray &byteArray, StockRzrqDateList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    data.push_back(yyyymmddToTime(f.toString()));
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockDzjyDateListParse(const QByteArray &byteArray, StockDzjyDateList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    data.push_back(yyyymmddToTime(f.toString()));
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}

InfoRet InfoJsonParser::stockDzjyListParse(const QByteArray &byteArray, StockDzjyList &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){
        if(doc.isObject())
        {
            QMap<QString, QVariant> docMap =doc.toVariant().toMap();
            if(docMap.contains("code"))
                ret.code = docMap["code"].toInt();
            if(docMap.contains("message"))
                ret.message = docMap["message"].toString();
            if(docMap.contains("data"))
            {
                QVariantList fLst = docMap["data"].toList();
                foreach(const QVariant &f, fLst){
                    StockDzjyItem item;
                    QMap<QString, QVariant> fMap = f.toMap();
                    if(fMap.contains("date"))
                        item.date = yyyymmddToTime(fMap["date"].toString());
                    if(fMap.contains("stockcode"))
                        item.stockcode = fMap["stockcode"].toString();
                    if(fMap.contains("stockname"))
                        item.stockname = fMap["stockname"].toString();
                    if(fMap.contains("deal_price"))
                        item.deal_price = fMap["deal_price"].toDouble();
                    if(fMap.contains("close_price"))
                        item.last_close = fMap["close_price"].toDouble();
                    if(fMap.contains("change_ratio"))
                        item.change_ratio = fMap["change_ratio"].toDouble();
                    if(fMap.contains("premium_rate"))
                        item.premium_rate = fMap["premium_rate"].toDouble();
                    if(fMap.contains("volume"))
                        item.volume = (fMap["volume"].toDouble()*10000);//万股转换成股
                    if(fMap.contains("turnover"))
                        item.turnover = fMap["turnover"].toDouble()*10000;//万元转换成元
                    if(fMap.contains("yyb_buy"))
                        item.yyb_buy = fMap["yyb_buy"].toString();
                    if(fMap.contains("yyb_sell"))
                        item.yyb_sell = fMap["yyb_sell"].toString();
                    data.push_back(item);
                }
            }
        }
    }else{
        ret.code = -1;
        ret.message = "failed";
    }

    return ret;
}
InfoRet InfoJsonParser::newStockCalandarParse(const QByteArray &byteArray, NewStockCalandars &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<NewStockCalandar> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            NewStockCalandar it;
            it.time = yyyymmddToTime(f.toMap()["sgrq"].toString());
            it.number = f.toMap()["number"].toInt();
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::newStockPreViewParse(const QByteArray &byteArray, NewStockPreViews &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<StockXgrlItem> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            StockXgrlItem it;
            QMap<QString, QVariant> fMap = f.toMap();
            it.stockcode =fMap["stockcode"].toString();
            it.stockname = fMap["stockname"].toString();
            if(!fMap["fxjg"].toString().isEmpty())
                it.fxjg =fMap["fxjg"].toDouble();
            else  if(!fMap["ycfxjg"].toString().isEmpty())
                it.ycfxjg = fMap["ycfxjg"].toDouble();
            if(fMap.contains("sgrq"))
                it.sgrq = yyyymmddToTime(fMap["sgrq"].toString());

            if(!fMap["sgsx"].toString().isEmpty())
                it.sgsx = fMap["sgsx"].toDouble();
            else if(!fMap["ycsgsx"].toString().isEmpty())
                it.ycsgsx = fMap["ycsgsx"].toDouble();

            if(!fMap["syl"].toString().isEmpty())
                it.syl    = fMap["syl"].toDouble();
            else if(!fMap["ycsyl"].toString().isEmpty())
                it.ycsyl    = fMap["ycsyl"].toDouble();

            if(fMap.contains("fxzs"))
                it.fxzs = fMap["fxzs"].toDouble();
            if(fMap.contains("wsfxl"))
                it.wsfxl = fMap["wsfxl"].toDouble();
            if(fMap.contains("zqrq"))
                it.zqrq = yyyymmddToTime(fMap["zqrq"].toString());

            if(!fMap["zql"].toString().isEmpty())
                it.zql = fMap["zql"].toDouble();

            if(fMap.contains("zgs"))
                it.zgs = fMap["zgs"].toString();

            if(!fMap["wszqjkr"].toString().isEmpty())
                it.zqjkrq = yyyymmddToTime(fMap["wszqjkr"].toString());

            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}


InfoRet InfoJsonParser::newStockTodaysBidParse(const QByteArray &byteArray, NewStockTodaysBids &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<StockXgrlItem> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            StockXgrlItem it;
            QMap<QString, QVariant> fMap = f.toMap();
            it.stockcode  = fMap["stockcode"].toString();
            it.stockname = fMap["stockname"].toString();
            if(!fMap["fxjg"].toString().isEmpty())
                it.fxjg = fMap["fxjg"].toDouble();
            else if(!fMap["ycfxjg"].toString().isEmpty())
                it.ycfxjg = fMap["ycfxjg"].toDouble();
            it.sgrq = yyyymmddToTime(fMap["sgrq"].toString());
            it.fxzs = fMap["fxzs"].toDouble();
            it.wsfxl = fMap["wsfxl"].toDouble();
            if(!fMap["sgsx"].toString().isEmpty())
                it.sgsx = fMap["sgsx"].toDouble();
            else  if(!fMap["ycsgsx"].toString().isEmpty())
                it.ycsgsx = fMap["ycsgsx"].toDouble();
            if(!fMap["syl"].toString().isEmpty())
                it.syl    = fMap["syl"].toDouble();
            else  if(!fMap["ycsyl"].toString().isEmpty())
                it.ycsyl    = fMap["ycsyl"].toDouble();
            if(!fMap["zqrq"].toString().isEmpty())
                it.zqrq = yyyymmddToTime(fMap["zqrq"].toString());
            if(!fMap["wszqjkr"].toString().isEmpty())
                it.zqjkrq = yyyymmddToTime(fMap["wszqjkr"].toString());
            if(!fMap["ssrq"].toString().isEmpty())
                it.ssrq = yyyymmddToTime(fMap["ssrq"].toString());
            if(!fMap["zql"].toString().isEmpty())
                it.zql = fMap["zql"].toDouble();
            it.zgs =fMap["zgs"].toString();
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::newStockMarketTodayParse(const QByteArray &byteArray, NewStockMarketTodays &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<StockXgrlItem> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            StockXgrlItem it;
            QMap<QString, QVariant> fMap = f.toMap();
            it.stockcode  = fMap["stockcode"].toString();
            it.stockname = fMap["stockname"].toString();
            if(!fMap["fxjg"].toString().isEmpty())
                it.fxjg = fMap["fxjg"].toDouble();
            else  if(!fMap["ycfxjg"].toString().isEmpty())
                it.ycfxjg = fMap["ycfxjg"].toDouble();
            it.total_rate = fMap["zzf"].toDouble();
            it.sgrq = yyyymmddToTime(fMap["sgrq"].toString());
            it.fxzs = fMap["fxzs"].toDouble();
            it.wsfxl = fMap["wsfxl"].toDouble();
            if(!fMap["sgsx"].toString().isEmpty())
                it.sgsx = fMap["sgsx"].toDouble();
            else if(!fMap["ycsgsx"].toString().isEmpty())
                it.ycsgsx = fMap["ycsgsx"].toDouble();
            if(!fMap["syl"].toString().isEmpty())
                it.syl = fMap["syl"].toDouble();
            else if(!fMap["ycsyl"].toString().isEmpty())
                it.ycsyl = fMap["ycsyl"].toDouble();
            if(!fMap["zqrq"].toString().isEmpty())
                it.zqrq = yyyymmddToTime(fMap["zqrq"].toString());
            if(!fMap["wszqjkr"].toString().isEmpty())
                it.zqjkrq = yyyymmddToTime(fMap["wszqjkr"].toString());
            if(!fMap["ssrq"].toString().isEmpty())
                it.ssrq = yyyymmddToTime(fMap["ssrq"].toString());
            it.zql = fMap["zql"].toDouble();

            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::newStockMarketIssuedParse(const QByteArray &byteArray, NewStockMarketIssueds &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<StockXgrlItem> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            StockXgrlItem it;
            QMap<QString, QVariant> fMap = f.toMap();
            it.stockcode  = fMap["stockcode"].toString();
            it.stockname = fMap["stockname"].toString();

            if(!fMap["fxjg"].toString().isEmpty())
                it.fxjg = fMap["fxjg"].toDouble();
            else if(!fMap["ycfxjg"].toString().isEmpty())
                it.ycfxjg = fMap["ycfxjg"].toDouble();

            it.sgrq = yyyymmddToTime(fMap["sgrq"].toString());
            it.fxzs = fMap["fxzs"].toDouble();
            it.wsfxl = fMap["wsfxl"].toDouble();

            if(!fMap["sgsx"].toString().isEmpty())
                it.sgsx = fMap["sgsx"].toDouble();
            else if(!fMap["ycsgsx"].toString().isEmpty())
                it.ycsgsx = fMap["ycsgsx"].toDouble();
            if(!fMap["syl"].toString().isEmpty())
                it.syl = fMap["syl"].toDouble();
            else if(!fMap["ycsyl"].toString().isEmpty())
                it.ycsyl = fMap["ycsyl"].toDouble();

            if(!fMap["zqrq"].toString().isEmpty())
                it.zqrq = yyyymmddToTime(fMap["zqrq"].toString());
            if(!fMap["wszqjkr"].toString().isEmpty())
                it.zqjkrq = yyyymmddToTime(fMap["wszqjkr"].toString());
            if(!fMap["ssrq"].toString().isEmpty())
                it.ssrq = yyyymmddToTime(fMap["ssrq"].toString());

            it.zql = fMap["zql"].toDouble();

            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::newStockIPOPerformanceParse(const QByteArray &byteArray, NewStockIPOPerformances &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<StockXgrlItem> tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            StockXgrlItem it;
            QMap<QString, QVariant> fMap = f.toMap();
            it.stockcode  = fMap["stockcode"].toString();
            it.stockname = fMap["stockname"].toString();

            if(!fMap["fxjg"].toString().isEmpty())
                it.fxjg = fMap["fxjg"].toDouble();
            else if(!fMap["ycfxjg"].toString().isEmpty())
                it.ycfxjg = fMap["ycfxjg"].toDouble();

            it.mzyqhl = fMap["mzyqhl"].toLongLong();
            it.total_rate = fMap["zzf"].toDouble();
            if(!fMap["ssrq"].toString().isEmpty())
                it.ssrq = yyyymmddToTime(fMap["ssrq"].toString());
            if(!fMap["sl"].toString().isEmpty())
                it.sl = fMap["sl"].toInt();

            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}
/*
"type": 2,
      "stockcode": "sh603855",
      "stockname": "华荣股份",
      "record_date": "20181129",
      "dividend_date": "20181130",
      "detail": "每10股派2.0元",
      "time": "1542964809"
*/
InfoRet InfoJsonParser::companyOptionEventsParse(const QByteArray &byteArray, CompanyOptionEvents &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        CompanyOptionEvents tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            CompanyOptionEvent it;
            it.stockcode = f.toMap()["stockcode"].toString();
            it.stockname = f.toMap()["stockname"].toString();
            if(f.toMap().contains("type")) it.type = f.toMap()["type"].toInt();//	数据类型，枚举值及含义（1：龙虎榜，2：分红送转，3：财报）
            if(f.toMap().contains("change_ratio")) it.change_ratio=f.toMap()["change_ratio"].toDouble();//	龙虎榜当日涨跌幅，单位：%
            if(f.toMap().contains("buy")) it.buy =f.toMap()["buy"].toDouble();//	龙虎榜买入额，单位：元
            if(f.toMap().contains("sell")) it.sell=f.toMap()["sell"].toDouble();//	龙虎榜卖出额，单位：元
            if(f.toMap().contains("net_buy")) it.net_buy=f.toMap()["net_buy"].toDouble();//	龙虎榜净买入额，单位：元
            if(f.toMap().contains("reason")) it.reason=f.toMap()["reason"].toString();//	龙虎榜上榜原因
            if(f.toMap().contains("record_date")) it.record_date=f.toMap()["record_date"].toUInt();//	股权登记日，格式：YYYYMMDD
            if(f.toMap().contains("dividend_date")) it.dividend_date=f.toMap()["dividend_date"].toUInt();//	除权除息日，格式：YYYYMMDD
            if(f.toMap().contains("detail")) it.detail=f.toMap()["detail"].toString();//	分红送转详情
            if(f.toMap().contains("date")) it.date= f.toMap()["date"].toUInt(); //	财报发布日期，格式：YYYYMMDD
            if(f.toMap().contains("url")) it.url=f.toMap()["url"].toString();//	财报H5详情页地址
            if(f.toMap().contains("time")) it.time=f.toMap()["time"].toUInt();//	数据更新时间戳
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::plateNewsListsParse(const QByteArray &byteArray, PlateNewsLists &data, int &page)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        bool existPage = doc.toVariant().toMap()["data"].toMap().contains("nextpage");
        page = existPage?doc.toVariant().toMap()["data"].toMap()["nextpage"].toInt():-1;
        PlateNewsLists tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toMap()["list"].toList();
        foreach(const QVariant &f, fLst){
            PlateNewsList it;
            if(f.toMap().contains("news_id")) it.news_id = f.toMap()["news_id"].toString();
            if(f.toMap().contains("title")) it.title = f.toMap()["title"].toString();
            if(f.toMap().contains("source")) it.source = f.toMap()["source"].toString();
            if(f.toMap().contains("news_intro")) it.news_intro = f.toMap()["news_intro"].toString();
            if(f.toMap().contains("news_tag")){
                it.news_tag = f.toMap()["news_tag"].toInt();
                it.tagname = converTagType(it.news_tag);
            }
            if(f.toMap().contains("news_time")) it.news_time = f.toMap()["news_time"].toLongLong();
            if(f.toMap().contains("url")) it.url = f.toMap()["url"].toString();
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::plateNewsDetailsParse(const QByteArray &byteArray, PlateNewsDetails &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        PlateNewsDetails tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            PlateNewsDetail it;
            if(f.toMap().contains("news_id")) it.news_id = f.toMap()["news_id"].toString();
            if(f.toMap().contains("news_content")) it.news_content = f.toMap()["news_content"].toString();
            if(f.toMap().contains("news_time")) it.news_time = f.toMap()["news_time"].toUInt();
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::indexNewsListsParse(const QByteArray &byteArray, IndexNewsLists &data, int &page)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        bool existPage = doc.toVariant().toMap()["data"].toMap().contains("nextpage");
        page = existPage?doc.toVariant().toMap()["data"].toMap()["nextpage"].toInt():-1;
        IndexNewsLists tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toMap()["list"].toList();
        foreach(const QVariant &f, fLst){
            IndexNewsList it;
            if(f.toMap().contains("news_id"))   it.news_id = f.toMap()["news_id"].toString();
            if(f.toMap().contains("title"))     it.title = f.toMap()["title"].toString();
            if(f.toMap().contains("source"))    it.source = f.toMap()["source"].toString();
            if(f.toMap().contains("news_intro")) it.news_intro = f.toMap()["news_intro"].toString();
            if(f.toMap().contains("news_time"))     it.news_time = f.toMap()["news_time"].toLongLong();
            if(f.toMap().contains("url"))       it.url = f.toMap()["url"].toString();
            if(f.toMap().contains("news_tag"))  {
                it.news_tag = f.toMap()["news_tag"].toInt();
                it.tagname = converTagType(it.news_tag);
            }
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::indexNewsDetailsParse(const QByteArray &byteArray, IndexNewsDetails &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        IndexNewsDetails tmpLst;
        QVariantList fLst = doc.toVariant().toMap()["data"].toList();
        foreach(const QVariant &f, fLst){
            IndexNewsDetail it;
            if(f.toMap().contains("news_id")) it.news_id = f.toMap()["news_id"].toString();
            if(f.toMap().contains("news_content")) it.news_content = f.toMap()["news_content"].toString();
            if(f.toMap().contains("news_time")) it.news_time = f.toMap()["news_time"].toUInt();
            tmpLst.append(it);
        }
        data = tmpLst;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }

    return ret;
}

InfoRet InfoJsonParser::topicInvestParse(const QByteArray &byte, TopicInvest &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVector<TodayChance> recentHotspot;
        QVector<TodayChance> todayChance;
        QVariantMap fMap = doc.toVariant().toMap()["data"].toMap();
        foreach(const QVariant &f, fMap["today_chance"].toList()){
            TodayChance it;
            it.rank = f.toMap()["rank"].toInt();
            it.topic_id = f.toMap()["topic_id"].toString();
            it.topic_name = f.toMap()["topic_name"].toString();
            it.topic_describe = f.toMap()["topic_describe"].toString();
            it.update_time = f.toMap()["time"].toUInt();
            QVariantList sLst = f.toMap()["related_stockcode"].toList();
            for(QVariant s : sLst) it.related_stock.push_back(s.toString());
            todayChance.append(it);
        }
        foreach(const QVariant &f, fMap["zcx_chance"].toList()) {
            TodayChance it;
            it.rank = f.toMap()["rank"].toInt();
            it.topic_id = f.toMap()["topic_id"].toString();
            it.topic_name = f.toMap()["topic_name"].toString();
            it.topic_describe = f.toMap()["topic_describe"].toString();
            it.update_time = f.toMap()["time"].toUInt();
            QVariantList sLst = f.toMap()["related_stockcode"].toList();
            for(QVariant s : sLst) it.related_stock.push_back(s.toString());
            recentHotspot.append(it);
        }

        data.todayChance    = todayChance;
        data.recentHotspot  =  recentHotspot;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }
    return ret;
}

InfoRet InfoJsonParser::topicInvestDetailParse(const QByteArray &byte, TopicInvestDetail &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError)
    {//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantMap fMap = doc.toVariant().toMap()["data"].toMap();
        TopicInvestDetail::STDetail detail;
        detail.topic_name = fMap["detail"].toMap()["topic_name"].toString();
        detail.significance = fMap["detail"].toMap()["significance"].toString();
        detail.sustainability = fMap["detail"].toMap()["sustainability"].toString();
        detail.reason = fMap["detail"].toMap()["reason"].toString();
        detail.intro = fMap["detail"].toMap()["intro"].toString();

        QVector<TopicInvestDetail::STStocks> stocks;
        for(const QVariant &var:fMap["stock_list"].toList())
        {
            TopicInvestDetail::STStocks stk;
            stk.stockcode = var.toMap()["stockcode"].toString();
            stk.stockname = var.toMap()["stockname"].toString();
            stocks.push_back(stk);
        }

        data.detail = detail;
        data.stock = stocks;
     } else {
        ret.code = -1;
        ret.message = "failed...";
    }
    return ret;
}

InfoRet InfoJsonParser::topicInvestNewsParse(const QByteArray &byte, TopicInvestNews &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantMap fMap = doc.toVariant().toMap()["data"].toMap();
        if(fMap.contains("nextpage")) {
             data.nextpage = fMap["nextpage"].toInt();
        } else {
            data.nextpage = -1;
        }

        QVector<TopicInvestNews::STNewList> news;
        foreach(const QVariant &f, fMap["list"].toList()) {
            TopicInvestNews::STNewList newdt;
            newdt.news_id = f.toMap()["news_id"].toString();
            newdt.title = f.toMap()["title"].toString();
            newdt.source = f.toMap()["source"].toString();
            newdt.tag = f.toMap()["tag"].toInt();
            newdt.intro = f.toMap()["intro"].toString();
            newdt.time = f.toMap()["time"].toUInt();
            news.push_back(newdt);
        }
        data.datas = news;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }
    return ret;
}

InfoRet InfoJsonParser::OptionalWeeklyParse(const QByteArray &byte, OptionalWeekly &data)
{
    InfoRet ret;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byte, &err);
    if(err.error == QJsonParseError::NoError){//
        ret.code = doc.toVariant().toMap()["code"].toInt();
        ret.message = doc.toVariant().toMap()["message"].toString();
        QVariantMap sumMap = doc.toVariant().toMap()["data"].toMap()["summary"].toMap();
        OptionalWeekly::STSummary sum;
        QVector<OptionalWeekly::STDetail> details;
        sum.begin_date = sumMap["begin_date"].toUInt();
        sum.end_date = sumMap["end_date"].toUInt();
        sum.index_rate_all = sumMap["index_rate_all"].toDouble() * 100;
        sum.optional_rate_all = sumMap["optional_rate_all"].toDouble() * 100;

        QVector<OptionalWeekly::STSummary::STIndexAndOptionRateDaily> indexs, opts;
        QVariantList indexList = sumMap["index_rate_daily"].toList(),
                optList = sumMap["optional_rate_daily"].toList();
        for(const QVariant &var:indexList){
            OptionalWeekly::STSummary::STIndexAndOptionRateDaily index;
            index.Rate = var.toMap()["Rate"].toDouble() * 100;
            index.CalcDay = var.toMap()["CalcDay"].toUInt();
            indexs.push_back(index);
        }
        for(const QVariant &var:optList){
            OptionalWeekly::STSummary::STIndexAndOptionRateDaily opt;
            opt.Rate = var.toMap()["Rate"].toDouble() * 100;
            opt.CalcDay = var.toMap()["CalcDay"].toUInt();
            opts.push_back(opt);
        }

        sum.indexs = indexs;
        sum.options = opts;

        QVariantList detailList = doc.toVariant().toMap()["data"].toMap()["detail"].toList();
        for(const QVariant &var : detailList) {
            OptionalWeekly::STDetail deta;
            deta.stockcode = var.toMap()["stockcode"].toString();
            deta.stockname = var.toMap()["stockname"].toString();
            deta.rate_all = var.toMap()["rate_all"].toDouble()*100;
            deta.per_income = var.toMap()["per_income"].toDouble();
            deta.money_in_all = var.toMap()["money_in_all"].toLongLong();
            deta.money_out_all = var.toMap()["money_out_all"].toLongLong();
            deta.pzyd = var.toMap()["pzyd"].toInt();
            deta.lhb = var.toMap()["lhb"].toInt();
            deta.fhsz = var.toMap()["fhsz"].toInt();
            deta.cb = var.toMap()["cb"].toInt();
            details.push_back(deta);
        }

        data.detail = details;
        data.sum = sum;
    } else {
        ret.code = -1;
        ret.message = "failed...";
    }
    return ret;
}

TagType InfoJsonParser::converTagType(const QString &tag)
{
    int type = UnknownTag;
    if(tag == "强烈利好")
    {
        type = StrongBullTag;
    } else if(tag=="利好"){
        type = BullTag;
    } else if(tag == "中性") {
        type = NeutralTag;
    } else if(tag == "利空"){
        type = BadNewsTag;
    } else if(tag == "强烈利空") {
        type = StrongBadNewsTag;
    }

    return (TagType)type;
}

//	新闻标签(默认0，1：强烈利好，2：利好，3：中性，4：利空，5：强烈利空)
QString InfoJsonParser::converTagType(int type)
{
    switch(type) {
    case 0:return "";
    case 1:return "强烈利好";
    case 2:return "利好";
    case 3:return "中性";
    case 4:return "利空";
    case 5:return "强烈利空";
    }
}
