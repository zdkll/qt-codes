#ifndef HZMainWnd_H
#define HZMainWnd_H


#include "framewidget.h"
#include "windowcontrolbox.h"
#include "pubmsgdef.h"


class NavigatorBar;
class MainTitleBar;
class MainSideBar;
class MainStatusBar;
class MainNavigatorBar;
class HzPageHelper;
class ShortCutManager;
class HZReloginDlg;
class AllTimeDealDialog;
class HZMainWnd : public FrameLessWidget
{
    Q_OBJECT
public:
    explicit HZMainWnd(FrameWidget *parent = 0,QString instName="root_frame");
    ~HZMainWnd();

private:
    void createWg();
public slots:
    void slotsNetIoReady(bool ready);
private:
    //接收命令中心消息
    virtual void onMessage(const QString& senderPath,const MessageParams& params);
    //接收命令中心get消息
    virtual bool onProcessGetMessage(const QString& path,MessageParams& params,Any& outData);
    //消息解析
    void proceSwitchPageCmd(const QString &senderPath,const MessageParams &params);
    //处理成交明细窗口
    void proceTickDlg(const QString& code,bool bShow);
    void closeEvent(QCloseEvent *event);
private:
    MainTitleBar   *m_pTitleBar;
    MainSideBar  *m_sideBar;
    MainStatusBar      *m_statusBar;
    MainNavigatorBar *m_navigatorBar;
    HzPageHelper* m_pageHelper;
    ShortCutManager *mShortCut;
    HZReloginDlg *mReloginDlg;
    AllTimeDealDialog* m_dealDlg = nullptr;
};

#endif // HZMainWnd_H
