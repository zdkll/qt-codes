#include "logindialog.h"

#define LOG_ERROR_NONE              0
#define LOG_ERROR_CANCELED       -1

#define LOGINDLG_WIDTH    620
#define LOGINDLG_HEIGHT   375

#define LOGIN_USER          0
#define LOGIN_GUEST       1


#include <QBoxLayout>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QProgressBar>
#include <QLabel>
#include <QToolButton>
#include <QApplication>
#include <QEventLoop>
#include <QTabWidget>

#include <QDebug>
#include <QDir>
#include <QtCore>
#include <QPushButton>
#include <QKeyEvent>

#include "titlebar.h"
#include "hzline.h"
#include "pubmsgdef.h"

using namespace HZLogIn;

LogInDialog::LogInDialog(QWidget *parent ,QString instName)
    :FrameLessDialog(parent,instName),m_ewmDialog(0)
{
    m_logInOpt   = new LogInOpt(this);

    createWg();
    m_logInType = HZLogIn::UserLogIn;

    connect(m_logInOpt,&LogInOpt::logInStateChanged,this,&LogInDialog::slotLogInStateChanged);
    connect(m_logInOpt,&LogInOpt::logInFinished,this,&LogInDialog::accept);

    this->setFixedSize(LOGINDLG_WIDTH,LOGINDLG_HEIGHT);
    initWg();
    //自动登录
    if(m_logInOpt->logInCfg().isAutoLogin && m_userCbx->count()>0){
        QTimer::singleShot(1000,this,SLOT(slotLogIn()));
    }
}


LogInDialog::~LogInDialog()
{

}

void  LogInDialog::onMessage(const QString& senderPath,const MessageParams& params)
{
    qDebug()<<"dialog recv message";
    auto it = params.find(HZ_CMD);

    if(it != params.end()){
        QString   cmd = it->second;
        if(cmd == SHOW_LOGIN_DIALOG)
            this->exec();
    }
}


void LogInDialog::slotCurrentIndexChange(const int &index)
{
    if(index<0 || index>=m_logInOpt->logInCfg().loggedUsers.size()) return;
    m_pwdEdit->setText(m_logInOpt->logInCfg().loggedUsers[index].password);
}

void LogInDialog::slotRegister()
{
    //注册账号....
    qDebug()<<"register user.......";
}

void LogInDialog::slotForgetPwd()
{
    //找回密码...
    qDebug()<<"forget password.......";
}



void LogInDialog::slotLogIn()
{
    qDebug()<<"slot user log------------------";
    //正在登录，取消
    initLog();
    //判断登录类型
    m_logInType = UserLogIn;//m_centerTabWg->currentIndex()==0?UserLogIn:PhoneMsgLogIn;
    //分别检查参数
    QStringList args;
    if(m_logInType == UserLogIn){
        QString userName =  m_userCbx->currentText().trimmed();
        if(userName.isEmpty()){
            m_retLabel->setText(QStringLiteral("请输入账号"));
            return;
        }
        QString pwdStr =  m_pwdEdit->text().trimmed();
        if(pwdStr.isEmpty()){
            m_retLabel->setText(QStringLiteral("请输入密码"));
            return;
        }
        args<<userName<<pwdStr;
    }else{
        QString phoneNum =  m_phoneNumEdit->text().trimmed();
        if(phoneNum.isEmpty())
            return;
        QRegularExpression regExpression("[1][3,4,5,7,8]\\d{9}");
        QRegularExpressionMatch matreg =  regExpression.match(phoneNum);
        if(!matreg.hasMatch())
            return;
        QString pwdStr =  m_verifyNumEdit->text().trimmed();
        if(pwdStr.isEmpty())
            return;
        args<<phoneNum<<pwdStr;
    }

    //登录逻辑由LogInOpt 实现
    m_logInOpt->logIn(m_logInType,args);
}

void LogInDialog::slotLogGuest()
{
    qDebug()<<"slot guest log------------------";
    initLog();
    //正在登录，取消
    m_logInType = HZLogIn::GuestLogIn;
    m_logInOpt->logIn(m_logInType);
}

void LogInDialog::slotEwmLogIn()
{
    initLog();
    m_logInType = HZLogIn::EWMLogIn;

}

void LogInDialog::slotGetVerifyNum()
{
    //检查手机号
    QString phoneNumStr = m_phoneNumEdit->text();
    //长度
    if(phoneNumStr.length()<11){
        return;
    }
    //格式
    QRegularExpression regExpression("[1][3,4,5,7,8]\\d{9}");
    QRegularExpressionMatch matreg =  regExpression.match(phoneNumStr);
    if(!matreg.hasMatch())
        return;

    m_logInOpt->getVerifyNum(phoneNumStr);
}

void LogInDialog::slotGetEwm()
{
    if(!m_ewmDialog)
        m_ewmDialog = new EWMDialog(this);
    m_ewmDialog->exec();
}

#include <hzauthenopt.h>
void  LogInDialog::slotLogInStateChanged(LogInOpt::LogInState state)
{
    HZAuthenOpt::instance()->loginStateChanged();
    switch (state) {
    case LogInOpt::LoggingIn:
        m_logBtn->setEnabled(false);
        break;
    case LogInOpt::CancelingLogIn:
        m_logBtn->setEnabled(false);
        break;
    case LogInOpt::Logged:
        m_logBtn->setEnabled(true);

        //广播登录消息
    {
        qDebug()<<"broad cast logged msg";
        MessageParams msgParams;
        msgParams.emplace(HZ_CMD,SWITCH_USER);
        broadcastMessage(msgParams);
    }
        break;
    case LogInOpt::ExistingLogIn:
        m_logBtn->setEnabled(false);
        break;
    default:  //UnLogged
        m_logBtn->setEnabled(true);
        //qDebug()<<m_logInOpt->errString();
        m_retLabel->setText(m_logInOpt->errString());
        break;
    }
}

void LogInDialog::slotRecvVerifyNum()
{
    m_getVerNumBtn->setText("120"+QStringLiteral("秒"));
}

void LogInDialog::initLog()
{
    m_retLabel->clear();
}

///创建页面
void LogInDialog::createWg()
{
    QVBoxLayout  *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,9);

    //顶部
    QWidget *topWidget = createTopWg();
    mainLayout->addWidget(topWidget);

    //中部 账号登录-手机号登录
    QWidget *centerWg = createCenterWg();
    mainLayout->addWidget(centerWg);

    //底部 提示-二维码
    mainLayout->addWidget(createBottomWg());
    mainLayout->addStretch(1);

    this->setLayout(mainLayout);
}


void LogInDialog::initWg()
{
    connect(m_userCbx,SIGNAL(currentIndexChanged(int )),this,SLOT(slotCurrentIndexChange(int)));

    HZLogIn::LogInConfig  logInConfig = m_logInOpt->logInCfg();

    m_rmPwdCbx->setChecked(logInConfig.isRembPwd);
    m_autoLogCbx->setChecked(logInConfig.isAutoLogin);

    //初始化用户列表
    for(int i=0;i<logInConfig.loggedUsers.size();i++)
        m_userCbx->addItem(logInConfig.loggedUsers[i].userName);
    if(!logInConfig.loggedUsers.isEmpty())
        m_userCbx->setCurrentIndex(0);


    //    //注册和找回密码
    //    connect(m_regBtn,SIGNAL(clicked()),this,SLOT(slotRegister()));
    //    connect(m_fgtPwBtn,SIGNAL(clicked()),this,SLOT(slotForgetPwd()));

    //登录
    connect(m_logBtn,SIGNAL(clicked()),this,SLOT(slotLogIn()));
    connect(m_guestLogInBtn,SIGNAL(clicked()),this,SLOT(slotLogGuest()));

    // //获取验证码
    //    connect(m_getVerNumBtn,SIGNAL(clicked()),this,SLOT(slotGetVerifyNum()));
    // //获取二维码
    //    connect(m_getVerNumBtn,SIGNAL(clicked()),this,SLOT(slotGetVerifyNum()));
}


TitleBar *LogInDialog::createTitleBar()
{
    TitleBar *m_titleBar =  new TitleBar(this,"logintitlebar");

    QHBoxLayout *titleLayout = new  QHBoxLayout;
    titleLayout->setContentsMargins(0,0,0,0);

    WindowControlBox *controlBox_   =  new WindowControlBox(this,this);

    titleLayout->addStretch(1);

    //设置
    titleLayout->addWidget(controlBox_);

    controlBox_->setControlFlags(WindowControlBox::WindowMinimizeButton|WindowControlBox::WindowCloseButton);

    titleLayout->setSpacing(4);
    m_titleBar->setLayout(titleLayout);

    //connect(settingBtn,&QPushButton::clicked, [this]{m_logSetDlg->exec();});

    return m_titleBar;
}

QWidget *LogInDialog::createTopWg()
{
    QWidget *topWidget = new QWidget();
    topWidget->setObjectName("topWidget");
    topWidget->setFixedHeight(160);

    QVBoxLayout *topVLayout = new QVBoxLayout;
    topVLayout->setContentsMargins(0,0,0,0);

    TitleBar *m_titleBar =   createTitleBar();
    m_titleBar->setFocusPolicy(Qt::NoFocus);
    this->m_pHelper->setTitleHeight(160);
    m_pHelper->setWidgetResizable(false);  //设置窗体可缩放
    topVLayout->addWidget(m_titleBar);
    topVLayout->addStretch(1);
    topWidget->setLayout(topVLayout);

    return  topWidget;
}

//账号登录
QWidget  *LogInDialog::createAccountLogWg()
{
    QWidget *wg = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    //用户名-----------------
    m_userCbx = new QComboBox(this);
    m_userCbx->setEditable(true);
    m_userCbx->setFixedSize(300,30);
    m_userCbx->lineEdit()->setPlaceholderText(QStringLiteral("用户名/手机号/邮箱"));

    mainLayout->addWidget(m_userCbx,0,Qt::AlignCenter);
    mainLayout->addSpacing(9);

    //密码-
    m_pwdEdit  = new QLineEdit(this);
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setFixedSize(300,30);
    m_pwdEdit->setPlaceholderText(QStringLiteral("密码"));
    mainLayout->addWidget(m_pwdEdit,0,Qt::AlignCenter);
    mainLayout->addSpacing(2);

    //登录提示-
    m_retLabel = new QLabel(this);
    m_retLabel->setObjectName("logretLabel");
    mainLayout->addWidget(m_retLabel);
    m_retLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addSpacing(2);

    //记住密码-自动登录-游客登录
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(160,0,160,0);
    m_rmPwdCbx   = new QCheckBox(QStringLiteral("记住密码"));
    m_autoLogCbx = new QCheckBox(QStringLiteral("自动登录"));

    m_guestLogInBtn = new QPushButton(QStringLiteral("游客登录"));
    m_guestLogInBtn->setObjectName("guestLogInBtn");

    btnLayout->addWidget(m_rmPwdCbx);
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_autoLogCbx);
    btnLayout->addStretch(1);
    btnLayout->addWidget(m_guestLogInBtn);

    mainLayout->addLayout(btnLayout);

    wg->setLayout(mainLayout);

    //自动登录和记住密码
    connect(m_autoLogCbx,&QCheckBox::clicked,[this]{
        this->m_logInOpt->setAutoLogIn(m_autoLogCbx->isChecked());
    });
    connect(m_rmPwdCbx,&QCheckBox::clicked,[this](){
        this->m_logInOpt->setRmbPwd(m_rmPwdCbx->isChecked());
    });

    return wg;
}

//短信登录
QWidget  *LogInDialog::createMsgLogWg()
{
    QWidget        *msgLogwg = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(9);
    mainLayout->setContentsMargins(0,4,0,0);

    //手机号
    m_phoneNumEdit = new QLineEdit(this);
    m_phoneNumEdit->setPlaceholderText(QStringLiteral("手机号"));
    m_phoneNumEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{11,11}")));

    mainLayout->addWidget(m_phoneNumEdit);

    //验证码
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    m_verifyNumEdit = new QLineEdit(this);
    m_verifyNumEdit->setPlaceholderText(QStringLiteral("验证码"));
    m_getVerNumBtn = new QPushButton(QStringLiteral("获取验证码"));
    m_getVerNumBtn->setObjectName("getVerNumBtn");
    hLayout1->addWidget(m_verifyNumEdit);
    hLayout1->addWidget(m_getVerNumBtn);
    mainLayout->addLayout(hLayout1);

    mainLayout->addStretch(1);
    msgLogwg->setLayout(mainLayout);

    return msgLogwg;
}

//登录  注册 找回密码等
QWidget  *LogInDialog::createCenterWg()
{
    QWidget *centerWg = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(9);
    mainLayout->setContentsMargins(0,21,0,9);

    m_centerTabWg = new QTabWidget(this);
    m_centerTabWg->tabBar()->hide();
    m_centerTabWg->addTab(createAccountLogWg(),QStringLiteral("账号登录"));
    //m_centerTabWg->addTab(createMsgLogWg(),QStringLiteral("短信登录"));
    mainLayout->addWidget(m_centerTabWg);

    //登录按钮
    m_logBtn = new  QPushButton(QStringLiteral("登录"),this);
    m_logBtn->setObjectName("loginBtn");
    m_logBtn->setFixedSize(300,30);
    m_logBtn->setDefault(true);
    mainLayout->addWidget(m_logBtn,0,Qt::AlignCenter);

    //注册账号，找回密码，游客登录
    //    QHBoxLayout *btnLayout = new QHBoxLayout;
    //    m_regBtn = new QPushButton(QStringLiteral("注册账户"));
    //    m_regBtn->setObjectName("regBtn");
    //    btnLayout->addWidget(m_regBtn);

    //    m_fgtPwBtn = new QPushButton(QStringLiteral("找回密码"));
    //    m_fgtPwBtn->setObjectName("fgtpwdBtn");
    //    btnLayout->addWidget(m_fgtPwBtn);

    centerWg->setLayout(mainLayout);

    return centerWg;
}

QWidget  *LogInDialog::createBottomWg()
{
    QWidget *bottomWg = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->setContentsMargins(12,0,12,0);
    QLabel *leftInfoLabel = new QLabel(QStringLiteral("非投资咨询类软件产品，不提供投资咨询服务"),this);
    leftInfoLabel->setObjectName("infoLabel");
    mainLayout->addWidget(leftInfoLabel,0,Qt::AlignBottom);

    mainLayout->addStretch(1);
    QLabel *rightInfoLabel = new QLabel(QStringLiteral("客服热线：400-020-2898"),this);
    rightInfoLabel->setObjectName("infoLabel");
    mainLayout->addWidget(rightInfoLabel,0,Qt::AlignBottom);

    //    QPushButton *getEwmBtn = new QPushButton("erwei\nma");
    //    getEwmBtn->setFixedWidth(50);
    //    mainLayout->addWidget(getEwmBtn);

    bottomWg->setLayout(mainLayout);

    //    connect(getEwmBtn,SIGNAL(clicked(bool)),this,SLOT(slotGetEwm()));

    return bottomWg;
}


EWMDialog::EWMDialog(QWidget *parent  ,QString instName)
    :FrameLessDialog(parent,instName)
{
    m_pHelper->setWidgetResizable(false);  //设置窗体可缩放
    createWg();
}

void EWMDialog::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    TitleBar *titleBar = new TitleBar(this);

    titleBar->createTitleBar();
    titleBar->setControlFlags(WindowControlBox::WindowCloseButton);
    titleBar->setFixedHeight(30);
    titleBar->setWinTitle("");
    m_pHelper->setTitleHeight(30);
    mainLayout->addWidget(titleBar);

    QVBoxLayout *contentLayout = new QVBoxLayout;
    QLabel *hzLabel = new QLabel(QStringLiteral("汇正财经"));
    contentLayout->addWidget(hzLabel,0,Qt::AlignCenter);
    contentLayout->setContentsMargins(0,4,0,9);

    QLabel *ewmLabel = new QLabel(this);
    ewmLabel->setFixedSize(120,120);
    QString iconPath = QApplication::applicationDirPath()+"/cfg/icon/erweima.png";

    ewmLabel->setPixmap(QPixmap(iconPath).scaled(120,120));

    contentLayout->addWidget(ewmLabel,0,Qt::AlignCenter);

    QLabel *infoLabel = new QLabel(QStringLiteral("请使用汇正财经app扫一扫登录"));
    contentLayout->addWidget(infoLabel,0,Qt::AlignCenter);

    mainLayout->addLayout(contentLayout);
    this->setLayout(mainLayout);
}
