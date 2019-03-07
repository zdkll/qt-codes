#ifndef WebMainPage_H
#define WebMainPage_H
/********************************************************************
created:
author: min.cheng
purpose: 页面的WEB相关内容
*********************************************************************/

#include "hzcomponent_global.h"
#include  "basepage.h"
#include <basewebview.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebEngineProfile>

#include <QNetworkCookieJar>
#include <QNetworkCookie>

#include <QNetworkReply>
#include <QWebEngineProfile>
#include <hzcfg.h>
#include <hzauthenopt.h>

class BaseWebView;

class InternalWebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    InternalWebPage(QWebEngineProfile *profile, QObject *parent = nullptr)
        :QWebEnginePage(profile,parent)
    {

    }

    ~InternalWebPage()
    {

    }

    void setInstanceName(const QString &instname)
    {
        m_instName = instname;
    }

    void setProxyNetMananger(QNetworkAccessManager *mananger)
    {
        m_manager = mananger;
    }

    void setCookies(const QList<QNetworkCookie> &cookies)
    {
        this->cookies = cookies;
    }
protected:
    bool certificateError(const QWebEngineCertificateError &error) override
    {
        return true;
    }

    virtual bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
    {
        qDebug() << Q_FUNC_INFO << "url:" << url << "type:" << type << "ismainframe:" << isMainFrame;
        if(m_instName == "product_page" && type == NavigationTypeLinkClicked)
        {
            QNetworkRequest request;
            request.setUrl(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html");
            request.setRawHeader("token", HZAuthenOpt::instance()->logUserInfo().accessToken.toUtf8());
            m_manager->cookieJar()->setCookiesFromUrl(cookies,url);
            QNetworkReply *m_reply = m_manager->get(request);
            connect(m_reply,&QNetworkReply::finished,[=]{
                setHtml(m_reply->readAll());
                m_reply->deleteLater();
            });
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                      this, SLOT(slotError(QNetworkReply::NetworkError)));

            return true;
        }

        return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }


public slots:
    void slotError(QNetworkReply::NetworkError err)
    {
        qDebug() <<Q_FUNC_INFO << err;
    }

private:
    QList<QNetworkCookie> cookies;
    QNetworkAccessManager *m_manager;
//    QNetworkReply *m_reply;
    QString m_instName;
};

class HZCOMPONENTSHARED_EXPORT WebMainPage : public BasePage
{
public:
    explicit WebMainPage(QWidget *parent = 0,QString m_cacheInstName="");

    static void initialization();

    ~WebMainPage(){}

    virtual void afterActive();

    virtual void afterDeactive();

private:
    InternalWebPage *m_webpage;
    BaseWebView *webView;
    QString getUrlByInstName(const QString& m_cacheInstName);
    QString m_cacheUrl, m_cacheInstName;
    static QHash<QString,QString> gUrlMap;
    QNetworkAccessManager *m_manager;
};

#endif // WebMainPage_H
