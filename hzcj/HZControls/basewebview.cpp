#include <QApplication>
#include <QDesktopWidget>
#include <QWebEngineProfile>
#include <QWebEnginePage>

#include "basewebview.h"
#include "titlebar.h"
#include <QWebEngineProfile>
#include <QWebEngineSettings>

//InternalWebView--------
InternalWebView::InternalWebView(QUrl url, QWidget *parent )
    :QWebEngineView(parent),url_(url)
{
    this->setUrl(url);
    qDebug() << "InternalWebView(QUrl url, QWidget *parent )";

    QWebEngineSettings *defaultSettings = QWebEngineSettings::globalSettings();
    defaultSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    defaultSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    defaultSettings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    defaultSettings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false);

    QWebEngineProfile *defaultProfile = QWebEngineProfile::defaultProfile();
    defaultProfile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    QString userAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36";
    defaultProfile->setHttpUserAgent(userAgent);

//    QNetworkCookie cookieInfo("PASS_ID", );   //name=value
//            QWebEngineCookieStore *cookieStore = page()->profile()->cookieStore();
//    cookieStore->setCookie(cookieInfo, urlOrigin); //此处添加urlorigin
//    page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
}

InternalWebView::InternalWebView(QWidget *parent):QWebEngineView(parent)
{
    qDebug() << "InternalWebView(QWidget *parent):QWebEngineView(parent)";
}

InternalWebView::~InternalWebView()
{
    //qDebug() << "~InternalWebView()";
}

//QWebEngineView *InternalWebView::createWindow(QWebEnginePage::WebWindowType type)
//{
//    Q_UNUSED(type)
//    InternalWebView* pView = new InternalWebView();
//    PopWebView *pp = new PopWebView(pView);
//    pp->show();

//    BaseView::instance().addWindow(pp);

//    return pp->webView();
//}

//PopWebView----------
PopWebView::PopWebView(InternalWebView* view,QWidget* parent,QString instName)
    :PopupWindow(parent,instName)
{
    pWebView_ = view;
    this->setAttribute(Qt::WA_DeleteOnClose);
    m_strTile = instName;
    setWidget(view);
    connect(view, SIGNAL(titleChanged(QString)), m_pTitleBar, SLOT(setWinTitle(QString)));
}

PopWebView::~PopWebView()
{
    BaseView::instance().remove(this);
    qDebug()<<"PopWebView::~PopWebView()";
}

InternalWebView *PopWebView::webView()
{
    return pWebView_;
}

void PopWebView::resizeEvent(QResizeEvent *event)
{
    if(pWebView_)
        pWebView_->resize(this->size());
}

//BaseWebView--------------------------------------------------
BaseWebView::BaseWebView(QUrl &url, BaseWidget *parent, QString instName)
    :BaseWidget(parent,instName)
{
    pWebView_ = new InternalWebView(url,this);
    initLayout();

    //qDebug()<<"create base widget";
}
void BaseWebView::initLayout()
{
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(pWebView_);
    setLayout(pLayout);
}

BaseWebView::~BaseWebView()
{
}



//BaseView------------------------------------------
BaseView::~BaseView()
{
    qDeleteAll(m_windows);
    m_windows.clear();
}


BaseView &BaseView::instance()
{
    static BaseView baseView;
    return baseView;
}

QVector<PopupWindow *> BaseView::windows()
{
    return m_windows;
}

void BaseView::addWindow(PopupWindow *mainWindow)
{
    if (m_windows.contains(mainWindow))
        return;
    m_windows.prepend(mainWindow);
    qDebug() << "BaseView::addWindow";
}

void BaseView::remove(PopupWindow *window)
{
    int pos = m_windows.indexOf(window);
    if(pos != -1)
    {
        m_windows.erase(m_windows.begin()+pos);
        qDebug() << "BaseView::remove";
    }
}

bool BaseView::eventFilter(QObject *watched, QEvent *event){
    switch(event->type()){
    case QEvent::Close:
        foreach (PopupWindow *ppWg, this->windows()) {
            ppWg->close();
            qDebug() << "BaseView::eventFilter";
        }
        break;
    case QEvent::Hide:
        foreach (PopupWindow *ppWg, this->windows()) {
            ppWg->hide();
        }
        break;
    case QEvent::Show:
        foreach (PopupWindow *ppWg, this->windows()) {
            ppWg->show();
        }
        break;
    }

    return QObject::eventFilter(watched,event);
}
