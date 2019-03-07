#include "loginopt.h"

#define  JSON_KEY_ISRMBPWD            "IsRembPwd"
#define  JSON_KEY_ISAUTOLOGIN        "IsAutoLogin"
#define  JSON_KEY_USERS                    "Users"
#define  JSON_KEY_USERNAME            "UserName"
#define  JSON_KEY_PASSWORD           "Password"

#define USERPASSWORD_KEY             "\x1f\x1e\x22\x23\x34\x45\x56\x67\x78\x89\x9a\xab\xbc\x2d\x01\xe2"

#define LOGIN_FILE                              "login.json"

#include <QApplication>

#include "hzopenssl.h"
#include "hzuserdatamgr.h"
#include "hzdirhelper.h"

#include "hzauthenopt.h"
#include "datacontroller.h"

#include "hzcfg.h"
#include "localstocksource.h"

#define Wait(sec)  {\
    QTime t; t.start();   \
    while(t.elapsed()<sec*1000) \
    QApplication::processEvents(); \
    };

LogInOpt::LogInOpt(QObject *parent )
    :QObject(parent)
{
    init();

    m_authenOpt =  HZAuthenOpt::instance();
    ServerInfo serverInfo = Hzcfg::getInstance()->serverInfo();
    qDebug()<<serverInfo.authurl <<serverInfo.apiurl;
    m_authenOpt->setAuthenApiUrl(serverInfo.authurl,serverInfo.apiurl);
}

QString LogInOpt::errString()
{
    return m_authenOpt->errString();
}

void LogInOpt::logIn(const HZLogIn::logInType &type,const QStringList &args)
{
    m_logInType = type;
    //开始登录....
    if(m_logInType == HZLogIn::UserLogIn || m_logInType == HZLogIn::GuestLogIn){

        m_logInState = LoggingIn;
        emit logInStateChanged(m_logInState);

        if(m_logInType == HZLogIn::UserLogIn)
            m_reqId =m_authenOpt->login(args[0],args[1]);
        else
            m_reqId = m_authenOpt->loginGuest();
        //等待3s用户登录完成
        bool ok = m_authenOpt->waitForFinshed();
        if(!ok){
            m_logInState = UnLogged;
            emit logInStateChanged(m_logInState);
            return;
        }

        //用户登录需要获取用户权限信息
        if(m_authenOpt->isUserLogged()){
            //查询用户权限信息
            m_authenOpt->queryPermission();
            ok =  m_authenOpt->waitForFinshed();
            if(!ok){
                existLogIn();
                return;
            }
            if(!ok){
                existLogIn();
                return;
            }
            //获取自选股
            m_authenOpt->queryZXStock();
            ok =  m_authenOpt->waitForFinshed();

            m_logInConfig.logUser.userName = args[0];
            m_logInConfig.logUser.password  = args[1];
        }

        //后处理
        postProcess();
    }

    m_logInState = Logged;
    emit logInStateChanged(m_logInState);
}

void LogInOpt::cancelLogIn() //取消登录
{
    m_logInState = CancelingLogIn;
    emit logInStateChanged(m_logInState);

    m_authenOpt->reqCancel(m_reqId);
}

void  LogInOpt::existLogIn()
{
    m_logInState = ExistingLogIn;
    emit logInStateChanged(m_logInState);

    m_authenOpt->logout();
    bool ok = m_authenOpt->waitForFinshed();
    if(ok){
        //退出登录服务器
        DataController::instance()->logOut();
        m_logInState = UnLogged;
        emit logInStateChanged(m_logInState);
    }
}

void LogInOpt::reLogIn()
{
    m_logInState = ExistingLogIn;
    emit logInStateChanged(m_logInState);

    Wait(1);
    m_logInState = UnLogged;
    emit logInStateChanged(m_logInState);

    m_logInState = LoggingIn;
    emit logInStateChanged(m_logInState);

    Wait(1);
    m_logInState =  Logged;
    emit logInStateChanged(m_logInState);
}

void LogInOpt::getVerifyNum(const QString &phoneNum)
{
    //获取短信验证码...
}

void LogInOpt::init()
{
    //登录文件记录文件
    QString logInFile =  HzUserDataMgr::getInstance()->getAllUsersDir()+LOGIN_FILE;
    m_logInConfig = readLogInCfg(logInFile);
}

void LogInOpt::setLogInState(const LogInState &state)
{
    m_logInState = state;
    emit logInStateChanged(m_logInState);
}

void LogInOpt::postErrorTip(const int &errCode)
{
    //    switch (errCode) {
    //    case LOG_ERROR_NONE:
    //        m_infoTipLbl->clear();
    //        break;
    //    case LOG_ERROR_CANCELED:
    //        m_infoTipLbl->setText(QStringLiteral("操作被取消!"));
    //        break;

    //    case NetworkError:
    //        m_infoTipLbl->setText(QStringLiteral("网络故障!"));
    //        break;

    //    case SystemError:
    //        m_infoTipLbl->setText(QStringLiteral("系统错误，请联系技术人员!"));
    //        break;

    //    case ServiceError:
    //        m_infoTipLbl->setText(QStringLiteral("用户名或密码错误!"));
    //        break;

    //    default:
    //        m_infoTipLbl->setText(QStringLiteral("未知错误!"));
    //        break;
    //    }

}

void LogInOpt::postProcess()
{
    //先保存股票修改
    LocalStockSource::saveStocks();

    HZAuthen::UserInfo userInfo = m_authenOpt->logUserInfo();
    //用户数据切换
    HzUserDataMgr::getInstance()->switchUser(QString::number(userInfo.uid),userInfo.userType == HZAuthen::Guest);//

    LocalStockSource::init();

    //保存和更新登录信息
    QString logInFile = HzUserDataMgr::getInstance()->getAllUsersDir()+LOGIN_FILE;
    saveLogConfig(m_logInConfig,logInFile);

    //登录后端服务器
    ServerInfo serverInfo = Hzcfg::getInstance()->serverInfo();
    DataController::instance()->setServer(serverInfo.quoteSrv.ip,serverInfo.quoteSrv.port);

    //登录数据服务器
    DataController::instance()->logIn(userInfo.uid,userInfo.accessToken);

    emit  logInFinished();
}

HZLogIn::LogInConfig LogInOpt::readLogInCfg(const QString &fileName)
{
    QJsonDocument doc ;
    HZLogIn::LogInConfig logInConfig;
    if(!HzDirHelper::readJson(doc,fileName))
        return logInConfig;

    QJsonObject obj = doc.object();
    //qDebug()<<obj;
    logInConfig.isRembPwd = obj.value(JSON_KEY_ISRMBPWD).toBool();
    logInConfig.isAutoLogin = obj.value(JSON_KEY_ISAUTOLOGIN).toBool();

    QJsonArray userArray = obj.value(JSON_KEY_USERS).toArray();

    for(int i=0;i<userArray.size();i++){
        HZLogIn::User user;
        user.userName = userArray.at(i).toObject().value(JSON_KEY_USERNAME).toString();

        QString encryptPwd = userArray.at(i).toObject().value(JSON_KEY_PASSWORD).toString();
        if(!encryptPwd.isEmpty()){
            user.password  =  HZOpenssl::strAesDecode(USERPASSWORD_KEY,encryptPwd);
        }
        logInConfig.loggedUsers.append(user);
    }
    return logInConfig;
}

bool LogInOpt::createLogInFile(const QString &loginFile)
{
    QJsonObject obj;
    obj.insert(JSON_KEY_ISRMBPWD,false);
    obj.insert(JSON_KEY_ISAUTOLOGIN,false);

    QJsonArray jarray;
    obj.insert(JSON_KEY_USERS,jarray);

    QJsonDocument doc;
    doc.setObject(obj);

    return HzDirHelper::saveJson(doc,loginFile);
}

void LogInOpt::saveLogConfig(const  HZLogIn::LogInConfig  &logInConfig,const QString &fileName)
{
    QJsonObject rootObj;

    rootObj[JSON_KEY_ISRMBPWD]    =   logInConfig.isRembPwd;
    rootObj[JSON_KEY_ISAUTOLOGIN] =  logInConfig.isAutoLogin;

    //登录用户
    HZLogIn::User logUser =  logInConfig.logUser;
    qDebug()<<"saveLogConfig :"<<logUser.userName<<logUser.password;
    QList<HZLogIn::User> loggedUsers = logInConfig.loggedUsers;
    if(!logUser.userName.isEmpty()){
        for(int i=0;i<loggedUsers.size();i++){
            if(loggedUsers[i].userName == logUser.userName){
                loggedUsers.removeAt(i);
                break;
            }
        }
        if(!logInConfig.isRembPwd)
            logUser.password.clear();

        loggedUsers.prepend(logUser);
    }

    QJsonArray userArray ;
    for(int i=0;i<loggedUsers.size();i++){
        QJsonObject obj;
        obj[JSON_KEY_USERNAME] = loggedUsers[i].userName;
        QString passwd                      = loggedUsers[i].password;
        if(!passwd.isEmpty()){
            obj[JSON_KEY_PASSWORD]  =  HZOpenssl::strAesEncode(USERPASSWORD_KEY, passwd);
        }else
            obj[JSON_KEY_PASSWORD]  = "";

        userArray.append(obj);
    }

    rootObj[JSON_KEY_USERS] = userArray;

    QJsonDocument doc;
    doc.setObject(rootObj);
    QString logInFile = HzUserDataMgr::getInstance()->getAllUsersDir()+LOGIN_FILE;

    HzDirHelper::saveJson(doc,logInFile);
}

