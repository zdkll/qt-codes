/********************************************************************
created:2018/07
author: dk.zhang
purpose:用户登录相关的功能类，支持登录、退出、获取权限等，每个命令控制串行依次执行
            2018/08/27 作为统一的对外接口
*********************************************************************/
#ifndef HZAUTHENOPT_H
#define HZAUTHENOPT_H

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QQueue>
#include <QMap>
#include <QHash>

#include "authencmdexe.h"
#include "authenpublicdef.h"
#include "hzauthentication_global.h"

class HZAUTHENTICATIONSHARED_EXPORT HZAuthenOpt : public QObject
{
    Q_OBJECT
public:
    //登录状态
    enum LogStatus{
        UnLogged = 0,     //未登录
        GuestLogged = 1,//游客登录
        UserLogged  = 2  //用户登录
    };
    static  HZAuthenOpt *instance();

    ~HZAuthenOpt();

    // 设置 服务器域名
    void setAuthenApiUrl(const QString &auth_url,const QString &api_url);

    //取消某个请求
    void reqCancel(int id);

    //以下函数返回命令Id，如果为-1表示失败，比如用户没有登录
    int login(const QString &userName,const QString &psword);
    //  注销当前用户
    int logout();
    // 游客登录
    int  loginGuest();
    //   刷新accessToken
    int refreshAccessToken();
    //   获取用户权限列表
    int queryPermission();

    //以下接口只针对用户使用，游客自选股请使用其他接口
    // 获取用户信息
    int queryUserProfile();
    //   获取用户自选股
    int queryZXStock();
    // 更新自选股,注意只是更新已经自选的股票，将最新股票重新排序
    int updateZXStock(const QStringList &zxstocks);
    //   添加自选股
    int addZXStock(const QString &stock);
    //删除自选股
    int delZXStock(const QString &stock);

    //添加一个同步等待的接口
    bool waitForFinshed(int msec = 3000);

    //获取信息相关，命令执行完成后，调相应接口获取对应信息--------------
    // 当前登录用户信息 uid token expire...
    HZAuthen::UserInfo  logUserInfo(){return m_d->userInfo;}
    //当前用户类型  未登录、游客，用户
    HZAuthen::UserType userType(){return m_d->userInfo.userType;}
    //获取当前登录状态 未登录、游客登录，用户登录
    LogStatus logStatus(){return m_logStatus;}

    //获取查询用户资料
    HZAuthen::UserProfile getUserProfile() const {return m_d->userProfile;}

    //获取查询自选股结果 自选股列表 SortNo 排序
    QVector<QString>  getZXStock() const {return m_d->zxStocks;}

    //判断是自选股
    bool isHas(const QString& code){return m_d->zxStocks.contains(code);}
    // 当前用户是否登录  未登录、登录
    const bool isUserLogged(){return m_logStatus==UserLogged;}

    //用户权限信息
    QList<HZAuthen::PermissionItem> getUserPermissions(){return m_d->permissions.values();}
    //判断用户当前是否有某权限
    bool hasPermission(HZAuthen::UserPermissionType typeId);

    //获取错误信息
    int errorCode();
    QString errString();

signals:
    //命令完成信号，命令id 和 error，error = 0表示无错误
    void	commandFinished(int id, int error);
    void loginStateChanged();

protected slots:
    //当前请求完成
    void slotRequestFinished(int id,const QJsonObject &obj);

private:
    HZAuthenOpt(QObject *parent = 0);
    void       init();
    HZAuthen::AuthenError authenError(const int &errorCode);

    //保存用户资料
    void readUserProfile(const QJsonObject& jobj,HZAuthen::UserProfile &profile);
    //保存自选股
    void readZXStocks(const QJsonObject& jobj,QVector<QString> &zxstocks);

    bool downloadProfileIcon(const QString &url,const QString &iconFile);

private:
    //数据
    struct DataPrivate
    {
        void clearData(){
            zxStocks.clear();
            permissions.clear();
            currentStocks.clear();
        }
        //命令记录，方便返回时候做处理
        QHash<int,AuthenCmdExe::Command>  cmdHash;

        HZAuthen::UserInfo     userInfo;   //登录用户
        HZAuthen::UserProfile userProfile; //用户资料
        QVector<QString>       zxStocks;  //自选股

        QHash<int,HZAuthen::PermissionItem>  permissions;//用户权限

        QHash<int,QString>   currentStocks;
    };

    static HZAuthenOpt           *m_instance;
    AuthenCmdExe                 *m_cmdExe;
    QJsonObject                      m_logParamObj;
    HZAuthen::UserInfo            m_userInfo;

    DataPrivate                       *m_d;
    QMap<int,QString>            m_cmdMap;

    LogStatus                          m_logStatus;

    //结果
    struct Ret{
        int          errCode = 0;
        QString  errString;
    }m_ret;

};

#endif // HZAUTHENOPT_H
