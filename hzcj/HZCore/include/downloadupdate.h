/********************************************************************
created:   2018/05/09
author:    dk.zhang
purpose: 下载更新类，包括下载更新版本信息文件和下载更新包
*********************************************************************/
#ifndef DOWNLOADUPDATE_H
#define DOWNLOADUPDATE_H

#define  UpdateVerInfo_File   "/version.json"
#define  Update_File             "/hzcj_update.zip"

#include <QObject>
#include <QTimer>
#include "hzcore_global.h"

class QNetworkReply;
class QNetworkAccessManager;
class QFile;
class QEventLoop;
class  HZCORESHARED_EXPORT  DownloadUpdate :public QObject
{
    Q_OBJECT
public:
    //下载类型：版本信息，差包和全包
    enum DownloadType
    {
        NoDownload   = 0,
        VersionInfo         = 1,
        PartialPacket      = 2,
        CompletePacket = 3
    };
    DownloadUpdate(QObject *parent = 0);

    /**
     * @brief 阻塞模式下载文件
     * @param[in]   url 下载地址
     * @param[out] ok  是否成功
     * @param[in]    downloadType 下载类型，主要是给更新用，下载更新信息文件还是下载更新包
     * @retval  返回下载文件路径
     */
    QString downloadBlocked(const QUrl &url,bool *ok=0,const DownloadType &downloadType = VersionInfo) ;
    /**
     * @brief 非阻塞模式下载文件
     * @param[in]   url 下载地址
     * @param[out] ok  是否成功
     * @param[in]    downloadType 下载类型，主要是给更新用，下载更新信息文件还是下载更新包
     * @retval
     */
    void downloadUnBlocked(const QUrl &url, bool *ok=0,const DownloadType &downloadType = VersionInfo);
    /**
     * @brief 停止当前下载任务
     */
    void abort();

protected slots:
    void replyReadyRead();
    void replyFinished();
    void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void finished(const  QString &fileName);
    void downloadProgress(const int &progress);

private:
    QFile *openFileForWrite(const QString &fileName);
    bool    openDownloadFile(const DownloadType &downloadType);

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply                *m_reply;
    QFile                                *m_file;

    //hzcj 临时目录
    QString                              m_tmpPath;

    QString                              m_fileName;
    QEventLoop                     *m_eventLoop;

};

#endif // DOWNLOADUPDATE_H
