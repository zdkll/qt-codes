/********************************************************************
created:2019.2.22
author: juntang.xie
purpose:搜索列表模型
*********************************************************************/
#ifndef SEARCHSTOCKMODEL_H
#define SEARCHSTOCKMODEL_H
#include <QAbstractTableModel>

#define CODEROLE (Qt::UserRole + 1)


struct SearchStockData{
    QString strCode;    //股票代码
    QString strName;    //股票名称
};

class SearchStockModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SearchStockModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;


    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;


    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


    void upDateData(QList<SearchStockData> &list);

    void clearData();

private:
    QList<SearchStockData> m_list;

};

#endif // SEARCHSTOCKMODEL_H
