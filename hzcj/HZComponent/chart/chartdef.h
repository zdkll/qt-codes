#ifndef CHARTDEF_H
#define CHARTDEF_H
#include <qobject.h>
#include <qcolor.h>
#include <string>
#include <vector>
#include <functional>

#include "cfgdef.hpp"

class BaseChart;

////////////////////////////////////对象间消息传递定义////////////////

//切换合约相关信息 switch_stock_info?market=sh&obj=60000&period=1&indid=0
#define SwitchStockInfo "switch_stock_info"

//指标切换命令 switch_indicator_info?id=0
#define SwitchIndicatorInfo "switch_indicator_info"

//K线数据请求 kline_req_info?market=sh&obj=60000&period=1&count=300&dirc=-1
#define KlineReqInfo "kline_req_info"
#define KlineReqType 0x0001

//K线订阅 kine_sub_info?market=sh&obj=60000
#define KlineSubInfo "kline_sub_info"
#define KlineSubType 0x0002

//界面清理  用于界面样式初始，比如切换合约后，清理上一次的绘图，防止本次请求失败后，界面却显示上一个合约
#define ChartUiClear "chart_ui_clear"

///////////////////////////////////****尺寸*****////////////////
//每个容器的标题部分高度
#define ContainerTitleHeight 20
//标题和图形区域间隔
#define TitleAndChartInterval 8
//K线和分时区域的Y轴坐标宽度
#define YaxisKlineMinWidth 60
//蜡烛图占单根数据区域的百分比
#define CandleRate 0.5f
//时间轴高度
#define TimeXaxisHeight 20
//K线默认宽度
#define KlineDefaultWidth 10.0f
//K线缩略默认宽度
#define KlineThumDefaultWidth 6.0f
//最大K线宽度
#define KlineMaxWidth 50.0f
//最小K线宽度
#define KlineMinWidth 1.0f

//K横坐标等分次数
#define XaxisSeparate  9
//K线时间轴间隔最低宽度
#define KlineTimeXaxisMinWidth 70

//K线网格高度
#define KlineGridHight 25.0

//分时网格水平等分
#define MinGridHSeparate 28.0
//分时网格高度
#define MinGridHight 25.0
//标题栏项目之间的间隔
#define TitleTextInternal 10

///////////////////////////////////////数据常量/////////////////////////////////
//指标向前依赖数据
#define IndiDependNum 200
//K线单次请求最大数量
#define KlineReqMaxCount 500
//分时展示最大天数
#define MinMaxDay -10

//////////////////////////////////////chart相关属性宏/////////////////////////////
//隐藏坐标轴
#define ChartHideLeftXaxis 0x0001
//隐藏右边轴
#define ChartHideRightXaxis 0x0002
//小图显示 比如迷你盯盘等区域 主要用于图像适配
#define ChartThumbnailChart  0x0004
//////////////////////////////////////chart相关属性宏/////////////////////////////

//////////////////////////////////////chart 标题相关宏/////////////////////////////
#define ChartTitleWithClose 0x0008
#define ChartTitleWithSetting 0x0010
#define ChartTitleWithRestorationComBox 0x0020
#define ChartTitleWithPlusBt 0x0040
#define ChartTitleWithSubBt 0x0080
#define ChartTitleWithIndiSelBt 0x0100
//////////////////////////////////////chart 标题相关宏End/////////////////////////////

//////////////////////////////////////分时相关属性宏/////////////////////////////
//显示股票名称
#define MinTitleShowObjName 0x0200
//显示  “分时走势”字样
#define MinTitleShowCanvasName 0x0400
//显示 "均线 xxx"
#define MinTitleShowAvgData 0x0800
//显示 分时实时价格的水平线
#define MinChartDrawRealLine 0x1000
//显示 股票代码  60000.sh
#define MinChartShowObjCode 0x00002000



/**
* @brief  用于浮点数比较
* @param[in] fa
* @param[in] fb
* @retval  返回0表示相等 1 表示fa大于fb 返回-1表示fa小于fb
*/
static int compareFloat(double fa,double fb,double epsinon = 0.00001f)
{
    int ret = 0;
    double val = fa-fb;
    if ((val >= - epsinon) && (val <= epsinon))
    {
        ret = 0;
    }else if(val > 0)
    {
         ret = 1;
    }else if(val < 0)
    {
        ret = -1;
    }
    return ret;
}


enum RestorationType
{
    RestorationBeforType = 0,
    RestorationBackType,
    RestorationNone,
};

struct YTickItem{
    int pos;
    double value;
    QString label;
};


enum ContainerType{
    mainType = 0,
    attachType,
    crossType
};

enum AreaIndex{
    mainArea =0,
    Area1,
    Area2,
    Area3,
    Area4,
    Area5,
    TimeArea,
    crossArea,
    AreaSize
};

//单根K线状态  上涨 下跌 相等
enum KlineStatus
{
    KlineStatusUp = 0,
    KlineStatusDown,
    KlineStatusEqual
};

struct ContainerInfo
{
    QString containerClass;    //容器类名称
    QString chartClass;        //chart类名称
    bool bShowTitle = true;
    bool bShowTitleCloseButton = true;
    bool bShowTitleSetButton = true;
    int index = INT_MAX;      //下标
    QString cmd;              //创建时初始命令
    int nTopMargin = 1;
    int nBottomMargin = 1;

    //比例计算
    int nMultiple = 1;        //除去自动宽度后，剩余高度被均分，nMultiple表示几倍高度


    //固定高度 固定高度区域配置在开始或末尾
    bool bFixed = false;      //是否固定高度
    int nHeight = INT_MAX;    //当指定为固定高度时有效

    //指定 上下边距
    bool bFillArea = false;    //是否和绘图区域重叠

    //如果处于选中状态 是否绘制选中样式
    bool bDrawSelectedStyle = true;
};

enum ChartType
{
    ChartMainType = 0, //主图
    ChartAttachType,   //指标附图
    ChartCrossType,    //十字光标图层
    ChartTimeAxisType, //时间轴
};

struct ChartMargins
{
    int leftMargin=0;
    int rightMargin=0;
    int topMargin=0;
    int bottomMargin=0;
    ChartMargins(){}
    ChartMargins(int left,int right,int top,int bottom):
        leftMargin(left),
        rightMargin(right),
        topMargin(top),
        bottomMargin(bottom)
    {

    }
};

struct ChartItemInfo
{
    QString chartClass;        //chart类名称
    QString chartProvider;     //数据提供者
    ChartType type =  ChartAttachType;   //图层类型
    ChartMargins margins;
    int padding = 0; //上边填充区域
    int nHeight = INT_MAX;
    bool bFillArea = false;    //是否和绘图区域重叠
    //是否允许当前区域可选
    bool bSelectedStyle = false;
    bool bShow = true;

    //属性值
    int attr = 0;

    //指标ID
    int indiId = 0;


    //标题相关
    QString titleClass;  //标题类名
    int titleAttr = 0;       //标题属性
    //区域ID
    int id=-1;
};

//容器标题显示文本项
struct TitleTextItem
{
    QString str;      //绘制文本项内容
    QColor clr = "#b1bbc3";       //绘制文本项的颜色
    QString family = QStringLiteral("SimSun");
    int fontSize = 12;
    bool bBold = false;
    int nInterval=0;    //距离下一个文本项的距离 （两个项目间的间隔）
    int fixedWidth = INT_MAX;  //固定宽度
};

//命令项
enum ChartCmdType
{
   ChartCmdClose = 0,
   ChartCmdSettingIndi,
};

//主图更新通知回调 （之前采用信号，但发觉信号在高频率下效率很低，所以自己采用回调了）
typedef std::function<void(QString)> XAxisDataChangeCallBack;
//指标坐标刷新后，通知外部观察者
typedef std::function<void(bool,int,BaseChart*)> IndiAxisRefreshCallBack;
//通知容器区域标题更新文本并绘制
typedef std::function<void(const std::vector<TitleTextItem>&)> TitleTextUpdateCallBack;


#endif // CHARTDEF_H
