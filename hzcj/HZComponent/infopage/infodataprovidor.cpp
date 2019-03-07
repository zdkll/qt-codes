#include "infodataprovidor.h"
#include <hzinfocenter.h>
#include <httpdata.h>

#include "infodef.h"
#include "infostyle.h"
#include "mvcpublicdef.h"
#include "zxstockopt.h"

#include <hzlog.h>

static int getTagType(const QString &tagName)
{
    for(int i=0; i<6; ++i) {
        if(gTagTexts[i] == tagName) {
            return i;
        }
    }
    return 0;
}


/**
 * @brief InfoHotProvidor::InfoHotProvidor 热门快讯
 */
InfoHotProvidor::~InfoHotProvidor()
{
    qDebug() << Q_FUNC_INFO;
    cancleReq();
}

void InfoHotProvidor::initializatItems(bool isClear, const QVariant &var)
{
    //重新请求
    if(isClear){
        m_currentTag = var.toInt();
        m_nextPage = 0;
        cancleReq();

        auto fun = std::bind(&InfoHotProvidor::hotInfoCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
        m_reqId = HzInfoCenter::getInstance()->reqHostNews(m_currentTag, m_nextPage, 3, fun);
        abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
        return;
    }

    QVariantMap map;
    map["tag"] = 0;
    map["source"] = 3;
    requestListForParam(map);
}

void InfoHotProvidor::requestListForParam(const QVariantMap &var)
{
    int tag     = var["tag"].toInt();
    int source  = var["source"].toInt();
    bool isSameTag(m_currentTag==tag);
    int nextpage = isSameTag ? m_nextPage : 0;//tag发生变化，页从最新的开始
    m_isFirst = nextpage==0?1:0;
    if(isSameTag) {
        if(m_nextPage == -1) {//最后一页，无需再请求了
            return;
        }
    } else {
        m_currentTag = tag;
    }

    cancleReq();
    auto fun = std::bind(&InfoHotProvidor::hotInfoCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    m_reqId = HzInfoCenter::getInstance()->reqHostNews(tag, nextpage, source, fun);
    abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void InfoHotProvidor::cancleReq()
{
    if(m_reqId != -1) HzInfoCenter::getInstance()->cancle(m_reqId);
}

void InfoHotProvidor::hotInfoCallBack(const InfoRet &ret, HostNews &news, int page)
{
    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = m_isFirst?(page==-1?-2:page):page;//page -2 表示首次请求数据为空
    if(ret.code != 0) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page << "code:" << ret.code;
        vMap["data"] = "";
        updateList(vMap);
        return;
    }

    QList<STHotInfoData> datas;
    for(const HostNewItem &record : news){
        STHotInfoData data;
        data.id         = record.id;
        data.title      = record.title;
        data.time       = record.time;
        data.tag        = getTagType(record.tag);
        data.source     = record.source;
        data.content    = record.desc;
        data.codeNames  = record.stockCodes;
        data.url        = record.url;
        bool isEmpty = data.codeNames.isEmpty();
        data.relateCode = isEmpty?"":record.stockCodes.first();
        data.relateName = isEmpty?"":Httpdata::instance()->getCodeNameByCode(record.stockCodes.first());
        data.isCus      = ZXStockOpt::instance()->isHas(data.relateCode);
        datas.append(data);
    }

    vMap["data"] = QVariant::fromValue(datas);
    updateList(vMap);
    m_nextPage = page;
}

/**
 * @brief InfoHotProvidor::InfoHotProvidor 新闻列表(个股/自选股)
 */
InfoNewsProvidor::~InfoNewsProvidor()
{
    qDebug() << Q_FUNC_INFO;
    cancleReq();
}

void InfoNewsProvidor::initializatItems(bool isClear, const QVariant &var)
{
    Q_UNUSED(isClear);
    QVariantMap map;
    map["tag"] = 0;
    map["stockcode"] = var;
    map["source"] = 3;
    requestListForParam(map);
}

void InfoNewsProvidor::requestListForParam(const QVariantMap &var)
{
    qDebug() << Q_FUNC_INFO << 1;

    cancleReq();

    qDebug() << Q_FUNC_INFO << 2;

    //是否重新请求
    bool isClear = var["clear"].toBool();
    QVector<QString> stocks = var["stockcode"].value<QVector<QString>>();
    auto fun = std::bind(&InfoNewsProvidor::newsInfoCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    if(isClear) {
        m_currentTag = var["tag"].toInt();
        m_nextPage = 0;
        m_reqId = HzInfoCenter::getInstance()->reqNewsInfo(stocks, m_nextPage, m_currentTag, 3, fun);
        abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
        qDebug() << Q_FUNC_INFO << 3;
        return;
    }


    int tag = var["tag"].toInt();
    int source = var["source"].toInt();
    bool isSameTag(m_currentTag==tag);
    int nextpage = isSameTag ? m_nextPage : 0;//tag发生变化，页从最新的开始
    m_isFirst = nextpage==0?1:0;
    if(isSameTag) {
        if(m_nextPage == -1) {//最后一页，无需再请求了
            return;
        }
    } else {
        m_currentTag = tag;
    }

    qDebug() << Q_FUNC_INFO << 4;
    m_reqId = HzInfoCenter::getInstance()->reqNewsInfo(stocks, nextpage, m_currentTag, source, fun);
    abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);

    qDebug() << Q_FUNC_INFO << 5;
}

void InfoNewsProvidor::cancleReq()
{
    if(m_reqId != -1) HzInfoCenter::getInstance()->cancle(m_reqId);
}

void InfoNewsProvidor::newsInfoCallBack(const InfoRet &ret, const NewsInfo &news, int page)
{
    qDebug() << Q_FUNC_INFO;
    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = m_isFirst?(page==-1?-2:page):page;;
    if(ret.code != 0 ) {//页无效或没有页
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page << "code:" << ret.code;
        vMap["data"] = "";
        updateList(vMap);
        return;
    }

    QList<STOptionalNews> datas;
    for(const NewInfoItem &record : news){
        STOptionalNews data;
        data.id         = record.id;
        data.stockcode  = record.stockcode;
        data.stockname  = data.stockname.isEmpty()?Httpdata::instance()->getCodeNameByCode(record.stockcode):record.stockname;
        if(data.stockname.isEmpty()) continue;
        data.title      = record.title;
        data.desc       = record.desc;
        data.tag        = getTagType(record.tag);//record.type;//record.tag;
        data.time       = record.time;
        data.source     = record.source;
        data.isCus      = ZXStockOpt::instance()->isHas(record.stockcode);// true;//false true is custom stock
        data.url        = record.url;
        data.chg        = Default_Item_String;
        data.change     = Default_Item_String;
        data.related_stockcode = record.related_stockcode;
        datas.append(data);
    }

    vMap["data"] = QVariant::fromValue(datas);
    updateList(vMap);
    m_nextPage = page;
}

void InfoNultiHourProvidor::initializatItems(bool isClear, const QVariant &var)
{
    if(isClear) m_nextPage = 0;
    QVariantMap map;
    map["source"] = 3;
    requestListForParam(map);
}

void InfoNultiHourProvidor::requestListForParam(const QVariantMap &var)
{
    Q_UNUSED(var)
    int source = var["source"].toInt();
    m_isFirst = m_nextPage==0?1:0;

    cancleReq();
    auto fun = std::bind(&InfoNultiHourProvidor::multiHourInfoCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_reqId = HzInfoCenter::getInstance()->reqFlashNew(m_nextPage, source, fun);
    abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void InfoNultiHourProvidor::cancleReq()
{
    if(m_reqId != -1) HzInfoCenter::getInstance()->cancle(m_reqId);
}

void InfoNultiHourProvidor::multiHourInfoCallBack(const InfoRet &ret, const FlashNews &hours, int page)
{
    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code != 0) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page << "code:" << ret.code;
        vMap["data"] = "";
        updateList(vMap);
        return;
    }

    QList<STMultiHour> multiDatas;
    for(const FlashNewItem &record:hours){
        STMultiHour data;
        data.content = record.title;
        QString timeStr = QDateTime::fromTime_t(record.time).toString("yyyyMMdd");
        if(!m_times.contains(timeStr)){
            data.timeRoot = true;
            data.time = record.time;
            m_times.append(timeStr);
            multiDatas.push_back(data);
            data.timeRoot = false;
            multiDatas.push_back(data);
        }else{
            data.timeRoot = false;
            data.time = record.time;
            multiDatas.push_back(data);
        }
    }

    vMap["data"] = QVariant::fromValue(multiDatas);
    updateList(vMap);
    m_nextPage = page;
}

///
/// \brief InfoAnncProvidor::initializatItems
/// \param var
///
InfoAnncProvidor::~InfoAnncProvidor()
{
    cancleReq();
}

void InfoAnncProvidor::initializatItems(bool isClear, const QVariant &var)
{
    if(isClear) m_nextPage = 0;
    QVariantMap map;
    map["tag"] = 0;
    map["stockcode"] = var;
    map["source"] = 3;
    m_currentTag = 0;
    requestListForParam(map);
}

void InfoAnncProvidor::requestListForParam(const QVariantMap &var)
{
    cancleReq();

    //重新请求
    QVector<QString> stocks = var["stockcode"].value<QVector<QString>>();
    auto fun = std::bind(&InfoAnncProvidor::anncInfoCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    int isClear = var["clear"].toBool();
    if(isClear) {
        m_currentTag = var["tag"].toInt();
        m_nextPage = 0;
        m_reqId = HzInfoCenter::getInstance()->reqAnncsInfo(stocks, m_nextPage, m_currentTag, 3, fun);
        abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
        return;
    }

    int tag = var["tag"].toInt();
    int source = var["source"].toInt();
    bool isSameTag(m_currentTag==tag);
    int nextpage = isSameTag ? m_nextPage : 0;//tag发生变化，页从最新的开始
    m_isFirst = nextpage==0?1:0;
    if(isSameTag) {
        if(m_nextPage == -1) {//最后一页，无需再请求了
            return;
        }
    } else {
        m_currentTag = tag;
    }

    m_reqId = HzInfoCenter::getInstance()->reqAnncsInfo(stocks, nextpage, tag, source, fun);
    abnormalReq(m_reqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void InfoAnncProvidor::cancleReq()
{
    if(m_reqId != -1) HzInfoCenter::getInstance()->cancle(m_reqId);
}

void InfoAnncProvidor::anncInfoCallBack(const InfoRet &ret,const AnncsInfo &anns, int page)
{
    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = m_isFirst?(page==-1?-2:page):page;//page -2 表示首次请求数据为空
    if(ret.code != 0) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page << "code:" << ret.code;
        vMap["data"] = "";
        updateList(vMap);
        return;
    }

    QList<STAnnc> datas;
    for(const AnncInfoItem &record : anns){
        STAnnc data;
        data.id         = record.id;
        data.stockcode  = record.stockcode;
        data.stockname  = record.stockname.isEmpty()? Httpdata::instance()->getCodeNameByCode(record.stockcode):record.stockname;
        if(data.stockname.isEmpty()) continue;
        data.title      = record.title;
        data.desc       = record.desc;
        data.tag        = getTagType(record.tag);//record.type;//record.tag;
        data.time       = record.time;
        data.source     = record.source;
        data.isCus      = ZXStockOpt::instance()->isHas(record.stockcode);// true;//false true is custom stock
        data.url        = record.url;
        datas.append(data);
    }

    vMap["data"] = QVariant::fromValue(datas);
    updateList(vMap);
    m_nextPage = page;
}

///
/// \brief InfoMarketCalandarProvidor::initializatItems
/// \param var
///
InfoMarketCalandarProvidor::~InfoMarketCalandarProvidor()
{
    cancleReq();
}

void InfoMarketCalandarProvidor::initializatItems(bool isClear, const QVariant &var)
{
    QString date = var.toString();
    if(date.isEmpty()) return;
    QVariantMap map;
    map["date"] = date;
    requestListForParam(map);
}

void InfoMarketCalandarProvidor::requestListForParam(const QVariantMap &var)
{
    cancleReq();
    QString date = var["date"].toString();
    auto fun = std::bind(&InfoMarketCalandarProvidor::marketCalandarInfoCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_reqId = HzInfoCenter::getInstance()->reqMarketCalendar(date, fun);
    //abnormalReq(mReqId, mIsFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void InfoMarketCalandarProvidor::cancleReq()
{
    if(m_reqId != -1) HzInfoCenter::getInstance()->cancle(m_reqId);
}

void InfoMarketCalandarProvidor::marketCalandarInfoCallBack(const InfoRet &ret, const MarketCalendar &records)
{
    QVariantMap vMap;
    MarketCalendar temp  = records;
    vMap["data"] =  QVariant::fromValue(temp);
    updateList(vMap);
}

///
/// \brief InfoEconomicCalandarProvidor::initializatItems
/// \param var
///
InfoEconomicCalandarProvidor::~InfoEconomicCalandarProvidor()
{
    cancleReq();
}

void InfoEconomicCalandarProvidor::initializatItems(bool isClear, const QVariant &var)
{
    QString date = var.toString();
    if(date.isEmpty()) return;
    QVariantMap map;
    map["date"] = date;
    requestListForParam(map);
}

void InfoEconomicCalandarProvidor::requestListForParam(const QVariantMap &var)
{
    cancleReq();
    QString date = var["date"].toString();
    auto meetFun = std::bind(&InfoEconomicCalandarProvidor::majorMeetInfoCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_meetReqId = HzInfoCenter::getInstance()->reqMajorMeeting(date, meetFun);
    //abnormalReq(mReqId, mIsFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);

    auto financialFun = std::bind(&InfoEconomicCalandarProvidor::financialReportInfoCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_financialReqId = HzInfoCenter::getInstance()->reqFinancialReport(date, financialFun);
    //abnormalReq(mReqId, mIsFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);

    auto economicFun = std::bind(&InfoEconomicCalandarProvidor::economicDataInfoCallBack, this, std::placeholders::_1, std::placeholders::_2);
    m_economicReqId = HzInfoCenter::getInstance()->reqEconomicData(date, economicFun);
    //abnormalReq(mReqId, mIsFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void InfoEconomicCalandarProvidor::cancleReq()
{
    if(m_economicReqId != -1)    HzInfoCenter::getInstance()->cancle(m_economicReqId);
    if(m_financialReqId != -1)   HzInfoCenter::getInstance()->cancle(m_financialReqId);
    if(m_meetReqId != -1)        HzInfoCenter::getInstance()->cancle(m_meetReqId);
}

void InfoEconomicCalandarProvidor::majorMeetInfoCallBack(const InfoRet &ret, const MajorMeeting &meets)
{
    ZXStockOpt *zxsPtr = ZXStockOpt::instance();
    MajorMeeting tmpDatas(meets);
    for(int i=0; i<tmpDatas.datas.count(); ++i) {
        for(int k=0; k<tmpDatas.datas[i].relate_stock.count(); ++k) {
            QString stockcode(tmpDatas.datas[i].relate_stock[k].stockcode);
            tmpDatas.datas[i].relate_stock[k].isCustom = zxsPtr->isHas(stockcode);
        }
    }

    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = "";
    vMap["data"] = QVariant::fromValue(meets);
    updateList(vMap);
}


void InfoEconomicCalandarProvidor::financialReportInfoCallBack(const InfoRet &ret, const FinancialReport &reports)
{
    ZXStockOpt *zxsPtr = ZXStockOpt::instance();
    FinancialReport tmpDatas(reports);
    for(int i=0; i<tmpDatas.datas.count(); ++i) {
        bool isCustom = zxsPtr->isHas(tmpDatas.datas[i].code);
        qDebug() << Q_FUNC_INFO << "isCustom:" << isCustom << "code:" << tmpDatas.datas[i].code;
        tmpDatas.datas[i].isCustom = isCustom;
    }

    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = "";
    vMap["data"] = QVariant::fromValue(tmpDatas);
    updateList(vMap);
}

void InfoEconomicCalandarProvidor::economicDataInfoCallBack(const InfoRet &ret, const EconomicData &datas)
{
    QVariantMap vMap;
    vMap["code"] = ret.code;
    vMap["page"] = "";
    vMap["data"] = QVariant::fromValue(datas);
    updateList(vMap);
}
