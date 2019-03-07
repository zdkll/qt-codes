#include "hzinfoadapter.h"
#include <hzlog.h>
/****************************************************************************************
 *
 *  热门资讯
 *
 ******************************************************************************************/
void HZRMZXInfo::requestList()
{
    if(m_nextPage == -1) return;
    /*取消上一次请求*/
    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    auto fun = std::bind(&HZRMZXInfo::hostNewsCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqHostNews(0,m_nextPage, 2,fun);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZRMZXInfo::hostNewsCallBack(const InfoRet &ret, HostNews &news, int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;//page -2 表示首次请求数据为空;
    if(ret.code == -1) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page;
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const HostNewItem &item : news){
        STVListData data;
        data.id     = item.id;
        data.body   = item.title;
        data.url    = item.url;
        data.times  = QDateTime::fromTime_t(item.time).toString(" yyyy-MM-dd HH:mm:ss ");
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;

    vmap["data"] = QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}


/****************************************************************************************
 *
 *  7*24 资讯
 *
 ******************************************************************************************/
void HZ7Multiply24Info::requestList()
{
    if(m_nextPage == -1) return;
    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    auto fun = std::bind(&HZ7Multiply24Info::reqFlashNewCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqFlashNew(m_nextPage, 2, fun);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZ7Multiply24Info::reqFlashNewCallBack(const InfoRet &ret, FlashNews &news, int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code == -1) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page;
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const FlashNewItem &item : news){
        STVListData data;
        data.id     = item.id;
        data.body   = item.title;
        data.url    = item.url;
        data.times  = QDateTime::fromTime_t(item.time).toString(" yyyy-MM-dd HH:mm:ss ");
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;
    vmap["data"] = QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}


/****************************************************************************************
 *
 *  新闻
 *
 ******************************************************************************************/
void HZNewsInfo::requestList()
{
    QVector<QString> stockList;
    stockList = m_codeVec;
    if(stockList.isEmpty() || m_nextPage==-1) return;
    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    auto fun = std::bind(&HZNewsInfo::reqNewsCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqNewsInfo(stockList, m_nextPage, 0, 2, fun);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZNewsInfo::reqNewsCallBack(const InfoRet& ret,const NewsInfo& news,int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code == -1) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page;
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const NewInfoItem &item : news){
        STVListData data;
        data.id     = item.id;
        data.body   = item.title;
        data.times  = QDateTime::fromTime_t(item.time).toString(" yyyy-MM-dd HH:mm:ss ");
        data.desc = item.desc;
        data.tag = item.tag;
        data.source = item.source;
        data.type = item.type;
        data.url = item.url;
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;

    vmap["data"] = QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}

/****************************************************************************************
 *
 *  公告
 *
 ******************************************************************************************/
void HZAnncsInfo::requestList()
{
    QVector<QString> stockList;
    stockList = m_codeVec;
    if(stockList.isEmpty() || m_nextPage == -1)
        return;

    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    auto fun = std::bind(&HZAnncsInfo::reqAnncsCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqAnncsInfo(stockList, m_nextPage, 0, 2, fun);//&stockList, int nPage, int tag, int source
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZAnncsInfo::reqAnncsCallBack(const InfoRet& ret,const AnncsInfo& news,int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code == -1) {
        qDebug() << Q_FUNC_INFO << "req failed!" << "page:" << page;
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const AnncInfoItem &item : news){
        STVListData data;
        data.id     = item.id;
        data.body   = item.title;
        data.times  = QDateTime::fromTime_t(item.time).toString(" yyyy-MM-dd HH:mm:ss ");
        data.desc = item.desc;
        data.tag = item.tag;
        data.source = item.source;
        data.type = item.type;
        data.url    = item.url;
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;
    vmap["data"] = QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}


/****************************************************************************************
 *
 *  研报
 *
 ******************************************************************************************/
void HZResearchReport::requestList()
{
    QVector<QString> stockList;
    stockList = m_codeVec;
    if(stockList.isEmpty() || m_nextPage == -1)
        return;

    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    auto fun = std::bind(&HZResearchReport::reqResearchCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqResearchReport(stockList[0], m_nextPage, fun);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZResearchReport::reqResearchCallBack(const InfoRet & ret, const ResearchReports & news, int page)
{
    Q_UNUSED(ret)

    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code == -1) {
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const ResearchReportItem &item : news){
        STVListData data;
        data.id     = item.id;
        data.body   = item.iTitle;
        data.times  = QDateTime::fromTime_t(item.iTime).toString(" yyyy-MM-dd HH:mm:ss ");
        data.desc   = item.iDescription;
        data.tag    = item.iTagName;
        data.type   = item.type;
        data.source = item.iFrom;
        data.type   = item.type;
        data.url    = item.url;
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;
    vmap["data"] =  QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}


/****************************************************************************************
 *
 *  板块新闻
 *
 ******************************************************************************************/
void HZPlateNewsInfo::requestList()
{
    if(m_codeVec.isEmpty() || m_nextPage == -1) return;
    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    PlateNewsListsCallBack fun = std::bind(&HZPlateNewsInfo::reqPlateNewsCallBack, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqPlateNewsLists(m_codeVec.first(), m_nextPage, fun);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZPlateNewsInfo::reqPlateNewsCallBack(const InfoRet &ret, const PlateNewsLists &news, int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code == -1) {
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(const PlateNewsList &item : news){
        STVListData data;
        data.id     = item.news_id;
        data.body   = item.title;
        data.times  = QDateTime::fromTime_t(item.news_time).toString(" yyyy-MM-dd HH:mm:ss ");
        data.desc = item.news_intro;
        data.tag    = item.tagname;
        data.source = item.source;
        data.type = item.news_tag;
        data.url    = item.url;
        datas.append(data);
    }

    STInfoData infoData;
    infoData.datas = datas;

    vmap["data"] =  QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}

/****************************************************************************************
 *
 *  指数新闻
 *
 ******************************************************************************************/
void HZIndexNewsInfo::requestList()
{
    if(m_nextPage == -1)
        return;

    cancleReq();
    m_isFirst = m_nextPage==0?1:0;
    IndexNewsListsCallBack fun = std::bind(&HZIndexNewsInfo::reqIndexNewsCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    mReqId = HzInfoCenter::getInstance()->reqIndexNewsLists(fun, m_nextPage);
    abnormalReq(mReqId, m_isFirst?(m_nextPage==-1?-2:m_nextPage):m_nextPage);
}

void HZIndexNewsInfo::reqIndexNewsCallBack(const InfoRet &ret, const IndexNewsLists &news, int page)
{
    QVariantMap vmap;
    vmap["code"] = ret.code;
    vmap["page"] = m_isFirst?(page==-1?-2:page):page;
    if(ret.code != 0) {
        updateList(vmap);
        return;
    }

    QList<STVListData> datas;
    for(int i=0; i<news.count(); ++i) {
        STVListData dt;
        dt.id       = news[i].news_id;
        dt.body     = news[i].title;
        QString tmpTime = QDateTime::fromTime_t(news[i].news_time).toString(" yyyy-MM-dd HH:mm:ss ");
        dt.times    = tmpTime;
        dt.desc     = news[i].news_intro;
        dt.tag      = news[i].tagname;
        dt.type     = news[i].news_tag;
        dt.source   = news[i].source;
        dt.url      = news[i].url;
        datas.append(dt);
    }

    STInfoData infoData;
    infoData.datas = datas;
    vmap["data"] =  QVariant::fromValue(infoData);
    updateList(vmap);
    m_nextPage = page;
}
