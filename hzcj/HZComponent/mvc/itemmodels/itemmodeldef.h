/********************************************************************
created:2018/05/10
author: dk.zhang
purpose:ItemModel 依赖的声明文件
*********************************************************************/
#ifndef ITEMMODELDEF_H
#define ITEMMODELDEF_H

#include <QtCore>

//行情每一列对应标识
enum eColumnIdx
{
    //基本字段 1-9
    ColNo = 0,                     //序号

    ColStockCode       = 1,   //股票码
    ColBKCode           = 2,   //板块码
    ColBKStockCode   = 3,  //股票和板块同时显示

    ColStockName      = 4,   //股票名称

    ColNumber = 5,//次数，个数等
    ColDate = 6,//日期- 从时间戳转换 MM-dd
    ColFile  = 7,  //文件
    ColNoZero = 8,//序号:从0开始
    ColFullDate = 9,//日期:yyyy-MM-dd

    //股票\板块\指数 10
    ColPrice    = 10,       //最新，根据昨收价判断颜色
    ColUpDownPrice = 11,    //涨跌
    ColRate = 12,                 //涨幅

    ColVolume = 13,       //总量-手
    ColCurVolume = 14,         //现量-手
    ColTurnoverRate = 15,  //换手率%

    ColTurnover = 17,       //总额
    ColHighestPrice = 18,  //最高价
    ColLowestPrice = 19,  //最低价
    ColOpenPrice = 20,     //今开盘
    ColPlainPrice = 21,     //普通价格
    ColPreClosePrice = ColPlainPrice, //昨收盘
    ColRatio = 22,              //量比
    ColCommittee = 23,     //委比
    ColBuySellBuy = 24,    //委差
    ColAvgPrice  = 25,      //均价
    ColNpVolume = 26,     //内盘成交量-手
    ColWpVolume = 27,    //外盘成交量-手
    ColNwpRate   = 28, // 内外盘比
    ColPbv = 29,               //市净率

    ColTotStock = 30,       //总股本
    ColTotValue = 31,       //总市值
    ColCirStock= 32,        //流通股本
    ColCirValue= 33,         //流通市值

    ColPed         = 34, //市盈率(动)
    ColSa           = 35,//振幅
    ColSpeedRate = 36,//涨速

    ColBuyPrice = 37,     //买一
    ColSellPrice = 38,    //卖一

    ColRiseFallNum = 39,  //涨跌家数
    ColRiseFallRatio=40,//涨跌比
    ColProfit = 41,          //收益
    ColAvgStock = 42,   //平均股本

    ColThreeRate  = 43, //3日涨幅
    ColFiveRate      = 44, //5日涨幅
    ColThreeTuvRate   = 45, //3日换手
    ColFiveTuvRate      = 46,  //5日换手
    ColRiseDayNum    = 47, //连涨
    ColRiseStock         =48,//领涨股
    ColRiseStockRate  =49,//领涨股涨幅
    ColRiseNum           = 50,
    ColFallNum            = 51,
    ColMainNetput    = 52,   //主力净流入
    ColBlockName     = 53, //所属板块行业

    ColRatePercent   = 54,    //涨跌幅%显示百分号
    ColRateYearYet = 55,   //年初至今涨幅
    ColRatePrice    = 56,    //根据ColRate涨幅计算颜色的价格
    ColVolStock = 57,//单位：股
    ColRiseStockCode = 58,//领涨股代码
    ColTime = 59,     //时间 hh:mm

    //相似K线
    ColSimilar = 70,//相似度
    ColSmkStockCode = 71,//相似牛股代码
    ColSmkStockName = 72,//相似牛股名称
    ColViewSmk = 73,//查看相似度

    //资金--  100 - 200
    ColAuctionVolume     = 100,  //集合竞价成交量
    //当日
    ColLeaderInput     = 101,  //主力资金
    ColLeaderOutput  = 102,
    ColLeaderNetput  = 103, //主力净流入
    ColLeaderRate     = 104,

    ColMaxInput     = 105,  //超大单
    ColMaxOutput  = 106,
    ColMaxNetput  = 107,
    ColMaxRate     = 108,

    ColBigInput    = 109,  //大单资金
    ColBigOutput = 110,
    ColBigNetput = 111,
    ColBigRate    = 112,

    ColMidInput    = 113,  //中单资金
    ColMidOutput = 114,
    ColMidNetput = 115,
    ColMidRate    = 116,

    ColMinInput    = 117,  //小单资金
    ColMinOutput = 118,
    ColMinNetput = 119,
    ColMinRate    = 120,

    //3日 121
    ColThreeLeaderNetput = 123,

    //5日 141
    ColFiveLeaderNetput = 143,

    //持平家数
    ColFlatNum  = 144,

    //每天的净流入
    ColOneDayNetput = 145,
    ColThreeDayNetput = 146,
    ColFiveDayNetput = 147,
    ColTenDayNetput = 148,


    //集合竞价 200---220
    ColAuctionRate = 200, //集合竞价涨幅
    ColLeaderFund = 201,//主力资金
    ColAuctionVol    = 202,//竞价成交
    ColPreRate = 203,//昨涨幅
    ColAutionQaunRatio = 204,//竞价量比
    ColAuctionTurnover   = 205,//竞价成交额

    //涨停监测 220 -240
    ColRfmUpLimitOpenTime     = 220, //涨停打开时间
    ColRfmDownLimitOpenTime = 221, //跌停打开时间
    ColRfmUpLimitTime     = 222, //封板时间
    ColRfmDownLimitTime = 223,//跌停时间
    ColContUpLimitRate    = 224,//涨停连板
    ColContDownLimitRate= 225, //跌停连板
    ColUpLimitVolume       = 226,//涨停封单
    ColDownLimitVolume = 227,//跌停封单
    ColFriedTime    = 228, //炸板时间
    ColFriedNum    = 229, //炸板次数
    ColRfmHistCont = 230, //往日连板

    //资金异动  250+
    ColThirtyRate     = 250, //30分钟涨幅
    ColActiveNetput = 251, //主动净买
    ColThirtyActiveNetput = 252, //30分钟主动净买
    ColActiveRate = 253, //主动占比
    ColThreeActiveNetput = 254, //3日主动净买
    ColFiveActiveNetput   = 255, //5日主动净买

    //龙虎榜
    ColNetput        = 258, //258净买入
    ColTotalBuyIn  = 259, //259累计买入
    ColTotalSellOut= 260,//260累计卖出
    //
    ColYybName  = 310,
    ColStockNum = 311, //股票数
    ColYybId = 312, //营业部Id
    ColThreeSucRate = ColThreeTuvRate,//45 3日成功率
    ColCfStock = ColStockName,//成分股票

    ColLastNetput = ColNetput,//251 最近净买入
    ColLastOperate = ColStockName,//4 最近操作
    ColDrRate   = ColRate, //12 当日涨跌幅
    ColRankNum = ColNumber,  //5 上榜次数

    //龙虎榜 -营业部详情
    ColHistSb = ColStockName,//4 历史上榜
    ColGBuySucRate = ColThreeSucRate, //45 跟买成功率
    ColTradeDirc = 313, //买入或者卖出，买卖方向

    ColBuyTurnover = 314, // 买总额
    ColSellTurnover = 315,//  买总额

    ColSlFund  = 316, //实力资金

    //大宗交易350 -
    ColDealPrice  = 350,//成交价
    ColClosePrice = 351, //当日收盘价
    ColOverPriceRate = 352,//溢价率
    ColIDzjyVol = ColVolStock,//57 单位：股
    ColYybBuy = ColYybName,//310 买方营业部
    ColYybSell = 353,//310 卖方营业部

    //融资融券-400-450
    ColRzmre = 400,  //融资买入额
    ColRzjmre = 401, //融资净买入额
    ColRzye = 402,//融资余额
    ColRzche = 403,//融资偿还额
    ColRqyl = 404,//融券余量
    ColRqmcl = 405,//融券卖出量
    ColRqchl = 406,//融券偿还量
    ColRzrqye = 407, //融资融券余额

    //新股日历-450+
    ColFxzs  = 450, //发行总数

    ColSgrq = ColFullDate, //申购日期
    ColWsfxl = 451, //网上发行量：万股
    ColSgsx  = 452, //申购上限：万股

    ColZqgbrq = 453, //中签公布日期
    ColZqjkrq  = 454, //中签缴款日期
    ColZql       = 455, //中签率
    ColZgs      = ColFile, //7 招股书
    ColSsrq    = 457,//上市日期
    ColSl        = 458,//连板
    ColTotalRate = 459,//累计涨跌幅
    ColMzyqhl    = 460,//每中一签获利
    ColFxjg  = 461, //发行价格
    ColFxsyl  = 462, //发行市盈率

    //1000+
    ColUnUsed = 1000 //预留自定义
};


//列序号-通用
typedef quint32 ColumnIdx;


//排序类型：不排序;自动排序;手动排序
enum SortType
{
    DisableSort = 0,
    AutoSort     = 1,
    CustomSort= 2,
};

//每列的信息
struct ColumnInfo
{
    ColumnIdx        idx;//列标号
    QString             label;//列标签
    float                   width;//列宽度
    SortType           sortType;//排序类型
    Qt::Alignment    alignment;//水平对齐
};


struct SortIndicator{
    int                   column = -1;
    Qt::SortOrder  order    = Qt::AscendingOrder;
};

#endif // ITEMMODELDEF_H
