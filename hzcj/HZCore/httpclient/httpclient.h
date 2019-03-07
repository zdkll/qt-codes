#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
/********************************************************************
created:
author: min.cheng
purpose: http上传，下载封装
*********************************************************************/
#include <QObject>
#include "httpclientdef.h"

#include "hzcore_global.h"

class HttpHelper;
class HZCORESHARED_EXPORT HttpClient:public QObject
{
    Q_OBJECT
public:
    HttpClient(QObject *parent=Q_NULLPTR);

    ~HttpClient();

    void cancle(qint64 id);

    /**
     * @brief 异步请求数据
     * @param[in] url 请求地址
     * @param[in] funObj 回调函数
     * @param[in] maxMsec 最大处理时间，默认-1不设置，否则超时将发生错误
     * @retval 返回请求ID
     */
    qint64 get(const QString& url,HttpClientCallBack funObj,int maxMsec = -1);

    /**
     * @brief  同步请求数据
     * @param[in]  url 请求地址
     * @param[out] buffer 返回数据
     * @param[out] code用于判断返回的错误
     * @param[in]  msec 用于处理同步超时
     * @retval
     */
    void syncGet(const QString& url,QByteArray& buffer,QNetworkReply::NetworkError& code,int msec = 3000);

    /**
     * @brief 异步post数据
     * @param[in] url 请求地址
     * @param[in] params 请求的参数 key value
     * @param[in] funObj 回调函数
     * @param[in] maxMsec 最大处理时间，默认-1不设置，否则超时将发生错误
     * @retval 返回请求ID
     */
    qint64 post(const QString& url,const QMap<QString,QString>& params,HttpClientCallBack funObj,int maxMsec = -1);

    /**
     * @brief  同步post数据
     * @param[in]  url 请求地址
     * @param[in] params 请求的参数 key value
     * @param[out] buffer 返回数据
     * @param[out] code用于判断返回的错误
     * @param[in]  msec 用于处理同步超时
     * @retval
     */
    void syncPost(const QString& url,const QMap<QString,QString>& params,QByteArray& buffer,QNetworkReply::NetworkError& code,int msec = 3000);
protected slots:
    void slotsFinished(qint64 id);
private:
    void clearReqList();
    qint64 genId();
private:
    QMap<qint64,HttpHelper*> reqList_;
    QMap<qint64,HttpHelper*> finishMap_;
    QNetworkAccessManager* netWork_;
    static qint64 m_reqId;
};

#endif // HTTPCLIENT_H
