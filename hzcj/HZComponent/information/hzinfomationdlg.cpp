#include "hzinfomationdlg.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QSpacerItem>
#include <QScrollBar>
#include <QWebEngineProfile>
#include "hzinfoadapterinterface.h"
#include <httpdata.h>
#include <QTimer>

const static QHash<int, QString> gTitles = {
    {HZInfomationDlg::Host_News,     QStringLiteral("热门资讯")},
    {HZInfomationDlg::Flash_News,    QStringLiteral("7*24快讯")},
    {HZInfomationDlg::News_Info,     QStringLiteral("相关新闻")},
    {HZInfomationDlg::Anncs_Info,    QStringLiteral("相关公告")},
    {HZInfomationDlg::Research_Report, QStringLiteral("相关研报")},
    {HZInfomationDlg::Major_Meeting,     QStringLiteral("重大会议")},
    {HZInfomationDlg::Financial_Report, QStringLiteral("财报发布")},
    {HZInfomationDlg::Plate_News,    QStringLiteral("相关新闻")},
    {HZInfomationDlg::Index_News,    QStringLiteral("相关新闻")},
};

HZInfomationDlg::HZInfomationDlg(QWidget *parent)
    :QDialog(parent)
{
    init();
}

HZInfomationDlg *HZInfomationDlg::instance()
{
    static HZInfomationDlg inst;
    return &inst;
}

HZInfomationDlg::~HZInfomationDlg()
{
    if(mSubject != nullptr){
        delete mSubject;
        mSubject = nullptr;
    }
}

void HZInfomationDlg::setVisiter(int type, const HZInfoAdapterInterface *adapter)
{
    if(mSubject != nullptr)
        delete mSubject;

    switch(type) {
    case Host_News:
        mSubject = new HZRMZXInfo(adapter);
        break;
    case Flash_News:
        mSubject = new HZ7Multiply24Info(adapter);
        break;
    case News_Info:
        mSubject = new HZNewsInfo(adapter);
        break;
    case Anncs_Info:
        mSubject = new HZAnncsInfo(adapter);
        break;
    case Research_Report:
        mSubject = new HZResearchReport(adapter);
        break;
    case Major_Meeting:break;
    case Financial_Report:break;
    case Plate_News:
        mSubject = new HZPlateNewsInfo(adapter);
        break;
    case Index_News:
        mSubject = new HZIndexNewsInfo(adapter);
        break;
    }

    mSubject->setVisitor(m_master);
    m_master->setAdapter(mSubject);

    QString disText="";
    const QVector<QString> stocks = mSubject->getCodes();
    if(!stocks.isEmpty()) disText= stocks.first();
    QString stockName = Httpdata::instance()->getCodeNameByCode(disText);
    if(!stockName.isEmpty())
        stockName = QString("-%1(%2)").arg(stockName).arg(disText.mid(2));
    m_title->setText(gTitles[type]+stockName);

    exec();
}

void HZInfomationDlg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton
            && m_titleFrame->rect().contains(e->pos())) {
        m_mousePoint = e->globalPos() - pos();
        e->accept();
        m_dragWindow = true;
    }
}

void HZInfomationDlg::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - m_mousePoint);
        e->accept();
    }
}

void HZInfomationDlg::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragWindow = false;
}

//bool HZInfomationDlg::eventFilter(QObject *w, QEvent *e)
//{
//    if(w == this) {
//        //qDebug() << Q_FUNC_INFO << e->type();
//        for(int i=0; i<10; ++i)
//            qApp->processEvents();
//    }

//    return QDialog::eventFilter(w,e);
//}

void HZInfomationDlg::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);
    for(int i=0; i<10; ++i)
        qApp->processEvents();
}

#if 0
void HZInfomationDlg::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
#endif

void HZInfomationDlg::init()
{
    //installEventFilter(this);
    setFixedSize(910,605);
    setObjectName("HZInfomationDlg");
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);

    m_dragWindow = false;
    m_titleFrame = new QFrame;
    m_master = new HZVListMaster;

    m_titleFrame->setObjectName("TitleFrame");
    m_titleFrame->setFixedHeight(30);
    m_master->setObjectName("Marster");

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);

    vLayout->addWidget(m_titleFrame);
    vLayout->addWidget(m_master);
    setLayout(vLayout);

    m_title = new QLabel;
    m_title->setObjectName("InfomationTitle");
    m_title->setAlignment(Qt::AlignCenter);

    m_closeBtn = new QPushButton;
    m_closeBtn->setFixedSize(19,19);
    m_closeBtn->setObjectName("InfomationClose");
    connect(m_closeBtn, &QPushButton::clicked, [=]{
        m_master->clearWeb();
        accept();
        hide();
        m_master->clearAll();
    });

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,12,0);
    hLayout->setSpacing(4);

    hLayout->addWidget(m_title);
    hLayout->addWidget(m_closeBtn);
    hLayout->setStretch(0,1);
    hLayout->setStretch(1,0);

    m_titleFrame->setLayout(hLayout);
    m_titleFrame->layout()->setContentsMargins(6,7,8,9);
}

///
/// \brief HZInfomationDlgEx::initSingleWebPage
///
HZInfomationDlgEx *HZInfomationDlgEx::instance()
{
    static HZInfomationDlgEx inst;
    return &inst;
}

HZInfomationDlgEx::~HZInfomationDlgEx()
{
    if(mWebView != nullptr)
        delete mWebView;
    mWebView = nullptr;
}

void HZInfomationDlgEx::setPageInfo(const QString &title, const QString &url)
{
    m_title->setText(title);
    mWebView->setUrl(url);
    exec();
}

void HZInfomationDlgEx::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton
            && m_titleFrame->rect().contains(e->pos())) {
        m_mousePoint = e->globalPos() - pos();
        e->accept();
        m_dragWindow = true;
    }
}

void HZInfomationDlgEx::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - m_mousePoint);
        e->accept();
    }
}

void HZInfomationDlgEx::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragWindow = false;
}

void HZInfomationDlgEx::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);
    for(int i=0; i<10; ++i)
        qApp->processEvents();
}

/*
void HZInfomationDlgEx::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
*/

HZInfomationDlgEx::HZInfomationDlgEx(QWidget *parent)
    :QDialog(parent)
{
    initSingleWebPage();
}

void HZInfomationDlgEx::initSingleWebPage()
{
    setFixedSize(910,605);
    setObjectName("HZInfomationDlg");
    setWindowFlags(Qt::FramelessWindowHint);

    m_dragWindow = false;
    m_titleFrame = new QFrame;

    mWebView = new HZInfoWebPageMarster(this);
    HZInfoWebPage *mWebPage = new HZInfoWebPage(PopWindow, QWebEngineProfile::defaultProfile());
    mWebView->setWebPage(mWebPage);

    m_titleFrame->setObjectName("TitleFrame");
    m_titleFrame->setFixedHeight(30);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);

    vLayout->addWidget(m_titleFrame);
    vLayout->addWidget(mWebView);
    setLayout(vLayout);

    m_title = new QLabel;
    m_title->setObjectName("InfomationTitle");
    m_title->setAlignment(Qt::AlignCenter);

    m_closeBtn = new QPushButton;
    m_closeBtn->setFixedSize(19,19);
    m_closeBtn->setObjectName("InfomationClose");
    connect(m_closeBtn, &QPushButton::clicked, [=]{
       hide();
    });

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,12,0);
    hLayout->setSpacing(4);

    hLayout->addWidget(m_title);
    hLayout->addWidget(m_closeBtn);
    hLayout->setStretch(0,1);
    hLayout->setStretch(1,0);

    m_titleFrame->setLayout(hLayout);
    m_titleFrame->layout()->setContentsMargins(6,7,8,9);
}
