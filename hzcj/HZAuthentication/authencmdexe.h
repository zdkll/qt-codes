/********************************************************************
created:2018/02/27
author: dk.zhang
purpose:用户相关网络命令排队执行
*********************************************************************/

#ifndef AUTHENCMDEXE_H
#define AUTHENCMDEXE_H

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <Any.hpp>

class AuthenCmdExe : public QObject
{
    Q_OBJECT
public:
    //命令类型
    enum Command{
        NoneCommand = 0,
        //authen
        LogIn,       //post
        ReLogIn,   //post
        LogGuest,
        LogOut ,
        RefreshToken ,
        GetPermission ,

        //----------------------------------
        ApiCmd,

        //api获取用户信息
        GetUserProfile,//get
        GetZXStock, //get
        UpdateZXStock, //post
        AddZXStock, //post
        DelZXStock  //post
    };
    AuthenCmdExe(QObject *parent = 0);

    //是否完成当前任务
    bool isFinished(){return m_QueCmds.isEmpty();}
    //设置服务器域名
    void setAuthenApiUrl(const QString &auth_url,const QString &api_url);
    void setAccessToken(const QString &accessToken){m_accessToken = accessToken;}
    //执行命令，var表示http请求发送内容,一般为QJsonObject,返回命令Id
    int runCmd(Command cmd,const QVariant &var = QVariant());
    //取消命令
    void  cancelCmd(int id);

    //获取错误码
    int errorCode(){return m_errorCode;}
    QString errString(){return m_errString;}

    void clearAllCms();

signals:
    void  commandStarted(int id);
    void  commandFinished(int id,const QJsonObject &obj = QJsonObject());

    //所有任务执行完成
    void  allFinished();

private:
    int     getId(){return global_id++;}
    void   runQueuedCmd();

    QString md5Authen();//用户登录md5化

    QString randString() ;//随机序列
    QString  getHostMacAddress();  //获取Mac地址

    enum  CmdStatus
    {
        Wait       = 0,
        Canceled,
        Running,
        Finished
    };

    struct CommandInfo{
        CommandInfo(Command  cd,const QVariant& dat=QVariant())
            :cmd(cd),data(dat){}
        bool isRunning(){return status==Running;}
        int               id = -1;
        Command  cmd;
        CmdStatus status = Wait;
        QVariant     data;
    };
private slots:
    //执行命令
    void startCmd(const CommandInfo& cmdInfo);
    //网络请求返回完成
    void finishedCmd();

private:
    QNetworkAccessManager   m_nam;
    QNetworkReply                  *reply;

    int                                       global_id = 0;
    int                                       m_errorCode;
    QString                               m_errString;

    QString                               m_macAddress;
    QString                               m_accessToken;

    //命令队列
    QQueue<CommandInfo>    m_QueCmds;

    //每种服务对应请求Url map
    QHash<Command,QString> m_urlHash;
    QHash<Command,QString> m_cmdHash;
};

#endif // AUTHENCMDEXE_H
