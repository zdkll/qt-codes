/********************************************************************
created: 2018/06/05
author: df.zhang
purpose: 登录界面
*********************************************************************/
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMap>

#include "hzcomponent_global.h"
#include "framewidget.h"
#include "loginopt.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QLabel;
class QCheckBox;
class LogInOpt;
class QTabWidget;
class EWMDialog;
class HZCOMPONENTSHARED_EXPORT LogInDialog : public FrameLessDialog
{
    Q_OBJECT
public:
    LogInDialog(QWidget *parent = 0,QString instName= "login_dialog");
    ~LogInDialog();

    //处理消息
    virtual void onMessage(const QString& senderPath,const MessageParams& params);

signals:
    void commandFinished();

private slots:
    //选中用户列表某一用户，设置其对应的密码
    void slotCurrentIndexChange(const int &index);

    void slotRegister();

    void slotForgetPwd();
    //用户登录，包括手机和短信
    void slotLogIn();
    //游客登录
    void slotLogGuest();
    //二维码登录,收到服务器消息后调用此槽函数
    void slotEwmLogIn();

    //获取验证码
    void slotGetVerifyNum();
    //获取二维码
    void slotGetEwm();
    //登录状态改变
    void  slotLogInStateChanged(LogInOpt::LogInState);

    void slotRecvVerifyNum();

private:
    void createWg();
    QWidget  *createTopWg();
    TitleBar    *createTitleBar();
    QWidget  *createAccountLogWg();
    QWidget  *createMsgLogWg();
    QWidget  *createCenterWg();
    QWidget  *createBottomWg();

    void          initWg();

    //
    void initLog();

private:
    LogInOpt   *m_logInOpt; //登录相关逻辑实现类
    HZLogIn::logInType   m_logInType;
    //控件----
    QComboBox        *m_userCbx;
    QLineEdit            *m_pwdEdit,*m_phoneNumEdit,*m_verifyNumEdit;
    QCheckBox         *m_rmPwdCbx,*m_autoLogCbx;
    QPushButton       *m_logBtn,*m_getVerNumBtn,
    *m_regBtn,*m_fgtPwBtn,*m_guestLogInBtn;
    QLabel                *m_retLabel;
    QTabWidget        *m_centerTabWg;

    EWMDialog         *m_ewmDialog;
};

//二维码对话框
class EWMDialog : public FrameLessDialog
{
public:
    EWMDialog(QWidget *parent = 0,QString instName="EWMDialog");

private:
    void createWg();
};

#endif // LOGINDIALOG_H
