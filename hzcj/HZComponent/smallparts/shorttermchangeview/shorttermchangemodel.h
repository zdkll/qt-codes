/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自QAbstractTableModel 自定义短线异动表格模型
*********************************************************************/
#ifndef ShortTermChangeModel_H
#define ShortTermChangeModel_H
#include <QAbstractTableModel>
#include "datacontroller.h"
#include "ShortTermChangestyledwidget.h"
using namespace HZData;
#define MYDATAROLE  Qt::UserRole + 1


struct ShortTermData
{
    QString strName;
    QString strCode;
    QString strTime;
    QString strType;
    QString strVal;
    int short_type ;
};

Q_DECLARE_METATYPE(ShortTermData)

enum Field{
    FieldCode = 0,
    FieldName,
    FieldTime,
    FieldVal
};

class ShortTermChangeModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ShortTermChangeModel(QObject *parent = nullptr);

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
    QString getStrVal(const ShortMotion &shortMotion);

private:
    ShortTermData m_ShortTermData;

    QVector<ShortTermData> m_shortTermDataVec;

    ShortTermChangeStyledWidget m_styledWidget;
};

#endif // ShortTermChangeModel_H
