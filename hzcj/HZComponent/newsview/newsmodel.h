/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QAbstractTableModel 自定义公告资讯表格模型
*********************************************************************/
#ifndef NEWSMODEL_H
#define NEWSMODEL_H
#include <QAbstractTableModel>
#include "vlistdef.h"

#define MYDATAROLE  Qt::UserRole + 1

class NewsView;
class NewsModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NewsModel(QObject *parent = nullptr);

    //返回列表行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //返回列表列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //返回索引引用的项目的给定角色下存储的数据。
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //更新列表数据
    void updateList(const QList<STVListData> &datas);

    //返回当前已有数据列表
    QList<STVListData> getRecords() const { return m_dataList;}

    /**
     * @brief clearAll
     */
    void clearAll();

private:
    void refrush();

private:
    QList<STVListData> m_dataList;
    friend class NewsWidget;
};

#endif // NEWSMODEL_H
