#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include "baseitemmodel.h"
#include "navistockdynamodel.h"

/**
 * @brief 自定义排序model 基类类，依赖AbstractBaseModel
 */
class BaseSortFilterProxyModel:public QSortFilterProxyModel
{
public:
    BaseSortFilterProxyModel(AbstractBaseModel *model,QObject *parent = Q_NULLPTR);
    ~BaseSortFilterProxyModel();

    //返回数据model
    AbstractBaseModel *sourceModel(){return m_sourceModel;}

    //读其他列数据(存在的列)
    QVariant                 indexData(const int &row,const ColumnIdx &inColumn) const;
    //某个字段的值,可以是不显示的列
    QVariant                 colIdxData(const int &row,const ColumnIdx &inColumn) const;

    //手动排序字段
    SortField sortFieldFromColumn(const int &column){return m_sourceModel->sortFieldFromColumn(column);}

    //排序类型
    SortType sortType(const int &column)const {return m_sourceModel->sortType(column);}

    //根据列标号返回列序号
    int columnByIdx(const ColumnIdx &idx)const{return m_sourceModel->columnByIdx(idx);}

    //获取所有列的信息
    const QVector<ColumnInfo> &columnInfos(){return m_sourceModel->columnInfos();}

protected:
    AbstractBaseModel *m_sourceModel;
    QCollator                 m_collator;
};

//行情排行数据列表排序，只有部分数据，手动排序情况(目前默认服务器排序)下不自动排序
class DynaSortFilterProxyModel : public BaseSortFilterProxyModel
{
public:
    DynaSortFilterProxyModel(AbstractBaseModel *model,QObject *parent = Q_NULLPTR);

protected:
    bool  lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};

//排行行情数据列表排序，有全部数据，全部自动排序
class SortFilterProxyModel : public BaseSortFilterProxyModel
{
public:
    SortFilterProxyModel(AbstractBaseModel *model,QObject *parent = Q_NULLPTR)
        :BaseSortFilterProxyModel(model,parent){this->setDynamicSortFilter(true);}

protected:
    bool  lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const  Q_DECL_OVERRIDE;
};


//导航列表排序
class NaviSortFilterProxyModel:public QSortFilterProxyModel
{
public:
    NaviSortFilterProxyModel(NaviStockDynaModel *model,QObject *parent = Q_NULLPTR);

    void setNaviSortRole(int role);

protected:
    bool  lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const  Q_DECL_OVERRIDE;

private:
    NaviStockDynaModel *m_sourceModel;
    int                                m_sortRole;
};

#endif // SORTFILTERPROXYMODEL_H
