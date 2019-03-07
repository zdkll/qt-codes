/********************************************************************
created:  2018
author: juntang.xie
purpose:派生自titlebar ，为主界面设计的标题栏
*********************************************************************/
#ifndef MAINTITLEBAR_H
#define MAINTITLEBAR_H

#include <titlebar.h>
#include "minimornitorwnd.h"
#include "hzauthenopt.h"


class WindowControlBoxEx:public WindowControlBox
{
    Q_OBJECT
public:
    explicit WindowControlBoxEx(QWidget* parent,QWidget* controlWindow,bool showMinButton = true,bool showMaxButton = true,QString instName="");

    virtual void OnClose();
};


class QHBoxLayout;
class QLabel;
class QMenu;
class QAction;
class QPushButton;
class MiniMornitorWnd;
class KlineWidget;
class   MainTitleBar : public TitleBar
{
    Q_OBJECT
public:
    MainTitleBar(QWidget *parent = 0,QString instName="");

    ~MainTitleBar()
    {
        if(m_pWnd)
            delete m_pWnd;
        m_pWnd = nullptr;
    }

    virtual void onMessage(const QString& senderPath,const MessageParams& params);

    void showMainWnd();

    virtual void afterActive();

protected:

    /**
     * @brief mouseDoubleClickEvent
     *  双击标题栏进行界面的最大化/还原
     * @param event
     *  QMouseEvent *
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);


private:
    void createTitleBar();
    void createMenu();
    void createButton();
    void getUerInfo();
    QMenu* createSysMenu();
    QMenu* createQuoteMenu();
    QMenu* createInfoMenu();
    QMenu* createAnalysMenu();
    QMenu* createHelpMenu();
    QMenu* createServerMenu();

    void updateMaximize();


private Q_SLOTS:
      void actionTriggered(QAction *action);

      //登录
      void showLogInDialog();
      //意见反馈
      void showFeedBackDialog();
      //关于界面
      void showAboutDlg();
      //退出提示
      void showExitDialog();
private:
      QHBoxLayout *m_pMainLayout;
      QPushButton *m_pLoginBtn, *m_pRegisterBtn;
      QPushButton *m_pMessageBtn, *m_pBacktrackBtn;
      MiniMornitorWnd *m_pWnd;
      int m_reqUserInfoId = -1;
      HZAuthenOpt::LogStatus m_LoginStatus;

//      KlineWidget *m_klineWidget;
};

#endif // MAINTITLEBAR_H
