/********************************************************************
created:
author: dk.zhang
purpose:行情页左侧股票导航栏
*********************************************************************/
#ifndef NAVILISTVIEW_H
#define NAVILISTVIEW_H

#include <QListView>
#include "basewidget.h"
#include "baseitemmodel.h"
#include "navistockdynamodel.h"
#include "sortfilterproxymodel.h"

#include "hzcomponent_global.h"
#include "viewstyleproperties.h"
#include "navistockprovider.h"

#include "addzxstockview.h"
#include "hzobject.h"

class   NaviListView : public QListView ,public HZObject,public NaviStyledProperities
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)

    Q_PROPERTY(QColor numberColor READ numberColor WRITE setNumberColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

public:
    NaviListView(QWidget *parent = 0);

    void afterActive();
    void afterDeactive();
    //设置排序 HZData::none_sort，表示回复原始顺序
    void setSortIndicator(SortField sortField,bool is_desc);
    //设置当前股票
    //void setStockCode(const QString& stockCode);
    virtual void   setFilterFlag(int filterFlag);
    virtual void   sortIndicatorChanged(SortField sortField,bool is_desc);

    //相对位置切换股票
    void switchToUpDownStock(int isUp,int step);

    void setAutoSelected(bool enable = true);

signals:
    void  selectedItem(const QString &stockCode);

protected slots:
    void   itemClicked(const QModelIndex &index);
    virtual void  currentRangeChanged(int start,int count);
    void   slotModelReset();
    void   slotCurrentRangeChanged();

protected:
    void  currentChanged(const QModelIndex &current, const QModelIndex &previous);

    //SortRole: Stock_No_Role Stock_Rate_Role
    void initListView(NaviStockDynaModel *stockDynaModel,NaviStockProvider *dataProvider,int sortRole = Stock_No_Role);
    void showEvent(QShowEvent *e);
    void selectStock(const QString &stockCode);

    SortIndicator                         m_sortIndicator;
    NaviStockDynaModel           *m_dataModel;
    NaviSortFilterProxyModel     *m_sortModel;

    NaviStockProvider                *m_dataProvider;
    QHash<SortType,int>            m_sortRoleHash;
    bool m_isAutoSelected = false;
    QItemSelectionModel          *m_selectionModel;
};


//自选股，历史浏览:不可排序，按照默认顺序排列
class   NaviZXHistListView : public NaviListView
{
    Q_OBJECT
public:
    NaviZXHistListView(LocalStockType type,QWidget *parent = 0);

signals:
    void   rowCountChanged(int count);

};


//自选股View
class  NaviZXListView : public BaseWidget
{
    Q_OBJECT
public:
    NaviZXListView(QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    void setStockCode(const QString &stock);

    void switchToUpDownStock(bool isUp,int step);

    void  setListViewAutoSelected(bool enable = true){m_NaviZXListView->setAutoSelected(enable);}
signals:
    void  selectedItem(const QString &stockCode);
    void signalRowCountChanged(int count);

protected slots:
    void rowCountChanged(int );

private:
    NaviZXHistListView *m_NaviZXListView;
    AddZXStockView    *m_addZXStockView;
};


//指数, 初始按照原始顺序排，可以按照涨跌幅排序
class  NaviIndexListView : public NaviListView
{
    Q_OBJECT
public:
    NaviIndexListView(QWidget *parent = 0);

    void setSortOrder(bool is_desc);

    virtual void   sortIndicatorChanged(SortField sortField,bool is_desc);

    void OnStockCodeChange();
};



//板块，filterFlag 切换，可按照涨跌幅排序
class  NaviBlockListView : public NaviListView
{
public:
    NaviBlockListView(QWidget *parent = 0);

};

#endif // NAVILISTVIEW_H
