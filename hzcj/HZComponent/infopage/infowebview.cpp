#include "webpopupwindow.h"
#include "infowebview.h"
#include "webpopupwindow.h"
#include "ui_certificateerrordialog.h"
#include "ui_passworddialog.h"
#include "infostyle.h"
#include <QAuthenticator>
#include <QMessageBox>
#include <QWebEnginePage>
#include <QWebEngineCertificateError>
#include <QWidget>
#include <QDialog>
#include <QUrl>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QNetworkReply>
#include <QTimer>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QJsonDocument>
#include <QJsonParseError>

#include <datacontroller.h>
#include "zxstockopt.h"

///
/// \brief HZInfoMarketSnapshotParse::HZInfoMarketSnapshotParse
/// \param msghandler
/// \param json
///
HZInfoMarketSnapshotParse::HZInfoMarketSnapshotParse(HZInfoMessageHander *msghandler, const QString &json)
    :m_msgTranslate(msghandler)
    ,m_content(json)
{

}

void HZInfoMarketSnapshotParse::parse()
{
    QJsonParseError err;
    QVector<STHotInfoDetailMarketInfo> datas;
    QJsonDocument doc = QJsonDocument::fromJson(m_content.toUtf8(), &err);
    if(err.error == QJsonParseError::NoError) {
        m_type = doc.toVariant().toMap()["type"].toInt();
        QVariantList lst = doc.toVariant().toMap()["value"].toList();
        foreach(const QVariant &record, lst){
            STHotInfoDetailMarketInfo data;
            data.stock_code = record.toString();
            datas.append(data);
        }
    }

#if 0 //web debug
    QJsonArray array;
    QJsonObject obj;
    obj["type"] = mType;
    foreach( const STHotInfoDetailMarketInfo &info,datas){
        QJsonObject obj;
        obj["stock_name"]   = QStringLiteral("华泰证券");
        obj["stock_code"]   = info.stock_code;
        obj["chg"]          = "0.5";
        obj["change"]       = "0.5";
        array.append(obj);
    }

    obj["value"] = array;

    //发送到web端
    QJsonDocument doc1(obj);
    qDebug() << Q_FUNC_INFO << "json doc1:" << doc1.toJson();
    mMsgTranslate->sendMessage(doc1.toJson());

#else
    //请求快照
    if(datas.isEmpty()) return;
    reqSnapShot(datas);
#endif
}

void HZInfoMarketSnapshotParse::reqSnapShot(const QVector<STHotInfoDetailMarketInfo> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i){
        stdlst.push_back(stocks[i].stock_code.toStdString());
    }

    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::Stock_Mask;
    auto fun = std::bind(&HZInfoMarketSnapshotParse::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void HZInfoMarketSnapshotParse::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(err != 0) {
        qDebug() << Q_FUNC_INFO << "error:" << err;
        return;
    }

    QJsonArray array;
    QJsonObject obj;
    obj["type"] = m_type;
    HZData::Stock *slStocls = (HZData::Stock *)data;
    if(slStocls) {
        for(int i=0; i<num; i++){
            QJsonObject obj;
            obj["stock_name"]   = QString::fromStdString(slStocls[i].stock_name);
            obj["stock_code"]   = QString::fromStdString(slStocls[i].stock_code);
            obj["chg"]          = slStocls[i].rate == INVALID_DOUBLE? "--" : QString::number(slStocls[i].rate, 'f', 2)+"%";
            obj["change"]       = slStocls[i].up_down_price == INVALID_DOUBLE? "--" : QString::number(slStocls[i].up_down_price, 'f', 2);
            obj["now"]          = slStocls[i].last_price  == INVALID_DOUBLE? "--" : QString::number(slStocls[i].last_price, 'f', 2);
            obj["custom"]       = ZXStockOpt::instance()->isHas(QString::fromStdString(slStocls[i].stock_code));// true;//false true is custom stock
            array.append(obj);
        }
    }

    //发送到web端
    QJsonDocument doc;
    obj["value"] = array;
    doc.setObject(obj);
    m_msgTranslate->sendMessage(doc.toJson());
}

///
/// \brief HZInfoMessageHander::HZInfoMessageHander
/// \param parent
///
HZInfoMessageHander::HZInfoMessageHander(QObject *parent)
    :QObject(parent)
{

}

void HZInfoMessageHander::sendMessage(const QString &text)
{
    s_text = text;
    emit sendText(s_text);
}

void HZInfoMessageHander::infoMessageCallback(const QString &json)
{
    if(json.isEmpty()) return;
    qDebug() << Q_FUNC_INFO << "json:" << json;
    IWebJsonParse *parseBase = nullptr;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);
    if(err.error == QJsonParseError::NoError) {
        int tp = doc.toVariant().toMap()["type"].toInt();
        switch(tp) {
        case 0: {
            parseBase = new HZInfoMarketSnapshotParse(this, json);
            if(parseBase != nullptr) parseBase->parse();
        }break;
        case 1:
        {
            //跳转股票详情页
            QVariantList tp = doc.toVariant().toMap()["value"].toList();
            if(tp.isEmpty()) return;
            QString stockcode(tp.first().toString());
            qDebug() << Q_FUNC_INFO << stockcode;
            gInfoStyle->signalStockClicked(stockcode);
        }break;
        default:{
             //...
        }
        }
    }
}


///
/// \brief WebPage::WebPage
/// \param profile
/// \param parent
///
HZInfoWebPage::HZInfoWebPage(int windowType, QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
    m_channel = new QWebChannel;
    m_channel->registerObject(QStringLiteral("content"), &m_hander);
    setWebChannel(m_channel);

    setBackgroundColor(windowType==PopWindow ? gInfoStyle->m_webPopWndBg : gInfoStyle->m_webInsertWndBg);
    connect(this, &QWebEnginePage::authenticationRequired, this, &HZInfoWebPage::handleAuthenticationRequired);
    connect(this, &QWebEnginePage::proxyAuthenticationRequired, this, &HZInfoWebPage::handleProxyAuthenticationRequired);
}

HZInfoWebPage::~HZInfoWebPage()
{
    if(m_channel!=nullptr) {
        delete m_channel;
        m_channel = nullptr;
    }
}

bool HZInfoWebPage::certificateError(const QWebEngineCertificateError &error)
{
    return true;
}

void HZInfoWebPage::handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth)
{
    QWidget *mainWindow = view()->window();
    QDialog dialog(mainWindow);
    dialog.setModal(true);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Ui::PasswordDialog passwordDialog;
    passwordDialog.setupUi(&dialog);

    passwordDialog.m_iconLabel->setText(QString());
    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

    QString introMessage(tr("Enter username and password for \"%1\" at %2")
                         .arg(auth->realm()).arg(requestUrl.toString().toHtmlEscaped()));
    passwordDialog.m_infoLabel->setText(introMessage);
    passwordDialog.m_infoLabel->setWordWrap(true);

    if (dialog.exec() == QDialog::Accepted) {
        auth->setUser(passwordDialog.m_userNameLineEdit->text());
        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
    } else {
        // Set authenticator null if dialog is cancelled
        *auth = QAuthenticator();
    }
}

void HZInfoWebPage::handleProxyAuthenticationRequired(const QUrl &, QAuthenticator *auth, const QString &proxyHost)
{
    QWidget *mainWindow = view()->window();
    QDialog dialog(mainWindow);
    dialog.setModal(true);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Ui::PasswordDialog passwordDialog;
    passwordDialog.setupUi(&dialog);

    passwordDialog.m_iconLabel->setText(QString());
    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

    QString introMessage = tr("Connect to proxy \"%1\" using:");
    introMessage = introMessage.arg(proxyHost.toHtmlEscaped());
    passwordDialog.m_infoLabel->setText(introMessage);
    passwordDialog.m_infoLabel->setWordWrap(true);

    if (dialog.exec() == QDialog::Accepted) {
        auth->setUser(passwordDialog.m_userNameLineEdit->text());
        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
    } else {
        // Set authenticator null if dialog is cancelled
        *auth = QAuthenticator();
    }
}



///
/// \brief HZInfoWebView::HZInfoWebView
/// \param parent
///
HZInfoWebView::HZInfoWebView(QWidget *parent)
    : QWebEngineView(parent)
{
    loadSetting();
}

void HZInfoWebView::setPage(HZInfoWebPage *page)
{
    QWebEngineView::setPage(page);
}

QWebEngineView *HZInfoWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    switch (type) {
    case QWebEnginePage::WebDialog: {
        WebPopupWindow *popup = new WebPopupWindow(page()->profile());
        return popup->view();
    }
    }
    return this;
}

void HZInfoWebView::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
}

void HZInfoWebView::loadSetting()
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


///
/// \brief HZInfoWebPageMarster::HZInfoWebPageMarster
/// \param parent
///
HZInfoWebPageMarster::HZInfoWebPageMarster(QWidget *parent)
    :QWidget(parent)
{
    m_webView = new HZInfoWebView;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(m_webView);
    setLayout(hLayout);
}

HZInfoWebPageMarster::~HZInfoWebPageMarster()
{
//    delete mWebView;
//    mWebView = nullptr;
}

void HZInfoWebPageMarster::setUrl(const QUrl &url)
{
    if(url.isValid()) {
        m_webView->setUrl(url);
    }
}

void HZInfoWebPageMarster::loadUrl(const QUrl &url)
{
    if(url.isValid()) {
        m_webView->load(url);
    }
}

void HZInfoWebPageMarster::setWebPage(HZInfoWebPage *page)
{
//    mWebPages.push_back(page);
    m_webView->setPage(page);
}

//void HZInfoWebPageMarster::clearAll()
//{
//    auto it = mWebPages.begin();
//    for(;it!=mWebPages.end(); ++it) {
//        delete *it;
//    }
//}

