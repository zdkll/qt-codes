#ifndef HZINFODATA_H
#define HZINFODATA_H
/********************************************************************
created:
author: min.cheng
purpose: 定义资讯数据结构
*********************************************************************/
#include <QVector>
#include <functional>
#include <QMetaType>
#include "hzpublic.hpp"

#define  RewInfoInvalidId -1

/**
  * @brief 资讯请求ID
  */
typedef qint64 InfoReqId;
/**
  * @brief 资讯返回结构
  */
struct InfoRet
{
    qint64 id = INVALID_INT64;
    int code = -1;        //成功为0 失败-1
    QString message="";
};
//0：全部，1：强烈利好，2：利好，3：中性，4：利空，5：强烈利空
enum TagType
{
    UnknownTag= 0,
    StrongBullTag, //"强烈利好"
    BullTag,    //"利好"
    NeutralTag, //"中性"
    BadNewsTag, //"利空"
    StrongBadNewsTag, //"强烈利空"

};
/////////////////////////////热门资讯begin///////////////////////////////////////////
/**
  * @brief 热门资讯
  */
struct HostNewItem
{
    QString id="";
    QString title="";
    QString tag="";
    TagType type = UnknownTag;
    QVector<QString> stockCodes;
    QString source="";
    QString desc="";
    qint64 time = INVALID_INT64;
    QString url="";//detail
};

//热门资讯
typedef QVector<HostNewItem> HostNews;
//热门资讯回调函数  返回信息    返回数据   int返回-1表示数据
typedef std::function<void(const InfoRet&,HostNews&,int)> HostNewsCallBack;

/**
  * @brief 热门资讯详情
  */
struct HostNewDetail
{
    QString title="";
    QString content="";
    QString tag="";
    TagType type =  UnknownTag;
    QVector<QString> stockCodes;
    QString source="";
    qint64 time = INVALID_INT64;
};

//热门资讯详情回调函数
typedef std::function<void(const InfoRet&,const HostNewDetail&)> HostNewDetailCallBack;

/////////////////////////////热门资讯end///////////////////////////////////////////

/////////////////////////////7*24小时快讯begin/////////////////////////////////////
/**
  * @brief 7*24小时快讯
  */
struct FlashNewItem
{
    QString id="";
    QString title="";
    QString url;
    qint64 time = INVALID_INT64;
};

//7*24小时快讯
typedef QVector<FlashNewItem> FlashNews;
//7*24小时快讯回调函数 InfoRet=>返回数据  FlashNews=>返回数据 int=>nextpage
typedef std::function<void(const InfoRet&,FlashNews&,int)> FlashNewsCallBack;


/**
  * @brief 7*24小时快讯详情
  */
struct FlashNewDetail
{
    QString title="";
    QString source="";
    qint64 time = INVALID_INT64;
    QString content="";
};

//7*24小时快讯详情回调函数
typedef std::function<void(const InfoRet&,const FlashNewDetail&)> FlashNewDetailCallBack;

/////////////////////////////7*24小时快讯end/////////////////////////////////////

/////////////////////////////财务请求begin/////////////////////////////////////
//财务数据营业收入
struct FinanceTaking
{
    struct FinanceTakingItem
    {
        QString year=""; //年份 例如2018
        QVector<double> quarterVec; //季度营业收入 0 第一季度 1 第二季度 。。。。（可能只有一个季度）
        double yearOnYearGrowth = INVALID_DOUBLE; //该年同比增长
    };
    QString overview="";
    QVector<FinanceTakingItem> dataList;
};

//关键指标
struct FinanceKeyindex
{
    struct EarningsPerShareChart
    {
        QString year="";
        double earnings=INVALID_DOUBLE; //每股收益
    };
    QString title="";
    QString eps=""; //该季度每股收益
    QString epsNum=""; //该季度每股收益 例如 0.4元
    double earningsPerShare=INVALID_DOUBLE; //每股收益
    double netAssetValuePreShare =INVALID_DOUBLE; //每股净资产
    double roe=INVALID_DOUBLE; //净资产收益率
    QVector<EarningsPerShareChart> earningsPerShareChart; //年份对应的每股收益
};

//profit（利润）
struct FinanceProfit
{
    struct ProfitChartData
    {
        QString year=""; //年份
        double retainedProfit=INVALID_DOUBLE; //净利润
        double retainedProfitsUp =INVALID_DOUBLE; //同比增长
    };
    QString title="";
    QString overview=""; //季报概括
    double grossRevenus =INVALID_DOUBLE; //营业总收入
    double operatingProfit =INVALID_DOUBLE; //营业利润
    double retainedProfits =INVALID_DOUBLE; //净利润
    double grossRevenUp =INVALID_DOUBLE; //营业总收入同比增长
    double operatinProfitUp =INVALID_DOUBLE; //营业利润同比增长
    double retainedProfitsUp=INVALID_DOUBLE;//净利润同比增长
    QVector<ProfitChartData> chartData; //用于列表绘图
};

//balanceSheet（资产负债）
struct FinanceBalanceSheet
{
    QString title="";  //季度
    double totalAssets = INVALID_DOUBLE; //资产总计
    double totalLiabilities = INVALID_DOUBLE; //负债总计
    double totalOwnersEquity = INVALID_DOUBLE; //所有者权益总计
    double assetLiabilityRatio = INVALID_DOUBLE; //资产负债率
};

//cashFlow（现金流量）
struct FinanceCashFlow
{
    QString title="";
    double operatingCashFlow=INVALID_DOUBLE;
    double netInvestmentCash=INVALID_DOUBLE;
    double netAmountOfFinancingCash=INVALID_DOUBLE;
};

/**
  * @brief 财务数据
  */
struct FinanceInfo
{
    FinanceTaking taking;     //营业收入信息
    FinanceKeyindex keyIndex; //关键指标
    FinanceProfit profit;     //利润表
    FinanceBalanceSheet balanceSheet;//（资产负债）
    FinanceCashFlow cashFlow;//（现金流量）
};

//财务数据回调 InfoRet=>返回数据  FinanceInfo=>返回数据
typedef std::function<void(const InfoRet&,const FinanceInfo&)> FinanceInfoCallBack;

/////////////////////////////财务请求End/////////////////////////////////////


/////////////////////////////研报列表请求begin/////////////////////////////////////
struct ResearchReportItem
{
    qint64  iTime = INVALID_INT64;    //此条数据时间戳
    QString id="";       //研报对应的唯一ID
    QString iDescription="";//	简述
    QString iFrom="";  //来源
    QString iTitle=""; //标题
    QString iTagName=""; //标签
    QString url;
    QString stockcode;
    QString tag;
    QVector<QString> rerated_stockcode;
    TagType type =  UnknownTag;
};
typedef QVector<ResearchReportItem> ResearchReports;
typedef std::function<void(const InfoRet&,const ResearchReports&,int)> ResearchReportCallBack;
/////////////////////////////研报列表请求end///////////////////////////////////////


/////////////////////////////研报详情请求begin/////////////////////////////////////
struct ResearchReportDetail
{
    QString id="";             //id
    QString iContent="";       //研报详细内容
    QString iTitle="";         //研报标题
    QString iNewsID="";        //研报唯一ID
    QString iDescription="";   //研报摘要
    qint64  iTime = INVALID_INT64;          //研报更新时间
    QString iFrom="";          //研报来源
    QString iCode="";          //研报股票代码
    QVector<QString> iRelatedStock;  //研报相关股票代码
    QString iThirdUrl="";      //研报PDF链接
    QString iTagName="";       //研报标签（利空，利好等）
    TagType type =  UnknownTag;
};
typedef std::function<void(const InfoRet&,const ResearchReportDetail&)> ResearchReportDetailCallBack;
/////////////////////////////研报详情请求end///////////////////////////////////////

/////////////////////////////新闻请求end///////////////////////////////////////
struct NewInfoItem
{
    QString id="";
    QString stockcode="";
    QString stockname="";
    QString title="";
    QString desc="";
    QString tag="";
    QString source="";
    qint64 time = INVALID_INT64;
    QVector<QString> related_stockcode;
    TagType type =  UnknownTag;
    QString url="";//detail
};
typedef QVector<NewInfoItem> NewsInfo;
typedef std::function<void(const InfoRet&,const NewsInfo&,int)> NewsInfoCallBack;
/////////////////////////////新闻请求end///////////////////////////////////////

/////////////////////////////新闻详情请求end///////////////////////////////////////
struct NewInfoDetail
{
    QString stockcode="";
    QString title="";
    QString desc="";
    QString content="";
    QString tag="";
    QString source="";
    qint64 time = INVALID_INT64;
    QVector<QString> related_stockcode;
    TagType type =  UnknownTag;
};

Q_DECLARE_METATYPE(NewInfoDetail)

typedef std::function<void(const InfoRet&,const NewInfoDetail&)> NewInfoDetailCallBack;
/////////////////////////////新闻详情请求end///////////////////////////////////////


/////////////////////////////公告数据请求begin///////////////////////////////////////
struct AnncInfoItem
{
    QString id="";
    QString stockcode="";
    QString stockname="";
    QString title="";
    QString desc="";
    QString tag="";
    QString source="";
    qint64 time=INVALID_INT64;
    TagType type = UnknownTag;
    QString url="";//detail
};
Q_DECLARE_METATYPE(AnncInfoItem)

typedef QVector<AnncInfoItem> AnncsInfo;
typedef std::function<void(const InfoRet&,const AnncsInfo&,int)> AnncInfoCallBack;
////////////////////////////////公告数据请求end///////////////////////////////////////

/////////////////////////////公告详情数据请求begin///////////////////////////////////////
struct AnncInfoDetail
{
    QString stockcode="";
    QString title="";
    QString desc="";
    QString content="";
    QString tag="";
    QString source="";
    qint64 time=INVALID_INT64;
    TagType type = UnknownTag;
    QVector<QString> related_stockcode;
};

typedef std::function<void(const InfoRet&,const AnncInfoDetail&)> AnncInfoDetailCallBack;
////////////////////////////////公告详情数据请求end///////////////////////////////////////


////////////////////////////////股市日历数据请求////////////////////////////////////////////
////////////////////////////////股市日历数据请求////////////////////////////////////////////
//股市日历数据请求
struct MarketCalendar
{
    struct FHPXData
    {
        QString stockcode="";
        QString stockname="";
        QString record_date="";
        QString dividend_date="";
        QString detail="";
        qreal now;
        qreal chg;
    };

    struct TFPData
    {
        QString stockcode="";
        QString stockname="";
        QString suspension_date="";
        QString resumption_date="";
        QString reason="";
        qreal now;
        qreal chg;
    };

    struct XSJJData
    {
        QString stockcode="";
        QString stockname="";
        QString lifted="";
        QString scale="";
        qreal now;
        qreal chg;
    };
    struct ZFData
    {
        QString stockcode="";
        QString stockname="";
        QString issue_price="";
        QString issue_num="";
        QString issue_date="";
        qreal now;
        qreal chg;
    };
    struct YJPLData
    {
        QString stockcode="";
        QString stockname="";
        QString eps="";
        QString naps="";
        QString roe="";
        QString net_profit="";
        qreal now;
        qreal chg;
    };

    QVector<FHPXData> FHPXDatas;
    QVector<TFPData> TFPDatas;
    QVector<XSJJData> XSJJDatas;
    QVector<ZFData> ZFDatas;
    QVector<YJPLData> YJPLDatas;
};

struct MarketCalandarTimes
{
    QVector<quint64> datas;
};
Q_DECLARE_METATYPE(MarketCalendar)
Q_DECLARE_METATYPE(MarketCalendar::FHPXData)
Q_DECLARE_METATYPE(MarketCalendar::TFPData)
Q_DECLARE_METATYPE(MarketCalendar::XSJJData)
Q_DECLARE_METATYPE(MarketCalendar::ZFData)
Q_DECLARE_METATYPE(MarketCalendar::YJPLData)

typedef std::function<void(const InfoRet&, const MarketCalendar&)> MarketCalendarCallBack;
typedef std::function<void(const InfoRet&, const MarketCalandarTimes&)> MarketCalandarTimesCallBack;
////////////////////////////////////////////////////////////////////////////


//////////////////////////////////资讯相关数据begin/////////////////////////////////////////
//重大会议
struct MajorMeeting
{
    struct RelateStock
    {
        QString stockcode;
        QString stockname;
        bool isCustom;
    };
    struct MajorMeetPrivate
    {
        QString id="";
        QString title="";
        QString url;//detail
        QStringList relate_board;
        QVector<RelateStock> relate_stock;
        //QVector<QPair<QString,QString>> relate_stock;
    };

    QVector<MajorMeetPrivate> datas;
};

Q_DECLARE_METATYPE(MajorMeeting)
Q_DECLARE_METATYPE(MajorMeeting::MajorMeetPrivate)

//重大会议详情
struct MajorMeetingDetail
{
    quint32 time=INVALID_INT32; //会议时间戳
    QString title="";
    QString content="";
    QStringList relate_board;
    QVector<QPair<QString,QString>> relate_stock;
};
Q_DECLARE_METATYPE(MajorMeetingDetail)


//财报发布列表
struct FinancialReport
{
    struct FinancialItem
    {
        QString code="";//股票代码
        QString name="";//股票名称
        QString url;//detail
        bool detail=false;//是否有详情，布尔值
        bool isCustom=false;
    };

    QVector<FinancialItem> datas;
};

Q_DECLARE_METATYPE(FinancialReport)
Q_DECLARE_METATYPE(FinancialReport::FinancialItem)
//财报发布列表详情
struct FinancialReportDetail
{
    QString title="";	//标题
    QString content="";	//内容
    quint32 time = INVALID_INT32;	//财报发布时间戳
};
Q_DECLARE_METATYPE(FinancialReportDetail)

//经济数据
struct EconomicData
{
    struct EconomicDataItem {
        QString name="";//	名称
        QString pre_value="";//	前值
        QString exp_value="";//	预测值
        QString act_value="";//	公布值
        QString region="";//	地区
        QString source="";//	来源
        quint32 time =  INVALID_UINT32;//	公布时间戳
    };

    QVector<EconomicDataItem> datas;
};
struct EconomicDataTimes
{
    QVector<quint64> datas;
};
Q_DECLARE_METATYPE(EconomicDataTimes)
Q_DECLARE_METATYPE(EconomicData)
Q_DECLARE_METATYPE(EconomicData::EconomicDataItem)

struct EconomicCalendar
{
    EconomicData economicsData;
    MajorMeeting majorMeet;
    FinancialReport financialReport;
};

Q_DECLARE_METATYPE(EconomicCalendar)

typedef std::function<void(const InfoRet&, const EconomicDataTimes&)> EconomicDataTimesCallBack;
typedef std::function<void(const InfoRet&, const EconomicData&)> EconomicDataCallBack;
typedef std::function<void(const InfoRet&, const MajorMeeting&)> MajorMeetingCallBack;
typedef std::function<void(const InfoRet&, const MajorMeetingDetail&)> MajorMeetingDetailCallBack;
typedef std::function<void(const InfoRet&, const FinancialReport&)> FinancialReportCallBack;
typedef std::function<void(const InfoRet&, const FinancialReportDetail&)> FinancialReportDetailCallBack;
////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////资讯相关end/////////////////////////////////////////

/////////////////////////////////龙虎榜单begin/////////////////////////////////////////
/**
 * @brief 个股榜单相关
 */
enum StockLhbType
{
    LhbtTotalList = 0,   //总榜单
    LhbtOrganization,    //机构榜单
    LhbBusinessDepartmentStock,   //营业部个股榜单
};

struct StockLhbTypeItem
{
    QString stockcode = "";
    QString stockname = "";
    double close_price=INVALID_DOUBLE;
    double change_ratio=INVALID_DOUBLE;
    double buy=INVALID_DOUBLE;
    double sell=INVALID_DOUBLE;
    double net_buy=INVALID_DOUBLE;   //净买入
    QString block = "";    //板块名称
};

struct StockLhbDatas
{
    int nShCOunt=INVALID_INT32;  //上证数量
    int nSzCount=INVALID_INT32;  //深证数量
    QVector<StockLhbTypeItem> datas;
};
typedef std::function<void(const InfoRet&,const StockLhbDatas&)> StockLhbDatasCallBack;


/**
 * @brief 营业部当日榜单
 */
struct StockYybDrbItem
{
    QString id="";         //营业部id
    QString name="";       //营业部名称
    int stockCount=INVALID_INT32;     //参与股票数
    double net_buy=INVALID_DOUBLE;     //净买入
    double ratio_3d=INVALID_DOUBLE;    //3日跟买成功率，单位：%，空字符窜表示未统计到成功率
    QString stockName;//股票名称以“,”拼接
    QString stockCode; //股票代码
};
typedef QVector<StockYybDrbItem>  StockYybDrbList;
//回调函数  返回信息    返回数据   int返回-1表示数据到头或为空
typedef std::function<void(const InfoRet&,const StockYybDrbList&,int)> StockYybDrbCallBack;


/**
 * @brief 营业部实力榜单
 */
struct StockYybSlbItem
{
    QString id="";         //营业部id
    QString name="";       //营业部名称
    double max_amount=INVALID_DOUBLE;  //实力资金，单位：元
    long long max_amount_date=INVALID_INT64;  //实力资金所在日期
    double net_buy=INVALID_DOUBLE;             //最近净买入，单位：元
    long long net_buy_date=INVALID_INT64;     //最近净买入所在日期
    QString stockname="";          //最近操作股票名称
    QString stockcode="";          //最近操作股票代码
    long long stock_date=INVALID_INT64;       //最近操作日期
};
typedef QVector<StockYybSlbItem>  StockYybSlbList;
//回调函数  返回信息    返回数据   int返回-1表示数据到头或为空
typedef std::function<void(const InfoRet&,const StockYybSlbList&,int)> StockYybSlbCallBack;


/**
 * @brief 营业部活跃榜
 */
struct StockYybHybItem
{
    QString id="";                 //营业部id
    QString name="";               //营业部名称
    int count_3m=INVALID_INT32;    //近三月上榜次数
    double buy=INVALID_DOUBLE;     //买总额，单位：元
    double sell=INVALID_DOUBLE;    //卖总额，单位：元
    QString stockname="";          //最近操作股票名称
    QString stockcode="";          //最近操作股票代码
    double    stock_rate = INVALID_DOUBLE;
    long long stock_date=INVALID_INT64;       //最近操作日期
};
typedef QVector<StockYybHybItem>  StockYybHybList;
//回调函数  返回信息    返回数据   int返回-1表示数据到头或为空
typedef std::function<void(const InfoRet&,const StockYybHybList&,int)> StockYybHybCallBack;


/**
 * @brief 个股龙虎榜详情
 */
struct StockLhbDetialItem
{
    QString id="";         //营业部id
    QString name="";       //营业部名称
    double buy=INVALID_DOUBLE;         //买，单位：元
    double sell=INVALID_DOUBLE;        //卖，单位：元
    double ratio_3d=INVALID_DOUBLE;   //3日跟买成功率，单位：%，空字符窜表示未统计到成功率
};

struct StockLhbDetialListItem
{
    double buy=INVALID_DOUBLE;          //买入总计，单位：元
    double sell=INVALID_DOUBLE;         //卖出总计，单位：元
    double net_buy=INVALID_DOUBLE;      //净买入，单位：元
    QString reason="";      //上榜原因
    QVector<StockLhbDetialItem> top5_buy;  //买入前5营业部，数组格式
    QVector<StockLhbDetialItem> top5_sell; //卖出前5营业部，数组格式
};

struct StockLhbDetial
{
    long long date=INVALID_INT64;      //上榜日期
    QString change_ratio=""; //涨跌幅，单位：%
    double close_price=INVALID_DOUBLE;   //收盘价，单位：元
    QVector<StockLhbDetialListItem> data;
};

typedef QVector<StockLhbDetial>  StockLhbDetialList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockLhbDetialList&)> StockLhbDetialCallBack;

/**
 * @brief 营业部详情
 */
//跟买数据
struct StockYybDetailStatistics
{
    QString label="";
    int count=INVALID_INT32;   //上榜次数
    double buy=INVALID_DOUBLE;
    double sell=INVALID_DOUBLE;
    double ratio=INVALID_DOUBLE; //跟买成功率，单位：%，空字符窜表示未统计到成功率
};

//历史操作
struct StockYybDetaiHistory
{
    long long date=INVALID_INT64;      //操作日期
    QString stockcode="";
    QString stockname="";
    int dirc = 0;                 //0为买入，1为卖出
    double buy=INVALID_DOUBLE;
    double sell=INVALID_DOUBLE;
    double change_ratio=INVALID_DOUBLE;
};

struct StockYybDetail
{
    QString name;
    QVector<StockYybDetailStatistics> statistics;
    QVector<StockYybDetaiHistory> history;
};
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockYybDetail&)> StockYybDetailCallBack;

/**
 * @brief 龙虎榜榜单日期
 */
typedef QVector<long long> StockLhbDateList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockLhbDateList&)> StockLhbDateListCallBack;


/**
 * @brief 除权出息
 */

//除权计算公式
struct StockDrItem
{
    long long cqcxr=INVALID_INT64; //除权除息日 时间戳
    double sgbl=0; //送股比例（每10股），例如：3，表示10送3
    double zgbl=0; //转股比例（每10股），例如：2，表示10转2
    double xjfhbl=0; //现金分红比例（每10股），例如：3，表示10派3
    double pxgbl=0;  //配新股比例   待服务器提供
    double pxgj =0;  //配新股价     待服务器提供
    double cashDividends = 0; //现金红利
    double percentageChange = 0; //流通股份变动比例

    double multiply_forward=0;  //前复权乘数
    double add_forward=0;       //前复权加数
    double multiply_backward=0; //后复权乘数
    double add_backward=0;	  //后复权加数
};
typedef QVector<StockDrItem> StockDrList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockDrList&)> StockDrListCallBack;


//////////////////////////////////龙虎榜单end/////////////////////////////////////////

//////////////////////////////////融资融券begin/////////////////////////////////////////
//融券融券个股数据榜单
struct StockRzrqTopItem
{
    QString stockcode="";  //股票代码
    QString stockname="";  //股票名称
    double close_price=INVALID_DOUBLE; //收盘价
    double change_ratio=INVALID_DOUBLE; //涨跌幅
    double up_down_price = INVALID_DOUBLE;//涨跌额

    double rzmre=INVALID_DOUBLE;  //融资买入额，单位：元
    double rzjmre=INVALID_DOUBLE; //融资净买入额，单位：元
    double rzye=INVALID_DOUBLE;   //融资余额，单位：元
    double rzche=INVALID_DOUBLE;  //融资偿还额，单位：元

    //zdk 修改 量 为 INT64
    INT64 rqyl=INVALID_INT64;   //融券余量，单位：股
    INT64 rqmcl=INVALID_INT64;  //融券卖出量，单位：股
    INT64 rqchl=INVALID_INT64;  //融券偿还量，单位：股

    double rzrqye=INVALID_DOUBLE; //融资融券余额，单位：元

};
typedef QVector<StockRzrqTopItem>  StockRzrqTopList;
//回调函数  返回信息    返回数据   int返回下页数据    -1表示数据到头或为空
typedef std::function<void(const InfoRet&,const StockRzrqTopList&,int)> StockRzrqTopListCallBack;

//融资融券板块榜接口
struct StockBoardRzrqTopItem
{
    QString boardcode=""; //板块代码
    QString boardname=""; //板块名称
    double rzmre=INVALID_DOUBLE; //融资买入额，单位：元

    //增加行情字段
    double last_price = INVALID_DOUBLE; //现价
    double up_down_price = INVALID_DOUBLE;//涨跌额
    double rate = INVALID_DOUBLE;//涨跌幅
    std::string risest_stock = "--";  //领涨股
    double three_rate= INVALID_DOUBLE;
    double five_rate= INVALID_DOUBLE;
    double turnover_rate= INVALID_DOUBLE;
    double three_tuv_rate = INVALID_DOUBLE;
};
typedef QVector<StockBoardRzrqTopItem> StockBoardRzrqTopList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockBoardRzrqTopList&)> StockBoardRzrqTopListCallBack;

//板块成分股融资融券数据接口
struct StockBoardRzrqList
{
    StockRzrqTopList dataList;
    int count=INVALID_INT32;
};
//回调函数  返回信息    返回数据   int返回下页数据    -1表示数据到头或为空
typedef std::function<void(const InfoRet&,const StockBoardRzrqList&,int)> StockBoardRzrqListCallBack;

//个股融资融券详情接口
struct StockRzrqDetailItem
{
    long long date=INVALID_INT64;      //日期
    double rzye=INVALID_DOUBLE;         //融资余额，单位：元
    double rqye=INVALID_DOUBLE;         //融券余额，单位：元
    double rzrqye=INVALID_DOUBLE;       //融资融券余额，单位：元
    double rzmre=INVALID_DOUBLE;        //融资买入额，单位：元
    double rzjmre=INVALID_DOUBLE;       //融资净买入额，单位：元
    double rzche=INVALID_DOUBLE;       //融资偿还额，单位：元

    INT64 rqyl=INVALID_INT64;        //融券余量，单位：股
    INT64 rqmcl=INVALID_INT64;       //融券卖出量，单位：股
    INT64 rqchl=INVALID_INT64;       //融券偿还量，单位：股,有正有负
};
struct StockRzrqDetailList
{
    QVector<StockRzrqDetailItem> dataList;
    double valueMax=DBL_MIN;  //余额最大值
    double valueMin=DBL_MAX;  //余额最小值
};
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockRzrqDetailList&)> StockRzrqDetailListCallBack;

//两市融资融券数据接口
struct StockMarketRzrqItem
{
    long long date=INVALID_INT64;     //日期
    double rzrqye=INVALID_DOUBLE;     //融资融券余额，单位：元
    double rzye=INVALID_DOUBLE;       //融资余额，单位：元
    double rqye=INVALID_DOUBLE;       //融券余额，单位：元
};
struct StockMarketRzrqList
{
    QVector<StockMarketRzrqItem> dataList;
    double valueMax=DBL_MIN;  //余额最大值
    double valueMin=DBL_MAX;  //余额最小值
};
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockMarketRzrqList&)> StockMarketRzrqListCallBack;


//融资融券日历接口
typedef QVector<INT64> StockRzrqDateList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockRzrqDateList&)> StockRzrqDateListCallBack;
/////////////////////////////////////融资融券end/////////////////////////////////////////


/////////////////////////////////////大宗交易begin/////////////////////////////////////////
//大宗交易日历接口
typedef QVector<INT64> StockDzjyDateList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockDzjyDateList&)> StockDzjyDateListCallBack;

//个股大宗交易接口 /stock/dzjy
struct StockDzjyItem
{
    QString stockcode;                  //股票代码
    QString stockname;                  //股票名称
    long long date=INVALID_INT64;       //日期
    double deal_price=INVALID_DOUBLE;   //成交价，单位：元
    double last_close=INVALID_DOUBLE;  //收盘价，单位：元
    double change_ratio=INVALID_DOUBLE; //当日涨跌幅，单位：%
    double premium_rate=INVALID_DOUBLE; //溢价率，单位：%
    UINT64  volume=INVALID_UINT64;    //成交量，单位：(万股已经转换成股);
    double turnover=INVALID_DOUBLE;     //成交额，单位：(万元已经转换成元)
    QString  yyb_buy="";                //买入营业部
    QString  yyb_sell="";               //卖出营业部
};
typedef QVector<StockDzjyItem> StockDzjyList;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const StockDzjyList&)> StockDzjyListCallBack;

////////////////////////////////////////大宗交易end/////////////////////////////////////////


////////////////////////////////////////新股日历 start/////////////////////////////////////////
//新股日历
struct NewStockCalandar
{
    long long time;//	日期
    int number;//	当日申购数量
};

typedef QVector<NewStockCalandar> NewStockCalandars;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const NewStockCalandars&)> NewStockCalandarCallBack;


//新股日历结构体
struct StockXgrlItem
{
    QString stockname;//	股票名称
    QString stockcode;//	股票名称
    double fxjg = INVALID_DOUBLE;//	发行价格，单位：元
    double ycfxjg = INVALID_DOUBLE;//	预测发行价格，单位：元

    double last_price = INVALID_DOUBLE;//最新价

    double up_down_price = INVALID_DOUBLE;//涨跌 %
    double rate = INVALID_DOUBLE;//涨跌幅  %
    double total_rate = INVALID_DOUBLE;//累计涨跌幅 %

    long long sgrq = INVALID_INT64;//	申购日期，格式：YYYYMMDD

    double sgsx = INVALID_DOUBLE;//	申购上限，单位：万股
    double ycsgsx = INVALID_DOUBLE;//	预测申购上限，单位：万股

    double syl = INVALID_DOUBLE;//	市盈率，单位：%
    double ycsyl = INVALID_DOUBLE;//预测 市盈率，单位：%

    double zql = INVALID_DOUBLE;//中签率，单位:%

    double fxzs = INVALID_DOUBLE;//发行总数，单位：万股
    double wsfxl = INVALID_DOUBLE;//网上发行量，单位：万股
    long long ssrq = INVALID_INT64;//上市日期，格式：YYYYMMDD
    long long zqrq = INVALID_INT64;//中签日期，格式：YYYYMMDD
    long long zqjkrq = INVALID_INT64;//	中签缴款日期，格式：YYYYMMDD

    QString zgs;//	招股书
    int sl = INVALID_INT32; //连板
    INT64 mzyqhl = INVALID_INT64;//每中一签获利，单位：元
};

///新股预告
typedef QVector<StockXgrlItem> NewStockPreViews;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const NewStockPreViews&)> NewStockPreViewCallBack;

///今日申购
typedef QVector<StockXgrlItem> NewStockTodaysBids;
//回调函数  返回信息    返回数据
typedef NewStockPreViewCallBack NewStockTodaysBidCallBack;

///今日上市
typedef QVector<StockXgrlItem> NewStockMarketTodays;
//回调函数  返回信息    返回数据
typedef NewStockPreViewCallBack NewStockMarketTodayCallBack;

///今日上市
typedef QVector<StockXgrlItem> NewStockMarketIssueds;
//回调函数  返回信息    返回数据
typedef NewStockPreViewCallBack NewStockMarketIssuedCallBack;

///新股上市表现
typedef QVector<StockXgrlItem> NewStockIPOPerformances;
//回调函数  返回信息    返回数据
typedef NewStockPreViewCallBack NewStockIPOPerformanceCallBack;
////////////////////////////////////////新股日历 end/////////////////////////////////////////


////////////////////////////////////////公司大事 start///////////////////////////////////////////////////////
struct CompanyOptionEvent
{
    int type=INT_MAX;//	数据类型，枚举值及含义（1：龙虎榜，2：分红送转，3：财报）
    QString stockcode="";//	股票代码
    QString stockname="";//	股票名称
    qreal change_ratio=DBL_MAX;//	龙虎榜当日涨跌幅，单位：%
    qreal buy=DBL_MAX;//	龙虎榜买入额，单位：元
    qreal sell=DBL_MAX;//	龙虎榜卖出额，单位：元
    qreal net_buy=DBL_MAX;//	龙虎榜净买入额，单位：元
    QString reason="";//	龙虎榜上榜原因
    quint32 record_date=UINT_MAX;//	股权登记日，格式：YYYYMMDD
    quint32 dividend_date=UINT_MAX;//	除权除息日，格式：YYYYMMDD
    QString detail="";//	分红送转详情
    quint32 date=UINT_MAX; //	财报发布日期，格式：YYYYMMDD
    QString url="";//	财报H5详情页地址
    quint32 time=UINT_MAX;//	数据更新时间戳
};
Q_DECLARE_METATYPE(CompanyOptionEvent)
typedef QVector<CompanyOptionEvent> CompanyOptionEvents;

//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const CompanyOptionEvents&)>CompanyOptionEventsCallBack;

////////////////////////////////////////公司大事 end///////////////////////////////////////////////////////


////////////////////////////////////////板块新闻列表 start///////////////////////////////////////////////////////
//板块新闻列表
struct PlateNewsList{
    QString news_id="";//	新闻ID
    QString title="";//	新闻标题
    QString source="";//	新闻来源
    QString news_intro="";//	新闻摘要
    int news_tag=INT_MAX;//	新闻标签(默认0，1：强烈利好，2：利好，3：中性，4：利空，5：强烈利空)
    QString tagname="";
    qint64 news_time=INT64_MAX;//	新闻发布时间，格式时间戳
    QString url;//详情页url
};

typedef QVector<PlateNewsList> PlateNewsLists;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const PlateNewsLists&,int)>PlateNewsListsCallBack;


//板块新闻详情
struct PlateNewsDetail
{
    QString news_id="";//	新闻ID
    QString news_content="";//	新闻内容
    qint64 news_time=INT64_MAX;//	新闻发布时间，时间戳格式
};

typedef QVector<PlateNewsDetail> PlateNewsDetails;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const PlateNewsDetails&)>PlateNewsDetailsCallBack;

//指数新闻列表接口
struct IndexNewsList
{
    QString news_id="";//	新闻ID
    QString title="";//	新闻标题
    QString source="";//	新闻来源
    QString news_intro="";//	新闻摘要
    int news_tag=INT_MAX;//	新闻标签(默认0，1：强烈利好，2：利好，3：中性，4：利空，5：强烈利空)
    QString tagname="";
    qint64 news_time=INT64_MAX;//	新闻发布时间，时间戳格式
    QString url;//详情页
};

typedef QVector<IndexNewsList> IndexNewsLists;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const IndexNewsLists&,int)>IndexNewsListsCallBack;


//指数新闻详情
struct IndexNewsDetail
{
    QString news_id="";//	新闻ID
    QString news_content="";//	新闻内容
    qint64 news_time=INT64_MAX;//	新闻发布时间，时间戳格式
};
typedef QVector<IndexNewsDetail> IndexNewsDetails;
//回调函数  返回信息    返回数据
typedef std::function<void(const InfoRet&,const IndexNewsDetails&)>IndexNewsDetailsCallBack;

////////////////////////////////////////板块新闻列表 end///////////////////////////////////////////////////////


/////////////////////////////////////////主题投资 start///////////////////////////////////////////////////////////
struct TodayChance
{
    int rank;//排名
    QString topic_id="";	//主题id
    QString topic_name="";	//主题名称
    QString topic_describe="";	//主题描述
    quint32 update_time;	//更新时间，时间戳格式
    QVector<QString> related_stock;	//相关股票
};

/*主题投资*/
struct TopicInvest
{
    QVector<TodayChance> todayChance,recentHotspot;
};

/*主题投资详情*/
struct TopicInvestDetail
{
    struct STDetail
    {
        QString topic_name;//	主题名称
        QString significance;//	重要性（数字代表星的数量，默认为0）
        QString sustainability;//	持续性（数字代表星的数量，默认为0）
        QString reason;//	主题驱动
        QString intro;//	主题对应的介绍
    };

    struct STStocks{
        QString stockcode;//	股票代码
        QString stockname;//	股票名称
    };

    STDetail detail;
    QVector<STStocks> stock;//	相关个股
};

/*相关资讯*/
struct TopicInvestNews
{
    struct STNewList
    {
        QString news_id;//	新闻id
        QString title;//	资讯标题
        QString source;//	资讯来源
        QString intro;//	资讯内容
        int tag;//	资讯对应的标签，枚举类型（1：强烈利好，2：利好，3：中性，4：利空，5：强烈利空）
        quint32 time;//	更新时间，时间戳格式
    };
    int nextpage;//下一页页码
    QVector<STNewList> datas;
};

typedef std::function<void(const InfoRet&,const TopicInvest&)> TopicInvestCallBack;
typedef std::function<void(const InfoRet&,const TopicInvestDetail&)> TopicInvestDetailCallBack;
typedef std::function<void(const InfoRet&,const TopicInvestNews&)> TopicInvestNewsCallBack;
/////////////////////////////////////////主题投资 end///////////////////////////////////////////////////////////


/////////////////////////////////////////自选股周报 start///////////////////////////////////////////////////////////
/*自选股周报*/
struct OptionalWeekly
{
    struct STSummary
    {
        struct STIndexAndOptionRateDaily
        {
            double Rate;//涨幅，乘以100得到百分比
            quint32 CalcDay; //日期，格式为：YYYYMMDD
        };
        quint32 begin_date;//	周报开始日期
        quint32 end_date;//	周报结束日期
        double index_rate_all;//	大盘总涨幅，乘以100得到百分比
        double optional_rate_all;//自选股总涨幅，乘以100得到百分比
        double optional_rate_daily;//	自选股每日涨幅，数组格式，详见每日涨幅说明
        QVector<STIndexAndOptionRateDaily> indexs;//	大盘每日涨幅，数组格式，详见每日涨幅说明
        QVector<STIndexAndOptionRateDaily> options;//	自选股总涨幅，乘以100得到百分比
    };

    struct STDetail
    {
        QString stockcode;//	股票代码
        QString stockname;//	股票名称
        qlonglong money_in_all;//	资金净流入，单位：厘
        qlonglong money_out_all;//	资金净流出，单位：厘
        double per_income;//	每股收益，单位：厘
        double rate_all;//	周涨幅，乘以100得到百分比
        int pzyd;//	盘中异动次数
        int lhb;//	龙虎榜次数
        int fhsz;//	分红送转次数
        int cb;//	财报次数
    };

    STSummary sum;
    QVector<STDetail> detail;
};
typedef OptionalWeekly::STSummary::STIndexAndOptionRateDaily STIndexAndOptionRateDaily;
typedef std::function<void(const InfoRet&,const OptionalWeekly&)> OptionalWeeklyCallBack;
Q_DECLARE_METATYPE(OptionalWeekly)
Q_DECLARE_METATYPE(OptionalWeekly::STSummary)
Q_DECLARE_METATYPE(OptionalWeekly::STDetail)
Q_DECLARE_METATYPE(OptionalWeekly::STSummary::STIndexAndOptionRateDaily)

/////////////////////////////////////////自选股周报 end///////////////////////////////////////////////////////////



enum InfoReqType{
    HostNewsReq = 0,
    HostDetailReq ,
    FlashNewsReq  ,
    FlashNewDetailReq ,
    NewsReq ,
    NewsDetailReq ,
    FinanceInfoReq ,
    ResearchReportReq ,
    ResearchReportDetailReq,
    NewsInfoReq,
    NewInfoDetailReq ,//10
    AnncsInfoReq,//11
    AnncInfoDetailReq,
    MarketCalendarReq,
    MarketCalendarTimesReq,
    EconomicDataTimesReq,
    EconomicDataReq,
    MajorMeetingReq,
    MajorMeetingDetailReq,
    FinancialReportReq,
    FinancialReportDetailReq,
    StockLhbDatasReq,
    StockLhbDetialReq,
    StockYybDetailReq,
    StockYybDrbReq,
    StockYybSlbReq,
    StockYybHybReq,
    StockLhbDateListReq,
    StockDrListReq,
    StockRzrqTopListReq,
    StockBoardRzrqTopListReq,
    StockBoardRzrqListReq,
    StockRzrqDetailListReq,
    StockMarketRzrqListReq,
    StockRzrqDateListReq,
    StockDzjyDateListReq,
    StockDzjyListReq,
    NewStockPreViewsReq,/*新股日历*/
    NewStockCalandarsReq,
    NewStockTodaysBidsReq,
    NewStockMarketTodaysReq,
    NewStockMarketIssuedsReq,
    NewStockIPOPerformancesReq,
    CompanyOptionEventsReq,
    PlateNewsListsReq,
    PlateNewsDetailsReq,
    IndexNewsListsReq,
    IndexNewsDetailsReq,
    TopicInvestReq, /*主题投资*/
    TopicInvestDetailReq,
    TopicInvestNewsReq,
    OptionalWeeklyReq
};


struct ReqInfo
{
    InfoReqType type;
    HostNewsCallBack hostNewsFun;
    HostNewDetailCallBack hostNewDetailFun;
    FlashNewsCallBack flashNewsFun;
    FlashNewDetailCallBack flashNewDetailFun;
    FinanceInfoCallBack financeInfoFun;
    ResearchReportCallBack researchReportFun;
    ResearchReportDetailCallBack researchReportDetailFun;
    NewsInfoCallBack newsInfoCallBackFun;
    NewInfoDetailCallBack newInfoDetailCallBackFun;//10
    AnncInfoCallBack anncInfoCallBackFun;//11
    AnncInfoDetailCallBack anncInfoDetailCallBackFun;//12
    MarketCalendarCallBack marketCalendarFun;
    MarketCalandarTimesCallBack marketCalendarTimesFun;
    EconomicDataCallBack economicDataFun;
    EconomicDataTimesCallBack economicDataTimesFun;
    MajorMeetingCallBack majorMeetingFun;
    MajorMeetingDetailCallBack majorMeetingDetailFun;
    FinancialReportCallBack financialReportFun;
    FinancialReportDetailCallBack financialReportDetailtFun;
    StockLhbDatasCallBack stockLhbDatasFun;
    StockLhbDetialCallBack stockLhbDetialFun;
    StockYybDetailCallBack stockYybDetailFun;
    StockYybDrbCallBack stockYybDrbFun;
    StockYybSlbCallBack stockYybSlbFun;
    StockYybHybCallBack stockYybHybFun;
    StockLhbDateListCallBack stockLhbDateListFun;
    StockDrListCallBack stockDrListFun;
    StockRzrqTopListCallBack StockRzrqTopListFun;
    StockBoardRzrqTopListCallBack StockBoardRzrqTopListFun;
    StockBoardRzrqListCallBack StockBoardRzrqListFun;
    StockRzrqDetailListCallBack StockRzrqDetailListFun;
    StockMarketRzrqListCallBack StockMarketRzrqListFun;
    StockRzrqDateListCallBack StockRzrqDateListFun;
    StockDzjyDateListCallBack StockDzjyDateListFun;
    StockDzjyListCallBack StockDzjyListFun;
    NewStockPreViewCallBack NewStockPreViewFun;
    NewStockCalandarCallBack NewStockCalandarFun;
    NewStockTodaysBidCallBack NewStockTodaysBidFun;
    NewStockMarketTodayCallBack  NewStockMarketTodayFun;
    NewStockMarketIssuedCallBack NewStockMarketIssuedFun;
    NewStockIPOPerformanceCallBack NewStockIPOPerformanceFun;
    CompanyOptionEventsCallBack CompanyOptionEventsFun;//
    PlateNewsListsCallBack PlateNewsListsFun;
    PlateNewsDetailsCallBack PlateNewsDetailsFun;
    IndexNewsListsCallBack IndexNewsListsFun;
    IndexNewsDetailsCallBack IndexNewsDetailsFun;
    TopicInvestCallBack TopicInvestFun;
    TopicInvestDetailCallBack TopicInvestDetailFun;
    TopicInvestNewsCallBack TopicInvestNewsFun;
    OptionalWeeklyCallBack OptionalWeeklyFun;
};

#endif // HZINFODATA_H
