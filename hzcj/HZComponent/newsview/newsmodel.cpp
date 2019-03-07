#include "newsmodel.h"
#include <QVariant>

NewsModel::NewsModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int NewsModel::rowCount(const QModelIndex &parent) const
{
    return m_dataList.size();
}

int NewsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant NewsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    switch (role) {
    case MYDATAROLE:
        return QVariant::fromValue(m_dataList[row]);
        break;
    default:
        break;
    }
    return QVariant();
}

void NewsModel::updateList(const QList<STVListData> &datas)
{
    m_dataList += datas;
    if(m_dataList.size() > 99)
    {
        m_dataList = m_dataList.mid(0,99);
    }
    refrush();
}

void NewsModel::clearAll()
{
    m_dataList.clear();
    refrush();
}

void NewsModel::refrush()
{
    beginResetModel();
    endResetModel();
}
