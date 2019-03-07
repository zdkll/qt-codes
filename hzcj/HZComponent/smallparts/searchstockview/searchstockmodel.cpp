#include "searchstockmodel.h"
#include <QFont>

SearchStockModel::SearchStockModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

int SearchStockModel::rowCount(const QModelIndex &parent) const
{
    return m_list.size();
}

int SearchStockModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SearchStockModel::data(const QModelIndex &index, int role) const
{
    int nCol = index.column();
    int row = index.row();
    QString strCode = m_list[row].strCode;
    switch (role) {
    case Qt::DisplayRole:
    {
        if(0 == nCol)
        {
            return "     " +strCode.remove(0,2);
        }
        else
        {
            return m_list[row].strName;
        }
    }
    case Qt::FontRole:
    {

        if(0 == nCol)
        {
            QFont font;
            font.setBold(true);
            font.setFamily("Airal");
            font.setPixelSize(12);
            return font;
        }
        else
        {
            QFont font;
            font.setFamily("SimSun");
            font.setPixelSize(12);
            return font;
        }
    }
    case CODEROLE:
        return m_list[row].strCode;
    default:
        break;
    }
    return QVariant();
}

void SearchStockModel::upDateData(QList<SearchStockData> &list)
{
    m_list = list;

    this->beginResetModel();
    this->endResetModel();
}

void SearchStockModel::clearData()
{
    m_list.clear();
    this->beginResetModel();
    this->endResetModel();
}
