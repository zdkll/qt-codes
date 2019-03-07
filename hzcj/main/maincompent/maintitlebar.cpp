#include "maintitlebar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>


#include "minimornitorwnd.h"
#include "hzline.h"
#include "pubmsgdef.h"
#include <searchedit.h>
#include "feedbackdialog.h"
#include "exitdialog.h"
#include "hzaboutdlg.h"
#include "hzcjinfo.h"
#include "stockadddialog.h"
#include "stockwarndialog.h"
//#include "testkline/klinewidget.h"

enum TitleMenuItemId
{
    LoginId = 0,       //重新登录服务器
    MinimizeId,        //最小化设置
    ExitId,            //退出系统
    SelfStockId,       //自选股
    HsIndexId,         //沪深指数
    HsBlockId,         //沪深板块
    RiseRankId,        //涨幅排名
    HsStockId,         //沪深股票
    FundFlowId,             //资金流向
    HotInfoId,            //热门资讯
    InstInfoId,            //7*24小时资讯
    CalendarId,          //财经日历
    CalendarStockId,     //股市日历
    OptNewsId,           //自选股新闻
    PlateMonitId,           //板块监测
    SetBiddingId,           //集合竞价
    HzIndexId,           //汇正指标
    SimilarKlineId,           //相似K线
    PolicyId,           //策略
    DataId,           //数据
    PersonalId,           //个人中心
    ProductManualId,           //产品说明书
    FeedBackId,           //留言反馈
    AboutHzId,           //关于汇正
    CusHotlineId ,         //客服热线
    TestId
};

MainTitleBar::MainTitleBar(QWidget *parent,QString instName)
    :TitleBar(parent,instName),
      m_pMainLayout(new QHBoxLayout(this)),
      m_pWnd(new MiniMornitorWnd)
{
//    m_klineWidget = new KlineWidget;
    createTitleBar();
}

void MainTitleBar::onMessage(const QString &senderPath, const MessageParams &params)
{
    if(senderPath == BROAD_CASET)
    {
        auto it = params.find(HZ_CMD);
        if(it != params.end())
        {
            if(it->second == SWITCH_USER)
            {
                getUerInfo();
            }else if(it->second == SHOW_MINI_DLG)
            {
                if(!m_pWnd->isVisible())
                {
                    m_pWnd->show();
                    m_pWnd->afterActive();
                }
            }
        }
    }
}

void MainTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_controlBox->OnMaximinzed();
}

bool MainTitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    //    case QEvent::WindowTitleChange:
    //    {
    //        QWidget *pWidget = qobject_cast<QWidget *>(obj);
    //        if (pWidget)
    //        {
    //            m_pTitleLabel->setText(pWidget->windowTitle());
    //            return true;
    //        }
    //    }
    //    case QEvent::WindowIconChange:
    //    {
    //        QWidget *pWidget = qobject_cast<QWidget *>(obj);
    //        if (pWidget)
    //        {
    //            QIcon icon = pWidget->windowIcon();
    //            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
    //            return true;
    //        }
    //    }
    case QEvent::Move:
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    default:
        return QWidget::eventFilter(obj, event);
    }
}

void MainTitleBar::afterActive()
{
    if(m_pWnd->isVisible())
    {
        m_pWnd->afterDeactive();
        m_pWnd->afterActive();
    }
}

void MainTitleBar::showMainWnd()
{
    m_controlBox->onRestoreWindow();
}



void MainTitleBar::createTitleBar()
{
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);

    //创建按钮组
    createButton();

    getUerInfo();


    m_pMainLayout->addStretch();

    SearchEdit *edit = new SearchEdit;
    edit->setMinimumWidth(320);
    m_pMainLayout->addWidget(edit, 0 , Qt::AlignCenter );

    m_pMainLayout->addStretch();
    //创建菜单栏
    createMenu();

    m_pMainLayout->addSpacing(5);
    m_pMainLayout->addWidget(new HZLine(Qt::Vertical, this));
    m_pMainLayout->addSpacing(13);
    QPushButton *btn = new QPushButton();
    btn->setObjectName("mini");
    connect(btn, &QPushButton::clicked, [this]{

        m_controlBox->OnMinimized();
        if(!m_pWnd->isVisible())
        {
            m_pWnd->show();
            m_pWnd->afterActive();
        }
    });
    connect(this, &TitleBar::mainWndClose, [this]{
        m_pWnd->afterDeactive();
        m_pWnd->close();
    });
    connect(m_pWnd, &MiniMornitorWnd::maxiMize, [this]{

        m_controlBox->onRestoreWindow();
    });

    m_pMainLayout->addWidget(btn);
    m_pMainLayout->addSpacing(13);
    m_pMainLayout->addWidget(new HZLine(Qt::Vertical, this));
    //controlBox
    //    m_controlBox = new  WindowControlBoxEx(this,(QWidget *)this->parent(),true,true);
    m_controlBox = new  WindowControlBoxEx(this,this->parentWidget(),true,true);
    m_pMainLayout->addWidget(m_controlBox,0,Qt::AlignRight);
    m_pMainLayout->setContentsMargins(0,0,0,0);


    this->setLayout(m_pMainLayout);
    layout()->setContentsMargins(0,0,0,0);

    connect(m_controlBox, &WindowControlBox::mainWndClose, this, &MainTitleBar::showExitDialog);

    //    connect(m_controlBox, &WindowControlBox::mainWndClose, this, &MainTitleBar::mainWndClose);
}

void MainTitleBar::createMenu()
{
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setObjectName("titlemenuBar");
    menuBar->addMenu(createSysMenu());
    menuBar->addMenu(createQuoteMenu());
    menuBar->addMenu(createInfoMenu());
    menuBar->addMenu(createAnalysMenu());
    menuBar->addMenu(createHelpMenu());
    menuBar->addMenu(createServerMenu());
    m_pMainLayout->addWidget(menuBar,0,Qt::AlignRight | Qt::AlignVCenter);
}

void MainTitleBar::createButton()
{
    m_pLoginBtn = new QPushButton(QStringLiteral("登录"));
    m_pRegisterBtn = new QPushButton(QStringLiteral("注册"));

    m_pMessageBtn = new QPushButton();
    m_pBacktrackBtn = new QPushButton();
    m_pLoginBtn->setObjectName("login");
    m_pRegisterBtn->setObjectName("register");
    m_pMessageBtn->setObjectName("message");
    m_pBacktrackBtn->setObjectName("backtrack");

    m_pMessageBtn->hide();

    connect(m_pLoginBtn, &QPushButton::clicked, this,&MainTitleBar::showLogInDialog);
    //    connect(m_pRegisterBtn, &QPushButton::clicked, [this]{QMessageBox::information(this, "test", "register");});
    connect(m_pMessageBtn, &QPushButton::clicked, [this]{QMessageBox::information(this, "test", "message");});
    connect(m_pBacktrackBtn, &QPushButton::clicked, [this]{this->MsgBack();});

    m_pMainLayout->addSpacing(14);
    m_pMainLayout->addWidget(m_pLoginBtn);
    m_pMainLayout->addSpacing(14);
    m_pMainLayout->addWidget(m_pRegisterBtn);
    m_pMainLayout->addSpacing(15);
    m_pMainLayout->addWidget(m_pMessageBtn);
    m_pMainLayout->addSpacing(12);
    m_pMainLayout->addWidget(new HZLine(Qt::Vertical,this));
    m_pMainLayout->addSpacing(12);
    m_pMainLayout->addWidget(m_pBacktrackBtn);
}

void MainTitleBar::getUerInfo()
{
    HZAuthenOpt *authenopt = HZAuthenOpt::instance();
    m_LoginStatus = authenopt->logStatus();

    if(m_LoginStatus == HZAuthenOpt::LogStatus::UserLogged)
    {
        m_reqUserInfoId = authenopt->queryUserProfile();
    }
    else if(m_LoginStatus == HZAuthenOpt::LogStatus::GuestLogged)
    {
        QPixmap pix(":/skin/default/img/1mainwindow/maintitlebar/head.png");
        m_pLoginBtn->setText("");
        m_pLoginBtn->setIcon(pix);
        m_pLoginBtn->setIconSize(QSize(28,28));

        m_pRegisterBtn->setText(QStringLiteral("游客007"));
    }

    connect(authenopt, &HZAuthenOpt::commandFinished, [=](int id, int error){
        if(error == 0)
        {
            if(m_reqUserInfoId == id)
            {
                m_reqUserInfoId= -1;
                if(m_LoginStatus == HZAuthenOpt::LogStatus::UserLogged)
                {
                    HZAuthen::UserProfile userProfile = authenopt->getUserProfile();


                    QPixmap pix(userProfile.icon);
                    m_pLoginBtn->setText("");
                    m_pLoginBtn->setIcon(pix);
                    m_pLoginBtn->setIconSize(QSize(28,28));


                    if(!userProfile.screenName.isEmpty())
                        m_pRegisterBtn->setText(userProfile.screenName);
                    else if(!userProfile.mobileNum.isEmpty())
                    {
                        m_pRegisterBtn->setText(userProfile.mobileNum.replace(3, 4, "XXXX"));
                    }
                    else
                        m_pRegisterBtn->setText(QStringLiteral("汇正222"));
                }
            }
        }
    });
}

QMenu *MainTitleBar::createSysMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("系统"),this);
    QActionGroup *group = new QActionGroup(this);

    QAction *action   = group->addAction(QStringLiteral("重新登录服务器"));
    action->setData(LoginId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("最小化设置"));
    action->setData(MinimizeId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("退出系统"));
    action->setData(ExitId);
    menu1->addAction(action);

//    action = group->addAction(QStringLiteral("测试"));
//    action->setData(TestId);
//    menu1->addAction(action);

    connect(group,&QActionGroup::triggered,this,&MainTitleBar::actionTriggered);
    return menu1;
}

QMenu *MainTitleBar::createQuoteMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("行情"),this);
    QActionGroup *group = new QActionGroup(this);

    QAction *action  = group->addAction(QStringLiteral("自选股"));
    action->setData(SelfStockId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("沪深指数"));
    action->setData(HsIndexId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("沪深板块"));
    action->setData(HsBlockId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("涨幅排名"));
    action->setData(RiseRankId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("沪深股票"));
    action->setData(HsStockId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("资金流向"));
    action->setData(FundFlowId);
    menu1->addAction(action);

    connect(group,&QActionGroup::triggered,this,&MainTitleBar::actionTriggered);
    return menu1;
}

QMenu *MainTitleBar::createInfoMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("资讯"),this);
    QActionGroup *group = new QActionGroup(this);

    QAction *action  = group->addAction(QStringLiteral("热门资讯"));
    action->setData(HotInfoId);
    menu1->addAction(action);

    action  = group->addAction(QStringLiteral("7*24小时资讯"));
    action->setData(InstInfoId);
    menu1->addAction(action);

    action  = group->addAction(QStringLiteral("经济日历"));
    action->setData(CalendarId);
    menu1->addAction(action);

    action  = group->addAction(QStringLiteral("股市日历"));
    action->setData(CalendarStockId);
    menu1->addAction(action);

    action  = group->addAction(QStringLiteral("自选股新闻"));
    action->setData(OptNewsId);
    menu1->addAction(action);


    connect(group,&QActionGroup::triggered,this,&MainTitleBar::actionTriggered);
    return menu1;
}

QMenu *MainTitleBar::createAnalysMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("分析"),this);
    QActionGroup *group = new QActionGroup(this);

    QAction *action  = group->addAction(QStringLiteral("板块监测"));
    action->setData(PlateMonitId);
    menu1->addAction(action);

    action = group->addAction(QStringLiteral("集合竞价"));
    action->setData(SetBiddingId);
    menu1->addAction(action);

    //    action = group->addAction(QStringLiteral("汇正指标"));
    //    action->setData(HzIndexId);
    //    menu1->addAction(action);

    //    action = group->addAction(QStringLiteral("相似K线"));
    //    action->setData(SimilarKlineId);
    //    menu1->addAction(action);

    //    action = group->addAction(QStringLiteral("策略"));
    //    action->setData(PolicyId);
    //    menu1->addAction(action);

    action = group->addAction(QStringLiteral("数据"));
    action->setData(DataId);
    menu1->addAction(action);
    connect(group,&QActionGroup::triggered,this,&MainTitleBar::actionTriggered);
    return menu1;
}

QMenu *MainTitleBar::createHelpMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("帮助"),this);
    QActionGroup *group = new QActionGroup(this);

    //    QAction *action = group->addAction(QStringLiteral("个人中心"));
    //    action->setData(PersonalId);
    //    menu1->addAction(action);

    //    action = group->addAction(QStringLiteral("产品说明书"));
    //    action->setData(ProductManualId);
    //    menu1->addAction(action);

    //    action = group->addAction(QStringLiteral("意见反馈"));
    //    action->setData(FeedBackId);
    //    menu1->addAction(action);

    QAction *action = group->addAction(QStringLiteral("关于汇正"));
    action->setData(AboutHzId);
    menu1->addAction(action);

    connect(group,&QActionGroup::triggered,this,&MainTitleBar::actionTriggered);
    return menu1;
}

QMenu *MainTitleBar::createServerMenu()
{
    QMenu *menu1 = new QMenu(QStringLiteral("客服"),this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *frame = new QFrame;
    frame->setObjectName("customerservice");
    frame->setFixedSize(165,68);
    layout->addWidget(frame);
    menu1->setLayout(layout);
    return menu1;
}

void MainTitleBar::updateMaximize()
{
    QWidget *pWindow = this->parentWidget();

    bool bMaximize = pWindow->isMaximized();

    m_controlBox->updateMaximize(bMaximize);
}


void MainTitleBar::actionTriggered(QAction *action)
{
    int nId = action->data().toInt();
    switch (nId) {
    case LoginId:   //重新登录服务器
        showLogInDialog();
        break;
    case MinimizeId:    //最小化设置
        m_controlBox->OnMinimized();
        break;
    case ExitId:        //退出系统
        showExitDialog();
        break;
//    case TestId:        //测试
//        {
//            m_klineWidget->show();
//        }
//        break;
    case SelfStockId:   //自选股
    {
        bool bRecod = false;
        if(getGlobalPageId() != option_stock_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(option_stock_page));
        msg.emplace(CHILD_WND_ID,QString::number(OptionListWndType));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case HsIndexId:     //沪深指数
    {
        bool bRecod = false;
        if(getGlobalPageId() != quote_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(QbzsListWndType));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case HsBlockId:     //沪深板块
    {
        bool bRecod = false;
        if(getGlobalPageId() != block_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(block_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(103));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case RiseRankId:    //涨幅排名
    {
        bool bRecod = false;
        if(getGlobalPageId() != quote_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(HszhInnerWndType));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case HsStockId:     //沪深股票
    {
        bool bRecod = false;
        if(getGlobalPageId() != quote_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(HsagListWndType));
        msg.emplace(THIRD_CHILD_WND_ID,QString::number(BtBaseQuoteListType));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case FundFlowId:    //资金流向
    {
        bool bRecod = false;
        if(getGlobalPageId() != quote_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(quote_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(HsagListWndType));
        msg.emplace(THIRD_CHILD_WND_ID,QString::number(BtFundFlowerFlowType));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case HotInfoId:     //热门资讯
    {
        bool bRecod = false;
        if(getGlobalPageId() != info_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(0));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case InstInfoId:    //7*24小时资讯
    {
        bool bRecod = false;
        if(getGlobalPageId() != info_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(1));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case CalendarId:    //财经日历
    {
        bool bRecod = false;
        if(getGlobalPageId() != info_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(2));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case CalendarStockId:    //股市日历
    {
        bool bRecod = false;
        if(getGlobalPageId() != info_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(3));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case OptNewsId:     //自选新闻
    {
        bool bRecod = false;
        if(getGlobalPageId() != info_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(info_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(4));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case PlateMonitId:  //板块监测
    {
        bool bRecod = false;
        if(getGlobalPageId() != block_list_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(block_list_page));
        msg.emplace(CHILD_WND_ID,QString::number(103));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case SetBiddingId:  //集合竞价
    {
        bool bRecod = false;
        if(getGlobalPageId() != data_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(CallAuctionWnd));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case HzIndexId:     //汇正指标
        QMessageBox::information(this, "test", QStringLiteral("汇正指标"));
        break;
    case SimilarKlineId:    //相似K线
        QMessageBox::information(this, "test", QStringLiteral("相似K线"));
        break;
    case PolicyId:      //策略
        QMessageBox::information(this, "test", QStringLiteral("策略"));
        break;
    case DataId:        //数据
    {
        bool bRecod = false;
        if(getGlobalPageId() != data_center_page)
            bRecod = true;
        std::map<QString,QString> msg;
        msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
        msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
        msg.emplace(CHILD_WND_ID,QString::number(CallAuctionWnd));
        sendMessage(ROOT_FRAME,msg,bRecod);
    }
        break;
    case PersonalId:    //个人中心
        QMessageBox::information(this, "test", QStringLiteral("个人中心"));
        break;
    case ProductManualId:    //产品说明书
        QMessageBox::information(this, "test", QStringLiteral("产品说明书"));
        break;
    case FeedBackId:    //留言反馈
        showFeedBackDialog();
        break;
    case AboutHzId:     //关于汇正
        showAboutDlg();
        break;
    case CusHotlineId:  //客服热线
        QMessageBox::information(this, "test", QStringLiteral("客服热线"));
        break;
    default:
        break;
    }
}

void MainTitleBar::showLogInDialog()
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,SHOW_LOGIN_DIALOG);
    QString path = ROOT_FRAME;
    path.append("//");
    path.append(LogIn_Dialog);
    sendMessage(path,msg,false);
}

void MainTitleBar::showFeedBackDialog()
{
    FeedbackDialog dlg;
    dlg.exec();
}

void MainTitleBar::showAboutDlg()
{
    HZAboutDlg dlg;
    QVariantMap infoMap;
    infoMap["version"]      = PRODUCT_VERSION;
    infoMap["productname"]  = PRODUCT_NAME;
    infoMap["releasetime"]  = RELEASE_TIME;
    infoMap["versioninfo"]  = TARGET_COPYRIGHT;
    dlg.setMsg(QVariant::fromValue(infoMap));
    dlg.exec();
}

void MainTitleBar::showExitDialog()
{
    QWidget *control = (QWidget *)this->parent();
    ExitDialog dlg;

    connect(&dlg, &ExitDialog::closemainwnd,[=](bool bflg){
        if(bflg)
        {
            control->close();
            if(m_pWnd->isVisible())
            {
                m_pWnd->afterDeactive();
                m_pWnd->close();
            }
        }
    });

    dlg.exec();
}

WindowControlBoxEx::WindowControlBoxEx(QWidget *parent, QWidget *controlWindow, bool showMinButton, bool showMaxButton, QString instName):
    WindowControlBox(parent, controlWindow, showMinButton, showMaxButton,instName)
{

}

void WindowControlBoxEx::OnClose()
{
    emit mainWndClose();
}



