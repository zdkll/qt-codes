#ifndef INFOJSONPARSER_H
#define INFOJSONPARSER_H
/********************************************************************
created:
author: min.cheng
purpose: 负责解释资讯服务器返回json数据到本地数据结构
*********************************************************************/
#include "hzinfodata.h"

class InfoJsonParser
{
public:
    InfoJsonParser();

    INT64 yyyymmddToTime(const QString &date);

    QString timeToYyyymmdd(INT64 time);

    //资讯数据
    InfoRet hostNew(const QByteArray& byteArray,HostNews& data,int& nextpage);

    //资讯详情
    InfoRet hostHostNewDetail(const QByteArray& byteArray,HostNewDetail& data);

    //7*24小时快讯
    InfoRet flashNew(const QByteArray& byteArray,FlashNews& data,int& nextpage);

    //7*24小时快讯详情
    InfoRet flashNewDetail(const QByteArray& byteArray,FlashNewDetail& data);

    //财务
    InfoRet financeInfo(const QByteArray& byteArray,FinanceInfo& data);

    //研报
    InfoRet researchReportInfo(const QByteArray& byteArray,ResearchReports& data, int& nextpage);

    //研报详情
    InfoRet researchReportDetailInfo(const QByteArray& byteArray,ResearchReportDetail& data);

    //新闻
    InfoRet newsInfo(const QByteArray& byteArray,NewsInfo& data,int& nextpage);

    //新闻详情
    InfoRet newInfoDetail(const QByteArray& byteArray,NewInfoDetail& data);

    //公告
    InfoRet anncsInfo(const QByteArray& byteArray,AnncsInfo& data,int& nextpage);

    //公告详情
    InfoRet anncInfoDetail(const QByteArray& byteArray,AnncInfoDetail& data);

    //股市日历
    InfoRet marketCalendarParse(const QByteArray &byteArray, MarketCalendar &data);

    //股市日历时间列表
    InfoRet marketCalendarTimesParse(const QByteArray &byteArray, MarketCalandarTimes &data);

    //经济数据日历表
    InfoRet economicDataTimesParse(const QByteArray &byte, EconomicDataTimes &data);

    //经济数据
    InfoRet economicDataParse(const QByteArray &byte, EconomicData &data);

    //重大事件
    InfoRet majorMeetingParse(const QByteArray &byte, MajorMeeting &data);

    //重大事件detail
    InfoRet majorMeetingDetailParse(const QByteArray &byte, MajorMeetingDetail &data);

    //财务发布
    InfoRet financialReportParse(const QByteArray &byte, FinancialReport &data);

    //财务发布详情
    InfoRet financialReportDetailParse(const QByteArray &byte, FinancialReportDetail &data);

    //龙虎榜个股榜单
    InfoRet stockLhbDatasParse(const QByteArray &byte,StockLhbDatas& data);

    //个股龙虎榜单详情
    InfoRet stockLhbDetialParse(const QByteArray &byte,StockLhbDetialList& data);

    //营业部详情
    InfoRet stockYybDetailParse(const QByteArray &byte,StockYybDetail& data);

    //营业部当日榜单
    InfoRet stockYybDrbParse(const QByteArray& byteArray,StockYybDrbList& data,int& nextpage);

    //营业部实力榜
    InfoRet stockYybSlbParse(const QByteArray& byteArray,StockYybSlbList& data,int& nextpage);

    //营业部活跃榜
    InfoRet stockYybHybParse(const QByteArray& byteArray,StockYybHybList& data,int& nextpage);

    //龙虎榜榜单日期
    InfoRet stockLhbDateListParse(const QByteArray& byteArray,StockLhbDateList& data);

    //除权出息
    InfoRet stockDrListParse(const QByteArray& byteArray,StockDrList& data);

    //融资融券个股榜单
    InfoRet stockRzrqTopListParse(const QByteArray& byteArray,StockRzrqTopList& data,int& nextpage);

    //融资融券板块榜接口
    InfoRet stockBoardRzrqTopListParse(const QByteArray& byteArray,StockBoardRzrqTopList& data);

    //板块成分股融资融券数据接口
    InfoRet stockBoardRzrqLisParse(const QByteArray& byteArray,StockBoardRzrqList& data,int& nextpage);

    //融券融券个股详情数据榜单
    InfoRet stockRzrqDetailListParse(const QByteArray& byteArray,StockRzrqDetailList& data);

    //两市融资融券数据接口
    InfoRet stockMarketRzrqListParse(const QByteArray& byteArray,StockMarketRzrqList& data);

    //融资融券日历接口
    InfoRet stockRzrqDateListParse(const QByteArray& byteArray,StockRzrqDateList& data);

    //大宗交易日历接口
    InfoRet stockDzjyDateListParse(const QByteArray& byteArray,StockDzjyDateList& data);

    //个股大宗交易接口
    InfoRet stockDzjyListParse(const QByteArray& byteArray,StockDzjyList& data);

    //新股预告接口
    InfoRet newStockPreViewParse(const QByteArray& byteArray,NewStockPreViews& data);

    //新股日历接口
    InfoRet newStockCalandarParse(const QByteArray& byteArray,NewStockCalandars& data);

    //今日申购接口
    InfoRet newStockTodaysBidParse(const QByteArray& byteArray,NewStockTodaysBids& data);

    //今日上市接口
    InfoRet newStockMarketTodayParse(const QByteArray& byteArray,NewStockMarketTodays& data);

    //已发行待上市接口
    InfoRet newStockMarketIssuedParse(const QByteArray& byteArray,NewStockMarketIssueds& data);

    //新股上市表现
    InfoRet newStockIPOPerformanceParse(const QByteArray& byteArray,NewStockIPOPerformances& data);

    //公司大事
    InfoRet companyOptionEventsParse(const QByteArray &byteArray, CompanyOptionEvents& data);

    //板块新闻列表
    InfoRet plateNewsListsParse(const QByteArray &byteArray, PlateNewsLists &data, int &page);

    //板块新闻详情
    InfoRet plateNewsDetailsParse(const QByteArray &byteArray, PlateNewsDetails &data);

    //指数新闻列表接口
    InfoRet indexNewsListsParse(const QByteArray &byteArray, IndexNewsLists &data, int &page);

    //指数新闻详情
    InfoRet indexNewsDetailsParse(const QByteArray &byte, IndexNewsDetails &data);

    //主题投资
    InfoRet topicInvestParse(const QByteArray &byte, TopicInvest &data);

    //主题投资详情
    InfoRet topicInvestDetailParse(const QByteArray &byte, TopicInvestDetail &data);

    //相关资讯
    InfoRet  topicInvestNewsParse(const QByteArray &byte, TopicInvestNews &data);

    //自选股周报
    InfoRet OptionalWeeklyParse(const QByteArray &byte, OptionalWeekly &data);



private:
    TagType converTagType(const QString& tag);
    QString converTagType(int type);
};

#endif // INFOJSONPARSER_H
