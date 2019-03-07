#include "HZMainWnd.h"
#include <QGridLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "micro.h"
#include "ThreadPool.hpp"
#include "hzlog.h"
#include "maintitlebar.h"
#include "mainnavigatorbar.h"
#include "mainsidebar.h"
#include "mainstatusbar.h"
#include "hzpagehelper.h"
#include "datacontroller.h"
#include "httpdata.h"

#include "homepage.h"
#include "minimornitorwnd.h"
#include "shortcut.h"
#include "hzrelogindlg.h"
#include "timedealdlg.h"
#include "basewebview.h"

HZMainWnd::HZMainWnd(FrameWidget *parent,QString instName) :
    FrameLessWidget(parent,instName)
  ,m_pageHelper(0)
  ,mShortCut(new ShortCutManager(this))
  ,mReloginDlg(new HZReloginDlg)
{
    m_pHelper->setTitleHeight(MAINTITLEBAR_HEIGHT);
    attachMessageBus(true);  //附加到消息中心
    createWg();
    connect(DataController::instance(),SIGNAL(signalNetIoReady(bool)),this,SLOT(slotsNetIoReady(bool)));
}

HZMainWnd::~HZMainWnd()
{
    //ThreadPool::GetInstance()->UnInit();
    delete mReloginDlg;
}


void HZMainWnd::createWg()
{
    HZ_AUTO_ELAPSED_TIMER;

    setObjectName("HZMainWnd");

    //关闭显示等事件传递到网页窗口管理器
    this->installEventFilter(&BaseView::instance());
    //初始化一个线程池
    ThreadPool::GetInstance()->Init();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(1);


    //sidebar-----
    m_sideBar = new MainSideBar(this,"mainsidebar");
    m_sideBar->setObjectName("mainsidebar");
    m_sideBar->setOwner(this);
    mainLayout->addWidget(m_sideBar, 0, 0, 3, 1);


    //titlebar------
    m_pTitleBar = new MainTitleBar(this,"maintitlebar");
    m_pTitleBar->setFixedHeight(MAINTITLEBAR_HEIGHT);
    m_pTitleBar->setObjectName("maintitlebar");
    m_pTitleBar->setOwner(this);
    mainLayout->addWidget(m_pTitleBar, 0, 1, 1, 1);

    installEventFilter(m_pTitleBar);

    //navibar
    m_navigatorBar = new MainNavigatorBar(this,"mainnavigatorbar");
    m_navigatorBar->setOwner(this);
    m_navigatorBar->setObjectName("mainnavigatorbar");
    mainLayout->addWidget(m_navigatorBar, 1, 1, 1, 1);

    //页面管理
    m_pageHelper = new HzPageHelper(this,"HzPageHelper");
    m_pageHelper->setOwner(this);
    mainLayout->addWidget(m_pageHelper, 2, 1);

    //Status bar
    m_statusBar = new MainStatusBar(this,"mainstatusbar");
    m_statusBar->setOwner(this);

    mainLayout->addWidget(m_statusBar, 3, 0, 1, 2);

    setLayout(mainLayout);
    m_pHelper->setTitleHeight(m_pTitleBar->height());

    //大小，位置
    QRect screenRt = QApplication::desktop()->screenGeometry();
    this->resize(screenRt.width()*3/4,screenRt.height()*3/4);

    this->move((screenRt.width() - width())/2,(screenRt.height() - height())/2);
    this->setMaximumSize(screenRt.width(), screenRt.height());

    //初始化第一页面
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(HZ_PAGE_ID,QString::number(home_page));
    setDefaultMsg(ROOT_FRAME,msg);

    //默认创建几个常用页面
    QVector<int> ids;
    ids.push_back(home_page);
    ids.push_back(stock_detail_page);
    m_pageHelper->defaultCreaePage(ids);

    //设置当前页
    m_pageHelper->setCurrPageId(home_page,DetailPageParam());
}


void HZMainWnd::slotsNetIoReady(bool ready)
{
    qDebug() << "HZMainWnd::slotsNetIoReady" << "ready: " << ready;
    m_statusBar->setConnect(ready);
    if(ready)
    {
        m_pageHelper->onNetIoReady(ready);
        m_statusBar->onNetIoReady(ready);
        m_pTitleBar->onNetIoReady(ready);
        m_navigatorBar->onNetIoReady(ready);
        //广播通知
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,HZ_NETIO_NOTIFY);
        msg.emplace(HZ_NETIO_STATUS,QString::number(1));
        broadcastMessage(msg);
    }
}

void HZMainWnd::onMessage(const QString &senderPath, const MessageParams &params)
{
    auto it = params.find(HZ_CMD);
    if(it != params.end())
    {
        if(it->second == ROOT_SWITCH_PAGE)
        {
            proceSwitchPageCmd(senderPath,params);
        }else if(it->second == SHOW_ALL_TIME_DEAL_DLG || it->second == HIDE_ALL_TIME_DEAL_DLG)
        {
            QString code="";
            auto stockIter = params.find(STOCK_ID);
            if(stockIter != params.end())
            {
                code = stockIter->second;
            }
            proceTickDlg(code,it->second == SHOW_ALL_TIME_DEAL_DLG?true:false);
        }
    }
}

bool HZMainWnd::onProcessGetMessage(const QString &path, MessageParams &params, Any &outData)
{
    auto it = params.find(HZ_GET);
    if(it != params.end())
    {
        if(it->second == HZ_PAGE_ID)
        {
            int id = m_pageHelper->getCurrPageId();
            if(id != InvalidPage)
            {
                outData = id;
            }
        }
    }
    return true;
}

void HZMainWnd::proceSwitchPageCmd(const QString &senderPath,const MessageParams &params)
{
    auto it = params.find(HZ_PAGE_ID);
    if(it != params.end())
    {
        DetailPageParam info;
        info.cmd = params;
        //股票代码
        auto stockIter = params.find(STOCK_ID);
        if(stockIter != params.end())
        {
            info.stock = stockIter->second;
        }

        //周期
        auto periodIter = params.find(PERIOD_TYPE);
        if(periodIter != params.end())
        {
            info.period = (DetialKlineMinType)periodIter->second.toInt();
        }

        //子窗口ID
        auto childIter = params.find(CHILD_WND_ID);
        if(childIter != params.end())
        {
            info.childWndId  = childIter->second.toInt();
        }

        //子窗口ID
        auto thirdChildIter = params.find(THIRD_CHILD_WND_ID);
        if(thirdChildIter != params.end())
        {
            info.thirdChildWndId  = thirdChildIter->second.toInt();
        }

        auto fromChildIter = params.find(FROM_CHILD_WND_ID);
        if(fromChildIter != params.end())
        {
            info.fromChildId  = fromChildIter->second.toInt();
        }

        auto timeIter = params.find(TIME_NUMBER);
        if(timeIter != params.end())
        {
            info.timeNumber  = timeIter->second.toInt();
        }

        auto winnersIter = params.find(WINNERS_LIST_ID);
        if(winnersIter != params.end())
        {
            info.winnersListId = winnersIter->second;
        }

        int id = it->second.toInt();
        if(this->isVisible())
            m_pTitleBar->showMainWnd();
        m_pageHelper->setCurrPageId(id,info);
    }
}

void HZMainWnd::proceTickDlg(const QString &code, bool bShow)
{
    if(!m_dealDlg)
    {
        QDesktopWidget *desk = QApplication::desktop();
        m_dealDlg = new AllTimeDealDialog(desk);
    }
    if(bShow && m_dealDlg->isHidden())
        m_dealDlg->show();
    if(bShow && code.isEmpty() == false)
    {
        m_dealDlg->afterDeactive();
        m_dealDlg->setStockCode(code);
        m_dealDlg->afterActive();
    }else if(m_dealDlg->isVisible())
    {
        m_dealDlg->afterDeactive();
        m_dealDlg->hide();
    }
}

void HZMainWnd::closeEvent(QCloseEvent *event)
{
    if(m_dealDlg)
    {
        m_dealDlg->close();
        delete m_dealDlg;
        m_dealDlg  = nullptr;
    }
}







