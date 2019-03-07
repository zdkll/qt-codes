#include "sortfilterproxymodel.h"

#include <QDebug>

BaseSortFilterProxyModel::BaseSortFilterProxyModel(AbstractBaseModel *model,QObject *parent)
    :QSortFilterProxyModel(parent),m_sourceModel(model)
{
    QSortFilterProxyModel::setSourceModel(model);
    this->setSortRole(Stock_Data_Role);

    QLocale locale(QLocale::Chinese,QLocale::SimplifiedHanScript,QLocale::China);
    m_collator.setLocale(locale);
}

BaseSortFilterProxyModel::~BaseSortFilterProxyModel()
{

}

QVariant BaseSortFilterProxyModel::indexData(const int &row,const ColumnIdx &inColumn) const
{
    int column = m_sourceModel->colIdxMap().value(inColumn,-1);

    return (column != -1)? this->index(row, column).data(Stock_Data_Role ):QVariant();
}

QVariant BaseSortFilterProxyModel::colIdxData(const int &row,const ColumnIdx &inColumn) const
{
    int mapRow = this->mapToSource(index(row,0)).row();
    return  m_sourceModel->colIdxData(mapRow,inColumn);
}

DynaSortFilterProxyModel::DynaSortFilterProxyModel(AbstractBaseModel *model,QObject *parent)
    :BaseSortFilterProxyModel(model,parent)
{
    this->setDynamicSortFilter(false);
}

bool  DynaSortFilterProxyModel::lessThan(const QModelIndex & , const QModelIndex & ) const
{
    return false;
}

///SortFilterProxyModel
bool  SortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    //特殊字段排序
    ColumnIdx idx = m_sourceModel->columnIdx(source_left.column());
    switch (idx) {
    case ColStockCode:
    case ColBKStockCode:
        return m_sourceModel->data(source_left,Stock_Data_Role).toString().right(6)
                <m_sourceModel->data(source_right,Stock_Data_Role).toString().right(6);
        break;
    case ColStockName: //股票名称
    case ColBlockName: //所属行业
        return m_collator.compare(m_sourceModel->data(source_left,Stock_Data_Role).toString()
                                  ,m_sourceModel->data(source_right,Stock_Data_Role).toString())<0;
        break;
    default:
        //自动排序根据字段数据类型的值排序
        return m_sourceModel->data(source_left,Stock_Data_Role )
                <m_sourceModel->data(source_right,Stock_Data_Role);
        break;
    }
}



///NaviSortFilterProxyModel---
NaviSortFilterProxyModel::NaviSortFilterProxyModel(NaviStockDynaModel *model,QObject *parent)
    :QSortFilterProxyModel(parent),m_sourceModel(model)
{
    m_sortRole = Stock_No_Role;
}

void NaviSortFilterProxyModel::setNaviSortRole(int role)
{
    m_sortRole = role;
    QSortFilterProxyModel::setSortRole(role);
}

bool  NaviSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if(m_sortRole == Stock_No_Role)
        return false;
    else
        return source_left.data(m_sortRole)<source_right.data(m_sortRole);
}

