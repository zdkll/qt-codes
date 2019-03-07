#include "hzinfocenter.h"
#include "httpclient.h"
#include "infojsonparser.h"
#include "hzcfg.h"

#include <QThread>
#define NewPageInitVal "0"

#define TIME_OUT_VALUE 2000

HzInfoCenter *HzInfoCenter::getInstance()
{
    static HzInfoCenter info;
    return &info;
}

void HzInfoCenter::cancle(InfoReqId id)
{
    auto it = m_reqMap.find(id);
    if(it != m_reqMap.end())
    {
        m_reqMap.erase(it);
        m_httpClient->cancle(id);
    }
}

InfoReqId HzInfoCenter::reqHostNews(int nPage,const HostNewsCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.hostNewsFun = fun;
    info.type = HostNewsReq;
    QString req = getInfoIp();
    req += "/info/hot?page=";
    req += QString::number(nPage);
    qDebug() << Q_FUNC_INFO << "url:" << req;
    InfoReqId id = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}


InfoReqId HzInfoCenter::reqHostNews(int tag, int page, int source, const HostNewsCallBack &fun)
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId() << QThread::currentThread();
    if(page == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.hostNewsFun = fun;
    info.type = HostNewsReq;
    QString req = getInfoIp();
    req += QString("/info/hot?tag=%1&page=%2&source=%3").arg(tag).arg(page).arg(source);
    qDebug() << Q_FUNC_INFO << "req:" << req;
    InfoReqId id = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqHostDetail(const QString &id, const HostNewDetailCallBack &fun)
{
    ReqInfo info;
    info.hostNewDetailFun = fun;
    info.type = HostDetailReq;
    QString req = getInfoIp();
    req += "/info/hot-detail?id=";
    req += id;
    InfoReqId ret = m_httpClient->get(req,m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqFlashNew(int nPage, int source, const FlashNewsCallBack &fun)
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId() << QThread::currentThread();
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.flashNewsFun = fun;
    info.type = FlashNewsReq;
    QString req = getInfoIp();
    req += QString("/info/flash?page=%1&source=%2").arg(nPage).arg(source);
    InfoReqId id = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}


InfoReqId HzInfoCenter::reqFlashNewDetail(const QString &id, const FlashNewDetailCallBack &fun)
{
    ReqInfo info;
    info.flashNewDetailFun = fun;
    info.type = FlashNewDetailReq;
    QString req = getInfoIp();
    req += "/info/flash-detail?id=";
    req += id;
    InfoReqId ret = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewsInfo(const QVector<QString> &stockList, int nPage, int tag, int source, const NewsInfoCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.newsInfoCallBackFun = fun;
    info.type = NewsInfoReq;
    QString req = getInfoIp();
    req += QString("/info/news?tag=%1&page=%2&source=%3").arg(tag).arg(nPage).arg(source);
    req += "&stockcode=";
    for(int i =0;i < stockList.size();i++)
    {
        req += stockList[i];
        if(i != stockList.size()-1)
        {
            req += ",";
        }
    }
    qDebug() << Q_FUNC_INFO << "req:" << req;
    InfoReqId id = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqNewInfoDetail(const QString &id, const NewInfoDetailCallBack &fun)
{
    ReqInfo info;
    info.newInfoDetailCallBackFun = fun;
    info.type = NewInfoDetailReq;
    QString req = getInfoIp();
    req += "/info/news-detail?id=";
    req += id;
    InfoReqId ret = m_httpClient->get(req,m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqAnncsInfo(const QVector<QString> &stockList, int nPage, const AnncInfoCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.anncInfoCallBackFun = fun;
    info.type = AnncsInfoReq;
    QString req = getInfoIp();
    req += "/info/annc?page=";
    req += QString::number(nPage);
    req += "&stockcode=";
    for(int i =0;i < stockList.size();i++)
    {
        req += stockList[i];
        if(i != stockList.size()-1)
        {
            req += ",";
        }
    }
    InfoReqId id = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqAnncsInfo(const QVector<QString> &stockList, int nPage, int tag, int source, const AnncInfoCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.anncInfoCallBackFun = fun;
    info.type = AnncsInfoReq;
    QString req = getInfoIp();
    req += QString("/info/annc?page=%1&tag=%2&source=%3").arg(nPage).arg(tag).arg(source);
    req += "&stockcode=";
    for(int i =0;i < stockList.size();i++)
    {
        req += stockList[i];
        if(i != stockList.size()-1)
        {
            req += ",";
        }
    }
    InfoReqId id = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqAnncInfoDetail(const QString &id, const AnncInfoDetailCallBack &fun)
{
    ReqInfo info;
    info.anncInfoDetailCallBackFun = fun;
    info.type = AnncInfoDetailReq;
    QString req = getInfoIp();
    req += "/info/annc-detail?id=";
    req += id;
    InfoReqId ret = m_httpClient->get(req,m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqFinanceInfo(const QString &stockCode, const FinanceInfoCallBack &fun)
{
    ReqInfo info;
    info.financeInfoFun = fun;
    info.type = FinanceInfoReq;
    QString req = QString("%1/stock/finance?stockcode=%2").arg(getInfoIp()).arg(stockCode);
    InfoReqId ret = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqResearchReport(const QString &stockCode, int page, const ResearchReportCallBack &fun)
{
    ReqInfo info;
    info.researchReportFun = fun;
    info.type = ResearchReportReq;
    QString req = QString("%1/info/research-report?tag=0&source=2&page=%2&stockcode=%3")
            .arg(getInfoIp()).arg(page).arg(stockCode);
    InfoReqId ret = m_httpClient->get(req,m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqResearchReportDetail(const QString &id, const ResearchReportDetailCallBack &fun)
{
    //~
    QMap<QString,QString> param;
    param["type"] = "yb";
    param["id"] = id;
    ReqInfo info;
    info.researchReportDetailFun = fun;
    info.type = ResearchReportDetailReq;
    QString req = getInfoIpFromQuote();
    req += "/detail";
    InfoReqId ret = m_httpClient->post(req,param,m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqMarketCalendar(const QString &date, const MarketCalendarCallBack &fun)
{
    ReqInfo info;
    info.marketCalendarFun = fun;
    info.type = MarketCalendarReq;
    QString req = getInfoIp();
    req += "/info/market-calendar?date=";
    req += date;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqMarketCalandarTimes(const MarketCalandarTimesCallBack &fun)
{
    ReqInfo info;
    info.marketCalendarTimesFun = fun;
    info.type = MarketCalendarTimesReq;
    QString req = getInfoIp();
    req += "/info/gsrl-date";
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqEconomicData(const QString &date, const EconomicDataCallBack &fun)
{
    ReqInfo info;
    info.economicDataFun = fun;
    info.type = EconomicDataReq;
    QString req = getInfoIp();
    req += "/info/economic-data?date=";
    req += date;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqEconomicDataTimes(const EconomicDataTimesCallBack &fun)
{
    ReqInfo info;
    info.economicDataTimesFun = fun;
    info.type = EconomicDataTimesReq;
    QString req = getInfoIp();
    req += "/info/dsqz-date";
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqMajorMeeting(const QString &date, const MajorMeetingCallBack &fun, int source)
{
    ReqInfo info;
    info.majorMeetingFun = fun;
    info.type = MajorMeetingReq;
    QString req = getInfoIp();
    req += QString("/info/meeting?date=%1&source=%2").arg(date).arg(source);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqMajorMeetingDetail(int id, const MajorMeetingDetailCallBack &fun)
{
    ReqInfo info;
    info.majorMeetingDetailFun = fun;
    info.type = FinancialReportReq;
    QString req = getInfoIp();
    req += QString("/info/meeting-detail?id=%1").arg(id);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqFinancialReport(const QString &date, const FinancialReportCallBack &fun, int source)
{
    ReqInfo info;
    info.financialReportFun = fun;
    info.type = FinancialReportReq;
    QString req = getInfoIp();
    req += QString("/info/financial-report?date=%1&source=%2").arg(date).arg(source);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqFinancialDetailReport(const QString &date, const QString &code, const FinancialReportDetailCallBack &fun)
{
    ReqInfo info;
    info.financialReportDetailtFun = fun;
    info.type = FinancialReportDetailReq;
    QString req = getInfoIp();
    req += QString("/info/financial-report-detail?date=%1&code=%2").arg(date).arg(code);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockLhbDatas(INT64 nTime, StockLhbType type, const StockLhbDatasCallBack &fun)
{
    ReqInfo info;
    info.stockLhbDatasFun = fun;
    info.type = StockLhbDatasReq;
    QString req = getInfoIp();
    req += QString("/stock/lhb?date=%1&type=%2").arg(m_parser->timeToYyyymmdd(nTime)).arg(type);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    qDebug()<<"reqStockLhbDatas:"<<req;
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockLhbDetial(INT64 nTime, const QString &stockcode, const StockLhbDetialCallBack &fun)
{
    ReqInfo info;
    info.stockLhbDetialFun = fun;
    info.type = StockLhbDetialReq;
    QString req = getInfoIp();
    req += QString("/stock/lhb-detail?date=%1&stockcode=%2").arg(m_parser->timeToYyyymmdd(nTime)).arg(stockcode);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockYybDetail(const QString &strId, const StockYybDetailCallBack &fun)
{
    ReqInfo info;
    info.stockYybDetailFun = fun;
    info.type = StockYybDetailReq;
    QString req = getInfoIp();
    req += QString("/stock/yyb-detail?id=%1").arg(strId);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockYybDrb(INT64 nTime, int nPage, const StockYybDrbCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.stockYybDrbFun = fun;
    info.type = StockYybDrbReq;
    QString req = getInfoIp();
    req += QString("/stock/yyb-drb?date=%1&page=%2").arg(m_parser->timeToYyyymmdd(nTime)).arg(nPage);
    InfoReqId id = m_httpClient->get(req, m_callFun);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqStockYybSlb(int nPage, const StockYybSlbCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.stockYybSlbFun = fun;
    info.type = StockYybSlbReq;
    QString req = getInfoIp();
    req += QString("/stock/yyb-slb?page=%1").arg(nPage);

    qDebug()<<"req:"<<req;
    InfoReqId id = m_httpClient->get(req, m_callFun);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqStockYybHyb(int nPage, const StockYybHybCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.stockYybHybFun = fun;
    info.type = StockYybHybReq;
    QString req = getInfoIp();
    req += QString("/stock/yyb-hyb?page=%1").arg(nPage);
    InfoReqId id = m_httpClient->get(req, m_callFun);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqStockLhbDateList(const StockLhbDateListCallBack &fun)
{
    ReqInfo info;
    info.stockLhbDateListFun = fun;
    info.type = StockLhbDateListReq;
    QString req = getInfoIp();
    req += QString("/stock/lhb-date");
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockDrList(const QString &stockcode, StockDrListCallBack &fun)
{
    ReqInfo info;
    info.stockDrListFun = fun;
    info.type = StockDrListReq;
    QString req = getInfoIp();
    req += QString("/stock/dr?stockcode=%1").arg(stockcode);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockRzrqTopList(INT64 nTime, int sort, int nPage, StockRzrqTopListCallBack &fun)
{
    if(nPage == -1)
    {
        return RewInfoInvalidId;
    }
    ReqInfo info;
    info.StockRzrqTopListFun = fun;
    info.type = StockRzrqTopListReq;
    QString req = getInfoIp();
    req += QString("/stock/rzrq-top?date=%1&page=%2&sort=%3").arg(m_parser->timeToYyyymmdd(nTime)).arg(nPage).arg(sort);
    qDebug()<<"req:"<<req;
    InfoReqId id = m_httpClient->get(req, m_callFun);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqStockBoardRzrqTopList(INT64 nTime,StockBoardRzrqTopListCallBack &fun)
{
    ReqInfo info;
    info.StockBoardRzrqTopListFun = fun;
    info.type = StockBoardRzrqTopListReq;
    QString req = getInfoIp();
    req += QString("/stock/board-rzrq-top?date=%1").arg(m_parser->timeToYyyymmdd(nTime));
    InfoReqId id = m_httpClient->get(req, m_callFun);
    m_reqMap[id] = info;
    return id;
}

InfoReqId HzInfoCenter::reqStockBoardRzrqList(INT64 nTime,const QString &boardcode, int nPage, StockBoardRzrqListCallBack &fun)
{
    ReqInfo info;
    info.StockBoardRzrqListFun = fun;
    info.type = StockBoardRzrqListReq;
    QString req = getInfoIp();
    req += QString("/stock/board-rzrq?date=%1&boardcode=%2&page=%3").arg(m_parser->timeToYyyymmdd(nTime)).arg(boardcode).arg(nPage);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockRzrqDetailList(INT64 nTime, const QString &stockcode, StockRzrqDetailListCallBack &fun)
{
    ReqInfo info;
    info.StockRzrqDetailListFun = fun;
    info.type = StockRzrqDetailListReq;
    QString req = getInfoIp();
    req += QString("/stock/rzrq-detail?date=%1&stockcode=%2").arg(m_parser->timeToYyyymmdd(nTime)).arg(stockcode);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockMarketRzrqList(INT64 nTime, StockMarketRzrqListCallBack &fun)
{
    ReqInfo info;
    info.StockMarketRzrqListFun = fun;
    info.type = StockMarketRzrqListReq;
    QString req = getInfoIp();
    req += QString("/stock/market-rzrq?date=%1").arg(m_parser->timeToYyyymmdd(nTime));
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockRzrqDateList(StockRzrqDateListCallBack &fun)
{
    ReqInfo info;
    info.StockRzrqDateListFun = fun;
    info.type = StockRzrqDateListReq;
    QString req = getInfoIp();
    req += QString("/stock/rzrq-date");
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockDzjyDateList(StockDzjyDateListCallBack &fun)
{
    ReqInfo info;
    info.StockDzjyDateListFun = fun;
    info.type = StockDzjyDateListReq;
    QString req = getInfoIp();
    req += QString("/stock/dzjy-date");
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqStockDzjyList(INT64 nTime, StockDzjyListCallBack &fun)
{
    ReqInfo info;
    info.StockDzjyListFun = fun;
    info.type = StockDzjyListReq;
    QString req = getInfoIp();
    req += QString("/stock/dzjy?date=%1").arg(m_parser->timeToYyyymmdd(nTime));
    qDebug()<<"reqStockDzjyList:"<<req;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockCalandars(NewStockCalandarCallBack &fun)
{
    ReqInfo info;
    info.NewStockCalandarFun = fun;
    info.type = NewStockCalandarsReq;
    QString req = getInfoIp();
    req += "/stock/xgrl";
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockPreViews(NewStockPreViewCallBack &fun)
{
    ReqInfo info;
    info.NewStockPreViewFun = fun;
    info.type = NewStockPreViewsReq;
    QString req = getInfoIp();
    req += "/stock/xgyg";
    qDebug()<<"reqNewStockPreViews:"<<req;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockTodaysBids(INT64 nTime,NewStockTodaysBidCallBack &fun)
{
    ReqInfo info;
    info.NewStockTodaysBidFun = fun;
    info.type = NewStockTodaysBidsReq;
    QString req = getInfoIp();
    req += "/stock/jrsg";
    if(nTime != -1)//按照日期取新股/今日申购
        req +="?date="+m_parser->timeToYyyymmdd(nTime);
    qDebug()<<"reqNewStockTodaysBids req:"<<req;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockMarketTodays(NewStockMarketTodayCallBack &fun)
{
    ReqInfo info;
    info.NewStockMarketTodayFun = fun;
    info.type = NewStockMarketTodaysReq;
    QString req = getInfoIp();
    req += "/stock/jrss";
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockMarketIssueds(NewStockMarketIssuedCallBack &fun)
{
    ReqInfo info;
    info.NewStockMarketIssuedFun = fun;
    info.type = NewStockMarketIssuedsReq;
    QString req = getInfoIp();
    req += "/stock/yfxdss";
    qDebug()<<"reqNewStockMarketIssueds :"<<req;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqNewStockIPOPerformances(NewStockIPOPerformanceCallBack &fun)
{
    ReqInfo info;
    info.NewStockIPOPerformanceFun = fun;
    info.type = NewStockIPOPerformancesReq;
    QString req = getInfoIp();
    req += "/stock/xgssbx";
    qDebug()<<"reqNewStockIPOPerformances:"<<req;
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqCompanyOptionEvents(const QVector<QString> &stockList, int source, int type, CompanyOptionEventsCallBack &fun)
{
    ReqInfo info;
    info.CompanyOptionEventsFun = fun;
    info.type = CompanyOptionEventsReq;
    QString req = getInfoIp();
    req += QString("/optional/events?type=%1&source=%2").arg(type).arg(source);
    req += "&stockcode=";
    for(int i =0;i < stockList.size();i++)
    {
        req += stockList[i];
        if(i != stockList.size()-1)
        {
            req += ",";
        }
    }
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqPlateNewsLists(const QString &platecode, int page, PlateNewsListsCallBack &fun)
{
    ReqInfo info;
    info.PlateNewsListsFun = fun;
    info.type = PlateNewsListsReq;
    QString req = getInfoIp();///info/plate-news-list
    req += QString("/info/plate-news-list?source=2&platecode=%1&page=%2").arg(platecode).arg(page);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqPlateNewsDetails(const QString &msgId, PlateNewsDetailsCallBack &fun)
{
    ReqInfo info;
    info.PlateNewsDetailsFun = fun;
    info.type = PlateNewsDetailsReq;
    QString req = getInfoIp();
    req += QString("/stock/plate-news-detail?msgId=%1").arg(msgId);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqIndexNewsLists(IndexNewsListsCallBack &fun, int page)
{
    ReqInfo info;
    info.IndexNewsListsFun = fun;
    info.type = IndexNewsListsReq;
    QString req = getInfoIp();
    req += QString("/info/index-news-list?source=2&page=%1").arg(page);
    InfoReqId ret = m_httpClient->get(req, m_callFun,TIME_OUT_VALUE);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqIndexNewsDetails(int news_id, IndexNewsDetailsCallBack &fun)
{
    ReqInfo info;
    info.IndexNewsDetailsFun = fun;
    info.type = IndexNewsDetailsReq;
    QString req = getInfoIp();
    req += QString("/stock/index-news-detail?news_id=%1").arg(news_id);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqTopicInvest(TopicInvestCallBack &fun)
{
    ReqInfo info;
    info.TopicInvestFun = fun;
    info.type = TopicInvestReq;
    QString req = getInfoIp();
    req += "/stock/topic-invest";
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqTopicInvestDetail(const QString& topic_id, TopicInvestDetailCallBack &fun)
{
    ReqInfo info;
    info.TopicInvestDetailFun = fun;
    info.type = TopicInvestDetailReq;
    QString req = getInfoIp();
    req += QString("/stock/topic-invest-detail?topic_id=%1").arg(topic_id);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqTopicInvestNews(int topic_id, int page, TopicInvestNewsCallBack &fun)
{
    ReqInfo info;
    info.TopicInvestNewsFun = fun;
    info.type = TopicInvestNewsReq;
    QString req = getInfoIp();
    req += QString("/stock/topic-invest-news?topic_id=%1&page=%2").arg(topic_id).arg(page);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

InfoReqId HzInfoCenter::reqOptionalWeekly(const QString &stockcodes, OptionalWeeklyCallBack &fun)
{
    ReqInfo info;
    info.OptionalWeeklyFun = fun;
    info.type = OptionalWeeklyReq;
    QString req = getInfoIp();
    req += QString("/optional/weekly?stockcode=%1").arg(stockcodes);
    InfoReqId ret = m_httpClient->get(req, m_callFun);
    m_reqMap[ret] = info;
    return ret;
}

HzInfoCenter::HzInfoCenter()
{
    m_httpClient  = new HttpClient();
    m_parser = std::shared_ptr<InfoJsonParser>(new InfoJsonParser());
    m_callFun.errorFun = std::bind(&HzInfoCenter::errorFun,this,std::placeholders::_1,std::placeholders::_2);
    m_callFun.finishedFun = std::bind(&HzInfoCenter::finishedFun,this,std::placeholders::_1,std::placeholders::_2);
    m_callFun.progressFun = std::bind(&HzInfoCenter::progressFun,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
}

HzInfoCenter::~HzInfoCenter()
{
    for(auto it = m_reqMap.begin();it != m_reqMap.end();it++)
    {
        m_httpClient->cancle(it.key());
    }
    delete m_httpClient;
    m_httpClient = nullptr;
    m_reqMap.clear();
}

void HzInfoCenter::finishedFun(qint64 id, const QByteArray &bytes)
{
    auto it = m_reqMap.find(id);
    //qDebug()<<"HzInfoCenter recv data reqId:"<<it.key()<<" service type:"<<it.value().type;
    if(it != m_reqMap.end())
    {
        ReqInfo req= it.value();
        switch (req.type) {
        case HostNewsReq:
        {
            HostNews data;
            int nextpage=-1;
            InfoRet ret = m_parser->hostNew(bytes,data,nextpage);
            ret.id = id;
            req.hostNewsFun(ret,data,nextpage);
        }
            break;
        case HostDetailReq:
        {
            HostNewDetail data;
            InfoRet ret = m_parser->hostHostNewDetail(bytes,data);
            ret.id = id;
            req.hostNewDetailFun(ret,data);
        }
            break;
        case FlashNewsReq:
        {
            FlashNews data;
            int nextpage=-1;
            InfoRet ret = m_parser->flashNew(bytes,data,nextpage);
            ret.id = id;
            req.flashNewsFun(ret,data,nextpage);
        }
            break;
        case FlashNewDetailReq:
        {
            FlashNewDetail data;
            InfoRet ret = m_parser->flashNewDetail(bytes,data);
            ret.id = id;
            req.flashNewDetailFun(ret,data);
        }
            break;
        case FinanceInfoReq:
        {
            FinanceInfo data;
            InfoRet ret = m_parser->financeInfo(bytes,data);
            ret.id = id;
            req.financeInfoFun(ret,data);
        }
            break;
        case ResearchReportReq:
        {
            ResearchReports data;
            int nextpage=-1;
            InfoRet ret = m_parser->researchReportInfo(bytes,data,nextpage);
            ret.id = id;
            req.researchReportFun(ret,data,nextpage);
        }
            break;
        case ResearchReportDetailReq:
        {
            ResearchReportDetail data;
            InfoRet ret = m_parser->researchReportDetailInfo(bytes,data);
            ret.id = id;
            req.researchReportDetailFun(ret,data);
        }
            break;
        case NewsInfoReq:
        {
            NewsInfo data;
            int nextpage=-1;
            InfoRet ret = m_parser->newsInfo(bytes,data,nextpage);
            ret.id = id;
            req.newsInfoCallBackFun(ret,data,nextpage);
        }
            break;
        case NewInfoDetailReq:
        {
            NewInfoDetail data;
            InfoRet ret = m_parser->newInfoDetail(bytes,data);
            ret.id = id;
            req.newInfoDetailCallBackFun(ret,data);
        }
            break;
        case AnncsInfoReq:
        {
            AnncsInfo data;
            int nextpage=-1;
            InfoRet ret = m_parser->anncsInfo(bytes,data,nextpage);
            ret.id = id;
            req.anncInfoCallBackFun(ret,data,nextpage);
        }
            break;
        case AnncInfoDetailReq:
        {
            AnncInfoDetail data;
            InfoRet ret = m_parser->anncInfoDetail(bytes,data);
            ret.id = id;
            req.anncInfoDetailCallBackFun(ret,data);
        }
            break;
        case MarketCalendarReq://股市日历
        {
            MarketCalendar data;
            InfoRet ret = m_parser->marketCalendarParse(bytes,data);
            ret.id = id;
            req.marketCalendarFun(ret, data);
        }break;
        case MarketCalendarTimesReq://股市日历时间列表
        {
            MarketCalandarTimes data;
            InfoRet ret = m_parser->marketCalendarTimesParse(bytes,data);
            ret.id = id;
            req.marketCalendarTimesFun(ret, data);
        }break;
        case EconomicDataTimesReq://经济日历时间列表
        {
            EconomicDataTimes data;
            InfoRet ret = m_parser->economicDataTimesParse(bytes, data);
            ret.id = id;
            req.economicDataTimesFun(ret, data);
        }break;
        case EconomicDataReq://经济数据
        {
            EconomicData data;
            InfoRet ret = m_parser->economicDataParse(bytes, data);
            ret.id = id;
            req.economicDataFun(ret, data);
        }break;
        case MajorMeetingReq://重大会议
        {
            MajorMeeting data;
            InfoRet ret = m_parser->majorMeetingParse(bytes, data);
            ret.id = id;
            req.majorMeetingFun(ret, data);
        }break;
        case MajorMeetingDetailReq://重大会议详情
        {
            MajorMeetingDetail data;
            InfoRet ret = m_parser->majorMeetingDetailParse(bytes, data);
            ret.id = id;
            req.majorMeetingDetailFun(ret, data);
        }break;
        case FinancialReportReq://财报发布列表
        {
            FinancialReport data;
            InfoRet ret = m_parser->financialReportParse(bytes, data);
            ret.id = id;
            req.financialReportFun(ret, data);
        }break;
        case FinancialReportDetailReq://财报发布详情详
        {
            FinancialReportDetail data;
            InfoRet ret = m_parser->financialReportDetailParse(bytes, data);
            ret.id = id;
            req.financialReportDetailtFun(ret, data);
        }break;
        case StockLhbDatasReq: //龙虎榜个股列表
        {
            StockLhbDatas data;
            InfoRet ret = m_parser->stockLhbDatasParse(bytes,data);
            ret.id = id;
            req.stockLhbDatasFun(ret, data);
        }break;
        case StockLhbDetialReq://龙虎榜个股详情
        {
            StockLhbDetialList data;
            InfoRet ret = m_parser->stockLhbDetialParse(bytes,data);
            ret.id = id;
            req.stockLhbDetialFun(ret, data);
        }break;
        case StockYybDetailReq: //营业部详情
        {
            StockYybDetail data;
            InfoRet ret = m_parser->stockYybDetailParse(bytes,data);
            ret.id = id;
            req.stockYybDetailFun(ret, data);
        }break;
        case StockYybDrbReq: //营业部当日榜单
        {
            StockYybDrbList data;
            int nextpage=-1;
            InfoRet ret = m_parser->stockYybDrbParse(bytes,data,nextpage);
            ret.id = id;
            req.stockYybDrbFun(ret,data,nextpage);
        }break;
        case StockYybSlbReq: //营业部实力榜单
        {
            StockYybSlbList data;
            int nextpage=-1;
            InfoRet ret = m_parser->stockYybSlbParse(bytes,data,nextpage);
            ret.id = id;
            req.stockYybSlbFun(ret,data,nextpage);
        }break;
        case StockYybHybReq: //营业部活跃榜
        {
            StockYybHybList data;
            int nextpage=-1;
            InfoRet ret = m_parser->stockYybHybParse(bytes,data,nextpage);
            ret.id = id;
            req.stockYybHybFun(ret,data,nextpage);
        }break;
        case StockLhbDateListReq: //龙虎榜榜单日期
        {
            StockLhbDateList data;
            InfoRet ret = m_parser->stockLhbDateListParse(bytes,data);
            ret.id = id;
            req.stockLhbDateListFun(ret,data);
        }break;
        case StockDrListReq: //除权出息接口
        {
            StockDrList data;
            InfoRet ret = m_parser->stockDrListParse(bytes,data);
            ret.id = id;
            req.stockDrListFun(ret,data);
        }break;
        case StockRzrqTopListReq: //融券融券个股数据榜单
        {
            StockRzrqTopList data;
            int nextpage=-1;
            InfoRet ret = m_parser->stockRzrqTopListParse(bytes,data,nextpage);
            ret.id = id;
            req.StockRzrqTopListFun(ret,data,nextpage);
        }break;
        case StockBoardRzrqTopListReq: //融资融券板块榜接口
        {
            StockBoardRzrqTopList data;
            InfoRet ret = m_parser->stockBoardRzrqTopListParse(bytes,data);
            ret.id = id;
            req.StockBoardRzrqTopListFun(ret,data);
        }break;
        case StockBoardRzrqListReq: //板块成分股融资融券数据接口
        {
            StockBoardRzrqList data;
            int nextpage=-1;
            InfoRet ret = m_parser->stockBoardRzrqLisParse(bytes,data,nextpage);
            ret.id = id;
            req.StockBoardRzrqListFun(ret,data,nextpage);
        }break;
        case StockRzrqDetailListReq: //融券融券个股详情数据榜单
        {
            StockRzrqDetailList data;
            InfoRet ret = m_parser->stockRzrqDetailListParse(bytes,data);
            ret.id = id;
            req.StockRzrqDetailListFun(ret,data);
        }break;
        case StockMarketRzrqListReq: //两市融资融券数据接口
        {
            StockMarketRzrqList data;
            InfoRet ret = m_parser->stockMarketRzrqListParse(bytes,data);
            ret.id = id;
            req.StockMarketRzrqListFun(ret,data);
        }break;
        case StockRzrqDateListReq: //融资融券日历接口
        {
            StockRzrqDateList data;
            InfoRet ret = m_parser->stockRzrqDateListParse(bytes,data);
            ret.id = id;
            req.StockRzrqDateListFun(ret,data);
        }break;
        case StockDzjyDateListReq: //大宗交易日历接口
        {
            StockRzrqDateList data;
            InfoRet ret = m_parser->stockDzjyDateListParse(bytes,data);
            ret.id = id;
            req.StockDzjyDateListFun(ret,data);
        }break;
        case StockDzjyListReq: //个股大宗交易接口
        {
            StockDzjyList data;
            InfoRet ret = m_parser->stockDzjyListParse(bytes,data);
            ret.id = id;
            req.StockDzjyListFun(ret,data);
        }break;
        case NewStockPreViewsReq: //新股预告
        {
            NewStockPreViews data;
            InfoRet ret = m_parser->newStockPreViewParse(bytes,data);
            ret.id = id;
            req.NewStockPreViewFun(ret,data);
        }break;
        case NewStockCalandarsReq: //新股日历
        {
            NewStockCalandars data;
            InfoRet ret = m_parser->newStockCalandarParse(bytes,data);
            ret.id = id;
            req.NewStockCalandarFun(ret,data);
        }break;
        case NewStockTodaysBidsReq: //今日申购
        {
            NewStockTodaysBids data;
            InfoRet ret = m_parser->newStockTodaysBidParse(bytes,data);
            ret.id = id;
            req.NewStockTodaysBidFun(ret,data);
        }break;
        case NewStockMarketTodaysReq: //今日上市
        {
            NewStockMarketTodays data;
            InfoRet ret = m_parser->newStockMarketTodayParse(bytes,data);
            ret.id = id;
            req.NewStockMarketTodayFun(ret,data);
        }break;
        case NewStockMarketIssuedsReq: //已发行待上市
        {
            NewStockMarketIssueds data;
            InfoRet ret = m_parser->newStockMarketIssuedParse(bytes,data);
            ret.id = id;
            req.NewStockMarketIssuedFun(ret,data);
        }break;
        case NewStockIPOPerformancesReq: //新股上市表现
        {
            NewStockIPOPerformances data;
            InfoRet ret = m_parser->newStockIPOPerformanceParse(bytes,data);
            ret.id = id;
            req.NewStockIPOPerformanceFun(ret,data);
        }break;
        case CompanyOptionEventsReq:
        {
            CompanyOptionEvents data;
            InfoRet ret = m_parser->companyOptionEventsParse(bytes,data);
            ret.id = id;
            req.CompanyOptionEventsFun(ret,data);
        }break;
        case PlateNewsListsReq:
        {
            PlateNewsLists data;
            int nextpage=-1;
            InfoRet ret = m_parser->plateNewsListsParse(bytes,data,nextpage);
            ret.id = id;
            req.PlateNewsListsFun(ret,data,nextpage);
        }break;
        case PlateNewsDetailsReq:
        {
            PlateNewsDetails data;
            InfoRet ret = m_parser->plateNewsDetailsParse(bytes,data);
            ret.id = id;
            req.PlateNewsDetailsFun(ret,data);
        }break;
        case IndexNewsListsReq:
        {
            IndexNewsLists data;
            int nextpage=-1;
            InfoRet ret = m_parser->indexNewsListsParse(bytes,data,nextpage);
            ret.id = id;
            req.IndexNewsListsFun(ret,data,nextpage);
        }break;
        case IndexNewsDetailsReq:
        {
            IndexNewsDetails data;
            InfoRet ret = m_parser->indexNewsDetailsParse(bytes,data);
            ret.id = id;
            req.IndexNewsDetailsFun(ret,data);
        }break;
        case TopicInvestReq:
        {
            TopicInvest data;
            InfoRet ret = m_parser->topicInvestParse(bytes,data);
            ret.id = id;
            req.TopicInvestFun(ret,data);
        }break;
        case TopicInvestDetailReq:
        {
            TopicInvestDetail data;
            InfoRet ret = m_parser->topicInvestDetailParse(bytes,data);
            ret.id = id;
            req.TopicInvestDetailFun(ret,data);
        }break;
        case TopicInvestNewsReq:
        {
            TopicInvestNews data;
            InfoRet ret = m_parser->topicInvestNewsParse(bytes,data);
            ret.id = id;
            req.TopicInvestNewsFun(ret,data);
        }break;
        case OptionalWeeklyReq:
        {
            OptionalWeekly data;
            InfoRet ret = m_parser->OptionalWeeklyParse(bytes,data);
            ret.id = id;
            req.OptionalWeeklyFun(ret, data);
        }break;
        default:
            break;
        }
    }
}

void HzInfoCenter::progressFun(qint64 id, qint64 bytesReceived, qint64 bytesTotal)
{

}

void HzInfoCenter::errorFun(qint64 id, QNetworkReply::NetworkError error)
{
    auto it = m_reqMap.find(id);
    if(it != m_reqMap.end())
    {
        InfoRet ret;
        ret.message = QStringLiteral("请求发送网络错误");
        ret.code = error;
        ret.id = id;
        ReqInfo req= it.value();
        switch (req.type) {
        case HostNewsReq:
        {
            req.hostNewsFun(ret,HostNews(),-1);
        }
            break;
        case HostDetailReq:
        {
            HostNewDetail data;
            req.hostNewDetailFun(ret,data);
        }
            break;
        case FlashNewsReq:
        {
            req.flashNewsFun(ret,FlashNews(),-1);
        }
            break;
        case FlashNewDetailReq:
        {
            FlashNewDetail data;
            req.flashNewDetailFun(ret,data);
        }
            break;
        case FinanceInfoReq:
        {
            FinanceInfo data;
            req.financeInfoFun(ret,data);
        }
            break;
        case ResearchReportReq:
        {
            ResearchReports data;
            req.researchReportFun(ret,data,-1);
        }
            break;
        case ResearchReportDetailReq:
        {
            ResearchReportDetail data;
            req.researchReportDetailFun(ret,data);
        }
            break;
        case NewsInfoReq:
        {
            NewsInfo data;
            req.newsInfoCallBackFun(ret,data,-1);
        }
            break;
        case NewInfoDetailReq:
        {
            NewInfoDetail data;
            req.newInfoDetailCallBackFun(ret,data);
        }
            break;
        case StockLhbDatasReq:
        {
            StockLhbDatas data;
            req.stockLhbDatasFun(ret, data);
        }break;
        case StockLhbDetialReq:
        {
            StockLhbDetialList data;
            req.stockLhbDetialFun(ret, data);
        }break;
        case StockYybDetailReq:
        {
            StockYybDetail data;
            req.stockYybDetailFun(ret, data);
        }break;
        case StockYybDrbReq:
        {
            StockYybDrbList data;
            req.stockYybDrbFun(ret,data,-1);
        }break;
        case StockYybSlbReq:
        {
            StockYybSlbList data;
            req.stockYybSlbFun(ret,data,-1);
        }break;
        case StockYybHybReq:
        {
            StockYybHybList data;
            req.stockYybHybFun(ret,data,-1);
        }break;
        case StockLhbDateListReq: //龙虎榜榜单日期
        {
            StockLhbDateList data;
            req.stockLhbDateListFun(ret,data);
        }break;
        case StockDrListReq: //除权出息接口
        {
            StockDrList data;
            req.stockDrListFun(ret,data);
        }break;
        case StockRzrqTopListReq: //融券融券个股数据榜单
        {
            StockRzrqTopList data;
            req.StockRzrqTopListFun(ret,data,-1);
        }break;
        case StockBoardRzrqTopListReq: //融资融券板块榜接口
        {
            StockBoardRzrqTopList data;
            req.StockBoardRzrqTopListFun(ret,data);
        }break;
        case StockBoardRzrqListReq: //板块成分股融资融券数据接口
        {
            StockBoardRzrqList data;
            req.StockBoardRzrqListFun(ret,data,-1);
        }break;
        case StockRzrqDetailListReq: //融券融券个股详情数据榜单
        {
            StockRzrqDetailList data;
            req.StockRzrqDetailListFun(ret,data);
        }break;
        case StockMarketRzrqListReq: //两市融资融券数据接口
        {
            StockMarketRzrqList data;
            req.StockMarketRzrqListFun(ret,data);
        }break;
        case StockRzrqDateListReq: //融资融券日历接口
        {
            StockRzrqDateList data;
            req.StockRzrqDateListFun(ret,data);
        }break;
        case StockDzjyDateListReq: //大宗交易日历接口
        {
            StockRzrqDateList data;
            req.StockDzjyDateListFun(ret,data);
        }break;
        case StockDzjyListReq: //个股大宗交易接口
        {
            StockDzjyList data;
            req.StockDzjyListFun(ret,data);
        }break;
        case NewStockPreViewsReq: //新股预告
        {
            NewStockPreViews data;
            req.NewStockPreViewFun(ret,data);
        }break;
        case NewStockCalandarsReq: //新股日历
        {
            NewStockCalandars data;
            req.NewStockCalandarFun(ret,data);
        }break;
        case NewStockTodaysBidsReq: //今日申购
        {
            NewStockTodaysBids data;
            req.NewStockTodaysBidFun(ret,data);
        }break;
        case NewStockMarketTodaysReq: //今日上市
        {
            NewStockMarketTodays data;
            req.NewStockMarketTodayFun(ret,data);
        }break;
        case NewStockMarketIssuedsReq: //已发行待上市
        {
            NewStockMarketIssueds data;
            req.NewStockMarketIssuedFun(ret,data);
        }break;
        case NewStockIPOPerformancesReq: //个股大宗交易接口
        {
            NewStockIPOPerformances data;
            req.NewStockIPOPerformanceFun(ret,data);
        }break;
        case CompanyOptionEventsReq:
        {
            CompanyOptionEvents data;
            req.CompanyOptionEventsFun(ret,data);
        }break;
        case PlateNewsListsReq:
        {
            PlateNewsLists data;
            req.PlateNewsListsFun(ret,data,-1);
        }break;
        case PlateNewsDetailsReq:
        {
            PlateNewsDetails data;
            req.PlateNewsDetailsFun(ret, data);
        }break;
        case IndexNewsListsReq:
        {
            IndexNewsLists data;
            req.IndexNewsListsFun(ret,data,-1);
        }break;
        case IndexNewsDetailsReq:
        {
            IndexNewsDetails data;
            req.IndexNewsDetailsFun(ret,data);
        }break;
        case TopicInvestReq:
        {
            TopicInvest data;
            req.TopicInvestFun(ret,data);
        }break;
        case TopicInvestDetailReq:
        {
            TopicInvestDetail data;
            req.TopicInvestDetailFun(ret,data);
        }break;
        case TopicInvestNewsReq:
        {
            TopicInvestNews data;
            req.TopicInvestNewsFun(ret,data);
        }break;
        case OptionalWeeklyReq:
        {
            OptionalWeekly data;
            req.OptionalWeeklyFun(ret,data);
        }break;
        default:
            break;
        }
    }
}

QString HzInfoCenter::getInfoIp()
{
    return Hzcfg::getInstance()->serverInfo().infoSysUrl;
    return "http://127.0.0.1:8081";
}

QString HzInfoCenter::getInfoH5Ip()
{
    return Hzcfg::getInstance()->serverInfo().infoStsH5Url;
    return "http://127.0.0.1:8081";
}

QString HzInfoCenter::getInfoIpFromQuote()
{
    return Hzcfg::getInstance()->serverInfo().infoQuoteUrl;
}
