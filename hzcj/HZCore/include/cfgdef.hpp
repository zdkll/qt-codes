#ifndef CFGDEF_HPP
#define CFGDEF_HPP
/********************************************************************
created:  20178-4-17
author: min.cheng
purpose: 统一定义配置项的数据结构
*********************************************************************/

#define Server_Key_Update_Version    "Update_Version"

//服务器
#define Server_Key_DataServer            "Data_Server"
#define Server_Key_AuthUrl                 "Auth_Url"
#define Server_Key_ApiUrl                   "Api_Url"
#define Server_Key_Sever_IP                "IP"
#define Server_Key_Sever_Port             "Port"
#define Server_Key_InfoSys                "Info_Sys_Url"
#define Server_Key_InfoSys_H5                "Info_Sys_H5_Url"
#define Server_Key_QuoteSys               "Info_Quote_Url"
#define Server_Key_DumpUrl                "Dump_Url"
#define Server_Key_Web_Product            "Web_Product"

#include <vector>
#include <map>
#include <memory>
#include <QHash>
#include <QColor>
#include <QVector>
#include <QMap>
///////////////////////////////////指标相关/////////////////////
enum IndiDrawArea
{
    indi_main =0,
    indi_attach,
};

struct IndiParam
{
    std::string name; //输出结果对应的名称
    int value;        //指标参数
    int minValue;     //指标最小参数
    int maxValue;     //指标最大参数
    std::string des;  //指标描述
};

enum IndiDrawType
{
    LINESTICK = 0, //直线
    ICONSTICK,     //图标
    VOLSTICK, //柱状
    COLORSTICK,//彩色棒状
    ENTITYRECTSTICK, //实现柱状,默认从0.0
    CMFBSTICK,// 筹码分布 集分绘制
    ENTITYRECTSTICKEX, //实体柱状，由外部指定上下点和填充颜色
    BUYSELLSTICK,//买卖点绘制 由外部指定点和填充颜色
    DUOKONGSTICK,//多空线绘制
    CIRCLESTICK, //绘制圆圈
};

struct IndiResult
{
    std::string screenname; //输出结果在标题区域的显示名称
    std::string dsc;
    bool bShowName;         //当前输出结果名称是否在标题处显示
    int isVisible;          //当前输出结果是否显示 1显示 0不显示
    int drawType;           //样式 IndiDrawType
    QMap<int,QColor> clrMap;   //当前结果的显示颜色

    // indi_draw_type_line  样式
    int linewidth = 1;  //输出线性宽度

    //indi_draw_type_icon 样式
    std::string icon;  //图标名称

};

struct IndiItemInfo
{
    int id;
    std::string name;
    std::string script;     //指标执行时的脚本
    std::string screenname; //标题区域显示的名称
    std::string strHelpUrl; //指标帮助的URL
    bool bLocalCalc;        //是否本地计算的指标
    int drawArea;           //绘制在主图区域还是附图区域  0表示主图 1表示副图

    std::vector<IndiParam> params;
    std::map<std::string,IndiParam> paramMap;

    std::vector<IndiResult> results;
    std::map<std::string,IndiResult> resultMap;
};

/*!
 * \brief tagSTCategories log
 *
 */
typedef struct tagSTCategories
{
    QString name;
    QString priority;
    QString appender;
    QString fileName;
    QString layout;
    QString conversionPattern;
}STCategories;

typedef std::vector<IndiItemInfo> InidItemList;
typedef std::map<std::string,IndiItemInfo> IndiItemMap;
typedef std::map<int,IndiItemInfo> IndiIndexMap;
typedef QHash<QString, QList<STCategories>> LogSTCateoriesHash;

struct Host{
    QString ip;
    int         port;
};

struct ServerEnvInfo
{
    QString envName;
    Host      dataSever;
    QString  loginUrl;
};


struct ServerInfo{
    QString updateInfoUrl;//更新信息地址
    QString authurl;//登录
    QString apiurl;//It中心用户操作
    Host quoteSrv;
    QString webproducturl;//web product url
    QString counturl;
    QString plateurl;
    QString indexurl;
    QString indexStockChildListUrl;
    QString stockurl;
    QString infoSysUrl;
    QString infoStsH5Url;
    QString infoQuoteUrl;
    QString dmpUploadUrl; //dump文件服务地址
};


#endif // CFGDEF_HPP
