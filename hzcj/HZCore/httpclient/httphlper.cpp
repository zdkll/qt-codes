#include "httphlper.h"
#include <QSslConfiguration>
#include "replytimeout.h"


HttpHelper::HttpHelper(QNetworkAccessManager *pNetWork, QObject *parent)
    :QObject(parent),
      pNetWork_(pNetWork)
{
//    pNetWork_->setConfiguration();
    reply_ = nullptr;
    currId_ = -1;
    maxMsec_= -1;
    pTimeout_ = nullptr;
    qDebug() << "HttpHelper";
}

HttpHelper::~HttpHelper()
{
    if( pTimeout_)
         delete  pTimeout_;
     pTimeout_ = nullptr;
     if(reply_)
         delete reply_;
     reply_ = nullptr;
     qDebug() << "~HttpHelper";
}

void HttpHelper::setParam(const QString &url, HttpClientCallBack funObj,int maxMsec)
{
    qDebug() << "HttpHelper::setParam url: " << url;
    if(url.isEmpty())
    {
        qDebug() << "HttpHelper::setParam is empty";
    }
    funObj_ = funObj;
    url_ = url;
    maxMsec_ = maxMsec;
}

qint64 HttpHelper::get()
{
    return currId_;
}

qint64 HttpHelper::post(const QByteArray &data)
{
    return currId_;
}


void HttpHelper::cancle()
{

}

QNetworkReply *HttpHelper::getRply()
{
    return reply_;
}

void HttpHelper::setId(qint64 id)
{
    currId_ = id;
}

qint64 HttpHelper::getId()
{
    return currId_;
}

HttpDownLoad::HttpDownLoad(QNetworkAccessManager* pNetWork,QObject *parent):HttpHelper(pNetWork,parent)
{

}

qint64 HttpDownLoad::get()
{
    qint64 id = -1;
    QNetworkRequest request;
    request.setUrl(url_);
    QSslConfiguration sslConfig;
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    reply_ = pNetWork_->get(request);
    if(reply_->error() == QNetworkReply::NoError)
    {
        connect(reply_,&QNetworkReply::readyRead,this,&HttpDownLoad::slotReplyReadyRead);
        connect(reply_,&QNetworkReply::finished,this,&HttpDownLoad::slotReplyFinished);
        connect(reply_,&QNetworkReply::downloadProgress,this,&HttpDownLoad::slotDownloadProgress);
        id = currId_;
    }
    if(maxMsec_ != -1)
        pTimeout_ = new QReplyTimeout(reply_,maxMsec_);
    return id;
}

qint64 HttpDownLoad::post(const QByteArray &data)
{
    qint64 id = -1;
    QNetworkRequest request;
    request.setUrl(url_);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QSslConfiguration sslConfig;
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    reply_ = pNetWork_->post(request,data);
    if(reply_->error() == QNetworkReply::NoError)
    {
        connect(reply_,&QNetworkReply::readyRead,this,&HttpDownLoad::slotReplyReadyRead);
        connect(reply_,&QNetworkReply::finished,this,&HttpDownLoad::slotReplyFinished);
        connect(reply_,&QNetworkReply::downloadProgress,this,&HttpDownLoad::slotDownloadProgress);
        id = currId_;
    }
    if(maxMsec_ != -1)
        pTimeout_ = new QReplyTimeout(reply_,maxMsec_);
    return id;
}

void HttpDownLoad::cancle()
{
    disconnect(reply_,&QNetworkReply::readyRead,this,&HttpDownLoad::slotReplyReadyRead);
    disconnect(reply_,&QNetworkReply::finished,this,&HttpDownLoad::slotReplyFinished);
    disconnect(reply_,&QNetworkReply::downloadProgress,this,&HttpDownLoad::slotDownloadProgress);
}

void HttpDownLoad::slotReplyReadyRead()
{
    buffer_.append(reply_->readAll());
}

void HttpDownLoad::slotReplyFinished()
{
    if(reply_->error() != QNetworkReply::NoError)
    {
        if(funObj_.errorFun)
            funObj_.errorFun(currId_,reply_->error());
    }
    else if(funObj_.finishedFun)
        funObj_.finishedFun(currId_,buffer_);
    emit signalFinished(currId_);
}

void HttpDownLoad::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(funObj_.progressFun && bytesReceived != 0 && bytesTotal != 0)
        funObj_.progressFun(currId_,bytesReceived,bytesTotal);
}

HttpUpLoad::HttpUpLoad(QNetworkAccessManager *pNetWork, QObject *parent):HttpHelper(pNetWork,parent)
{

}

qint64 HttpUpLoad::post(const QByteArray &data)
{
    return -1;
}

void HttpUpLoad::cancle()
{

}
