#ifndef INFODEF_H
#define INFODEF_H

#include <QtCore>
#include <QUrl>
#include <QPair>
#include <hzinfodata.h>

//annc - news
#define HZINFOITEM_INDEX_CNT       6
#define HZINFOITEM_INVALID_VALUE   -1

#define HZINFOITEM_OFFSET_Y        36
#define HZINFOITEM_INDEXBTN_H      30
#define HZINFOITEM_DISTANCE_LINE   18
#define HZINFOITEM_BUTTON_RADIUS   10
#define HZINFOITEM_TEXT_MARGIN     12

#define HZINFOITEM_LEFT_SPACE       22
#define HZINFOITEM_TOP_SPACE         14
#define HZINFOITEM_RIGHT_SPACE       16
#define HZINFOITEM_HEIGHT_SIZE      105

//MultiHour
#define HZINFOITEM_RECT_LEFT_SPACE     40
#define HZINFOITEM_ROOT_RADIUS_VALUE   10
#define HZINFOITEM_ROOT_WIDTH_VALUE    145
#define HZINFOITEM_ROOT_HEIGHT_VALUE   25

#define HZINFOITEM_CUSTOM_ROLE_ITEM        Qt::UserRole+1
#define HZINFOITEM_CUSTOM_ROLE_TIME_ROOT   Qt::UserRole+2
#define HZINFOITEM_CUSTOM_ROLE_TIME_NULL   Qt::UserRole+3
#define HZINFOITEM_CUSTOM_ROLE_TILE_URL    Qt::UserRole+4
#define HZINFOITEM_CUSTOM_ROLE_ITEM_TIME   Qt::UserRole+5
#define HZINFOITEM_CUSTOM_ROLE_ITEM_STOCKCODE   Qt::UserRole+6

#define CUSTOME_LIST_NULL           -2
#define REQUEST_OPERATOR_EXCEPTION  0
#define REQUEST_SUCCESS 0


static const QVector<QString> gTagTexts = {
    QStringLiteral("未标签"),
    QStringLiteral("强烈利好"),
    QStringLiteral("利好"),
    QStringLiteral("中性"),
    QStringLiteral("利空"),
    QStringLiteral("强烈利空"),
    QStringLiteral("全部")
};

enum
{
    Custom_Stock_Null,
    Request_Exception,
    Request_Null,
    Requst_Normal
};

enum EMIndexFilter{
    All_Index=0,
    Good_Index,         //利好
    Strong_Good_Index,  //强烈利好
    Neutral_Index,      //中性
    Negative_Index,     //利空
    Strong_Negative_Index,//强烈利空
};

enum EMWindowType
{
    PopWindow=2,
    InsertWindow,
};

enum EMCountryFlags
{
    All_Country=0,
    China_Country,
    Americ_Country,
    EuropeanUnion_Country,
    Japan_Country,
};

enum EMMarketItemType
{
    FHPX_Item,
    TFP_Item,
    SXJJ_Item,
    ZF_Item,
    YJZF_Item,
    MajorMeet_Item,
    FinancialReport_Item,
    EconomicData_Item
};

//enum EMEconomicItemType
//{
//    MajorMeet_Item,
//    FinancialReport_Item,
//    EconomicData_Item
//};

struct CalendarDetailsData
{
    int type;
    QString name;                   //板块名称
    QMap<QString,QString> datas;   //板块数据
    bool operator ==(const CalendarDetailsData &item) const {
        return item.type==this->type;
    }
};

/*!
 * \brief 热门资讯列表接口
 */
typedef struct tagSTHotInfoData{
    QString id;             //详情id
    int tag;            //1
    quint32 time;           //time_t
    bool isCus;         //是否自选？
    QString title;      //标题
    QString content;    //简介
    QString source;     //央妈网站
    QString url;        //detail
    QString relateName;
    QString relateCode;
    QVector<QString> codeNames;//name + code
    bool operator ==(const tagSTHotInfoData &item) const {
        return item.title==this->title && item.time == this->time;
    }
}STHotInfoData;

/*!
 * \brief The STOptionalNews struct 新闻列表接口
 */
struct STOptionalNews
{
    QString id;             //详情id
    QString stockcode;  //股票代码
    QString stockname; //股票名称
    QString title;      //标题
    QString desc;       //简介
    int tag;            //标签，如利好、利空、中性等等
    QString source;     //来源
    quint32 time;           //发布时间，时间戳格式
    QVector<QString> related_stockcode;//	相关股票代码，数组格式
    QString change;       //涨跌
    QString chg;          //涨跌幅
    bool isCus;         //是否自选
    QString url;        //detail
    bool operator ==(const STOptionalNews &item) const {
        return item.id==this->id;
    }
};

/*!
 * \brief The STAnnc struct 公告列表接口
 */
struct STAnnc
{
    QString id;             //详情id
    QString stockcode;  //股票代码
    QString stockname;  //股票名称
    QString title;      //标题
    QString desc;       //简介
    int tag;            //标签，枚举值（强烈利好、利好、中性、利空、强烈利空）
    qreal chg=DBL_MAX;          //涨幅
    QString source;     //来源
    bool isCus;         //是否自选
    quint32 time;           //发布时间，时间戳格式
    QString url;        //detail
    bool operator ==(const STAnnc &item) const {
        return item.id==this->id || item.time == this->time;
    }
};

/*!
 * \brief The STMultiHour struct 7*24 hour
 */
struct STMultiHour{
    QString id;
    QString title;    //标题
    QString content;  //内容
    QString source;   //来源
    quint32 time;         //发布时间，时间戳格式
    bool timeRoot;
    QString url;        //detail
    bool operator ==(const STMultiHour &item) const {
        return item.time==this->time;
    }
};


struct STEconmicMajorMeetUi
{
    struct StockInfoUi{
        QString stockname;
        QString stockcode;
        qreal chg;//涨跌幅
        bool isCustom;
    };
    int type;//0-title 1-board 2-stock 3-none
    QString id;
    QString title;
    QString url;//详情页
    QVector<StockInfoUi> rltBoard;
    QVector<StockInfoUi> rltStock;
};

struct STEconomicDataUi
{
    int type;//0-title 1-reigon and other 2-none
    QString name;//	名称
    QString pre_value="";//	前值
    QString exp_value="";//	预测值
    QString act_value="";//	公布值
    QString region="";//	地区
    QString source="";//	来源
    quint32 time=UINT_MAX;//	公布时间戳
};


Q_DECLARE_METATYPE(CalendarDetailsData)
Q_DECLARE_METATYPE(STHotInfoData)
Q_DECLARE_METATYPE(STOptionalNews)
Q_DECLARE_METATYPE(STAnnc)
Q_DECLARE_METATYPE(STMultiHour)
Q_DECLARE_METATYPE(STEconmicMajorMeetUi)
Q_DECLARE_METATYPE(STEconomicDataUi)
#endif // INFODEF_H
