/********************************************************************
created: 数据页面列表
author: dk.zhang
purpose:
*********************************************************************/
#ifndef DATAPAGETABLEVIEW_H
#define DATAPAGETABLEVIEW_H

#include "frozentableview.h"
#include "hzobject.h"
#include "baseitemmodel.h"
#include "hzcomponent_global.h"
#include "viewstyleproperties.h"

#include "hzinfocenter.h"

class BaseSortFilterProxyModel;
class AbstractDataBaseModel;
class BiddingItemModel;
class RFMonitorItemModel;
class StockRzrqDetailItemModel;
class XgrlStockProvider;
class BaseDataProvider;

class LhbYybDrbProvider;
class LhbStockProvider;
class YybSlbDrbProvider;
class LhbYybSlbProvider;
class LhbYybHybProvider;

class StockRzrqRankProvider;
class StockRzrqDetailProvider;
class BlockRzrqProvider;
class BlockStockRzrqProvider;

class StockDzjyProvider;

class BaseStockDynaHeaderView;

class LhbYybSbItemModel;
class LhbYybHoItemModel;

class StockStyledItemDelegate;
//固定前几列
class  DataPageBaseView : public FrozenView,public ViewStyleProperties,public HZObject
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    enum EnterType   //双击条目类型
    {
        EnterStock = 0, //进入股票
        EnterYyb = 1     //进入三营业部
    };
    DataPageBaseView(QWidget *parent = 0);
    virtual ~DataPageBaseView();

    void afterActive();
    void afterDeactive();

    void setSortIndicatorShown(bool show);

    virtual void setSortIndicator(SortField sortField,bool desc){}

signals:
    void itemSelected(const QString &stockCode);  //选中某股票
    void itemEntered(const QString &stockCode);   //双击或者Enter进入某股票

protected slots:
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotCurrentRangeChanged();
    void slotModelReset();
    void selectedItem(const QModelIndex &index);
    void enteredItem(const QModelIndex &index);

    virtual void showCountChanged(int count){}
    virtual void currentRangeChanged(int start,int count);
    virtual void sortIndicatorChanged(SortField sortField, bool is_desc);
    virtual void modelReset(){}

protected:
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);

    void setEnterType(EnterType type){m_enterType = type;}
    void initTableView(AbstractDataBaseModel *model,BaseDataProvider *provider,bool sort = false,bool zxstock = false);
    void initItemDelegates();

    AbstractDataBaseModel      *m_dataModel;
    BaseSortFilterProxyModel    *m_sortModel;
    BaseDataProvider                *m_dataProvider;

    BaseStockDynaHeaderView *m_headerView;

    QVector<ColumnInfo>           m_columnInfos;
private:
    SortIndicator                          m_sortIndicator;
    EnterType                              m_enterType;
    int m_showCount = 0;
};


//集合竞价列表
class  BiddingTableView : public DataPageBaseView
{
public:
    //集合竞价类型
    enum BiddingType
    {
        Bid_LeaderFund = 0, //主力资金
        Bid_QuantityRatio, //量比
        Bid_Rate,      //竞价涨幅
        Bid_PreRate, //昨收涨跌幅
        Bid_ZXStock  //自选
    };
    BiddingTableView(BiddingType type,QWidget *parent = Q_NULLPTR);

private:
    QString                     getModelName(BiddingType type);
    BaseDataProvider    *createDataProvider(BiddingItemModel *model,BiddingType type);

    BiddingType              m_bidType;
};


//涨停监测
class  RiseFallMonitorView : public DataPageBaseView
{
public:
    RiseFallMonitorView(HZData::RFMonitorType type,QWidget *parent = Q_NULLPTR);

private:
    QString                  getModelName(HZData::RFMonitorType type);
    HZData::RFMonitorType  m_RFMtype;
};

//资金异动
class   FundAbnormalView : public DataPageBaseView
{
    Q_OBJECT
public:
    FundAbnormalView(HZData::FundAbnormalType type,QWidget *parent = Q_NULLPTR);

protected slots:
    void slotSeletedStock(const QString &stockCode);
private:
    HZData::FundAbnormalType m_fatType;
};


//龙虎榜-个股榜单
class  LhbStockView : public DataPageBaseView
{
public:
    LhbStockView(StockLhbType  type,QWidget *parent = Q_NULLPTR);

    void setStockLhbType(StockLhbType  type);
    void setTime(UINT64 time_t);
    void currentRangeChanged(int  ,int  );

private:
    StockLhbType          m_stkLhbType;
    LhbStockProvider   *m_lhbStockProvider;
};

//龙虎榜-营业部当日榜
class  LhbYybDrbView : public DataPageBaseView
{
public:
    LhbYybDrbView(QWidget *parent = Q_NULLPTR);

    void setTime(UINT64 time_t);

protected:
    void currentRangeChanged(int  ,int );

private:
    LhbYybDrbProvider   *m_YybDrbProvider;
};

//龙虎榜-营业部实力榜
class  LhbYybSlbView : public DataPageBaseView
{
public:
    LhbYybSlbView(QWidget *parent = Q_NULLPTR);

protected:
    void currentRangeChanged(int  ,int );
private:
    LhbYybSlbProvider   *m_YybSlbProvider;
};


//龙虎榜-营业部活跃榜
class  LhbYybHybView : public DataPageBaseView
{
public:
    LhbYybHybView(QWidget *parent = Q_NULLPTR);

protected:
    void currentRangeChanged(int  ,int );

private:
    LhbYybHybProvider   *m_YybHybProvider;
};


//龙虎榜-营业部详细-上榜次数
class  LhbYybSbView : public DataPageBaseView
{
public:
    LhbYybSbView(QWidget *parent = Q_NULLPTR);

    void setYybDetailStatistics(const QVector<StockYybDetailStatistics>& statistics);

private:
    LhbYybSbItemModel *m_dataModel;
};

//龙虎榜-营业部详细-历史操作
class  LhbYybHoView : public DataPageBaseView
{
    Q_OBJECT
public:
    LhbYybHoView(QWidget *parent = Q_NULLPTR);

    void setYybDetaiHistory(const  QVector<StockYybDetaiHistory>& history);
protected:
    void modelReset(){}

private:
    LhbYybHoItemModel *m_dataModel;
};

//大宗交易
class  StockDzjyView : public DataPageBaseView
{
public:
    StockDzjyView(QWidget *parent = 0);

    void setTime(UINT64 time_t);

private:
    StockDzjyProvider *m_stkDzjyProvider;

};

//融资融券-个股买入榜
class  StockRzrqView : public DataPageBaseView
{
public:
    StockRzrqView(QWidget *parent = 0);

    void setTime(UINT64 time_t);

protected:
    void showCountChanged(int count);

private:
    StockRzrqRankProvider *m_stkRzrqProvider;
};

//融资融券-个股详情
class  StockRzrqDetailView : public DataPageBaseView
{
public:
    StockRzrqDetailView(QWidget *parent = 0);

    void setDataList(QVector<StockRzrqDetailItem> &dataList);

    //void setStockCode(const QString &stockCode);

    //void setTime(UINT64 time_t);

private:
    StockRzrqDetailProvider     *m_stkRzrqDetailProvider;
    StockRzrqDetailItemModel *m_model;
};

//融资融券-概念板块
class  BlockRzrqView : public DataPageBaseView
{
public:
    BlockRzrqView(QWidget *parent = 0);

    void setTime(UINT64 time_t);

private:
    BlockRzrqProvider *m_blockRzrqProvider;
};


//融资融券-板块成分股
class  BlockStockRzrqView : public DataPageBaseView
{
    Q_OBJECT
public:
    BlockStockRzrqView(QWidget *parent = 0);

    void setTime(UINT64 time_t);

    void setStockCode(const QString &blockCode);

signals:
    void bkStockCountChanged(int );

private:
    BlockStockRzrqProvider *m_bkStkRzrqProvider;
};




//新股日历--新股预告
class  XgrlTableView : public DataPageBaseView
{
    Q_OBJECT
    Q_PROPERTY(QPixmap filePixmap  READ filePixmap WRITE setFilePixmap)
    Q_PROPERTY(QPixmap fileHoverPixmap READ fileHoverPixmap WRITE setFileHoverPixmap)
    Q_PROPERTY(QPixmap ygPixmap READ ygPixmap WRITE setYgPixmap)
public:
    XgrlTableView(XgrlType type,QWidget *parent = 0);
    void setTime(UINT64 timet);

protected slots:
    void slotItemClicked(const QModelIndex &index);

protected:
    void showEvent(QShowEvent *e);

private:
    QString getModelName(XgrlType type);

    XgrlType                 m_xgrlType;
    XgrlStockProvider  *m_xgrlProvider;
    int                           m_fileColumn;

    static  bool m_isSettedPixmap;
};


#endif // DATAPAGETABLEVIEW_H
