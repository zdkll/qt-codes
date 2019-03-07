#include "hzauthenopt.h"

#include <iostream>
#include <string>
#include <cstdlib>

#include <hzcjinfo.h>
#include <hzlog.h>
#include <QDateTime>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDebug>
#include <QtCore>

#include "hzopenssl.h"
#include "httpclient.h"

using namespace std;

HZAuthenOpt *HZAuthenOpt::m_instance = NULL;

HZAuthenOpt::HZAuthenOpt(QObject *parent )
    :QObject(parent),m_cmdExe(new AuthenCmdExe(this))
    ,m_d(new DataPrivate),m_logStatus(UnLogged)
{
    init();

    connect(m_cmdExe,&AuthenCmdExe::commandFinished,this,&HZAuthenOpt::slotRequestFinished);
}

HZAuthenOpt *HZAuthenOpt::instance()
{
    if(!m_instance)
        m_instance = new HZAuthenOpt;
    return m_instance;
}

HZAuthenOpt::~HZAuthenOpt()
{
    delete m_d;
}

void HZAuthenOpt::setAuthenApiUrl(const QString &auth_url,const QString &api_url)
{
    m_cmdExe->setAuthenApiUrl(auth_url,api_url);
}

void HZAuthenOpt::reqCancel(int id)
{
    m_cmdExe->cancelCmd(id);
}

int HZAuthenOpt::login(const QString &userName,const QString &psword)
{
    if(m_logStatus != UnLogged)
        logout();
    //如果已经登录需要先退出
    m_logParamObj.insert("loginUser",userName);
    m_logParamObj.insert("password",psword);
    int id =  m_cmdExe->runCmd(AuthenCmdExe::LogIn,m_logParamObj);
    m_d->cmdHash.insert(id,AuthenCmdExe::LogIn);
    Log("LogInLOG","INFO","start logIn user %s",userName.toStdString().c_str());
    return id;
}

int HZAuthenOpt::logout()
{
    int id= m_cmdExe->runCmd(AuthenCmdExe::LogOut);
    m_d->cmdHash.insert(id,AuthenCmdExe::LogOut);
    Log("LogInLOG","INFO","logout current user.");
    return id;
}

int HZAuthenOpt::loginGuest()
{
    if(m_logStatus != UnLogged)
        logout();
    //如果已经登录需要先退出
    int id =  m_cmdExe->runCmd(AuthenCmdExe::LogGuest);
    m_d->cmdHash.insert(id,AuthenCmdExe::LogGuest);
    return id;
}

int HZAuthenOpt::refreshAccessToken()
{
    int id = m_cmdExe->runCmd(AuthenCmdExe::RefreshToken);
    m_d->cmdHash.insert(id,AuthenCmdExe::RefreshToken);
    return id;
}

int HZAuthenOpt::queryPermission()
{
    int id = m_cmdExe->runCmd(AuthenCmdExe::GetPermission);
    m_d->cmdHash.insert(id,AuthenCmdExe::GetPermission);
    return id;
}

// 获取用户信息
int HZAuthenOpt::queryUserProfile()
{
    if(!isUserLogged()){
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("用户未登录");
        return m_ret.errCode;
    }
    int id = m_cmdExe->runCmd(AuthenCmdExe::GetUserProfile);
    m_d->cmdHash.insert(id,AuthenCmdExe::GetUserProfile);
    return id;
}

// 获取用户自选股
int HZAuthenOpt::queryZXStock()
{
    if(!isUserLogged()){
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("用户未登录");
        return m_ret.errCode;
    }
    int id = m_cmdExe->runCmd(AuthenCmdExe::GetZXStock);
    m_d->cmdHash.insert(id,AuthenCmdExe::GetZXStock);
    return id;
}
// 更新自选股
int HZAuthenOpt::updateZXStock(const QStringList &zxstocks)
{
    if(!isUserLogged()){
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("用户未登录");
        return m_ret.errCode;
    }
    int id = m_cmdExe->runCmd(AuthenCmdExe::UpdateZXStock,zxstocks);
    m_d->cmdHash.insert(id,AuthenCmdExe::UpdateZXStock);
    return id;
}
//添加自选股
int HZAuthenOpt::addZXStock(const QString &stock)
{
    if(!isUserLogged()){
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("用户未登录");
        return m_ret.errCode;
    }
    if(m_d->zxStocks.size()>=100){
        m_ret.errCode = -1;
        m_ret.errString =QStringLiteral("自选股已经有100个，无法继续添加");
        return m_ret.errCode;
    }

    int id = m_cmdExe->runCmd(AuthenCmdExe::AddZXStock,stock);
    m_d->cmdHash.insert(id,AuthenCmdExe::AddZXStock);
    return id;
}
//删除自选股
int HZAuthenOpt::delZXStock(const QString &stock)
{
    if(!isUserLogged()){
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("用户未登录");
        return m_ret.errCode;
    }
    int id = m_cmdExe->runCmd(AuthenCmdExe::DelZXStock,stock);
    m_d->cmdHash.insert(id,AuthenCmdExe::DelZXStock);
    m_d->currentStocks.insert(id,stock);
    return id;
}

bool HZAuthenOpt::waitForFinshed(int msec)
{
    if(m_cmdExe->isFinished()) return (m_cmdExe->errorCode() == 0);
    QTimer t;
    t.setInterval(msec);
    t.setSingleShot(true);
    t.start();

    QEventLoop loop;
    bool ok = true;

    QMetaObject::Connection con1 = connect(m_cmdExe,&AuthenCmdExe::allFinished,&loop,&QEventLoop::quit);
    QMetaObject::Connection con2 = connect(&t,&QTimer::timeout,&loop,&QEventLoop::quit);
    loop.exec();
    disconnect(con1);
    disconnect(con2);

    //登录完成
    if(t.isActive()){
        t.stop();
        ok = (m_cmdExe->errorCode() == 0);
    }else//超时
    {
        m_ret.errCode = -1;
        m_ret.errString = QStringLiteral("操作超时");
        m_d->clearData();

        //清除所有历史
        m_cmdExe->clearAllCms();
        ok = false;
    }
    return ok;
}

bool HZAuthenOpt::hasPermission(HZAuthen::UserPermissionType typeId)
{
    auto it = m_d->permissions.find(typeId);
    if(it != m_d->permissions.end()){
        //判断时间
        uint curTime_t = QDateTime::currentDateTime().toTime_t();
        return (curTime_t>=it.value().startTime && curTime_t<=it.value().endTime);
    }
    return false;
}

int HZAuthenOpt::errorCode()
{
    int code = m_ret.errCode;
    m_ret.errCode = 0;
    return code;
}
QString HZAuthenOpt::errString()
{
    QString errString = m_ret.errString;
    m_ret.errString.clear();
    return errString;
}


///-----------------------------
void HZAuthenOpt::slotRequestFinished(int id,const QJsonObject &obj)
{
    auto it =  m_d->cmdHash.find(id);
    if(it == m_d->cmdHash.end())
        return;
    AuthenCmdExe::Command cmd = it.value();

    m_ret.errCode = m_cmdExe->errorCode();
    qDebug()<<m_cmdExe->errorCode()<<m_cmdExe->errString();
    //有错误直接返回
    if(m_ret.errCode != 0){
        m_d->currentStocks.remove(id);
        m_d->cmdHash.remove(id);
        m_ret.errString = m_cmdExe->errString();
        emit commandFinished(id,authenError(m_ret.errCode));
        return;
    }
    //无错误
    switch (cmd) {
    case AuthenCmdExe::LogIn:
    {
        m_logStatus = UserLogged;
        QJsonObject dataobj = obj.value("data").toObject();

        m_d->userInfo.userType = HZAuthen::User;
        m_d->userInfo.uid = dataobj.value("uid").toVariant().toULongLong();
        m_d->userInfo.accessToken = dataobj.value("accessToken").toString();
        m_d->userInfo.expireDate    = dataobj.value("expireDate").toInt();
        m_cmdExe->setAccessToken(m_d->userInfo.accessToken);

        //重新登录后清除用户信息
        m_d->clearData();
    }
        break;
    case AuthenCmdExe::LogGuest:
    {
        m_d->userInfo.userType = HZAuthen::Guest;
        m_logStatus = GuestLogged;
        QJsonObject dataobj = obj.value("data").toObject();

        m_d->userInfo.uid = dataobj.value("uid").toVariant().toULongLong();
        m_d->userInfo.accessToken = dataobj.value("accessToken").toString();
        m_d->userInfo.expireDate    = dataobj.value("expireDate").toInt();
        m_cmdExe->setAccessToken(m_d->userInfo.accessToken);

        //重新登录后清除用户信息
        m_d->clearData();
    }
        break;
    case AuthenCmdExe::LogOut:
    {
        m_logStatus = UnLogged;

        m_d->userInfo.reset();
        m_cmdExe->setAccessToken("");
        m_d->clearData();
    }
        break;
    case AuthenCmdExe::RefreshToken:
    {
        QJsonObject dataobj = obj.value("data").toObject();
        m_d->userInfo.accessToken =dataobj.value("accessToken").toString();
        m_d->userInfo.expireDate    =dataobj.value("expireDate").toInt();
    }
        break;
    case AuthenCmdExe::GetPermission:
    {
        QJsonObject dataobj = obj.value("data").toObject();
        //保存权限...
        QJsonArray   funcArray=dataobj.value("funcLists").toArray();
        QJsonObject fobj;
        HZAuthen::PermissionItem item;
        for(int i=0;i<funcArray.size();i++){
            fobj = funcArray[i].toObject();
            item.id = (HZAuthen::UserPermissionType)fobj.value("funcId").toInt();
            item.startTime = fobj.value("startDate").toVariant().toUInt();
            item.endTime = fobj.value("endDate").toVariant().toUInt();
            item.permissionName = fobj.value("funcName").toString();
            item.permissionNameEn = fobj.value("funcNameEn").toString();

            m_d->permissions.insert(item.id,item);
        }
    }
        break;
    case AuthenCmdExe::GetUserProfile:
    {
        QJsonObject dataobj = obj.value("data").toObject();
        qDebug()<<"recv user profile:"<<dataobj;
        //保存用户资料
        readUserProfile(dataobj,m_d->userProfile);

        if(!m_d->userProfile.profileImgWeb.isEmpty()){
            //下载头像
            QString imageUrl = m_d->userProfile.profileImgWeb;
            QString icon_file = qApp->applicationDirPath()+"/cfg/icon/user_profile"+imageUrl.mid(imageUrl.lastIndexOf("."));
            if(downloadProfileIcon(imageUrl,icon_file))
                m_d->userProfile.icon = icon_file;
            else
                m_d->userProfile.icon = qApp->applicationDirPath()+"/cfg/icon/default_profile.png";
        }else//本地默认头像
            m_d->userProfile.icon = qApp->applicationDirPath()+"/cfg/icon/default_profile.png";
    }
        break;
        //这三种api会返回当前股票，故需要保存
    case AuthenCmdExe::GetZXStock:
    case AuthenCmdExe::UpdateZXStock:
    case AuthenCmdExe::AddZXStock:
    {
        QJsonObject dataobj = obj.value("data").toObject();
        //保存股票...
        m_d->zxStocks.clear();
        readZXStocks(dataobj,m_d->zxStocks);
    }
        break;
    case AuthenCmdExe::DelZXStock:
    {
        QJsonObject dataobj = obj.value("data").toObject();
        m_d->zxStocks.removeOne(m_d->currentStocks.value(id));
        m_d->currentStocks.remove(id);
    }
        break;
    default:
        break;
    }
    m_d->cmdHash.remove(id);
    emit commandFinished(id, HZAuthen::NoError);

    //  QString info = m_cmdMap.value(cmd);
    //    if(errcode == 0)
    //    {Log("LogInLOG","INFO","%s finished",info.toStdString().c_str());}
    //    else
    //    { Log("LogInLOG","ERROR","%s failed,error code = %d.",info.toStdString().c_str(),errcode);}
}

void  HZAuthenOpt::init()
{
    //创建登录参数
    {
        //创建登录参数结构体
        m_logParamObj.insert("loginUser","");
        m_logParamObj.insert("password","");

        //客户端版本信息
        QJsonObject    clientInfoObj;
        clientInfoObj.insert("client", "pc");
        clientInfoObj.insert("version", PRODUCT_VERSION);

        m_logParamObj.insert("clientVersion",clientInfoObj);

        //设备信息
        //    QJsonObject    deviceInfoObj;
        //    deviceInfoObj.insert("IP",ipAddress());
        //    deviceInfoObj.insert("IMEI1","");

        //    m_logParamObj.insert("deviceInfo",deviceInfoObj);
    }

    //命令信息
    m_cmdMap.insert(AuthenCmdExe::NoneCommand,"");
    m_cmdMap.insert(AuthenCmdExe::LogIn,"User LogIn");
    m_cmdMap.insert(AuthenCmdExe::ReLogIn,"User LogIn");
    m_cmdMap.insert(AuthenCmdExe::LogGuest,"Log Guest");
    m_cmdMap.insert(AuthenCmdExe::LogOut,"Log Out");
    m_cmdMap.insert(AuthenCmdExe::RefreshToken,"Refresh Tocken");
    m_cmdMap.insert(AuthenCmdExe::GetPermission,"Get Permission");

}

void HZAuthenOpt::readUserProfile(const QJsonObject& jobj,HZAuthen::UserProfile &profile)
{
    profile.userId = jobj.value("userId").toVariant().toLongLong();
    profile.createAt = jobj.value("createAt").toVariant().toLongLong();
    profile.screenName = jobj.value("createAt").toString();
    profile.loginUser = jobj.value("loginUser").toString();
    profile.mobileNum = jobj.value("mobileNum").toString();
    profile.name = jobj.value("name").toString();
    profile.qq = jobj.value("qq").toString();
    profile.webchat = jobj.value("webchat").toString();
    profile.address = jobj.value("address").toString();
    profile.level = jobj.value("level").toInt();
    profile.gender = jobj.value("gender").toString();
    profile.profileImgMobile = jobj.value("profileImgMobile").toString();
    profile.profileImgWeb = jobj.value("profileImgWeb").toString();
    profile.pca = jobj.value("pca").toString();
    profile.zipcode = jobj.value("zipcode").toString();
}

void HZAuthenOpt::readZXStocks(const QJsonObject& jobj,QVector<QString> &zxstocks)
{
    QJsonArray jarray =  jobj.value("watchlists").toArray();
    zxstocks.resize(jarray.size());
    for(int i=0;i<jarray.size();i++)
        zxstocks[i] = jarray.at(i).toObject().value("stockCode").toString();
}

bool HZAuthenOpt::downloadProfileIcon(const QString &url,const QString &iconFile)
{
    HttpClient  httpClient;
    QByteArray retBuffer;
    QNetworkReply::NetworkError error = QNetworkReply::NoError;
    httpClient.syncGet(url,retBuffer,error,1000);
    if(error != QNetworkReply::NoError)
        return false;
    QFile file(iconFile);
    file.open(QIODevice::WriteOnly);
    file.write(retBuffer);
    file.close();
    return true;
}

HZAuthen::AuthenError HZAuthenOpt::authenError(const int &errorCode)
{
    if(errorCode == 0)
        return HZAuthen::NoError;
    else if(0<errorCode && errorCode<=QNetworkReply::UnknownServerError)
        return HZAuthen::NetworkError;
    else if(10000 < errorCode && errorCode<20000)
        return HZAuthen::SystemError;
    else  if(20000 < errorCode && errorCode<30000)
        return HZAuthen::ServiceError;
    else
        return HZAuthen::UnknownError;
}

