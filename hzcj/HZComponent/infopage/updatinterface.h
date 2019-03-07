/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef UPDATEINTERFACE_H
#define UPDATEINTERFACE_H

#include <QObject>
#include <QVector>
#include <QVariantMap>
#include "infopage_export.h"


///
/// \brief The IWebJsonParse class web页数据解析接口
///
class HZINFOPAGE_EXPORT IWebJsonParse
{
public:
    virtual ~IWebJsonParse(){}

    /**
     * @brief parse
     */
    virtual void parse()=0;

};

///
/// \brief The IUpdatePerformer class
///
class HZINFOPAGE_EXPORT IUpdatePerformer
{
public:
    virtual ~IUpdatePerformer(){}

    /**
     * @brief updateList 更新列表
     * @param var
     *
     */
    virtual void updateList(const QVariantMap &var)=0;

    /**
     * @brief updateDetail 内容更新
     * @param var
     */
    virtual void updateDetails(const QVariant &var)=0;

protected:
    int m_defaultTag{0};//记录当前tag值
    bool m_tagChanged{true};//当前tag是否发生变化
};

class HZINFOPAGE_EXPORT IUpdateRequest
{
public:
    virtual ~IUpdateRequest(){}

    /**
     * @brief initializatItems 初始化时的数据
     * @param isClear 是否请求历史数据
     * @return
     */
    virtual void initializatItems(bool isClear=false, const QVariant &var=QVariant())=0;//{return QVariant();}

    /**
     * @brief reqId 当前ID
     * @return
     */
    virtual QString itemId()=0;

    /**
     * @brief getTimes 获取当前页面的时间列表
     * @return
     */
    QVector<quint64> getCalandarTimes() { return mCalandarTimes;}


    /**
     * @brief reqId 设置当前ID
     * @return
     */
    void setItemId(const QString& id);
    void setVisitor(IUpdatePerformer *performer);
    virtual void requestDetails(const QString &id)=0;
    virtual void requestListForParam(const QVariantMap &var=QVariantMap())=0;
    virtual void cancleReq()=0;/*有些接口有多种请求~*/

    /**
     * @brief abnormalReq req为-1时处理异常
     * @param reqId
     * @param nextpage
     */
    void abnormalReq(int reqId, int nextpage);

protected:
    QString m_curId;
    int m_nextPage{0};
    QVector<quint64> mCalandarTimes;//数据层缓存接口过来的数据，无需每次请求时间列表
    IUpdatePerformer *mAbstractPer;
    QVariantMap mCurCfg;

    //updateList 更新列表
    void updateList(const QVariantMap& var);

    //updateContent 更新内容
    void updateDetails(const QVariant& var);
};

#endif // UPDATEINTERFACE_H
