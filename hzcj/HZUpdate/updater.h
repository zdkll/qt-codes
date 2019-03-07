/********************************************************************
created:2018/05
author: dk.zhang
purpose:更新实现类，支持多线程
*********************************************************************/
#ifndef UPDATER_H
#define UPDATER_H

#define Total_Progress               120
#define Parse_VersionInfo_Progress   0
#define Start_Download_Progress      10
#define Downloaded_Progress          110
#define Extract_Progress             110
#define Finshed_Progress             120

#include <QObject>

//等待
#define WaitSec(sec) {  QTime t; t.start();\
    while(t.elapsed()<1000*sec)   \
    QApplication::processEvents();\
    }

struct VerInfo;
class DownloadUpdate;
class Updater: public QObject
{
    Q_OBJECT
public:
    enum UpdateStep
    {
        NoUpdate   = 0,
        Downloaded = 1,
        ExtraCompress = 2
    };
    Updater(QObject *parent = 0);
     ~Updater();
     //当前更新步骤
    UpdateStep updateStep();
    //是否停止
    bool isStoped();

public slots:
    //开始更新，verfile 版本信息文件;updateDir 更新程序路径
    void startUpdate(const QString &verfile,const QString &updateDir);
    void cancel();
    void downloadProgress(const int &progress);
    void slotDownloadFinished();

private:
    bool isCanceled();
    void setUpdateStep(const UpdateStep &updateStep);

signals:
    void signalProgress(int progress, QString context);
    void signalVerInfo(const VerInfo &verInfo);

private:
    QString   m_updateDir;
    QMutex   m_mutex;
    bool         m_stoped;
    bool         m_canceled;
    DownloadUpdate *downloadUpdate;
    UpdateStep m_step;
    QString    m_tmpPath;

};


#endif // UPDATER_H
