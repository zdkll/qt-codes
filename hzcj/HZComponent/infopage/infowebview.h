/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFOWEBVIEW_H
#define INFOWEBVIEW_H

#include <QIcon>
#include <QWebEngineView>
#include <QWebChannel>
#include "updatinterface.h"
#include "infowebview.h"
#include "infodef.h"
#include <datamodeldefs/stockinfodef.h>
#include <datamodeldef.h>


struct STHotInfoDetailMarketInfo
{
    QString stock_name;
    QString stock_code;
    QString change;
    QString chg;
};

class HZInfoMessageHander;

///
/// \brief The HZInfoMarketSnapshotParse class Web行情快照获取封装类
///
class HZInfoMarketSnapshotParse : public IWebJsonParse
{
public:
    HZInfoMarketSnapshotParse(HZInfoMessageHander *msghandler, const QString &json);

    /**
     * @brief parse
     */
    void parse();

private:
    HZInfoMessageHander *m_msgTranslate;
    QString m_content;
    int m_type;

    //行情快照请求
    void reqSnapShot(const QVector<STHotInfoDetailMarketInfo> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err);
};


///
/// \brief The HZInfoMessageHander class
///
class HZInfoMessageHander : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY sendText)

public:
    HZInfoMessageHander(QObject *parent=nullptr);

    /**
     * @brief sendMessage
     * @param text
     */
    void sendMessage(const QString &text);

public slots:
    void infoMessageCallback(const QString &json);

signals:
    void sendText(const QString &text);

private:
    QString s_text;
    QString recieve_text;
};

///
/// \brief The HZInfoWebPage class
///
class HZInfoWebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    HZInfoWebPage(int windowType, QWebEngineProfile *profile, QObject *parent = nullptr);
    ~HZInfoWebPage();

protected:
    bool certificateError(const QWebEngineCertificateError &error) override;

private slots:
    void handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth);
    void handleProxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost);

private:
    QWebChannel *m_channel{nullptr};
    HZInfoMessageHander m_hander;
};

///
/// \brief The HZInfoWebView class
///
class HZInfoWebView : public QWebEngineView
{
    Q_OBJECT

public:
    HZInfoWebView(QWidget *parent = nullptr);

    /**
     * @brief setPage 頁面設置
     * @param page
     */
    void setPage(HZInfoWebPage *page);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

private:
    void loadSetting();
};

///
/// \brief The HZInfoWebPageMarster class
///
class HZInfoWebPageMarster : public QWidget
{
    Q_OBJECT

public:
    explicit HZInfoWebPageMarster(QWidget *parent = 0);
    ~HZInfoWebPageMarster();

    /**
     * @brief setUrl 设置页面链接
     * @param url
     */
    void setUrl(const QUrl &url);

    /**
     * @brief loadUrl 设置页面链接
     * @param url
     */
    void loadUrl(const QUrl &url);

    /**
     * @brief setWebPage  设置页面对象
     * @param page
     */
    void setWebPage(HZInfoWebPage *page);
//    void clearAll();

    HZInfoWebView *m_webView{nullptr};
//    QList<HZInfoWebPage *> mWebPages;
};

#endif //INFOWEBVIEW
