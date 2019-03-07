#ifndef AUTHENPUBLICDEF_H
#define AUTHENPUBLICDEF_H

#include <QtCore>

QT_BEGIN_NAMESPACE
namespace HZAuthen{
enum UserType{
    NoUser = 0,
    Guest = 1,
    User   =2,
};
struct  UserInfo{
    UserInfo():userType(NoUser),uid(0){}
    void reset(){userType = NoUser;uid = 0;}
    UserType     userType = NoUser; //1为游客，2为游客
    quint64  uid;
    QString accessToken;
    uint       expireDate;
};

//UserProfile  用户资料
struct  UserProfile{
    quint64 userId;     //用户id
    quint64 createAt;  //创建时间
    QString screenName;//昵称
    QString loginUser; //登录账号
    QString mobileNum; //手机号
    QString name;  //姓名
    QString qq;
    QString webchat;
    QString address;//住址
    int         level;  //用户等级
    QString gender; //性别
    QString profileImgMobile;
    QString profileImgWeb;
    QString pca;  //户籍
    QString zipcode; //邮编

    //头像：本地，默认或者保存
    QString  icon;
};

//用户权限的类型
enum UserPermissionType
{
    up_zbs = 0,    //直播室  zbs
    up_yk = 1,      //月刊      yk
    up_tzyf = 2,    //投资有方tzyf
    up_zk = 3,      //周刊      zk
    up_tzxt = 4,    //投资学堂tzxt
    up_dkx = 5,    //大咖秀    dkx
    up_dpzs = 6,  //盯盘助手 dpzs
    up_cjrl = 7,     //财经日历  cjrl
    up_dxjj = 8,    //短线掘金  dxjj
    up_nrzg = 9,  //牛人掌股  nrzg
    up_hzyj = 10, //汇正赢家  hzyj
    up_cynx = 11, //穿越牛熊  cynx
    up_cljq = 12,  //策略进取  cljq
    up_jgjj = 13,    //金股狙击  jgjj
    up_jhjj = 14,    //集合竞价  jhjj
    up_xskx = 15,  //相似K线   xskx
    up_lhxj   = 16, //龙虎先机   lhxj
    up_szkc = 17,//实战课程   szkc
    up_hztj = 18,//汇正投教   hztj
    up_zbhjjj = 19,//指标黄金狙击   zbhjjj
    up_zbdcxg = 20,//指标多彩选股   zbdcxg
    up_byzs = 21,//指标博弈致胜   byzs
    up_bdww = 22,//指标波段为王   bdww
    up_ztjc = 23//涨停监测   ztjc
};

struct PermissionItem
{
    UserPermissionType id;  //权限类型
    QString  permissionName;     //权限名称
    QString  permissionNameEn;//权限名称-拼音简称
    uint        startTime;
    uint        endTime;
};

//登录错误
enum AuthenError
{
    NoError = 0,
    NetworkError = 0001,//网络错误，链接错误或者请求协议地址等错误

    //以下为服务器返回错误
    SystemError  =10000, //系统级错，服务出错或者接口问题

    ServiceError = 20000,  //服务级错误,用户名密码错误，格式错误，账号过期等

    UnknownError = 30000
};

}
QT_END_NAMESPACE


#endif // AUTHENPUBLICDEF_H
