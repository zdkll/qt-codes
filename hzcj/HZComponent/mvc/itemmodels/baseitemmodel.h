/********************************************************************
created:2018/05/10
author: dk.zhang
purpose: 主要是包含所有Model 的基类AbstractBaseModel和一个依赖AbstractBaseModel 的排序Model
*********************************************************************/
#ifndef  BASEITEMMODEL_H
#define BASEITEMMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "mvcpublicdef.h"
#include "itemmodeldef.h"
#include "datamodeldef.h"

/**
 * @brief HZ所有表格数据model基类
 */
class AbstractBaseModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    // modelName 配置文件中对应Model名称
    AbstractBaseModel(const QString &modelName,QObject *parent = Q_NULLPTR);

    /*******************QTableView*************************/

    //想要获取的列数对应的Index
    int columnCount(const QModelIndex &parent  = QModelIndex()) const;

    // @brief 表头数据
    QVariant  headerData(int section, Qt::Orientation orientation, int role) const;


    /******************自定义**************************/
    //brief 获取某一列对应的宽度
    float columnWithHint(const int &column) const{ return m_columnInfos[column].width;}

    //@brief  获取列标号对应的列序号Map
    QMap<ColumnIdx,int> &colIdxMap(){return m_colIdxMap;}

    // 获取某一列的排序类型
    SortType  sortType(const int &column) const{return m_columnInfos[column].sortType;}

    //读其他列数据(存在的列)
    QVariant     indexData(const int &row,const ColumnIdx &inColumn) const;

    //获取某一项的值，根据列标号
    virtual QVariant colIdxData(const int &,const ColumnIdx &) const{return QVariant();}

    // @brief 获取某一列的排序字段
    const  SortField sortFieldFromColumn(const int &column){return m_sortFieldIdxMap.value(column,HZData::none_sort);}
    //获取某一列对应列标号
    ColumnIdx     columnIdx(const int &column) const {return m_idxColMap.value(column);}
    //根据列标号获取列的序号
    int columnByIdx(const ColumnIdx &idx){return m_colIdxMap.value(idx,-1);}
    //获取所有列的信息
    const  QVector<ColumnInfo> &columnInfos(){return m_columnInfos;}

    void setColumnLabel(int column,const QString &label);

    void setColumnInfo(int column,const ColumnInfo &colInfo);

private:
    void    readColumnsInfo(const QString &modelName);

private:
    QVector<ColumnInfo>          m_columnInfos;

    QMap<ColumnIdx,int>       m_colIdxMap;

    QMap<int,ColumnIdx>       m_idxColMap;

    QMap<int,SortField>            m_sortFieldIdxMap;

};




#endif // BASEITEMMODEL_H
