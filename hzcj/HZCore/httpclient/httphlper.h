#ifndef HTTPFILEHLPER_H
#define HTTPFILEHLPER_H
#include <QMap>
#include <QTimer>
#include "httpclientdef.h"
class QReplyTimeout;

class HttpHelper:public QObject
{
    Q_OBJECT
public:
  HttpHelper(QNetworkAccessManager* pNetWork,QObject *parent=Q_NULLPTR);
  ~HttpHelper();
  //maxMsec 表示处理数据的最大时间 -1 表示不设置限制。如果处理超时会断掉连接
  void setParam(const QString& url,HttpClientCallBack funObj,int maxMsec = -1);
  virtual qint64 get();
  virtual qint64 post(const QByteArray &data);

  virtual void cancle();
  QNetworkReply* getRply();
  void setId(qint64 id);
  qint64 getId();
signals:
    void signalFinished(qint64 id);
protected slots:

protected:
    QNetworkReply     *reply_;
    QNetworkAccessManager* pNetWork_;
    HttpClientCallBack funObj_;
    qint64 currId_;
    QString url_;
    int maxMsec_;
    QReplyTimeout *pTimeout_;
};



class HttpDownLoad:public HttpHelper
{
    Q_OBJECT
public:
  HttpDownLoad(QNetworkAccessManager* pNetWork,QObject *parent=Q_NULLPTR);
  HttpDownLoad();
  qint64 get();
  qint64 post(const QByteArray &data);
  void cancle();
protected slots:
    void slotReplyReadyRead();
    void slotReplyFinished();
    void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    QByteArray buffer_;
};

class HttpUpLoad:public HttpHelper
{
    Q_OBJECT
public:
  HttpUpLoad(QNetworkAccessManager* pNetWork,QObject *parent=Q_NULLPTR);
  qint64 post(const QByteArray &data);
  void cancle();
signals:
protected slots:
//    void slotWriteFinished();
//    void slotUploadProgress(qint64 bytesReceived, qint64 bytesTotal);
//    void slotError(QNetworkReply::NetworkError code);
private:
};

#endif // HTTPFILEHLPER_H
