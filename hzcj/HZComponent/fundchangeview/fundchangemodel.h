/********************************************************************
created:2018.10.30
author: juntang.xie
purpose:派生自QAbstractTableModel 自定义资金异动表格模型
*********************************************************************/
#ifndef FundChangeModel_H
#define FundChangeModel_H
#include <QAbstractTableModel>
#include "datacontroller.h"
#include "fundchangestyledwidget.h"
using namespace HZData;
#define MYDATAROLE  Qt::UserRole + 1


struct FundData
{
    QString strName;
    QString strCode;
    QString strTime;
    QString strType;
    int short_type ;
};

Q_DECLARE_METATYPE(FundData)

enum Field{
    FieldCode = 0,
    FieldName,
    FieldTime
};

class FundChangeModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FundChangeModel(QObject *parent = nullptr);

    //返回列表行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //返回列表列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //返回索引引用的项目的给定角色下存储的数据。
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //更新数据
    void upDateData(const QVector<ShortMotion> &fundVec);

    //清除数据
    void clearData();

private:
    void refrushModel();
    void translateData(const QVector<ShortMotion> &fundVec);
    QString textType(ShortMotionType type);
    QString getText(const ShortMotion &Motion, Field field);

private:
    FundData m_fundData;

    QVector<FundData> m_fundDataVec;

    FundChangeStyledWidget m_styledWidget;
};

#endif // FundChangeModel_H
