#include "updater.h"

#include <QDebug>
#include <Qurl>
#include <QApplication>

#include "downloadupdate.h"
#include "verinfoparse.h"
#include  "quazip/JlCompress.h"
#include "hzuserdatamgr.h"

Updater::Updater(QObject *parent)
    :QObject(parent),downloadUpdate(0)
    ,m_step(NoUpdate)
{
    qRegisterMetaType<VerInfo>("VerInfo");
    m_stoped = true;
    m_canceled = false;

    m_tmpPath = HzUserDataMgr::getInstance()->getAllUsersDir()+"/tmp";
}

Updater::~Updater()
{
    if(downloadUpdate)
        delete downloadUpdate;
}

void Updater::cancel()
{
    m_mutex.lock();
    m_canceled = true;
    m_mutex.unlock();

    downloadUpdate->abort();
}

void Updater::setUpdateStep(const UpdateStep &updateStep)
{
    m_mutex.lock();
    m_step = updateStep;
    m_mutex.unlock();
}

void Updater::startUpdate(const QString &verfile,const QString &updateDir)
{
    m_mutex.lock();
    m_stoped = false;
    m_step = NoUpdate;
    m_mutex.unlock();

    //1 解析更新信息文件
    emit signalProgress(Parse_VersionInfo_Progress, QStringLiteral("准备更新..."));
    VerInfoParse verInfoParse(verfile);
    VerInfo verInfo = verInfoParse.versionInfo();
    m_updateDir = updateDir;

    if(isCanceled()){
        m_mutex.lock();
        m_stoped = true;
        m_mutex.unlock();
        return;
    }
    //2 下载全包
    bool ok = false;
    emit signalProgress(Start_Download_Progress, QStringLiteral("正在下载更新包..."));
    if(!downloadUpdate){
        downloadUpdate = new DownloadUpdate();
        connect(downloadUpdate,&DownloadUpdate::downloadProgress,this,&Updater::downloadProgress);
        connect(downloadUpdate,&DownloadUpdate::finished,this,&Updater::slotDownloadFinished);
    }

    downloadUpdate->downloadUnBlocked(QUrl(verInfo.completePatch.url), &ok, DownloadUpdate::CompletePacket);
}

void  Updater::downloadProgress(const int &progress)
{
    if(isCanceled()) return;
    emit signalProgress(Start_Download_Progress + progress, "正在下载更新包...");
}

void Updater::slotDownloadFinished()
{
    m_mutex.lock();
    m_step = Downloaded;
    m_mutex.unlock();
    if(isCanceled()){
        m_mutex.lock();
        m_stoped = true;
        m_mutex.unlock();
        return;
    }

    qDebug()<<"download finished";
    emit signalProgress(Extract_Progress,QStringLiteral("下载完成"));

    //3 解压包-替换
    emit signalProgress(Extract_Progress,QStringLiteral("正在解压..."));
    //全包更新，将原来更新目录全部清除
    QDir dir(m_updateDir);
    dir.removeRecursively();
    QStringList cmpFiles = JlCompress::extractDir(m_tmpPath+Update_File,m_updateDir);
    qDebug()<<"compressed:"<<m_tmpPath+Update_File<<" to:"<<m_updateDir;
    qDebug()<<"compressed files:"<<cmpFiles;
    //等待1ms，允许用户取消
    WaitSec(1);

    m_mutex.lock();
    m_step = ExtraCompress;
    m_stoped = true;
    m_mutex.unlock();

    emit signalProgress(Finshed_Progress,QStringLiteral("更新完成..."));
}

bool Updater::isCanceled()
{
    bool canceled;
    m_mutex.lock();
    canceled = m_canceled;
    m_mutex.unlock();
    return canceled;
}
bool Updater::isStoped()
{
    bool stoped;
    m_mutex.lock();
    stoped = m_stoped;
    m_mutex.unlock();
    return stoped;
}
Updater::UpdateStep Updater::updateStep()
{
    UpdateStep step;
    m_mutex.lock();
    step = m_step;
    m_mutex.unlock();
    return step;
}
