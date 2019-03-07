/********************************************************************
created: 2018/08/07
author: dk.zhang
purpose:资金流向基类窗口
*********************************************************************/
#ifndef MONEYFLOWBASEVIEW_H
#define MONEYFLOWBASEVIEW_H

#include <QWidget>

#include "basewidget.h"
#include "frozentableview.h"
#include "viewstyleproperties.h"
#include "itemmodels/fundmodel.h"
#include "sortfilterproxymodel.h"
#include "hzcomponent_global.h"

//资金
class FundHeaderView;
class MoneyFlowTabView;
class BaseFundProvider;

class HZCOMPONENTSHARED_EXPORT FundBaseView : public BaseWidget, public ViewStyleProperties
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    enum MoneyFlowType
    {
        StockMoney = 0, //股票
        BKMoney     = 1, //板块
        IndexMoney = 2,  //指数
        BKStockMoney = 3, //板块和成分股
        OptionMoney = 4
    };
    //@param[in] 资金类型，分为个股，板块，板块及其成分股
    explicit FundBaseView( FrozenView::FrozenFlag frozenFlag,MoneyFlowType type,QWidget *parent = 0);

    // @brief 窗口激活和非激活状态
    virtual void afterActive();
    virtual void afterDeactive();

    // @brief 分别返回过滤值和板块名
    int filterFlag() const {return m_filterFlag;}
    QString filterDetail() const { return m_filterDetail;}

    // @retval 当前表格的股票码表
    //    virtual const QVector<HZData::StockCode> &stockTable() = 0;

    // @brief 获取显示窗口对应的表格
    FrozenBaseTableView   *tableView(){return m_tableView;}
    // @retval 资金表格的表头对象
    FundHeaderView  *headerView(){return m_headerView;}

    //@brief 获取水平和垂直滚动条
    QScrollBar *horizontalScrollBar();
    QScrollBar *verticalScrollBar();
public slots:
    //@param[in] filterFlag 分类,filter_detail 股票名称
    virtual void setFilterFlag(const int &filterFlag);
    virtual void setFilterDetail(const QString &filter_detail);
    virtual void setFilterInfo(const int &filterFlag,const QString &filter_detail);

signals:
    void itemEntered(const QString &stockCode);
    void itemSelected(const QString &stockCode);

protected slots:
    void showEvent(QShowEvent *e);
    void keyPressEvent(QKeyEvent *e);

    void slotEnteredItem(const QModelIndex &index);
    void slotItemSelected(const QModelIndex &index);
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotCurrentRangeChanged();
    void slotModelReset();


protected:
    void  initTabView(FundBaseModel *model,BaseFundProvider *dataProvider,bool sort = false);
    void  initItemDelegate();

    virtual void sortIndicatorChanged(const SortField &,const bool &desc );
    virtual void currentRangeChanged(int start ,int  count);
    virtual void modelReset();

    FundBaseModel                *m_dataModel;
    BaseSortFilterProxyModel *m_sortModel;
    QVector<ColumnInfo>        m_columnInfos;

    BaseFundProvider             *m_moneyProvider;
private:
    void  initWg(FrozenView::FrozenFlag frozenFlag);

    int         m_filterFlag;
    QString m_filterDetail;
    SortIndicator                    m_sortIndicator;
    FundHeaderView            *m_headerView;
    FrozenBaseTableView    *m_tableView;
    QScrollBar                     *m_horizScrollbar,*m_vertScrollbar;

    MoneyFlowType m_fundFlowType;
};

#endif // MONEYFLOWBASEVIEW_H
