/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZINFOADAPTERINTERFACE_H
#define HZINFOADAPTERINTERFACE_H

#include "vlistdef.h"
#include "infomation_export.h"

#include <QAbstractItemView>
#include <hzinfocenter.h>

class HZVListMaster;

/**
 * @brief The HZUpdateObserve class
 */
class INFOMATION_EXPORT IHZInfoUpdateObserve
{
public:
    virtual ~IHZInfoUpdateObserve(){}
    /**
     * @brief updateList
     * @param list
     */
    virtual void updateList(const QVariantMap& var)=0;//
};

///
/// \brief The HZInfoAdapterInterface class
///
class INFOMATION_EXPORT HZInfoAdapterInterface
{
public:
    virtual ~HZInfoAdapterInterface();

    /**
     * @brief reqId 当前ID
     * @return
     */
    QString itemId() const { return m_itemId; }

    /**
     * @brief reqId 设置当前ID
     * @return
     */
    void setItemId(const QString& id) { m_itemId = id; }

    /**
     * @brief requestList 拉去列表
     */
    virtual void requestList()=0;

    /**
     * @brief setVisitor
     * @param dlg
     */
    void setVisitor(IHZInfoUpdateObserve *obs);

    /**
     * @brief   设置股票代码
     * @param[in] codeVec 股票代码列表
     * @retval
     */
    void setCode(const QVector<QString> &codeVec);

    /**
     * @brief getCodes
     * @return
     */
    QVector<QString> getCodes() { return m_codeVec;}

    /**
     * @brief setNextpage
     * @param infoPage
     */
    void setNextpage(int infoPage){m_nextPage = infoPage;}

    /**
     * @brief getItems 初始化的列表数据
     * @return
     */
    STInfoData getItems(){ return m_mData;}

    /**
     * @brief setFirstDatas 设置第一次加载的数据
     * @param datas
     */
    void setFirstDatas(const STInfoData &datas) { m_mData = datas; }

    /*取消请求*/
    void cancleReq()
    {
        if(mReqId !=-1){
            HzInfoCenter::getInstance()->cancle(mReqId);
            mReqId = -1;
        }
    }

    /**
     * @brief updateList 更新列表
     * @param list
     */
    void updateList(const QVariantMap& var);//STInfoData

    /**
     * @brief abnormalReq req为-1时处理异常
     * @param reqId
     * @param nextpage
     */
    void abnormalReq(int reqId, int nextpage);

    //relate data
    QString m_itemId;
    STInfoData m_mData;
    int m_nextPage{0}, mReqId{-1};
    QString m_curId;
    bool m_isFirst{0};
    QVector<QString> m_codeVec;
    IHZInfoUpdateObserve *m_listVis;
};

#endif // HZINFOADAPTERINTERFACE_H
