#ifndef SMARTPANELDEF_H
#define SMARTPANELDEF_H
#include <QtCore>

#include <datamodeldefs/stockinfodef.h>


/**
 * @brief The CompanyType enum 大事类型
 */
enum CompanyType
{
    LongHuBang=1,
    FenHongSongZhuan,
    CaiBao
};

/**
 * @brief The EMPageType enum 页类型
 */
enum EMPageType{
    CompanyEvent_Page,
    SmartPlate_Page //盘中异动
};

/**
 * @brief The DrawType enum 绘制类型
 */
enum DrawType{
    Draw_Date, //日期标志 格式：yyyy-MM-dd
    Draw_Time, //详情时间标志
    Draw_Content, // 详情描述标志 详情展示，最多只展示两行
    Draw_ContentEx, // 详情描述标志 详情展示
    Draw_None // 停止标志(space)
};

/**
 * @brief The PanelItem struct 公司大事
 */
struct PanelItem
{
    DrawType type;  //draw graph
    int contentEx;// content ex
    int msgType;    //1：龙虎榜，2：分红送转，3：财报
    QString code;   //stockcode
    QString name;   //stockname
    QString title;  //title
    QString content;//descript

    bool smFlags=false; //鼠标点击跳转标志--财报详情的
    qreal change=DBL_MAX; //涨跌额
    qreal chg =DBL_MAX;   //涨跌幅
    qreal real_chg = DBL_MAX;
    qreal buy=DBL_MAX;//	龙虎榜买入额，单位：元
    qreal sell=DBL_MAX;//	龙虎榜卖出额，单位：元
    qreal net_buy=DBL_MAX;//	龙虎榜净买入额，单位：元
    QString reason="";//	龙虎榜上榜原因
    QString record_date=UINT_MAX;//	股权登记日，格式：YYYYMMDD
    QString dividend_date=UINT_MAX;//	除权除息日，格式：YYYYMMDD
    QString detail="";//	分红送转详情
    quint32 date=UINT_MAX; //	财报发布日期，格式：YYYYMMDD
    QString url="";//	财报H5详情页地址
    quint32 time=UINT_MAX;//	数据更新时间戳
    QString typeTwoMsg="";

    UINT32                    update_time = INVALID_UINT32;//更新时间
    UINT32                    last_price = INVALID_UINT32;//现价
    UINT64                    turnover = INVALID_UINT64;//成交额
    UINT64                    volume  = INVALID_UINT64; //成交量
};

#endif // SMARTPANELDEF_H
