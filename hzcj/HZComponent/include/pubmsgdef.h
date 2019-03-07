#ifndef PUBMSGDEF_H
#define PUBMSGDEF_H
/********************************************************************
created: 2018/5/31
author: min.cheng
purpose: 公共消息定义  比如页面发生的消息，其他窗口发送页面的消息等。同时还
         可以包括一个公共需要访问的结构。
         所有的消息都模仿HTTP样式，进行页面切换或传输
         例如：root_frame?switch_page=1&hide_panel=stockpanel
*********************************************************************/

//界面页面枚举值
enum PageId
{
    home_page = 0,           //首页
    option_stock_page,       //自选股页面
    quote_list_page,         //行情页面
    stock_detail_page,       //个股详情页
    block_list_page,         //板块页面
    info_center_page,        //资讯中心页面
    data_center_page,        //数据中心
    strategy_center_page,    //策略中心
    product_page,            //产品页面 更具权限来展示具体的产品

    hsstock_list_page,       //沪深个股页面
    hsindex_list_page,       //沪深指数页面
    block_monitor_page,      //板块监控页面
    finance_news_page,       //财经要闻页面
    live_video_page,         //视频直播
    websit_page,             //官网
    activity_center_page,    //活动中心
    short_msg_nuggets_page,  //短线掘金
    go_buy_page,             //一键购买
    kline_page,              //技术分析页面
    min_page,                //分时页面
    HsRankingPage           //沪深排行页面





    //test_page,
};

//子窗口类型
enum BtChildWndType
{
    BtBaseQuoteListType = 0,
    BtFundFlowerFlowType,
    BtDDEType,
};

//行情页面 子页面定义
#define HszhInnerWndType 0
#define HsagListWndType 1
#define ShagListWndWndType 2
#define SzagListWndWndType 3
#define HsfxListWndType 4
#define ZxbkListWndType 5
#define CybkListWndType 6
#define ZyzsListWndType 7
#define QbzsListWndType 8

//自选页面  子页面定义
#define OptionListWndType 0
#define OptionRecentBrowseWndType 1
#define OptionInfoWndType 2
#define OptionSmartMonitorWndType 3
#define OptionWeeklyNewspaperWndType 4

//数据页面  子页面定义
//集合竞价
#define CallAuctionWnd 0
//涨停监测
#define LimitUpMonitorWnd 1
//龙虎榜
#define WinnersListWnd 2
//资金异动
#define FundAbnormalWnd 3
//新股日历
#define NewStockCalendarWnd 4
//融资融券
#define BlockTradeWnd 5
//大宗交易
#define SecuritiesMarginTradingWnd 6
//龙虎榜营业部详情
#define WinnersListSalesOfficeDetailWnd 7
//融资融券详情
#define BlockTradeDetailWnd 8
//融资融券板块成分股
#define BlockTradeBkAndStocksWnd 9
//集合竞价
#define CallAuctionNoPermissionWnd 10
//涨停监测无权限
#define LimitUpMonitorChildNoPermissionWnd 11
//相似K線
#define SimilarKlinWnd 12
//相似K線无权限
#define SimilarKlinWndoPermissionWnd 13

//所有页面的instancName 等价于PageId 枚举值的中文字符


//用于详情页面 TAB的ID 标示
enum DetialKlineMinType
{
    TabMinType = 0,  //分时
    TabMultiMinType,   //多日分时
    TabKlineMinType, //一分钟
    TabKline5MinType, //5分钟
    TabKline15MinType, //15分钟
    TabKlineHourType, //1小时
    TabKlineDayType, //日K
    TabKlineWeek,    //周K
    TabKlineMonth,   //月K
    TabKlineMinUnknown,
};

//详情页类型
#define block_detail_id 0
#define index_detail_id 1
#define stock_detail_id 2

////////////////////////////////////页面漏油名称定义//////////////////////////////////////////
//主框架窗口
#define ROOT_FRAME "root_frame"
#define MAIN_NAVIGATOR_BAR "mainnavigatorbar"
#define MAIN_SIDER_BAR "mainsidebar"
#define MAIN_TITLE_BAR "maintitlebar"
#define MAIN_STATUS_BAR "mainstatusbar"

//登陆对话框
#define LogIn_Dialog   "login_dialog"

/////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////功能命令(公共)/////////////////////////////////////////////////

//框架相关命令
#define HZ_CMD     "hz_cmd"
#define HZ_GET     "hz_get"
#define HZ_PAGE_ID "page_id"
//一级命令外，扩展的二级命令
#define HZ_CHILD_CMD "hz_child_cmd"

////////////////////////////业务相关命名///////////////////////////////////////////////////////
//股票码
#define STOCK_ID "stock_id"
//股票名称
#define STOCK_NAME "stock_name"
//周期 取值为DetialKlineMinType 并非上传服务器的周期ID 需要确保这点
#define PERIOD_TYPE "period_type"
//龙虎榜Id
#define WINNERS_LIST_ID "winners_list_id"
//时加戳
#define TIME_NUMBER "time_number"
//子窗口ID  用于区域当前命令所触发额子窗口
#define CHILD_WND_ID "child_wnd_id"
//三级子窗口ID 目前最多支持三级
#define THIRD_CHILD_WND_ID  "third_child_wnd_id"
//命令来自的子窗口
#define FROM_CHILD_WND_ID "from_child_wnd_id"
//来回切换K线分时界面
#define SWITCH_KLINE_MIN "switch_kline_min"
//详情页类型
#define STOCK_DETAIL_TYPE "stock_detail_type"
//前后复权
#define SWITCH_DR_FB "switch_dr_fb"



/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////网络连接状态切换广播(公共)/////////////////////////////////////////////////
//命令标示
//网络状态切换通知
#define HZ_NETIO_NOTIFY "hz_netio_notify"
//网络状态 1 网络连接成功可读写  0 网络断开或初始化中 不可读写
#define HZ_NETIO_STATUS "hz_netio_status"
/*

 广播切换通知
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,HZ_NETIO_NOTIFY);
        msg.emplace(HZ_NETIO_STATUS,QString::number(1));
        broadcastMessage(msg);

*/

/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////自选股切换广播////////////////////////////////////////////////
//通知切换
#define HZ_SELFSTOCK_STATUS "hz_selfstock_status"
//切换完成后广播
#define SELFSTOCK_STATUS  "selfstock_status"
/*
 广播切换完成通知
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,HZ_SELFSTOCK_STATUS);
        msg.emplace(SELFSTOCK_STATUS, QString::number(true));
        broadcastMessage(msg);

*/
/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////主框架窗口切换页面////////////////////////////////////////////////
//通知切换
#define ROOT_SWITCH_PAGE "root_switch_page"
//切换完成后广播
#define PAGE_CHANGED "page_changed"
//显示迷你盯盘
#define SHOW_MINI_DLG "show_mini_dlg"
//显示短线异动界面
#define SHOW_DXYD_DLG "show_dxyd_dlg"
//显示逐笔弹框窗口
#define SHOW_ALL_TIME_DEAL_DLG "show_all_time_deal_dlg"
//隐藏逐笔窗口
#define HIDE_ALL_TIME_DEAL_DLG "hide_all_time_deal_dlg"

/*
 发送页面切换命令
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE)
        msg.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
        msg.emplace(STOCK_ID,"sh600031");    //股票和板块ID 如果是板块跳转，这个字段也要求赋值
        msg.emplace(PERIOD_TYPE,QString::number(TabKlineDayType)); //可选 周期
        msg.emplace(CHILD_WND_ID,1);                  //可选
        sendMessage(ROOT_FRAME,msg,true);

 广播切换完成通知
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,PAGE_CHANGED);
        msg.emplace(HZ_PAGE_ID,QString::number(option_stock_page));
        broadcastMessage(msg);

*/
/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////标题栏菜单命令//////////////////////////////////////////////
#define SHOW_LOGIN_DIALOG "show_login_dialog"
#define SWITCH_USER               "swicth_user"

#endif // PUBMSGDEF_H
