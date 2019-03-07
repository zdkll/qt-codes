/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFODATAPROVIDOR_H
#define INFODATAPROVIDOR_H

#include <QObject>

#include "hzinfodata.h"
#include "updatinterface.h"
#include "infodef.h"
#include "infopage_export.h"

/**
 * @brief The InfoHotProvidor class 热点数据包装类
 */
class HZINFOPAGE_EXPORT InfoHotProvidor : public IUpdateRequest
{
public:
    InfoHotProvidor(const QString &id=""):m_curId(id){}
    ~InfoHotProvidor();

    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    bool m_isFirst{false};
    QString m_curId;
    InfoReqId m_reqId{-1};
    int m_currentTag{0};//note:tag 发生变化时，保存的page需要置为0；

    void hotInfoCallBack(const InfoRet& ret,HostNews& news, int page);
};

///
/// \brief The InfoNewsProvidor class 新闻
///
class HZINFOPAGE_EXPORT InfoNewsProvidor : public IUpdateRequest
{
public:
    InfoNewsProvidor(const QString &id=""):m_curId(id){}
    ~InfoNewsProvidor();
    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    bool m_isFirst{false};
    QString m_curId;
    InfoReqId m_reqId{-1};
    int m_currentTag;//note:tag 发生变化时，保存的page需要置为0；

    void newsInfoCallBack(const InfoRet& ret,const NewsInfo& news,int page);
};

///
/// \brief The InfoAnncProvidor class 公告
///
class HZINFOPAGE_EXPORT InfoAnncProvidor : public IUpdateRequest
{
public:
    InfoAnncProvidor(const QString &id=""):m_curId(id){}
    ~InfoAnncProvidor();

    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    bool m_isFirst{false};
    QString m_curId;
    InfoReqId m_reqId{0};
    int m_currentTag{0};//note:tag 发生变化时，保存的page需要置为0；

    void anncInfoCallBack(const InfoRet&,const AnncsInfo&,int);
};

/**
 * @brief The InfoHotProvidor class 7*24数据包装类
 */
class HZINFOPAGE_EXPORT InfoNultiHourProvidor : public IUpdateRequest
{
public:
    InfoNultiHourProvidor(const QString &id=""):m_curId(id){}
    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    bool m_isFirst{false};
    QString m_curId;
    InfoReqId m_reqId{-1};
    int m_currentTag{0};//note:tag 发生变化时，保存的page需要置为0；
    QVector<QString> m_times;

    void multiHourInfoCallBack(const InfoRet& ret,const FlashNews& news,int page);
};

///
/// \brief The InfoMarketCalandarProvidor class 股市日历
///
class HZINFOPAGE_EXPORT InfoMarketCalandarProvidor : public IUpdateRequest
{
public:
    InfoMarketCalandarProvidor(const QString &id="")
        :m_curId(id){}
    ~InfoMarketCalandarProvidor();
    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    QString m_curId;
    InfoReqId m_reqId{-1};
    int m_currentTag;//note:tag 发生变化时，保存的page需要置为0；

    /**
     * @brief marketCalandarInfoCallBack 返回数据的回调
     */
    void marketCalandarInfoCallBack(const InfoRet&, const MarketCalendar&);
};

///
/// \brief The InfoEconomicCalandarProvidor class 经济日历
///
class HZINFOPAGE_EXPORT InfoEconomicCalandarProvidor : public IUpdateRequest
{
public:
    InfoEconomicCalandarProvidor(const QString &id="")
        :m_curId(id){}
    ~InfoEconomicCalandarProvidor();

    QString itemId() { return m_curId;}
    void initializatItems(bool isClear=false, const QVariant &var=QVariant());
    void requestDetails(const QString &id){Q_UNUSED(id);}
    void requestListForParam(const QVariantMap &var);
    void cancleReq();

private:
    QString m_curId;
    InfoReqId m_meetReqId{-1}, m_financialReqId{-1}, m_economicReqId{-1};
    int m_currentTag;//note:tag 发生变化时，保存的page需要置为0；

    void majorMeetInfoCallBack(const InfoRet&, const MajorMeeting &meets);
    void financialReportInfoCallBack(const InfoRet&, const FinancialReport &reports);
    void economicDataInfoCallBack(const InfoRet&, const EconomicData &datas);
};

#endif // INFODATAPROVIDOR_H
