#include "hzlog.h"
#include <log4cpp/Appender.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <string>
#include <stdarg.h>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <hzcfg.h>
#include <cfgdef.hpp>
#include <QTextCodec>
#include <QApplication>
#include <tchar.h>
#include <QDebug>
#include <hzuserdatamgr.h>

//#if defined(_MSC_VER)
//#define VSNPRINTF _vsnprintf
//#else
//#ifdef LOG4CPP_HAVE_SNPRINTF
//#define VSNPRINTF vsnprintf
//#else

#define ROLLFILE_MAX_COUNT 3
#define ROLLFILE_MAX_SIZE  20*1024*1024

static QTextCodec *gCodec = nullptr;
static QHash<QString, log4cpp::Priority::Value> gPriority = {
        {"NOTSET",  log4cpp::Priority::NOTSET},//800
        {"DEBUG",   log4cpp::Priority::DEBUG},
        {"INFO",    log4cpp::Priority::INFO},
        {"NOTICE",  log4cpp::Priority::NOTICE},
        {"WARN",    log4cpp::Priority::WARN},
        {"ERROR",   log4cpp::Priority::ERROR},
        {"CRIT",    log4cpp::Priority::CRIT},
        {"ALERT",   log4cpp::Priority::ALERT},
        {"FATAL",   log4cpp::Priority::FATAL},
        {"EMERG",   log4cpp::Priority::EMERG},//0
};

static std::string vform(const char* format, va_list args) {
    size_t size = 1024;
    char* buffer = new char[size];
    while(1) {
        va_list args_copy;

#if defined(_MSC_VER) || defined(__BORLANDC__)
        args_copy = args;
#else
        va_copy(args_copy, args);
#endif
        int n = _vsnprintf(buffer, size, format, args_copy);
        va_end(args_copy);
        // If that worked, return a string.
        if ((n > -1) && (static_cast<size_t>(n) < size)) {
            std::string s(buffer);
            delete [] buffer;
            return s;
        }

        // Else try again with more space.
        size = (n > -1) ?
                    n + 1 :   // ISO/IEC 9899:1999
                    size * 2; // twice the old size

        delete [] buffer;
        buffer = new char[size];
    }
}

AutoElapsedTimer::AutoElapsedTimer(const QString &funName)
{
    m_funName = funName;
    m_timer.start();
}

AutoElapsedTimer::~AutoElapsedTimer()
{
//    qDebug() << "file name:" << m_funName << "time:" << m_timer;
    qDebug() << m_funName << m_timer.elapsed();
    Log("APD","INFO","exec %s : %d ms", m_funName.toStdString().c_str(), m_timer.elapsed());
}

HZLog::HZLog()
{
    gCodec = QTextCodec::codecForName("GBK");
}

HZLog::HZLog(const HZLog &)
{}

HZLog::~HZLog()
{
    log4cpp::Category::shutdown();
}

void HZLog::initialization()
{ 
    LogSTCateoriesHash logCfg = Hzcfg::getInstance()->getLogCfgData();
    if(logCfg.isEmpty()) return;
    QHashIterator<QString, QList<STCategories>> it(logCfg);
    while (it.hasNext()) {
        it.next();
        QList<STCategories> appenders = it.value();
        for(STCategories cg:appenders) {
            log4cpp::Appender *appender=nullptr;
            QString fileName = qApp->applicationDirPath();
            fileName.append(QString("/%1").arg(cg.fileName));
            QByteArray data = gCodec->fromUnicode(fileName);
            if(cg.appender == "RollingFileAppender"){
                appender = new log4cpp::RollingFileAppender(cg.name.toStdString(), data.toStdString(),
                                                            ROLLFILE_MAX_SIZE, ROLLFILE_MAX_COUNT);
                log4cpp::PatternLayout *patternlayout = new log4cpp::PatternLayout();
                patternlayout->setConversionPattern(cg.conversionPattern.toStdString());
                appender->setLayout(patternlayout);
                appender->setThreshold(gPriority[cg.priority.toUpper()]);
            }else if(cg.appender == "FileAppender") {
                appender = new log4cpp::FileAppender(cg.name.toStdString(), data.toStdString());
                log4cpp::PatternLayout *patternlayout = new log4cpp::PatternLayout();
                patternlayout->setConversionPattern(cg.conversionPattern.toStdString());
                appender->setLayout(patternlayout);
                appender->setThreshold(gPriority[cg.priority.toUpper()]);
            }/*else if(...){
                //TODO
            }*/

            if(appender == nullptr) continue;
            log4cpp::Category& log = log4cpp::Category::getInstance(cg.name.toStdString());
            log.addAppender(appender);
        }
    }
}

void HZLog::LogInfo(const QString &name, const QString &priority, const char* stringFormat, ...)
{
    if(!gPriority.contains(priority))
        return;

#if 0
    va_list args;
    va_start(args,stringFormat);
    std::string str = vform(stringFormat,args);
    va_end(args);

    QByteArray data = gCodec->fromUnicode(str.c_str());
    log4cpp::Category& log = log4cpp::Category::getInstance(name.toStdString());
    log.log(gPriority[priority],data);
#else
    va_list args;
    va_start(args, stringFormat);
    log4cpp::Category& log = log4cpp::Category::getInstance(name.toStdString());
    log.logva(gPriority[priority],stringFormat,args);
    va_end(args);
#endif
}


HZLog& HZLog::operator=(const HZLog &)
{ return *this; }
