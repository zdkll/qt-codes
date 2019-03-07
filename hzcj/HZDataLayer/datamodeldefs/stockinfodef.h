#ifndef STOCKINFODEF_H
#define STOCKINFODEF_H

#include <QVector>
#include <QMetaType>
#include "datamodelpublic.h"


//股票行情有关
//=======================协议返回=================================
namespace  HZData {

struct StockMmp
{
    double   price   = INVALID_DOUBLE;
    UINT64  volume= INVALID_UINT64;
};

//全部动态行情信息
typedef struct  Str_Stock
{
    Str_Stock(){}
    std::string     stock_code;   //股票码
    std::string     stock_name;//股票名称
    std::string     block_name;//个股所属行业板块

    double  rate = INVALID_DOUBLE;  //涨幅
    double  last_price= INVALID_DOUBLE; //现价
    double  up_down_price= INVALID_DOUBLE;//涨跌额
    double	ratio = INVALID_DOUBLE ;//量比
    double	pbv = INVALID_DOUBLE;//市净率
    UINT32	trading_day=INVALID_UINT32  ;//交易日
    double	pre_close_price = INVALID_DOUBLE;//昨收盘
    double	open_price = INVALID_DOUBLE ;//今开盘
    double	highest_price = INVALID_DOUBLE;//最高价
    double	lowest_price = INVALID_DOUBLE ;	//最低价
    //委买卖
    StockMmp	stk_buy[5];		//买卖五档 //委差 = (买1+ ...+买5) -(卖1+...+卖出5) ;委比 = 委差/((买1+ ...+买5) +(卖1+...+卖出5) )
    StockMmp	stk_sell[5];		//买卖五档
    UINT64  volume = INVALID_UINT64;		//总量
    UINT64  cur_volume = INVALID_UINT64;	//现量
    double	turnover_rate = INVALID_DOUBLE;		//换手率%
    double	 pes = INVALID_DOUBLE;			//市盈率(静态)
    double  ped = INVALID_DOUBLE;       //市盈率(动)

    double turnover =   INVALID_DOUBLE; //总额
    double avg_price = INVALID_DOUBLE;//均价
    UINT32 update_time = INVALID_UINT32 ;//成交时间

    double	committee = INVALID_DOUBLE;//委比

    INT64  cir_stock =   INVALID_INT64;       //流通股本
    INT64  tot_stock =   INVALID_INT64;       //总股本
    double cir_value =   INVALID_DOUBLE;       //流通市值
    double tot_value =   INVALID_DOUBLE;       //总市值
    INT64  np_volume = INVALID_INT64;       //内盘成交量
    INT64  wp_volume = INVALID_INT64;       //外盘成交量

    INT32  trade_status = INVALID_INT32;   //交易状态
    INT64  avg_stock    = INVALID_INT64; //平均股本
    UINT32 rise            = INVALID_UINT32;  	//上涨个数
    UINT32 fall = INVALID_UINT32;      		//下跌个数
    UINT32 flat = INVALID_UINT32;      		//平盘个数
    double  rise_fall_ratio = INVALID_DOUBLE;   //涨跌比
    bool      is_block = false;         //是个股还是板块

    double   ro   = INVALID_DOUBLE;         //净资产
    double   roe = INVALID_DOUBLE;         //净资产收益率
    double   profit = INVALID_DOUBLE;         //收益
    double   upper_limit_rice = INVALID_DOUBLE;	 //涨停板价 昨收价 * (1 + 10%)
    double   down_limit_price = INVALID_DOUBLE;	 //跌停板价 昨收价 * (1 - 10%)
    bool      clear_sig = false;			//清盘信号,true为清盘,false为正常交易
    UINT32  buy_sell = INVALID_UINT32;       //买卖方向 买(2)卖(1)

    std::string    rise_stock= Default_Item_String;  //领涨股
    std::string    rise_code; //领涨股
    double  rise_stock_rate = INVALID_DOUBLE;

    //    INT64   leader_money_netput = INVALID_INT64;

    double nwp_rate =INVALID_DOUBLE;    //内盘外盘比例
    double sa = INVALID_DOUBLE;              //振幅
    double speed_rate = INVALID_DOUBLE;  //涨速(3分钟涨跌)
    double two_rate = INVALID_DOUBLE;      //2日涨幅
    double three_rate = INVALID_DOUBLE;   //3日涨幅
    double five_rate = INVALID_DOUBLE;       //5日涨幅
    double three_tuv_rate = INVALID_DOUBLE; //3日换手率
    double five_tuv_rate = INVALID_DOUBLE;    //5日换手率
    UINT64 detail_volume = INVALID_UINT64;   //逐笔量
    INT32  rise_day_num = INVALID_INT32; //连涨天数
    INT32  limit_up = INVALID_INT32; //涨停家数

    double  year_rate = INVALID_DOUBLE;//年初至今涨幅
}AllStock;


//股票缩略行情信息（现价,涨跌）
struct SLStock
{
    std::string stock_code;
    std::string stock_name;

    double last_price= INVALID_DOUBLE; //现价
    double pre_close_price = INVALID_DOUBLE;//昨收价
    double up_down_price= INVALID_DOUBLE;//涨跌额
    double rate = INVALID_DOUBLE;  //涨幅

    double	turnover_rate = INVALID_DOUBLE;//换手率%
    double  turnover =  INVALID_DOUBLE; //成交额

    //沪深综合
    double five_rate = INVALID_DOUBLE; //5日涨幅
    double speed_rate = INVALID_DOUBLE; //3分钟涨幅-涨速
    double  year_rate = INVALID_DOUBLE;//年初至今涨幅
};

//个股行情字段
struct Stock
{
    std::string stock_code;
    std::string stock_name;

    double  last_price= INVALID_DOUBLE; //现价
    double  up_down_price= INVALID_DOUBLE;//涨跌额
    double  rate = INVALID_DOUBLE;  //涨幅

    UINT64  volume = INVALID_UINT64;		//成交量

    //    INT64   leader_money_netput = INVALID_INT64;//主力净流入

    double  speed_rate = INVALID_DOUBLE;  //3分涨速
    double	turnover_rate = INVALID_DOUBLE;//换手率%
    double  turnover =   INVALID_DOUBLE; //成交额

    std::string  block_name = Default_Item_String;//所属行业

    double	highest_price = INVALID_DOUBLE;//最高价
    double	lowest_price = INVALID_DOUBLE ;	//最低价
    double	pre_close_price = INVALID_DOUBLE;//昨收盘
    double	open_price = INVALID_DOUBLE ;//今开盘

    double buy_price = INVALID_DOUBLE;//买入价
    double sell_price = INVALID_DOUBLE;//卖出价
    double sa = INVALID_DOUBLE; //振幅
    double	ratio = INVALID_DOUBLE ;//量比
    double	committee = INVALID_DOUBLE;//委比
    double	pbv = INVALID_DOUBLE;//市净率
    double	 ped = INVALID_DOUBLE;//市盈率(动态)
    INT64  cir_stock =   INVALID_INT64; //流通股本
    double cir_value =   INVALID_DOUBLE; //流通市值
    INT64  tot_stock =   INVALID_INT64;//总股本
    double tot_value =   INVALID_DOUBLE;//总市值

    double three_rate = INVALID_DOUBLE; //3日涨幅
    double five_rate = INVALID_DOUBLE;//5日涨幅
    double three_tuv_rate = INVALID_DOUBLE; //3日换手率
    double five_tuv_rate = INVALID_DOUBLE;//5日换手率

    double  year_rate = INVALID_DOUBLE;//年初至今涨幅
};

//板块行情字段
struct Block
{
    std::string stock_code;
    std::string stock_name;

    double pre_close_price = INVALID_DOUBLE;
    double last_price = INVALID_DOUBLE;

    double  rate = INVALID_DOUBLE;  //涨幅

    double speed_rate = INVALID_DOUBLE;  //3分涨速
    //INT64  leader_money_netput = INVALID_INT64;//主力净流入
    //double rate_from_year = INVALID_DOUBLE;//年初至今涨幅

    INT32  rise_day_num = INVALID_INT32; //连涨天数

    std::string   risest_stock=Default_Item_String;//领涨股
    std::string   risest_code;//领涨股代码
    double  risest_stock_rate = INVALID_DOUBLE;//领涨股涨幅
    UINT32 rise = INVALID_UINT32;  //上涨家数
    UINT32 flat  = INVALID_UINT32;   //平家数
    UINT32 fall  = INVALID_UINT32;   //下跌家数

    double three_rate = INVALID_DOUBLE; //3日涨幅
    double five_rate = INVALID_DOUBLE;//5日涨幅
    double  year_rate = INVALID_DOUBLE;//年初至今涨幅

    UINT64  volume = INVALID_UINT64;//总量
    double turnover = INVALID_DOUBLE; //总额

    double turnover_rate   = INVALID_DOUBLE;//换手率
    double three_tuv_rate = INVALID_DOUBLE; //3日换手率
    double cir_value =   INVALID_DOUBLE;//流通市值
    double tot_value =   INVALID_DOUBLE;//总市值
    INT32  limit_up = INVALID_INT32; //涨停家数

    UINT32 update_time = INVALID_UINT32 ;//成交时间

};

//指数行情字段
struct Index
{
    std::string stock_code;
    std::string stock_name;

    double  last_price= INVALID_DOUBLE; //现价
    double  up_down_price= INVALID_DOUBLE;//涨跌额
    double  rate = INVALID_DOUBLE;  //涨幅

    double turnover =   INVALID_DOUBLE; //成交额

    double	highest_price = INVALID_DOUBLE;//最高价
    double	lowest_price = INVALID_DOUBLE ;	//最低价
    double	pre_close_price = INVALID_DOUBLE;//昨收盘
    double	open_price = INVALID_DOUBLE ;//今开盘
};

//板块异动
struct BlockAbnormalItem
{
    std::string     stock_code;   //股票码
    std::string     stock_name;//股票名称
    UINT32        update_time = INVALID_UINT32 ;//成交时间
    double         rate = INVALID_DOUBLE;  //涨幅
    double         up_down_price= INVALID_DOUBLE;//涨跌额
};

//涨跌分布
struct  StockRiseFallInfo
{
    std::string block_code;		//板块代码
    std::string block_name;		//板块名称
    INT64 greater_seven = INVALID_INT64;      //大于7%
    INT64 up_five_seven = INVALID_INT64;      //%5-%7
    INT64 up_two_five = INVALID_INT64;          //%2-%5
    INT64 up_zero_two = INVALID_INT64;         //%0-%2
    INT64 down_zero_two = INVALID_INT64;     //%-2-%0
    INT64 down_two_five = INVALID_INT64;      //%-5-%-2
    INT64 down_five_seven = INVALID_INT64;   //%-5-%-7
    INT64 less_seven = INVALID_INT64;            //小于-7%
    INT64 limit_up_num = INVALID_INT64;         //涨停数
    INT64 up_num = INVALID_INT64;                 //涨家数
    INT64 limit_down_num = INVALID_INT64;     //跌停数
    INT64 down_num = INVALID_INT64;             //跌家数
    INT64 flat_num   = INVALID_INT64;               //平家数
    INT64 stop_num = INVALID_INT64;               //停盘数
};

//板块异动,第一个为板块，后面为成分股，成分股最多四个
typedef QVector<BlockAbnormalItem> BlockAbnormal;

//请求返回的板块异动数据
struct BlockAbnormalInfo
{
    QVector<BlockAbnormal> blockAbnormals;
    int totalCout;
};

//异动情况类型
enum AbnormalType
{
    speed_up = 0,    //加速上涨,涨跌幅
    speed_down = 1,//加速下跌,涨跌幅
    big_sell =2,         //大笔卖出,股本数
    big_buy=3,         //大笔买入,股本数
    rise_stop = 4,          //封涨停板,价格
    fall_stop =5            //封跌停板,价格
};

//个股短线异动
struct StockAbnormal
{
    std::string   stock_code;//异动股票
    std::string   stock_name;
    UINT32     update_time = INVALID_UINT32; //时间
    AbnormalType   abnormal_type;   //异动类型
    double       value;//值
};

//短线异动
struct StockAbnormalInfo
{
    QVector<StockAbnormal> blockAbnormals;
    int totalCout = 0;
};

//请求====================================
enum StockFieldMask
{
    AllStock_Mask = 0,
    StockCode_Mask,
    SLStock_Mask,
    Stock_Mask,
    Block_Mask,
    Index_Mask
};

//发送请求，范围方式批量拉取股票基础信息
struct ReqBatchDynaInfo
{
    INT32       filter = MarketAll;//过滤条件，对不同市场之类的排序
    std::string	filter_detail;//过滤条件的详细信息， eg：板块的某一种类型详细过滤
    SortField  sort_field ;//根据用户所选择的字段进行排序
    bool          is_desc =  true;//所需股票信息的排序方式,true表示降序
    INT32       index = 0;// 初始index位置
    INT32       count_ange =  0;//请求获取股票信息的条数

    StockFieldMask  mask = StockCode_Mask; //字段掩码,排行
};

//发送请求，固定股票代码，批量拉取股票基础信息
//返回Stock
struct ReqOptionalDynaInfo
{
    INT32                           kline_type = INVALID_INT32; //是否请求k线，请求哪种k线，如果传，stock_code只能有一个
    std::list<std::string>	    stock_code ;		//指定股票代码（带市场的统一代码）
    StockFieldMask  mask = AllStock_Mask; //字段掩码
};

//获取涨跌分布
typedef std::string  ReqStockRiseFallInfo;


//板块异动
struct ReqBlockAbnormalInfo
{
    std::string  block_code;      //起始板块位置，空字符串代表最新
    UINT32     update_time;     //发生的板块代码的时间
    INT32       count_range;       //表明请求拉取的数量
    bool          is_desc;              //true 表示降序(倒拉)
};

//个股短线异动
struct ReqStockAbnormalInfo
{
    INT32    index;            //起始index位置,-1 表示表示从最新位置拉，0表示第一个
    INT32    count_range; //表明请求拉取的数量
    bool       is_desc;        //true 表示降序(倒拉)
};

//盘中异动类型
enum MidAbnormalType
{
    ThirtyMinBuy       = 0,        //30分钟净买
    ThirtyMinSell       = 1,        //30分钟净卖
    MoneyMaxOrdBuy    = 2,       //超大单买
    MoneyBigOrdBuy      = 3,        //大单买
    MoneyMaxOrdSell     = 4,        //超大单卖
    MoneyBigOrdSell       = 5,        //大单卖
    UpFiveRate            = 6,        //涨停超过5%
    DownFiveRate        = 7,        //跌幅超过5%
    UpLimitRate         = 8,       //涨停
    DownLimitRate     = 9,       //跌停
    OpenUpLimitRate     = 10,       //打开涨停
    OpenDownLimitRate = 11       //打开跌停
};

//盘中异动请求
struct ReqMidAbnormalInfo
{
    int               type = 0; //1,大单买卖; 2,涨跌幅; 0,大单买卖&涨跌幅
    std::string    stock_code;
    INT32    index;         //从index位置开始拉，最老的一条index为0；倒拉时可以用-1表示最新一条
    UINT32  count;      //
    UINT32  direction; //0:倒拉，1:正拉；默认为倒拉
};

//盘中异动返回
struct MidAbnormal
{
    MidAbnormalType    abnormal_type;
    std::string                stock_code ;
    std::string                stock_name;
    UINT32                    update_time = INVALID_UINT32;
    float                         rate = INVALID_FLOAT; //异动时刻涨跌幅
    float                         last_price = INVALID_FLOAT;
    double                     turnover = INVALID_DOUBLE;
    UINT64                    volume  = INVALID_UINT64;
    float                          real_time_rate = INVALID_FLOAT;//实时涨跌幅
};

struct MidAbnormalInfo
{
    QVector<MidAbnormal> midAbnormals;
    int totalCount = 0;
};


//异动板块请求
struct ReqAbnormalBlockInfo
{
  int  filter; //全部板块、行业板块、概念板块、地域板块
};

}
Q_DECLARE_METATYPE(HZData::MidAbnormal)
Q_DECLARE_METATYPE(HZData::MidAbnormalInfo)
#endif // STOCKINFODEF_H
