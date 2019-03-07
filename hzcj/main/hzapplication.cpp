#include "hzapplication.h"
#include <hzlog.h>
#include <httpdata.h>
#include <hzcfg.h>
#include <prepareupdate.h>
#include <StyleSheet.h>
#include <QIcon>
#include <QSysInfo>
#include <dumpprocesser.h>
#include "hzmainwnd.h"
#include "login/logindialog.h"

#include <QDateTime>
#include "ThreadPool.hpp"
#include "localstocksource.h"

//test
#include "test.h"
#include "testkline/klinewidget.h"
#include "mvc/itemviews/simikview.h"
#include "mvc/itemviews/optionstockview.h"


HZApplication::HZApplication(QObject *parent):QObject(parent)
{
    //HZ_AUTO_ELAPSED_TIMER;
    m_app = nullptr;
}

HZApplication::~HZApplication()
{
}

int HZApplication::initInstance(int argc, char *argv[])
{
    if(QSysInfo::windowsVersion() == QSysInfo::WV_XP)
        QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL/*Qt::AA_UseDesktopOpenGL*/);
    //创建 APP实例，绑定退出事件
    m_app = new QApplication(argc,argv);
    connect(m_app,SIGNAL(aboutToQuit()),this,SLOT(onExitInstance()));

    //调用初始化函数，用于用户数据初始化
    return onInitInstance(*m_app);
}

void HZApplication::onExitInstance()
{
    HZObject::unInit();

    ThreadPool::GetInstance()->UnInit();

    LocalStockSource::unInit();
}

int HZApplication::onInitInstance(QApplication& app)
{
    //汇正对象初始化
    HZObject::init();

    //配置加载
    Hzcfg::getInstance()->loadCfg();

    //皮肤加载
    CStyleSheet::GetInstance()->SetStyle(CStyleSheet::DEFAULT);

    //dump处理
    DumpProcesser dump;
    dump.process();

#ifdef QT_NO_DEBUG
    //检查更新
    if(PrepareUpdate::instance()->checkAndUpdate()==0)
        return 0;
#endif

    //        SimiKView w;
    //        w.resize(800,200);
    //        w.afterActive();
    //        w.show();

    HZMainWnd w;

    LogInDialog logInDlg;
    if(!logInDlg.exec())
    {
        onExitInstance();
        return 0;
    }

    logInDlg.setOwner(&w);
    w.show();

    //    QEventLoop eventLoop;
    //    connect(gHZDataController,&DataController::signalNetIoReady,[&eventLoop]{
    //        if(gHZDataController->isNetIoReady())
    //            eventLoop.quit();
    //    });
    //    eventLoop.exec();

    //        AblBlockDynaView view;
    //        view.setFilterFlag(HZData::BlockWhole);
    //        view.show();
    //        view.afterActive();

    //    Test test;
    //    test.reqStockBatch();

//    Test tt;
//    tt.testReqTopicInvest();

    return app.exec();
}
