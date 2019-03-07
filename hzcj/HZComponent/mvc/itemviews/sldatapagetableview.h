/********************************************************************
created:
author: dk.zhang
purpose: 数据页面缩略列表
*********************************************************************/
#ifndef SLDATAPAGETABLEVIEW_H
#define SLDATAPAGETABLEVIEW_H

#include <QTableView>
#include "baseitemmodel.h"
#include "viewstyleproperties.h"
#include "hzobject.h"
#include "hzcomponent_global.h"

class BaseSortFilterProxyModel;
class AbstractDataBaseModel;
class BaseDataProvider;
class StockRzrqRankProvider;
class  SLDataPageTableView : public QTableView,public HZObject, public ViewStyleProperties
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    SLDataPageTableView(QWidget *parent = Q_NULLPTR);

    virtual void afterActive();
    virtual void afterDeactive();

signals:
    void itemEntered(const QString &stockCode);
    void itemSelected(const QString &stockCode);

protected slots:
    void slotCurrentRangeChanged();
    void slotModelReset();
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

protected:
    //子类自定义函数
    virtual void currentRangeChanged(int start,int count);

    virtual void sortIndicatorChanged(const SortField &sortField,const bool &desc);

    virtual void slotEnteredItem(const QModelIndex &index);

    virtual void modelReset(){}

    //sort
    void initTableView(AbstractDataBaseModel *model,BaseDataProvider *dataProvider,bool sort = false);

    void keyPressEvent(QKeyEvent *e);
    void currentChanged(const QModelIndex &current,
                        const QModelIndex &previous) Q_DECL_OVERRIDE;
    void  showEvent(QShowEvent *e);
    void  resizeEvent(QResizeEvent *e);

    QVector<ColumnInfo>              m_columnInfos;
    AbstractDataBaseModel         *m_dataModel;
    BaseSortFilterProxyModel       *m_sortModel;

    BaseDataProvider                   *m_dataProvider;

private:
    void initItemDelegate(const QVector<ColumnInfo> &columnInfos);

    bool                                       m_isAutoSelected;
    SortIndicator                          m_sortIndicator;
};

//融资融券个股净买入卖出额排行
class  StockRzrqRankTableView : public SLDataPageTableView
{
public:
    enum RzrqRankType
    {
        Rzrq_NetInput = 0,  //净买入额
        Rzrq_NetOutput = 1//净卖出额
    };
    StockRzrqRankTableView(RzrqRankType type,QWidget *parent = Q_NULLPTR);

    void setTime(UINT64 time_t);

private:
    RzrqRankType m_rzrqRankType;
    StockRzrqRankProvider  *m_rzrqRankProvider;
};


#endif // SLDATAPAGETABLEVIEW_H
