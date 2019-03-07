#include<QVBoxLayout>
#include <QEvent>
#include <QWebEngineSettings>
#include "webmainpage.h"
#include "basewebview.h"
#include "pubmsgdef.h"

QHash<QString,QString> WebMainPage::gUrlMap;

static void loadSetting()
{
    QWebEngineSettings *defaultSettings = QWebEngineSettings::globalSettings();
    defaultSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    defaultSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    defaultSettings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false);

    QWebEngineProfile *defaultProfile = QWebEngineProfile::defaultProfile();
    defaultProfile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    QString userAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36";
    defaultProfile->setHttpUserAgent(userAgent);
}

WebMainPage::WebMainPage(QWidget *parent, QString instName)
    :BasePage(parent,instName),m_cacheInstName(instName)
{
    loadSetting();
    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);

    m_manager = new QNetworkAccessManager;
    m_cacheUrl = getUrlByInstName(instName);

    QWebEngineProfile *defaultProfile = QWebEngineProfile::defaultProfile();
    m_webpage = new InternalWebPage(defaultProfile);
    webView = new BaseWebView(QUrl(m_cacheUrl)/*, this*/);
    webView->pWebView_->setPage(m_webpage);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(webView);
    setLayout(pLayout);

    qDebug() << Q_FUNC_INFO
             << "cache Url:" << m_cacheUrl
             << "inst name:" << m_cacheInstName
             << "token:" << HZAuthenOpt::instance()->logUserInfo().accessToken.toUtf8();

    m_webpage->setInstanceName(instName);

    if(instName == "product_page")
    {
        QNetworkRequest request;
        request.setUrl(QUrl(m_cacheUrl));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html");
        request.setRawHeader("token", HZAuthenOpt::instance()->logUserInfo().accessToken.toUtf8());
        QNetworkReply *reply = m_manager->get(request);
        connect(reply,&QNetworkReply::finished,[=]{
            m_webpage->setProxyNetMananger(m_manager);
            m_webpage->setHtml(reply->readAll());
            reply->deleteLater();
        });
    }
}

void WebMainPage::initialization()
{
    gUrlMap["finance_news_page"] = "http://www.shhzcj.com/info_cjtt";
    gUrlMap["live_video_page"] = "https://live.shhzcj.com/?pure=true";
    gUrlMap["websit_page"] = "http://www.shhzcj.com/";
    gUrlMap["activity_center_page"] = "https://cdn.shhzcj.com/html/hdzx/dxjj/index.html";
    gUrlMap["strategy_center_page"] = "https://pc.shhzcj.com/across";
    gUrlMap["short_msg_nuggets_page"] = "https://pc.shhzcj.com/dxjj/open";
    gUrlMap["go_buy_page"] = "https://pc.shhzcj.com/group/order/18/payment";
    gUrlMap["info_center_page"] = "http://www.shhzcj.com/info_cjtt";
    gUrlMap["product_page"] = Hzcfg::getInstance()->serverInfo().webproducturl;//"https://prepc.shhzcj.com/user";
}

void WebMainPage::afterActive()
{
    qDebug() << Q_FUNC_INFO << "cache Url:" << m_cacheUrl << "inst name:" << m_cacheInstName;

    if(m_cacheInstName == "product_page")
    {
        QNetworkRequest request;
        request.setUrl(QUrl(m_cacheUrl));
        request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html");
        request.setRawHeader("token", HZAuthenOpt::instance()->logUserInfo().accessToken.toUtf8());
        QNetworkReply *reply = m_manager->get(request);
        connect(reply,&QNetworkReply::finished,[=]{
            m_webpage->setProxyNetMananger(m_manager);
            m_webpage->setHtml(reply->readAll());
            reply->deleteLater();
        });
    }
    else
    {
        webView->pWebView_->load(m_cacheUrl);
    }
}

void WebMainPage::afterDeactive()
{

}


QString WebMainPage::getUrlByInstName(const QString &instName)
{
    QString url = "";
    auto it = gUrlMap.find(instName);
    if(it != gUrlMap.end())
        url = it.value();
    return url;
}
