#include "downloadupdate.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QEventLoop>

#include "hzuserdatamgr.h"

DownloadUpdate::DownloadUpdate(QObject *parent)
    :QObject(parent),m_nam(new QNetworkAccessManager(this))
    ,m_file(0),m_eventLoop(new QEventLoop(this))
{
    m_tmpPath = HzUserDataMgr::getInstance()->getAllUsersDir()+"tmp";
    QDir tmpDir(m_tmpPath);
    if(!tmpDir.exists())
        tmpDir.mkdir(m_tmpPath);
}
//阻塞模式
QString DownloadUpdate::downloadBlocked(const QUrl &url,bool *ok,const DownloadType &downloadType )
{
    if(ok)
        *ok = true;

    //打开下载文件
    if(!openDownloadFile(downloadType)){
        if(ok){
            *ok = false;
        }
        return QString();
    }

    bool isFinished = false;
    m_reply = m_nam->get(QNetworkRequest(url));
    connect(m_reply,&QNetworkReply::readyRead,this,&DownloadUpdate::replyReadyRead);
    connect(m_reply,&QNetworkReply::finished,[this,&isFinished]{ isFinished = true;m_eventLoop->quit();});

    //设置2s超时
    QTimer m_timer;
    m_timer.setInterval(2000);
    m_timer.setSingleShot(true);
    m_timer.start();
    connect(&m_timer,&QTimer::timeout,m_eventLoop,&QEventLoop::quit);

    //阻塞模式，等待完成
    m_eventLoop->exec();

    if(m_file){
        m_file->close();
        delete m_file;m_file = 0;}

    if(ok) *ok = isFinished;

    m_reply->deleteLater();

    emit  finished(m_fileName);
    return m_fileName;
}

//非阻塞模式
void DownloadUpdate::downloadUnBlocked(const QUrl &url, bool *ok,const DownloadType &downloadType)
{
    //打开下载文件
    if(!openDownloadFile(downloadType)){
        if(ok){
            *ok = false;
        }
        return ;
    }
   qDebug()<<"url:"<<url;
    m_reply = m_nam->get(QNetworkRequest(url));
    connect(m_reply,&QNetworkReply::readyRead,this,&DownloadUpdate::replyReadyRead);
    connect(m_reply,&QNetworkReply::finished,this,&DownloadUpdate::replyFinished);
    connect(m_reply,&QNetworkReply::downloadProgress,this,&DownloadUpdate::slotDownloadProgress);
    if(ok)
        *ok = true;
}


void DownloadUpdate::abort()
{
    if(m_reply)
        m_reply->abort();
}

void DownloadUpdate::replyReadyRead()
{
    m_file->write(m_reply->readAll());

}

bool   DownloadUpdate::openDownloadFile(const DownloadType &downloadType)
{
    //打开文件
    if(downloadType == VersionInfo){
        m_fileName = m_tmpPath+UpdateVerInfo_File;
    }
    else
    {
        m_fileName = m_tmpPath+Update_File;
    }
    m_file = openFileForWrite(m_fileName);
    if(!m_file){
        return false;
    }

    return true;
}

void DownloadUpdate::replyFinished()
{
     qDebug()<<"download error:"<<m_reply->error()<<m_reply->errorString()
            <<"file size:"<<m_file->size();
    m_file->close();
    delete m_file;m_file = 0;
    m_reply->deleteLater();
    emit  finished(m_fileName);
}


void DownloadUpdate::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int progress = 100*double(bytesReceived)/double(bytesTotal);

    emit downloadProgress(progress);
}

QFile *DownloadUpdate::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile());
    file->setFileName(fileName);
    if(file->exists())
        file->remove();
    if(!file->open(QIODevice::WriteOnly|QIODevice::Append)){
        file.reset();
    }

    return file.take();
}

