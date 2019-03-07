#ifndef HZCHILDWNDMGR_H
#define HZCHILDWNDMGR_H
/********************************************************************
created:min.cheng
author: min.cheng
purpose: 负责子页面的创建于管理
*********************************************************************/
#include <QObject>
#include <QMap>
#include "hzobject.h"
#include <QStackedWidget>
class DataCenterPageMainWnd;
struct ChildWndItem
{
    QWidget* pWnd = nullptr;
    HZObject* pObj = nullptr;
};

//struct CreateChildWndFun
//{
//   ChildWndItem(DataCenterPageMainWnd::*pFun)();
//};
//typedef ChildWndItem(DataCenterPageMainWnd::*CreateChildWndFun)();

//template <typename ClassCreater>
//typedef ChildWndItem(ClassCreater::*CreateChildWndFun)();

template <typename ClassCreater>
class HzChildWndMgr
{
public:
    explicit HzChildWndMgr(ClassCreater* pThis,QStackedWidget* pStackWnd);

    /**
     * @brief 获取当前堆栈窗口
     * @retval  返回堆栈窗口，用于布局
     */
    QWidget* getCurrStackWnd();

    /**
     * @brief 注册创建函数，用于后面延迟创建窗口
     * @param[in] fun 创建函数指针
     * @param[in] pageType 页面对应类型
     * @param[in] bDefault 默认页面
     * @retval
     */
    void registChildCreater(ChildWndItem(ClassCreater::*pFun)(),int pageType,bool bDefault = false);

    /**
     * @brief 判断是否注册了相关页面
     * @param[in] pageType
     */
    bool isRegistedType(int pageType);

    /**
     * @brief 获取当前窗口
     * @param[out] item 当前窗口 如果第一次获取，则会创建默认页
     * @retval 返回对应页面pageType
     */
    int currChildWnd(ChildWndItem& item);


    /**
     * @brief 根据页面类型 设置当前页面
     * @param[in] pageType 页面类型
     * @param[in] bActive 是否激活当前页面
     * @retval 返回当前页面实例
     */
    ChildWndItem setCurrWndByPageType(int pageType,bool bActive);

    /**
     * @brief 设置当前合约
     */
    void setCurrStock(const QString& currStock,bool bSetAll);

    /**
     * @brief 返回记录的当前页
     */
    int getCurrType();
private:
    struct InnerItem
    {
        ChildWndItem(ClassCreater::*pFun)();
        ChildWndItem item;
        int index;
    };
    //创建一个窗口
    ChildWndItem createChildWnd(int pageType,bool setCurrWnd = false);
    //根据页面获取到真实的对象实例
    ChildWndItem getChildItemRealPointer(int pageType);
    bool isCreated(int pageType);
    bool findItemFromPool(ChildWndItem(ClassCreater::*pFun)(),ChildWndItem& item);
    bool findItem(ChildWndItem(ClassCreater::*pFun)(),InnerItem& item);
private:
    QString m_stockCode;
    int m_defaultPageId = -1; //默认页面
    int m_currPageId = -1; //当前页面
    QMap<int,ChildWndItem(ClassCreater::*)()> m_typeFunRegistMap; //页面类型对应的创建函数
    ClassCreater* m_pThis; //外部对象实例
    QStackedWidget* m_pstackWnd;
    QMap<int,int> m_typeIndexMap; //key pageType页面类型  value stackWidget下标


    QVector<InnerItem> m_vecPool;//已经创建的对象池 每次创建前从这里获取下
};

template <typename ClassCreater>
bool HzChildWndMgr<ClassCreater>::findItemFromPool(ChildWndItem(ClassCreater::*pFun)(),ChildWndItem& item)
{
    bool bRet =false;
    for(int i =0;i < m_vecPool.size();i++)
    {
        if(m_vecPool[i].pFun == pFun)
        {
            bRet = true;
            item = m_vecPool[i].item;
            break;
        }
    }
    return bRet;
}


template <typename ClassCreater>
bool HzChildWndMgr<ClassCreater>::findItem(ChildWndItem(ClassCreater::*pFun)(),InnerItem& item)
{
    bool bRet =false;
    for(int i =0;i < m_vecPool.size();i++)
    {
        if(m_vecPool[i].pFun == pFun)
        {
            bRet = true;
            item = m_vecPool[i];
            break;
        }
    }
    return bRet;
}

template <typename ClassCreater>
HzChildWndMgr<ClassCreater>::HzChildWndMgr(ClassCreater *pThis, QStackedWidget *pStackWnd):
    m_pThis(pThis),
    m_pstackWnd(pStackWnd)
{

}

template <typename ClassCreater>
QWidget *HzChildWndMgr<ClassCreater>::getCurrStackWnd()
{
    return m_pstackWnd;
}

template <typename ClassCreater>
void HzChildWndMgr<ClassCreater>::registChildCreater(ChildWndItem(ClassCreater::*pFun)(),int pageType,bool bDefault)
{
    if(m_defaultPageId == -1)
        m_defaultPageId = pageType; //防止外部么有设置默认值
    if(bDefault)
        m_defaultPageId = pageType;
    m_typeFunRegistMap[pageType] = pFun;
}

template <typename ClassCreater>
bool HzChildWndMgr<ClassCreater>::isRegistedType(int pageType)
{
    bool bRet = false;
    if(m_typeFunRegistMap.contains(pageType))
    {
        bRet = true;
    }
    return bRet;
}

template <typename ClassCreater>
int HzChildWndMgr<ClassCreater>::currChildWnd(ChildWndItem &item)
{
    int id = m_pstackWnd->currentIndex();
    if(id == -1)
    {
        item = createChildWnd(m_defaultPageId,true);
    }else
    {
        ChildWndItem(ClassCreater::*fun)() = m_typeFunRegistMap[m_currPageId];
        findItemFromPool(fun,item);
    }
    return m_currPageId;
}

template <typename ClassCreater>
ChildWndItem HzChildWndMgr<ClassCreater>::setCurrWndByPageType(int pageType, bool bActive)
{
    ChildWndItem item;
    int currPage = currChildWnd(item);
    if(item.pObj) //当前页取消激活
        item.pObj->afterDeactive();
    //设置当前页面
    if(!isCreated(pageType))
    {
        item = createChildWnd(pageType,true);
    }else if(currPage != pageType)
    {
        int index  = m_typeIndexMap[pageType];
        m_pstackWnd->setCurrentIndex(index);
        item = getChildItemRealPointer(pageType);
        m_currPageId = pageType;
    }
    if(item.pObj && bActive)
        item.pObj->afterActive();
    return item;
}

template <typename ClassCreater>
void HzChildWndMgr<ClassCreater>::setCurrStock(const QString &currStock,bool bSetAll)
{
    m_stockCode = currStock;
    if(bSetAll)
    {
        for(int i = 0;i < m_vecPool.size();i++)
        {
            m_vecPool[i].item.pObj->setStockCode(currStock);
        }
    }else
    {
        ChildWndItem item;
        currChildWnd(item);
        item.pObj->setStockCode(currStock);
    }
}

template <typename ClassCreater>
int HzChildWndMgr<ClassCreater>::getCurrType()
{
    return m_currPageId;
}

template <typename ClassCreater>
ChildWndItem HzChildWndMgr<ClassCreater>::createChildWnd(int pageType,bool setCurrWnd)
{
    ChildWndItem item;
    if(m_typeFunRegistMap.contains(pageType))
    {
        qDebug() << "createChildWnd pageType: " << pageType;
        item = getChildItemRealPointer(pageType);//没获取到表示未创建过改对象实例
        ChildWndItem(ClassCreater::*fun)() = m_typeFunRegistMap[pageType];
        int index = -1;
        if(item.pObj == nullptr)
        {
            //ChildWndItem(ClassCreater::*cfun)() =(ChildWndItem(ClassCreater::*)())fun;
            qDebug() << "createChildWnd m_pThis->*fun ";
            item = (m_pThis->*fun)();
            item.pObj->setStockCode(m_stockCode);
            index  = m_pstackWnd->addWidget(item.pWnd);
            InnerItem vecItem;
            vecItem.item = item;
            vecItem.pFun = fun;
            vecItem.index = index;
            m_vecPool.push_back(vecItem);
        }else
        {
            InnerItem vecItem;
            findItem(fun,vecItem);
            index = vecItem.index;
        }
        m_typeIndexMap[pageType] = index;
        qDebug() << "createChildWnd pageType: " << pageType << "value: " << index;
        if(setCurrWnd && index != -1)
        {
            m_currPageId =pageType;
            m_pstackWnd->setCurrentIndex(index);
        }
    }
    return item;
}

template <typename ClassCreater>
ChildWndItem HzChildWndMgr<ClassCreater>::getChildItemRealPointer(int pageType)
{
    ChildWndItem item;
    if(m_typeFunRegistMap.contains(pageType))
    {
        auto fun = m_typeFunRegistMap[pageType];
        findItemFromPool(fun,item);
    }
    return item;
}

template <typename ClassCreater>
bool HzChildWndMgr<ClassCreater>::isCreated(int pageType)
{
    bool bRet = false;
    if(m_typeIndexMap.contains(pageType))
    {
        bRet  =true;
    }
    return bRet;
}
#endif // HZCHILDWNDMGR_H
