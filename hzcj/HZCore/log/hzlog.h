#ifndef HZLOG_H
#define HZLOG_H

#include <QString>
#include <QElapsedTimer>
#include "singleton.h"
#include "hzlogexport.h"

using std::string;

class HZLOG_EXPORT AutoElapsedTimer{
public:
    AutoElapsedTimer(const QString& funName);
    ~AutoElapsedTimer();

private:
    QElapsedTimer m_timer;
    QString m_funName;
};

class HZLOG_EXPORT HZLog : public singleton::Singleton<HZLog>
{
public:
    static HZLog & Instance();
    ~HZLog();

    /*!
     * \brief initialization 初始化配置和注册配置相关的appender
     * \param path 配置文件路径
     */
    void initialization();

    /*!
     * \brief LogInfo       输出日志
     * \param name          模块名称
     * \param priority      优先级别
     * \param stringFormat  格式化字串
     * \note Log("TICKCOUNT","INFO","hello info tickcount %d %s ",6,"556");
     */
    void LogInfo(const QString &name, const QString &priority, const char* stringFormat, ...);

private:
    HZLog();
    HZLog(const HZLog &);
    HZLog& operator=(const HZLog &);

private:
    friend singleton::Singleton<HZLog>;
};


#define gHZLogIns singleton::Singleton<HZLog>::Instance()
#define Log(name, prioprity, formatStr, ...) { gHZLogIns.LogInfo(name, prioprity, formatStr, __VA_ARGS__);}

#define HZ_AUTO_ELAPSED_TIMER AutoElapsedTimer autoTimer(Q_FUNC_INFO); Q_UNUSED(autoTimer);

#endif // HZLOG_H
