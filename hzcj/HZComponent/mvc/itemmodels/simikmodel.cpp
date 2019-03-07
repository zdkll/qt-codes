#include "simikmodel.h"

#define Smk_Stock_ColumnCount 4


SimiKHeaderItemModel::SimiKHeaderItemModel(QObject *parent)
    :QAbstractTableModel(parent)
    ,m_stkColumnCount(Smk_Stock_ColumnCount)
{
    m_priLabels<<QStringLiteral("入选牛股")<<QStringLiteral("相似牛股");
}

void SimiKHeaderItemModel::setColumnInfos(const QVector<ColumnInfo> &columnInfos)
{
    this->beginResetModel();
    m_columnInfos = columnInfos;
    this->endResetModel();
}

int SimiKHeaderItemModel::columnCount(const QModelIndex &parent ) const
{
    return m_columnInfos.size();
}

int SimiKHeaderItemModel::rowCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SimiKHeaderItemModel::data(const QModelIndex &index, int role) const
{
    int row  = index.row();
    int column = index.column();
    //表头文字
    if(role == SimK_Header_Data_Role)
    {
        if(row == 0) //第一排
        {
            if(column == m_stkColumnCount)
                return m_columnInfos[column].label;
            else if(column<m_stkColumnCount){
                return m_priLabels[0];
            }else
                return m_priLabels[1];
        }else //第二排
            return m_columnInfos[column].label;
    }else if(SimK_Header_Alignment_Role == role) //对齐
    {
        if(row == 0){
            if(column == m_stkColumnCount)
                return Qt::AlignCenter;
            else
                return Qt::AlignVCenter|Qt::AlignLeft;
        }else{
            return int(m_columnInfos[column].alignment);
        }
    }
    //是否该列排序
    else if(SimK_Header_IsSortIndex_Role == role)
    {
        if(m_sortIndicator.column == column)
            if(column == m_stkColumnCount || row>0)
                return true;
        return false;
    }
    //升序还是降序
    if(SimK_Header_SortOrder_Role == role)
        return int(m_sortIndicator.order);

    return QVariant();
}

void SimiKHeaderItemModel::setStkColumnCount(int count)
{
    m_stkColumnCount = count;
}

void SimiKHeaderItemModel::setSortIndicator(const int &sortIndex,const Qt::SortOrder &sortOrder)
{
    int old_column = m_sortIndicator.column;
    m_sortIndicator.column  = sortIndex;
    m_sortIndicator.order     = sortOrder;
    //更新数据
    if(old_column >=0){
        dataChanged(index(0,old_column),index(1,old_column));
    }
    emit dataChanged(index(0,sortIndex),index(1,sortIndex));
}


///SimiKModel----
void SimiKModel::setData(const QVector<SimiKData>& simkDatas)
{
    this->beginResetModel();
    m_simikDatas = simkDatas;
    this->endResetModel();
}

void SimiKModel::toggledItemExpaned(int row,bool expanded)
{
    m_simikDatas[row].isExpaned = expanded;

    emit dataChanged(index(row,columnCount()-1),index(row,columnCount()-1));
}

int SimiKModel::rowCount(const QModelIndex &/*parent*/ ) const
{
    return m_simikDatas.size();
}

#include <QBrush>
QVariant SimiKModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if(role == Stock_Data_Role){

        ColumnIdx colIdx =  columnIdx(column);
        switch (colIdx) {
        case ColStockCode:
            return  m_simikDatas[row].selectedStk.stockCode;
            break;
        case ColStockName:
            return  m_simikDatas[row].selectedStk.stockName;
            break;
        case ColFiveRate:
            return  m_simikDatas[row].selectedStk.fiveRate;
            break;
        case ColSimilar:
            return  m_simikDatas[row].similar;
            break;
        case ColSmkStockCode:
            return  m_simikDatas[row].similarStk.stockCode;
            break;
        case ColSmkStockName:
            return  m_simikDatas[row].similarStk.stockName;
            break;
        case ColPrice:
            return  m_simikDatas[row].similarStk.price;
            break;
        case ColRate:
            return  m_simikDatas[row].similarStk.rate;
            break;
        case ColViewSmk:
            return  m_simikDatas[row].isExpaned;
            break;
        default:
            break;
        }
    }
    //test
    if(column == 9)
    {
        if(role == Qt::DisplayRole){
            if(m_simikDatas[row].isExpaned) return QStringLiteral("收缩相似K线");
            else
                return QStringLiteral("查看相似K线");
        }
        else if(role == Qt::ForegroundRole)
            return QVariant(QBrush(Qt::white));
    }
    return QVariant();
}

