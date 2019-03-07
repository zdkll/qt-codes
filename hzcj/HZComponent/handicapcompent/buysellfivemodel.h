/********************************************************************
created:2018.9
author: juntang.xie
purpose: 买卖五档模型类
*********************************************************************/
#ifndef BUYSELLFIVEMODEL_H
#define BUYSELLFIVEMODEL_H
#include <QAbstractTableModel>
#include "datacontroller.h"
#include "quoteformat.h"
#define MyDataRole Qt::UserRole + 1

#pragma execution_character_set("utf-8")

/**
  * @brief 买卖五档数据结构
  */
typedef struct
{
    int nType;
    QString strTitle;
    QString strPrice;
    QString strVolume;
}DATABUYSELL;

Q_DECLARE_METATYPE(DATABUYSELL)

/**
  * @brief 买卖五档数据模型
  */
class BuySellFiveModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BuySellFiveModel(int type = 0, QObject *parent = nullptr);

    //返回数据行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //返回数据列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //返回索引引用的项目的给定角色下存储的数据
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);


private:
    void refrush();
    void init();
    void formatData(const HZData::AllStock &stock);
    void onStockOptionalDataArrived(ReqId id ,const QVector<HZData::AllStock > &stock, ErrCode errCode );

private:
    HZData::AllStock m_stock;

    DATABUYSELL m_dataBuy[5];
    DATABUYSELL m_dataSell[5];
    QStringList m_buyList;
    QStringList m_sellList;

    int m_nType;
};

#endif // BUYSELLFIVEMODEL_H
