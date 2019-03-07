#ifndef HZINFOCENTER_H
#define HZINFOCENTER_H
/********************************************************************
created:
author: min.cheng
purpose: 资讯数据获取接口
*********************************************************************/
#include <memory>
#include <QMap>
#include "hzinfocenter_global.h"
#include "httpclientdef.h"
#include "hzinfodata.h"
#include "hzinfodata.h"


class HttpClient;
class InfoJsonParser;

class HZINFOCENTERSHARED_EXPORT HzInfoCenter
{

public:
    static HzInfoCenter* getInstance();

    /**
     * @brief 取消请求
     */
    void cancle(InfoReqId id);

    /**
     * @brief 异步请求热门资讯
     * @param[in] nPage 0表示最新页，下一页请求的page来源于响应数据中的nextpage
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqHostNews(int nPage,const HostNewsCallBack& fun);

    /**
     * @brief reqHostNews  异步请求热门资讯
     * @param tag 标签
     * @param page 当前页
     * @param fun callback
     * @return 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqHostNews(int tag, int page, int source, const HostNewsCallBack &fun);

    /**
     * @brief 热门资讯详情
     * @param[in] id 需要资讯的ID
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID
     */
    InfoReqId reqHostDetail(const QString& id,const HostNewDetailCallBack& fun);

    /**
     * @brief 请求7*24小时快讯
     * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqFlashNew(int nPage, int source, const FlashNewsCallBack& fun);

    /**
     * @brief 7*24小时快讯详情
     * @param[in] id 需要快讯的ID
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID
     */
    InfoReqId reqFlashNewDetail(const QString& id,const FlashNewDetailCallBack& fun);

    /**
     * @brief 请求新闻
     * @param[in] stockList 股票列表
     * @param[in] npage
     * @param[in] tag 指定标签
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqNewsInfo(const QVector<QString>& stockList,int nPage, int tag, int source, const NewsInfoCallBack& fun);

    /**
     * @brief 请求新闻详情
     * @param[in] id 新闻详情的ID
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID
     */
    InfoReqId reqNewInfoDetail(const QString& id,const NewInfoDetailCallBack& fun);

    /**
     * @brief 请求公告
     * @param[in] stockList 股票列表
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqAnncsInfo(const QVector<QString>& stockList,int nPage,const AnncInfoCallBack& fun);

    /**
     * @brief 请求公告
     * @param[in] stockList 股票列表
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID 返回RewInfoInvalidId表示数据请求完了
     */
    InfoReqId reqAnncsInfo(const QVector<QString>& stockList,int nPage,int tag, int source, const AnncInfoCallBack& fun);

    /**
     * @brief 请求公告详情
     * @param[in] id 新闻详情的ID
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID
     */
    InfoReqId reqAnncInfoDetail(const QString& id,const AnncInfoDetailCallBack& fun);


    /**
     * @brief 请求财务
     * @param[in] stockCode 股票名称
     * @param[in] fun 回调函数
     * @retval
     */
    InfoReqId reqFinanceInfo(const QString& stockCode,const FinanceInfoCallBack& fun);

    /**
     * @brief 请求研报
     * @param[in] stockCode 股票名称
     * @param[in] fun 回调函数
     * @retval
     */
    InfoReqId reqResearchReport(const QString& stockCode, int page, const ResearchReportCallBack& fun);

    /**
     * @brief 研报详情
     * @param[in] id 需要快讯的ID
     * @param[in] fun 返回回调函数
     * @retval 返回请求ID
     */
    InfoReqId reqResearchReportDetail(const QString& id,const ResearchReportDetailCallBack& fun);

    /**
     * @brief reqMarketCalendar 股市日历
     * @param date 字符串类型的日期，须满足yyyyMMdd格式
     * @param fun  返回回调函数
     * @return  返回请求ID
     */
    InfoReqId reqMarketCalendar(const QString &date, const MarketCalendarCallBack &fun);


    /**
     * @brief reqMarketCalandarTimes 股市日历时间列表
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqMarketCalandarTimes(const MarketCalandarTimesCallBack &fun);

    /**
     * @brief reqEnonomicCalendar 经济数据
     * @param date 字符串类型的日期，须满足yyyyMMdd格式
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqEconomicData(const QString &date, const EconomicDataCallBack &fun);

    /**
     * @brief reqEconomicDataTimes  经济数据日历列表
     * @param fun
     * @return
     */
    InfoReqId reqEconomicDataTimes(const EconomicDataTimesCallBack &fun);
    /**
     * @brief reqEnonomicCalendar 重大会议
     * @param date 字符串类型的日期，须满足yyyyMMdd格式
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqMajorMeeting(const QString &date, const MajorMeetingCallBack &fun, int source=2);

    /**
     * @brief reqEnonomicCalendar 重大会议detail
     * @param id 字符串类型的日期，须满足yyyyMMdd格式
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqMajorMeetingDetail(int id, const MajorMeetingDetailCallBack &fun);

    /**
     * @brief reqEnonomicCalendar 财报发布列表
     * @param date 字符串类型的日期，须满足yyyyMMdd格式
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqFinancialReport(const QString &date, const FinancialReportCallBack &fun,int source=2);

    /**
     * @brief reqFinancialReport 财报发布列表详情
     * @param date  字符串类型的日期，须满足yyyyMMdd格式
     * @param code 股票代码 eg:666666
     * @param fun 返回回调函数
     * @return 返回请求ID
     */
    InfoReqId reqFinancialDetailReport(const QString &date, const QString &code, const FinancialReportDetailCallBack &fun);

    /**
     * @brief 龙虎榜个股榜单
     * @param[in]  nTime 时间戳
     * @param[in]  type 榜单类型
     * @retval 返回请求ID
     */
    InfoReqId reqStockLhbDatas(INT64 nTime,StockLhbType type,const StockLhbDatasCallBack& fun);

    /**
     * @brief  个股龙虎榜单详情 回调返回当前日期连续最大5天的数据
     * @param[in] nTime 时间戳
     * @param[in] stockcode 股票代码
     * @return 返回请求ID
     */
    InfoReqId reqStockLhbDetial(INT64 nTime,const QString& stockcode,const StockLhbDetialCallBack& fun);

    /**
     * @brief  营业部详情
     * @param[in] strId 营业部ID
     * @param[in] fun 回调函数
     * @return 返回请求ID
     */
    InfoReqId reqStockYybDetail(const QString& strId,const StockYybDetailCallBack& fun);

    /**
     * @brief  营业部当日榜单
     * @param[in] nTime 当日时间戳
     * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage 服务器默认返回20个数据
     * @return 返回请求ID
     */
    InfoReqId reqStockYybDrb(INT64 nTime,int nPage,const StockYybDrbCallBack& fun);

    /**
     * @brief  营业部实力榜
     * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage 服务器默认返回20个数据
     * @return 返回请求ID
     */
    InfoReqId reqStockYybSlb(int nPage,const StockYybSlbCallBack& fun);

    /**
     * @brief  营业部活跃榜
     * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage 服务器默认返回20个数据
     * @return 返回请求ID
     */
    InfoReqId reqStockYybHyb(int nPage,const StockYybHybCallBack& fun);

    /**
     * @brief  龙虎榜榜单日期
     * @return 返回请求ID
     */
    InfoReqId reqStockLhbDateList(const StockLhbDateListCallBack& fun);

    /**
     * @brief  除权出息接口
     * @param[in] stockcode 股票
     * @return 返回请求ID
     */
    InfoReqId reqStockDrList(const QString& stockcode,StockDrListCallBack& fun);

    /**
     * @brief 融资融券个股榜单
     * @param[in] nTime 当日时间戳
     * @param[in] sort 排序 0：买入额降序，1：净买入额降序，2：净卖出额降序
     * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage 服务器默认返回10个数据
     * @retval 返回请求ID
     */
    InfoReqId reqStockRzrqTopList(INT64 nTime,int sort,int nPage,StockRzrqTopListCallBack& fun);

    /**
      * @brief 融资融券板块榜接口
      * @param[in] nTime 当日时间戳
      * @retval 返回请求ID
      */
    InfoReqId reqStockBoardRzrqTopList(INT64 nTime,StockBoardRzrqTopListCallBack& fun);

    /**
      * @brief 板块成分股融资融券数据接口
      * @param[in] nTime 当日时间戳
      * @param[in] boardcode 板块代码
      * @param[in] nPage 请求页码，最新页填0 后面请求其他页请使用回调返回的nextpage 服务器默认返回？个数据
      * @retval 返回请求ID
      */
    InfoReqId reqStockBoardRzrqList(INT64 nTime,const QString& boardcode,int nPage,StockBoardRzrqListCallBack& fun);

    /**
      * @brief 个股融资融券详情接口
      * @param[in] nTime 当日时间戳
      * @param[in] stockcode 股票代码
      * @retval 返回请求ID
      */
    InfoReqId reqStockRzrqDetailList(INT64 nTime,const QString& stockcode,StockRzrqDetailListCallBack& fun);

    /**
      * @brief 两市融资融券数据接口 用于绘制融资融券曲线图
      * @param[in] nTime 当日时间戳
      * @retval 返回请求ID
      */
    InfoReqId reqStockMarketRzrqList(INT64 nTime,StockMarketRzrqListCallBack& fun);

    /**
      * @brief 融资融券日历接口
      * @retval 返回请求ID
      */
    InfoReqId reqStockRzrqDateList(StockRzrqDateListCallBack& fun);

    /**
      * @brief 大宗交易日历接口
      * @retval 返回请求ID
      */
    InfoReqId reqStockDzjyDateList(StockDzjyDateListCallBack& fun);

    /**
      * @brief 个股大宗交易接口
      * @param[in] nTime 当日时间戳
      * @retval 返回请求ID
      */
    InfoReqId reqStockDzjyList(INT64 nTime,StockDzjyListCallBack& fun);

    /**
     * @brief reqNewStockPreViews 新股预告
     * @return 返回请求ID
     */
    InfoReqId reqNewStockPreViews(NewStockPreViewCallBack &fun);

    /**
     * @brief reqNewStockCalandars 新股日历
     * @return 返回请求ID
     */
    InfoReqId reqNewStockCalandars(NewStockCalandarCallBack &fun);

    /**
     * @brief reqNewStockTodaysBids 今日申购,nTime,请求日期：-1表示请求最新的
     * @return 返回请求ID
     */
    InfoReqId reqNewStockTodaysBids(INT64 nTime,NewStockTodaysBidCallBack &fun);

    /**
     * @brief reqNewStockMarketTodays 今日上市
     * @param fun
     * @return 返回请求ID
     */
    InfoReqId reqNewStockMarketTodays(NewStockMarketTodayCallBack &fun);

    /**
     * @brief reqNewStockMarketIssueds 已发行待上市
     * @param fun
     * @return 返回请求ID
     */
    InfoReqId reqNewStockMarketIssueds(NewStockMarketIssuedCallBack &fun);

    /**
     * @brief reqNewStockIPOPerformances 新股上市表现
     * @param fun
     * @return 返回请求ID
     */
    InfoReqId reqNewStockIPOPerformances(NewStockIPOPerformanceCallBack &fun);

    /**
     * @brief reqCompanyOptionEvents 公司大事
     * @param stocks 股票代码，多个股票用逗号分隔
     * @param source 请求来源（可选，默认为1），枚举值及含义（1：APP，2：PC白色皮肤，3：PC黑色皮肤）
     * @param type 类型（可选，默认为0），枚举值及含义（0：所有，1：龙虎榜，2：分红送转，3：财报）
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqCompanyOptionEvents(const QVector<QString> &stocks, int source, int type, CompanyOptionEventsCallBack& fun);

    /**
     * @brief reqPlateNewsLists 板块新闻列表
     * @param platecode 板块id
     * @param page 页码
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqPlateNewsLists(const QString& platecode, int page, PlateNewsListsCallBack &fun);

    /**
     * @brief reqPlateNewsDetails 板块新闻详情
     * @param msgId 板块新闻id
     * @param fun   回调
     * @return 返回请求ID
     */
    InfoReqId reqPlateNewsDetails(const QString &msgId, PlateNewsDetailsCallBack &fun);

    /**
     * @brief reqIndexNewsLists 指数新闻列表接口
     * @param page 页码（可选，默认为0），0表示最新页
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqIndexNewsLists(IndexNewsListsCallBack &fun, int page=0);

    /**
     * @brief reqIndexNewsDetails 指数新闻详情
     * @param news_id 指数新闻id
     * @param data 回调
     * @return 返回请求ID
     */
    InfoReqId reqIndexNewsDetails(int news_id, IndexNewsDetailsCallBack &data);

    /**
     * @brief reqTopicInvest 主题投资
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqTopicInvest(TopicInvestCallBack &fun);

    /**
     * @brief reqTopicInvestDetail 主题投资详情
     * @param topic_id 主题id
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqTopicInvestDetail(const QString& topic_id, TopicInvestDetailCallBack &fun);

    /**
     * @brief TopicInvestNews 相关资讯
     * @param topic_id 主题id
     * @param page 页码，默认0
     * @param fun 回调
     * @return 返回请求ID
     */
    InfoReqId reqTopicInvestNews(int topic_id, int page, TopicInvestNewsCallBack &fun);


    /**
     * @brief reqOptionalWeekly 自选股周报
     * @param stockcode
     * @param fun
     * @return
     */
    InfoReqId reqOptionalWeekly(const QString &stockcodes, OptionalWeeklyCallBack &fun);


    //获取资讯地址H5 （目前与F10有关的网页，已在server.json文件中配置）
    QString getInfoH5Ip();

private:
    HzInfoCenter();
    ~HzInfoCenter();
    void finishedFun(qint64 id,const QByteArray& bytes);
    void progressFun(qint64 id,qint64 bytesReceived,qint64 bytesTotal);
    void errorFun(qint64 id,QNetworkReply::NetworkError error);
    //获取资讯地址
    QString getInfoIp();

    //获取资讯地址--行情服务器提供的
    QString getInfoIpFromQuote();

private:
    QMap<qint64,ReqInfo> m_reqMap;
    HttpClientCallBack m_callFun;
    HttpClient* m_httpClient;
    std::shared_ptr<InfoJsonParser> m_parser;
};

#define gHzInfoCenter  HzInfoCenter::getInstance()

#endif // HZINFOCENTER_H
