#include "timedealmodel.h"

#include <QColor>

TimeDealModel::TimeDealModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_horizLabels<<QStringLiteral("时间")
                <<QStringLiteral("成交价")
               <<QStringLiteral("手数");
}

void TimeDealModel::setData(const QVector<HZData::HZTick> &ticks)
{
    beginResetModel();
    m_ticks = ticks;
    endResetModel();
}

void TimeDealModel::clearData()
{
    if(m_ticks.size()<1) return;
    beginResetModel();
    m_ticks.clear();
    endResetModel();
}

int TimeDealModel::rowCount(const QModelIndex &parent ) const
{
    //三列
    return m_ticks.size();
}

int TimeDealModel::columnCount(const QModelIndex &parent ) const
{
    return 3;
}

QVariant TimeDealModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::Horizontal == orientation)
        if(role == Qt::DisplayRole){
            return m_horizLabels[section];
        }
    return QVariant();
}

QVariant  TimeDealModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();

    if(role == Tick_Data_Role){
        switch (column) {
        case 0:
            return m_ticks[row].update_time;
            break;
        case 1:
            return m_ticks[row].price;
            break;
        case 2:{
            return m_ticks[row].volume;
            }
            break;
        default:
            break;
        }
    }else if(Tick_Data_Direct_Role == role){
        if(column == 2)
            return m_ticks[row].access_way;
    }

    return QVariant();
}
