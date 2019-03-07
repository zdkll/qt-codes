/********************************************************************
created:2018.10.31
author: juntang.xie
purpose:派生自QAbstractTableModel 自定义龙虎榜买卖表格模型
*********************************************************************/

#ifndef LHBUYSELLMODEL_H
#define LHBUYSELLMODEL_H
#include <QAbstractTableModel>
#include "lhbuysellstyledwiget.h"
#include "hzinfodata.h"

#define LHYYBIDROLE  Qt::UserRole + 1

#pragma execution_character_set("utf-8")

class LhBuySellModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LhBuySellModel(QObject *parent = nullptr, int nType = 0);
    //返回表格行数
    virtual int rowCount(const QModelIndex &parent) const;
    //返回表格列数
    virtual int columnCount(const QModelIndex &parent) const;
    //返回索引引用的项目的给定角色下存储的数据
    virtual QVariant data(const QModelIndex &index, int role) const;
    //返回表头数据
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialListItem &data);

private:
    void refrushModel();

private:
    QStringList m_lists;

    LhBuySellStyledWiget m_styledWidget;

    QVector<StockLhbDetialItem> m_modeData;

    int m_nType;
};

#endif // LHBUYSELLMODEL_H
