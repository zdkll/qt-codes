#ifndef LOGINOPT_H
#define LOGINOPT_H

#include <QObject>
#include "loginpublicdef.h"

class HZAuthenOpt;
class LogInOpt : public QObject
{
    Q_OBJECT
public:
    enum LogInState{
        UnLogged   =0,
        LoggingIn,  //用户名密码发送，手机号验证码发送，收到服务器收到二维码验证信息
        CancelingLogIn,
        Logged,
        ExistingLogIn,
        WatingLogIn //等待登录，包括获知服务器已经发送验证码或者已经获取了二维码
    };
    LogInOpt(QObject *parent = 0);

    HZLogIn::LogInConfig logInCfg(){return m_logInConfig;}

    //记住密码-自动登录
    void setRmbPwd(const bool &enable){m_logInConfig.isRembPwd = enable;}
    void setAutoLogIn(const bool &enable){m_logInConfig.isAutoLogin = enable;}

    void getVerifyNum(const QString &phoneNum);

    LogInState  logInState(){return m_logInState;}

    QString errString();
public slots:
    void logIn(const HZLogIn::logInType &type,const QStringList &args = QStringList());

    void cancelLogIn(); //取消登录

    void  existLogIn();

    void reLogIn();

protected slots:

signals:
    void logInStateChanged(LogInState state);
    void sendPhoneVerify();//已经发送短信验证码成功
    void recvWem();    //收到服务器的二维码
    void logInFinished();

private:
    void init();
    void setLogInState(const LogInState &state);

    //读取保存登录用户信息
    HZLogIn::LogInConfig readLogInCfg(const QString &fileName);
    bool createLogInFile(const QString &loginFile);
    void saveLogConfig(const  HZLogIn::LogInConfig  &cfg,const QString &fileName);

    void postErrorTip(const int &errCode);

    //用户登录后处理---------
    void postProcess();

private:
    HZLogIn::LogInConfig      m_logInConfig;
    HZLogIn::logInType          m_logInType;
    LogInState                       m_logInState;
    HZAuthenOpt                  *m_authenOpt;

    int                                     m_reqId;
};

#endif // LOGINOPT_H
