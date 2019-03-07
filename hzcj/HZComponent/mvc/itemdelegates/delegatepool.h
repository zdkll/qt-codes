/********************************************************************
created:2018/6/20
author: dk.zhang
purpose:表列样式代理管理池，用于创建和管理样式代理对象
*********************************************************************/
#ifndef DELEGATEPOOL_H
#define DELEGATEPOOL_H

#include "Ioc.hpp"
#include "../itemmodels/baseitemmodel.h"
#include "styleditemdelegate.h"

class StockStyledItemDelegate;

//专用于行情列表样式
class DelegatePool
{
public:
    ~DelegatePool();
    static DelegatePool    *instance();
    //通过指定行的枚举值返回其对应的样式代理对象,一般不为零
    StockStyledItemDelegate   *itemDelegate(const ColumnIdx &idx);
    //获取已经创建的样式代理，不存在则返回空
    StockStyledItemDelegate   *getItemDelegate(const ColumnIdx &idx);

private:
    DelegatePool();

private:
    QHash<ColumnIdx,StockStyledItemDelegate *>    m_delegatesMap;
    IocContainer                                                          m_ioc;

   static std::unordered_map<ColumnIdx,std::string> sdelClassNameMap;
};

#endif // DELEGATEPOOL_H
