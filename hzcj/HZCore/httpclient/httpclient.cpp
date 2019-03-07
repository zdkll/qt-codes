#include "httpclient.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include "httphlper.h"
qint64 HttpClient::m_reqId = 0;

HttpClient::HttpClient(QObject *parent):QObject(parent)
{
    netWork_ = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{
    for(auto it = finishMap_.begin();it != finishMap_.end();it++)
    {
        delete it.value();
    }
    finishMap_.clear();
    clearReqList();
}

qint64 HttpClient::get(const QString &url, HttpClientCallBack funObj,int maxMsec)
{
   HttpDownLoad* pDownObj = new HttpDownLoad(netWork_);
   pDownObj->setId(genId());
   pDownObj->setParam(url,funObj,maxMsec);
   connect(pDownObj,SIGNAL(signalFinished(qint64)),this,SLOT(slotsFinished(qint64)));
   reqList_[pDownObj->getId()] = pDownObj;
   return pDownObj->get();
}

void HttpClient::cancle(qint64 id)
{
    HttpHelper* pHelper = nullptr;
    auto it = reqList_.find(id);
    if(it !=  reqList_.end())
    {
        pHelper = it.value();
        reqList_.erase(it);
        pHelper->cancle();
        delete pHelper;
    }
}

void HttpClient::syncGet(const QString &url, QByteArray &buffer, QNetworkReply::NetworkError &code,int msec)
{
    code = QNetworkReply::NoError;
    QTimer timer;
    timer.setInterval(msec);  // 设置超时时间 30 秒
    timer.setSingleShot(true);  // 单次触发

    // 请求 Qt 官网
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    //request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    QNetworkReply *pReply = netWork_->get(request);

    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();  // 启动事件循环
    if (timer.isActive()) {  // 处理响应
        timer.stop();
        if (pReply->error() != QNetworkReply::NoError) {
            code = pReply->error();
            // 错误处理
            qDebug() << "Error String : " << pReply->errorString();
        } else {
            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();
            if(nStatusCode == 200)
            {
               buffer = pReply->readAll();
            }else
            {
                code = QNetworkReply::UnknownServerError;
            }
            qDebug() << "Status Code : " << nStatusCode;
        }
    } else {  // 处理超时
        disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        pReply->abort();
        code = QNetworkReply::TimeoutError;
        qDebug() << "Timeout";
    }
    pReply->deleteLater();
}

qint64 HttpClient::post(const QString &url, const QMap<QString, QString> &params, HttpClientCallBack funObj, int maxMsec)
{
    QByteArray dataArray;
    for(auto it = params.begin();it != params.end();it++)
    {
        QString str = it.key()+"="+it.value();
        if(it + 1 != params.end())
        {
            str += "&";
        }
        dataArray.append(str);
    }
    HttpDownLoad* pDownObj = new HttpDownLoad(netWork_);
    pDownObj->setId(genId());
    pDownObj->setParam(url,funObj,maxMsec);
    connect(pDownObj,SIGNAL(signalFinished(qint64)),this,SLOT(slotsFinished(qint64)));
    reqList_[pDownObj->getId()] = pDownObj;
    return pDownObj->post(dataArray);
}

void HttpClient::syncPost(const QString &url, const QMap<QString, QString> &params, QByteArray &buffer, QNetworkReply::NetworkError &code, int msec)
{
    QJsonDocument doc;
    QJsonObject obj;
    for(auto it = params.begin();it != params.end();it++)
    {
        obj.insert(it.key(),it.value());
    }
    doc.setObject(obj);

    code = QNetworkReply::NoError;
    QTimer timer;
    timer.setInterval(msec);  // 设置超时时间 30 秒
    timer.setSingleShot(true);  // 单次触发

    // 请求 Qt 官网
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    //request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
    QNetworkReply *pReply = netWork_->post(request,doc.toJson());

    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();  // 启动事件循环
    if (timer.isActive()) {  // 处理响应
        timer.stop();
        if (pReply->error() != QNetworkReply::NoError) {
            code = pReply->error();
            // 错误处理
            qDebug() << "Error String : " << pReply->errorString();
        } else {
            QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();
            if(nStatusCode == 200)
            {
               buffer = pReply->readAll();
            }else
            {
                code = QNetworkReply::UnknownServerError;
            }
            qDebug() << "Status Code : " << nStatusCode;
        }
    } else {  // 处理超时
        disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        pReply->abort();
        code = QNetworkReply::TimeoutError;
        qDebug() << "Timeout";
    }
    pReply->deleteLater();
}

void HttpClient::slotsFinished(qint64 id)
{
    //删除上次数据
    for(auto it = finishMap_.begin();it != finishMap_.end();it++)
    {
        delete it.value();
    }
    finishMap_.clear();

    auto it = reqList_.find(id);
    if(it !=  reqList_.end()) //存放在完成对列，下次再删除
    {
        it.value()->cancle();
        finishMap_[id] = it.value();
        reqList_.erase(it);
    }

}

void HttpClient::clearReqList()
{
    for(auto it=  reqList_.begin();it != reqList_.end();it++)
    {
         it.value()->cancle();
        delete it.value();
    }
    reqList_.clear();
}

qint64 HttpClient::genId()
{
    if(m_reqId == LLONG_MAX)
        m_reqId = 0;
    return m_reqId++;
}
