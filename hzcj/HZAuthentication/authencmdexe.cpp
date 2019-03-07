#include "authencmdexe.h"

#include <QNetworkInterface>
#include <QHostAddress>
#include <QDateTime>

#include "hzopenssl.h"
#include "hzlog.h"

AuthenCmdExe::AuthenCmdExe(QObject *parent)
    :QObject(parent),reply(0)
{
    m_macAddress = getHostMacAddress();

    m_cmdHash = {{LogIn,"User logIn"}
                 ,{LogGuest,"Guest logIn"}
                 ,{GetUserProfile,"Get user profile"}
                 ,{GetZXStock,"Get zxstocks"}
                };
}

//执行命令，返回命令Id
int AuthenCmdExe::runCmd(Command cmd,const QVariant &var)
{
    CommandInfo cmdInfo(cmd,var);
    cmdInfo.id = getId();
    m_QueCmds.push_back(cmdInfo);
    runQueuedCmd();
    return  cmdInfo.id;
}

void  AuthenCmdExe::cancelCmd(int id)
{
    for(int i=0;i<m_QueCmds.size();i++){
        if(m_QueCmds[i].id == id)
            m_QueCmds[i].status = Canceled;
    }
}

void AuthenCmdExe::clearAllCms()
{
    m_QueCmds.clear();
    if(reply){
        delete reply;reply = NULL;
    }
}

void AuthenCmdExe::setAuthenApiUrl(const QString &auth_url,const QString &api_url)
{
    //用户登录相关
    m_urlHash.insert(LogIn,auth_url+"/api/client/login?");
    m_urlHash.insert(LogOut,auth_url+"/api/client/logout?");
    m_urlHash.insert(LogGuest,auth_url+"/api/client/guest?");
    m_urlHash.insert(RefreshToken,auth_url+"/api/client/refreshToken?");
    m_urlHash.insert(GetPermission,auth_url+"/api/client/permission?");

    //用户信息
    m_urlHash.insert(GetUserProfile,api_url+"/user/profile/me?");
    //自选股
    m_urlHash.insert(GetZXStock,api_url+"/stock/watchlist?");
    m_urlHash.insert(UpdateZXStock,api_url+"/stock/watchlist?");
    m_urlHash.insert(AddZXStock,api_url+"/stock/watchlist/new?");
    m_urlHash.insert(DelZXStock,api_url+"/stock/watchlist/del?");
}

void AuthenCmdExe::runQueuedCmd()
{
    if(m_QueCmds.isEmpty()){
        emit allFinished();
        return;
    }

    if(m_QueCmds.head().isRunning())
        return;

    while(m_QueCmds.head().status == Canceled){
        m_errorCode = -1;
        m_errString = "cmd is canceled";
        emit commandFinished(m_QueCmds.head().id);
        m_QueCmds.dequeue();
    }

    if(m_QueCmds.isEmpty()){
        emit allFinished();
        return;
    }
    m_QueCmds.head().status = Running;
    emit commandStarted( m_QueCmds.head().id);

    startCmd( m_QueCmds.head());
}

QString AuthenCmdExe::md5Authen()
{
    //用户登录相关签名
    QString urlSuffix;
    //时间戳
    QString  timestampStr  = QString::number(QDateTime::currentDateTime().toTime_t());
    //随机数
    QString retRandString = randString();
    //签名
    QString signatureStr = "nonce"+retRandString;
    signatureStr += "timestamp"+timestampStr;
    urlSuffix += "timestamp="+timestampStr;
    urlSuffix += "&nonce=" + retRandString;

    urlSuffix += "&signature="+HZOpenssl::HZMD5(signatureStr).toUpper();

    //其他接口暂时不需要签名
    return urlSuffix;
}

QString AuthenCmdExe::randString()
{
    QTime t0(0,0,0);
    QTime t = QTime::currentTime();
    int seed = t0.msecsTo(t);
    qsrand(seed);

    int rand= qrand();
    return (m_macAddress+QString::number(rand)).toLower();
}

QString AuthenCmdExe::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

void AuthenCmdExe::startCmd(const CommandInfo &cmdInfo)
{
    Command cmd = cmdInfo.cmd;

    if(cmd<ApiCmd)
    {
        QString urlString = m_urlHash.value(cmd) + md5Authen();
        //发送Get \ post
        QNetworkRequest request;
        request.setUrl(urlString);

        switch (cmd) {
        case LogIn:
        {
            QJsonDocument doc;
            QJsonObject obj = cmdInfo.data.toJsonObject();
            QString pwdStr = obj.value("password").toString();

            QString ret = HZOpenssl::HZMD5(pwdStr);
            QString retPwdStr;
            HZOpenssl::rsaPublicEncrypt(ret,retPwdStr);
            obj.insert("password",retPwdStr);
            doc.setObject(obj);
            //qDebug()<<"logIn url:"<<urlString;
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            //qDebug()<<doc.toJson();
            reply = m_nam.post(request,doc.toJson());
        }
            break;
        case LogOut:
        {
            request.setRawHeader("accessToken",m_accessToken.toLatin1());
            reply = m_nam.deleteResource(request);
            break;
        }
        case LogGuest:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            reply = m_nam.post(request,"");
        }
            break;
        case RefreshToken:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            request.setRawHeader("accessToken",m_accessToken.toLatin1());
            reply = m_nam.put(request,"");
        }
            break;
        case GetPermission:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            request.setRawHeader("accessToken",m_accessToken.toLatin1());
            reply = m_nam.get(request);
        }
            break;
        default:
            break;
        }
    }
    //-----------------api------------------------------
    else{
        QNetworkRequest request;
        QString urlString = m_urlHash.value(cmd);
        urlString +="accessToken="+m_accessToken;
        switch (cmd) {
        case GetUserProfile:
        {
            request.setUrl(urlString);
            reply = m_nam.get(request);
        }
            break;
        case GetZXStock:
        {
            qDebug()<<"url:"<<urlString;
            request.setUrl(urlString);
            reply = m_nam.get(request);
        }
            break;
        case UpdateZXStock:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            // request.setRawHeader("accessToken",m_accessToken.toLatin1());
            QStringList stockCodes = cmdInfo.data.toStringList();
            if(!stockCodes.isEmpty())
            {
                urlString+="&stockCode=";
                foreach (QString stock, stockCodes) {
                    urlString +=stock+",";
                }

                if(stockCodes.size()>0)
                    urlString.chop(1);
            }else
            {
                urlString+="&empty=true";
            }
            request.setUrl(urlString);
            qDebug()<<urlString;
            reply = m_nam.post(request,"");
        }
            break;
        case AddZXStock:
        case DelZXStock:
        {
            QNetworkRequest request;
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            request.setRawHeader("accessToken",m_accessToken.toLatin1());

            QString  stockCode = cmdInfo.data.toString();
            urlString+="&stockCode="+stockCode;
            request.setUrl(urlString);
            reply = m_nam.post(request,"");
        }
            break;
        default:
            break;
        }
    }
    connect(reply,&QNetworkReply::finished,this,&AuthenCmdExe::finishedCmd);
}

void AuthenCmdExe::finishedCmd()
{
    CommandInfo cmdInfo =  m_QueCmds.dequeue();
    m_errorCode = 0;
    m_errString.clear();
    //判断是否取消
    if(cmdInfo.status == Canceled){
        m_errorCode = -1;
        m_errString = "cmd is canceled";
        delete  reply; reply = NULL;
        emit commandFinished(cmdInfo.id);
        return;
    }

    //qDebug()<<"finishde cmd:"<<cmdInfo.cmd;
    QJsonObject obj;
    if(reply->error()){
        m_errorCode = reply->error();
        m_errString   = QStringLiteral("网络错误");
    }else{
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        //qDebug()<<doc.toJson();
        obj = doc.object();
        m_errorCode = obj.value("code").toInt();
        m_errString   = obj.value("message").toString();
    }
    auto it = m_cmdHash.find(cmdInfo.cmd);
    if(it != m_cmdHash.end()){
        if(m_errorCode == 0){
            Log("LogInLOG","INFO",QString("%1 sucess.").arg(it.value()).toStdString().c_str());
        }
        else{
            Log("LogInLOG","INFO",QString("%1 failed,errString:%2.").arg(it.value()).arg(m_errString).toStdString().c_str());
        }
    }
    delete  reply;reply=NULL;
    emit commandFinished(cmdInfo.id,obj);

    //继续执行缓存命令
    runQueuedCmd();
}

