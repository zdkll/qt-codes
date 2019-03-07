/********************************************************************
created: 2018.9.29
author: juntang.xie
purpose: 历史框模型
*********************************************************************/
#ifndef HistoryBoxModel_H
#define HistoryBoxModel_H
#include <QAbstractTableModel>
#include <QWidget>
#include <QDateTime>
#include <QColor>
#define STOCK_CODE_COLUMN 0   // 股票代码
#define STOCK_NAME_COLUMN 1   // 股票名称
#define STOCK_TYPE_COLUMN 2   // 股票类型
#define BUTTON_ADD_COLUMN 3   // 添加自选按钮
#define MY_USERDATA_ROLE Qt::UserRole+1
#pragma execution_character_set("utf-8")
/**
  * @brief 历史框列表数据
  */
typedef struct stockRecord
{     QString strCode; // 股票代码
      QString strName;  // 股票名称
        QString strType;  // 股票类型
          bool bChecked;  //是否是自选股

} StockRecord;
Q_DECLARE_METATYPE(StockRecord)

/**
  * @brief 历史框字体颜色设置窗体
  */
class HistoryStyledWidget:public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor ColFirColor READ ColFirColor WRITE setColFirColor DESIGNABLE true)
    Q_PROPERTY(QColor ColThrColor READ ColThrColor WRITE setColThrColor  DESIGNABLE true)
public:
    HistoryStyledWidget(QWidget* parent = nullptr):QWidget(parent){}
    ~HistoryStyledWidget(){}

    //获取第1列颜色
    QColor ColFirColor() const{ return m_colFirColor;}
    //设置第1列颜色
    void setColFirColor(const QColor color){ m_colFirColor = color;}
    //获取第3列颜色
    QColor ColThrColor() const{ return m_colThrColor;}
    //设置第3列颜色
    void setColThrColor(const QColor color){ m_colThrColor = color;}

private:
    QColor m_colFirColor;
    QColor m_colThrColor;
};

/**
  * @brief 搜索框列表模型
  */
class HistoryBoxModel:public QAbstractTableModel
{
public:
    HistoryBoxModel(QObject *parent = nullptr);
    ~HistoryBoxModel();

    //更新数据
    void updateData(QList<StockRecord> stockList);
    //返回列表行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //返回列表列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    //返回索引引用的项目的给定角色下存储的数据
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //将index的项目的角色数据设置为value
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    //返回给定索引的项标志
    virtual Qt::ItemFlags
    flags(const QModelIndex &index) const;

private:
    QList<stockRecord> m_stockList;

    HistoryStyledWidget m_styledWidget;
};


#endif // HistoryBoxModel_H
