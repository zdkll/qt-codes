#include "updatewindow.h"

#define  UpdateParam_File "/tmp/updateparam.txt"

#include <QApplication>
#include <QMessageBox>
#include <QStyle>
#include <QFile>
#include <QTime>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include "downloadupdate.h"

#include "hzuserdatamgr.h"
#include <hzlog.h>
#include <hzcfg.h>
#include <hzuserdatamgr.h>


QTextStream out;

void myMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    out<<msg<<endl;
}


int main(int argc, char *argv[])
{
    if (QSysInfo::windowsVersion() == QSysInfo::WV_XP) {
        // 为了支持XP
        QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    }
    QApplication a(argc, argv);

    QString logFileName = HzUserDataMgr::getInstance()->getAllUsersDir()+"/tmp/udpate_log.txt";

    QFile logfile(logFileName);
    logfile.open(QIODevice::ReadWrite|QIODevice::Text);
    out.setDevice(&logfile);

    qInstallMessageHandler(myMessageHandler);
    //汇正对象初始化
    HZObject::init();
    //配置加载
    Hzcfg::getInstance()->loadCfg();

    QString paramFile = HzUserDataMgr::getInstance()->getAllUsersDir()+UpdateParam_File;

    QFile    file(paramFile);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString verfile = in.readLine();
    QString updateDir = in.readLine();
    file.close();

    HZUpdateWindow up;
    up.show();

    up.startUpdate(verfile,updateDir);

    int ret =  a.exec();

    logfile.close();
    return ret;
}
