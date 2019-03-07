/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZINFOADAPTER_H
#define HZINFOADAPTER_H

#include <QObject>
#include <QVariant>

#include "vlistdef.h"
#include "hzinfoadapterinterface.h"
#include "infomation_export.h"

#include <httpclientdef.h>
#include <hzinfodata.h>
#include <hzinfocenter.h>

/**
 * @brief The HZRMZXInfo class 热门资讯
 */
class INFOMATION_EXPORT HZRMZXInfo : public HZInfoAdapterInterface
{
public:
    HZRMZXInfo(const QString &id) {m_itemId = id; }
    HZRMZXInfo(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = 0;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZRMZXInfo(){}

private:
    //拉去列表
    virtual void requestList();
    void hostNewsCallBack(const InfoRet& ret,HostNews& news, int page);
};

/**
 * @brief The HZ7And24Info class 7*24
 */
class INFOMATION_EXPORT HZ7Multiply24Info : public HZInfoAdapterInterface
{
public:
    HZ7Multiply24Info(const QString &id){m_itemId = id;}
    HZ7Multiply24Info(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZ7Multiply24Info(){}

private:
    void requestList();
    void reqFlashNewCallBack(const InfoRet&,FlashNews&,int);
};


/**
 * @brief The HZNewsInfo 新闻
 */
class INFOMATION_EXPORT HZNewsInfo : public HZInfoAdapterInterface
{
public:
    HZNewsInfo(const QString& id){m_itemId = id;}
    HZNewsInfo(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZNewsInfo(){}

private:
    void requestList();
    void reqNewsCallBack(const InfoRet&,const NewsInfo&,int);
};

/**
 * @brief The HZNewsInfo 板块新闻
 */
class INFOMATION_EXPORT HZPlateNewsInfo : public HZInfoAdapterInterface
{
public:
    HZPlateNewsInfo(const QString& id){m_itemId = id;}
    HZPlateNewsInfo(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZPlateNewsInfo(){}

private:
     void requestList();
    void reqPlateNewsCallBack(const InfoRet&ret ,const PlateNewsLists& data, int nextpage);
};


/**
 * @brief The HZNewsInfo 指数新闻
 */
class INFOMATION_EXPORT HZIndexNewsInfo : public HZInfoAdapterInterface
{
public:
    HZIndexNewsInfo(const QString& id){m_itemId = id;}
    HZIndexNewsInfo(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZIndexNewsInfo(){}

private:
    void requestList();
    void reqIndexNewsCallBack(const InfoRet&ret ,const IndexNewsLists& data, int nextpage);
};

/**
 * @brief The HZNewsInfo 公告
 */
class INFOMATION_EXPORT HZAnncsInfo : public HZInfoAdapterInterface
{
public:
    HZAnncsInfo(const QString& id){m_itemId = id;}
    HZAnncsInfo(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZAnncsInfo(){}


private:
    void requestList();
    void reqAnncsCallBack(const InfoRet&,const AnncsInfo&,int);
};


/**
 * @brief The HZResearchReport 研报
 */
class INFOMATION_EXPORT HZResearchReport : public HZInfoAdapterInterface
{
public:
    HZResearchReport(const QString& id){m_itemId = id;}
    HZResearchReport(const HZInfoAdapterInterface* info)
    {
        m_itemId     = info->itemId();
        m_nextPage  = info->m_nextPage;
        m_mData     = info->m_mData;
        m_codeVec   = info->m_codeVec;
    }

    ~HZResearchReport(){}

private:
    void requestList();
    void reqResearchCallBack(const InfoRet&,const ResearchReports&, int page);
};


#endif // HZINFOADAPTER_H
