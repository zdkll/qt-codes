#ifndef HTTPCLIENTDEF_H
#define HTTPCLIENTDEF_H
#include <functional>
#include <QNetworkReply>
//数据请求回调
struct HttpClientCallBack
{
    std::function<void(qint64,const QByteArray&)> finishedFun = nullptr;
    std::function<void(qint64,qint64,qint64)> progressFun = nullptr;
    std::function<void(qint64,QNetworkReply::NetworkError)> errorFun = nullptr;
};


#endif // HTTPCLIENTDEF_H
