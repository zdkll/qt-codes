#include "baseitemmodel.h"

#define  StockDyna_Labels_File        "/cfg/StockDynaLabels.json"

#define Key_ModelName                    "modelName"
#define Key_Margin                           "Margin"

#define Key_Columns                         "columns"

#define Key_ColIdx                             "ColIdx"
#define Key_Label                              "Label"
#define Key_Width                             "Width"
#define Key_SortType                        "SortType"
#define Key_SortField                        "SortField"
#define Key_Alignment                       "Alignment"

#include <assert.h>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

///AbstractBaseModel----------
AbstractBaseModel::AbstractBaseModel(const QString &modelName,QObject *parent )
    :QAbstractTableModel(parent)
{
    readColumnsInfo(modelName);
}

int AbstractBaseModel::columnCount(const QModelIndex &) const
{
    return m_columnInfos.size();
}

QVariant  AbstractBaseModel::indexData(const int &row,const ColumnIdx &inColumn) const
{
    int column = m_colIdxMap.value(inColumn,-1);
    if(column != -1){
        return this->index(row,column).data(Stock_Data_Role);
    }
    return QVariant() ;
}

void AbstractBaseModel::setColumnLabel(int column,const QString &label)
{
    m_columnInfos[column].label = label;
}

void AbstractBaseModel::setColumnInfo(int column,const ColumnInfo &colInfo)
{
    assert(0<=column && column<m_columnInfos.size());
    m_columnInfos.replace(column,colInfo);
}

QVariant  AbstractBaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal){
        if(role == Qt::DisplayRole)
            return m_columnInfos[section].label;
        if(role == Qt::TextAlignmentRole)
            return  (Qt::AlignVCenter | Qt::Alignment(m_columnInfos[section].alignment));
    }

    return QVariant();
}

void AbstractBaseModel::readColumnsInfo(const QString &modelName)
{
    QFile file(QApplication::applicationDirPath() + StockDyna_Labels_File);
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array =  doc.array();
    auto  it = array.begin();
    while(it!=array.end()){
        QJsonObject obj = (*it).toObject();
        if(obj.value(Key_ModelName).toString() == modelName){

            QJsonArray columnArray = obj.value(Key_Columns).toArray();

            auto  colIt = columnArray.begin();
            while(colIt != columnArray.end())
            {
                QJsonObject colObj = (*colIt).toObject();
                ColumnInfo info;
                info.idx    =  (ColumnIdx)colObj.value(Key_ColIdx).toInt();
                info.label = colObj.value(Key_Label).toString();
                info.width = colObj.value(Key_Width).toDouble();
                info.sortType = (SortType)colObj.value(Key_SortType).toInt();
                int align = colObj.value(Key_Alignment).toInt();
                info.alignment = Qt::AlignVCenter|Qt::Alignment(align);
                if(info.sortType == CustomSort){
                    m_sortFieldIdxMap.insert(m_columnInfos.size(),(SortField)colObj.value(Key_SortField).toInt());
                }

                m_colIdxMap.insert(info.idx,m_columnInfos.size());
                m_idxColMap.insert(m_columnInfos.size(),info.idx);
                m_columnInfos.append(info);

                colIt++;
            }
            break;
        }
        it++;
    }
}

