#include "historyboxmodel.h"
#include <QFont>


HistoryBoxModel::HistoryBoxModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

HistoryBoxModel::~HistoryBoxModel()
{

}

// 更新表格数据
void HistoryBoxModel::updateData(QList<StockRecord> stockList)
{
    m_stockList = stockList;
    beginResetModel();
    endResetModel();
}

// 行数
int HistoryBoxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);


    return m_stockList.size();
}

// 列数
int HistoryBoxModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

// 设置表格项数据
bool HistoryBoxModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int nColumn = index.column();
    stockRecord record = m_stockList.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    {
        if (nColumn == STOCK_CODE_COLUMN)
        {
            record.strCode = value.toString();
        }
        else if (nColumn == STOCK_NAME_COLUMN)
        {
            record.strName = value.toString();
        }
        else if (nColumn == STOCK_TYPE_COLUMN)
        {
            record.strType = value.toString();
        }
        m_stockList.replace(index.row(), record);
        emit dataChanged(index, index);
        return true;
    }
    case Qt::CheckStateRole:
    {
        if (nColumn == BUTTON_ADD_COLUMN)
        {
            record.bChecked = (value.toInt() == Qt::Checked);

            m_stockList.replace(index.row(), record);
            emit dataChanged(index, index);
            return true;
        }
    }
    default:
        return false;
    }
    return false;
}

// 表格项数据
QVariant HistoryBoxModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    stockRecord record = m_stockList.at(nRow);

    switch (role)
    {
    case Qt::TextColorRole:
        if(nColumn == STOCK_CODE_COLUMN || nColumn == STOCK_NAME_COLUMN)
            return m_styledWidget.ColFirColor();
        if(nColumn == STOCK_TYPE_COLUMN)
             return m_styledWidget.ColThrColor();
    case Qt::FontRole:
    {
        QFont font;
        if (nColumn == STOCK_CODE_COLUMN)
        {
            font.setPixelSize(14);
            font.setFamily("Arial");
            return QVariant(font);
        }
        if (nColumn == STOCK_NAME_COLUMN)
        {
            font.setPixelSize(14);
            font.setFamily("SimSun");
            return QVariant(font);
        }
        if (nColumn == STOCK_TYPE_COLUMN)
        {
            font.setPixelSize(14);
            font.setFamily("SimSun");
            return QVariant(font);
        }
    }
    case Qt::DisplayRole:
    {
        if (nColumn == STOCK_CODE_COLUMN)
        {
            return "   " + record.strCode;
        }
        else if (nColumn == STOCK_NAME_COLUMN)
        {
            return "   " + record.strName;
        }
        else if (nColumn == STOCK_TYPE_COLUMN)
        {
            // 之前返回的qint64，现在转换为字符串
            return record.strType;
        }

        return "";
    }
    case Qt::TextAlignmentRole:
    {
        if(index.column() == STOCK_TYPE_COLUMN)
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        else
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }
    case Qt::UserRole:
    {
        if (nColumn == BUTTON_ADD_COLUMN)
            return record.bChecked;
    }
    case MY_USERDATA_ROLE:
    {
        if (nColumn == BUTTON_ADD_COLUMN)
            return QVariant::fromValue(record);
    }
    case Qt::CheckStateRole:
    {
        if (nColumn == BUTTON_ADD_COLUMN)
            return record.bChecked ? Qt::Checked : Qt::Unchecked;
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 表格可选中
Qt::ItemFlags HistoryBoxModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == BUTTON_ADD_COLUMN)
         flags |= Qt::ItemIsUserCheckable;
    return flags;
}

