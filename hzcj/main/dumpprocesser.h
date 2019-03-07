#ifndef DUMPPROCESSER_H
#define DUMPPROCESSER_H

#include <QString>
#include <QNetworkAccessManager>

#include "breakpad/client/windows/handler/exception_handler.h"

class QNetworkReply;
class QFile;
class QHttpMultiPart;
class DumpProcesser : public QObject
{
    Q_OBJECT
public:
    DumpProcesser(QObject *parent = NULL);
    ~ DumpProcesser();

    //dump处理
    void process();

    static bool callback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded);
    void   postStartInfo();

protected slots:
    void postFinished();
    void postStartFinised();

private:
    QString getHostMacAddress();
    QString dumpPath();
    void postDumpFile(const QString &path);
    void clearDumpFiles(const QString &path);

    google_breakpad::ExceptionHandler *m_eh;
    QString m_dmpFilePrefix;

    QNetworkReply *m_reply;
    QNetworkAccessManager m_nam;
    QString m_ptUrl;

    QList<QFile *>  m_files;
    QHttpMultiPart *m_multiPart;

};

#endif // DUMPPROCESSER_H
