#include "dumpprocesser.h"
#include "hzuserdatamgr.h"

#include <QDebug>
#include <QDir>
#include <QApplication>
#include <DbgHelp.h>
#include <QDateTime>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHttpMultiPart>
#include <QHttpPart>

#include "hzcjinfo.h"
#include "hzcfg.h"

QString gdumpPath = "./";
QString gdmpFilePrefix = "";

DumpProcesser::DumpProcesser(QObject *parent)
    :QObject(parent),m_eh(0),m_reply(0)
    ,m_multiPart(0)
{
    m_dmpFilePrefix = QString("%1_%2_%3").arg(getHostMacAddress())
            .arg(QSysInfo::windowsVersion())
            .arg(PRODUCT_VERSION);//getHostMacAddress()
    gdmpFilePrefix = m_dmpFilePrefix.remove(":");//;
}

DumpProcesser::~DumpProcesser()
{
    if(m_eh)
        delete m_eh;
}

//breakpad
bool DumpProcesser::callback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded)
{
    if (succeeded) {
        //修改文件名
        QString dumpFile = QString("%1/%2.dmp").arg(QString::fromStdWString(dump_path))
                .arg(QString::fromStdWString(id));
        QFile file(dumpFile);
        qDebug()<<"dump file:"<<dumpFile;
        QString newFileName = QString("%1/%2_%3.dmp").arg(QString::fromStdWString(dump_path)).arg(gdmpFilePrefix)
                .arg(QDateTime::currentDateTime().toTime_t());
        file.rename(newFileName);
    }
    else {
        qDebug("dump save failed");
    }
    return succeeded;
}

#ifdef Q_OS_WIN
//dbghelp
LONG WINAPI TopLevelExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
    QString fileName = QString("%1/%2_%3.dmp").arg(gdumpPath).arg(gdmpFilePrefix).arg(QDateTime::currentDateTime().toTime_t());
    qDebug()<< "save dump to:"<<fileName;
    HANDLE hFile = CreateFile(fileName.toStdWString().c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    MINIDUMP_EXCEPTION_INFORMATION stExceptionParam;
    stExceptionParam.ThreadId    = GetCurrentThreadId();
    stExceptionParam.ExceptionPointers = pExceptionInfo;
    stExceptionParam.ClientPointers    = FALSE;
    //DumpType:MiniDumpWithFullMemory 比较大，dmp文件包含所有二进制信息；MiniDumpNormal，仅包含解析堆栈多需要的信息
    MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hFile,MiniDumpNormal,&stExceptionParam,NULL,NULL);
    CloseHandle(hFile);
    getchar();
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void DumpProcesser::process()
{
    //判断目录是否存在，不存在则创建
    QString dumpPath = qApp->applicationDirPath()+"/dumps";
    QDir dir(dumpPath);
    if(!dir.exists())
        dir.mkdir(dumpPath);
    gdumpPath = dumpPath;

    //判断是否配置了Dmp服务器,未配置表示不用上传
#ifdef QT_NO_DEBUG
    //    #if 1
    if(!Hzcfg::getInstance()->serverInfo().dmpUploadUrl.isEmpty()){
        m_ptUrl = Hzcfg::getInstance()->serverInfo().dmpUploadUrl;
        //发送登陆信息
        postStartInfo();
        //上传dmp文件
        postDumpFile(dumpPath);
    }else
        clearDumpFiles(dumpPath);
#else //清除dump缓存
    clearDumpFiles(dumpPath);
#endif


#if 1
    //breakpad
    m_eh = new google_breakpad::ExceptionHandler(
                dumpPath.toStdWString(), NULL, &DumpProcesser::callback, NULL,
                google_breakpad::ExceptionHandler::HANDLER_ALL);

#elif defined(Q_OS_WIN)
    //dbgHelp
    SetUnhandledExceptionFilter(TopLevelExceptionFilter);
#endif
}

QString DumpProcesser::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

QString DumpProcesser::dumpPath()
{
    QString allUserDir = HzUserDataMgr::getInstance()->getAllUsersDir();
    QString dump_path = allUserDir+"dumps";
    //qDebug()<<dump_path;
    QDir dir(dump_path);
    if(!dir.exists())
        dir.mkdir(dump_path);
    return dump_path;
}

void DumpProcesser::postDumpFile(const QString &path)
{
    QDir dir(path);
    QStringList filters;
    filters<<"*.dmp";
    QStringList fileList =  dir.entryList(filters,QDir::Files);
    if(fileList.size()<1) return;

    m_multiPart = new QHttpMultiPart(this);
    m_multiPart->setContentType(QHttpMultiPart::FormDataType);

    foreach (QString fileNane, fileList) {
        QFile  *file = new QFile(dir.absoluteFilePath(fileNane));
        file->open(QIODevice::ReadOnly);
        m_files.append(file);

        QHttpPart filePart;
        filePart.setBodyDevice(file);
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader ,
                           QString("form-data; name=\"dmpFiles\";filename=\"%1\"").arg(fileNane));

        m_multiPart->append(filePart);
    }
    QNetworkRequest request(QUrl(m_ptUrl+"/dumps"));

    m_reply =  m_nam.post(request,m_multiPart);
    connect(m_reply,SIGNAL(finished()),this,SLOT(postFinished()));
}

void DumpProcesser::postStartInfo()
{
    //版本号&日期
    QString  dateStr = QDate::currentDate().toString("yyyyMMdd");

    QJsonObject jobj;
    jobj.insert("Version",PRODUCT_VERSION);
    jobj.insert("Date",dateStr);

    QNetworkRequest request(QUrl(m_ptUrl+"/start?"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QJsonDocument doc;
    doc.setObject(jobj);
    QNetworkReply  *reply =  m_nam.post(request,doc.toJson());
    connect(reply,SIGNAL(finished()),this,SLOT(postStartFinised()));
}

void DumpProcesser::clearDumpFiles(const QString &path)
{
    QDir dir(path);
    QStringList filters;
    filters<<"*.dmp";
    QStringList fileList =  dir.entryList(filters,QDir::Files);
    if(fileList.size()<1) return;
    foreach (QString fileNane, fileList)
        QFile(dir.absoluteFilePath(fileNane)).remove();
}

void DumpProcesser::postFinished()
{
    qDebug()<<"m_reply finished: "<<m_reply->error()<<m_reply->errorString();
    //关闭并清除文件
    for(int i=0;i<m_files.size();i++){
        m_files[i]->close();
        if(!m_reply->error())//上传成功文件删除
            m_files[i]->remove();
    }
    qDeleteAll(m_files);
    m_files.clear();
    delete m_multiPart;m_multiPart = NULL;
    if(m_reply){
        m_reply->deleteLater();
        m_reply = 0;
    }
}

void DumpProcesser::postStartFinised()
{
    QNetworkReply *reply  = qobject_cast<QNetworkReply *>(sender());
    if(!reply->error())
        qDebug()<<"post start success!";
    else
        qDebug()<<"post start failed!";

    reply->deleteLater();
}
