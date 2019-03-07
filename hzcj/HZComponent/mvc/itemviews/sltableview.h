/********************************************************************
created:2018/07/16
author: dk.zhang
purpose:行情列表缩略图,包括基类和个股、板块和自选股缩略
*********************************************************************/
#ifndef SLTABLEVIEW_H
#define SLTABLEVIEW_H

#include "datamodeldef.h"
#include "stockdynaprovider.h"
#include "fundprovider.h"

#include "hzcomponent_global.h"
#include "basewidget.h"
#include "slbasetableview.h"


//行列固定范围缩略列表,行数根据View高度计算填满
class   SLTableView : public SLBaseTableview
{
    Q_OBJECT
public:
    SLTableView(QWidget *parent = 0);

protected:
    //计算显示行数
    void resizeEvent(QResizeEvent *e);

    void currentRangeChanged(int  ,int  ){}

private:
    int   m_rowCount;
};

//列范围固定，显示固定行数(根据View高度) ，订阅全部
class   SLFixedRowTableView :public SLBaseTableview
{
public:
    SLFixedRowTableView(QWidget *parent = 0);

    //是否显示排序标记
    void setSortIndicatorShown(bool show);
    //设置排序
    virtual void setSortIndicator(SortField sortField,bool desc);

    void setRowRange(int min,int max);

protected:
    void  resizeEvent(QResizeEvent *e);

    void currentRangeChanged(int  ,int  ){}

private:
    int   m_rowCount;
    QPair<int,int > m_rowRange;
};

//一般的行情缩略图，固定列范围，行数根据Filter 分类计算
class  SLDynaTableView : public SLBaseTableview
{
    Q_OBJECT
public:
    enum SLDynaType
    {
        SLStock = 0,
        SLBlock = 1   //板块缩略图
    };
    SLDynaTableView(SLDynaType type,QWidget *parent = 0);

protected slots:
    virtual void slotEnteredItem(const QModelIndex &index);

protected:
    void modelReset();
private:
    AbstractBaseStockModel *createModel(SLDynaType type);

    SLDynaType  m_slDynaType;
};


//沪深综合排行列表,显示一定的行数排行
class  HSZHTableView : public  SLFixedRowTableView
{
public:
    enum HSZHDynaType
    {
        RiseFallRate = 0,//涨跌幅
        Turnover = 1,//成交e
        TuvRate = 2,//换手率
        ThreeMinRate = 3,//3分钟涨跌
        FiveDayRate,//5日涨跌幅
        FromYearYet,//年初至今涨跌幅
    };
    HSZHTableView(HSZHDynaType type, QWidget *parent = Q_NULLPTR);

    void initSortIndicator(SortField sortField,bool desc);

private:
    AbstractBaseStockModel *createModel(HSZHDynaType type);
    HSZHDynaType m_hszhDynaType;
};

//自选，历史浏览，相关板块等缩略列表，股票数通过查询获取，只订阅快照,自动排序
class  SLOptionTableView : public SLBaseTableview
{
    Q_OBJECT
public:
    enum OptionDynaType
    {
        ZXStocks  = 0, //自选股
        HistStocks = 1, //历史浏览
        RelBlocks  = 2  //相关板块
    };
    SLOptionTableView(OptionDynaType type,QWidget *parent = Q_NULLPTR);

protected:
    void currentRangeChanged(int ,int){}
    void keyPressEvent(QKeyEvent *e);

private:
    AbstractBaseStockModel *createModel(OptionDynaType type);
    BaseStockDynaProvider   *createDynaProvider(AbstractBaseStockModel *model,OptionDynaType type);
    HZData::StockFieldMask   getMask(OptionDynaType type);

    OptionDynaType m_optionType;
};


//相关板块列表图
class  RelationBlockListView : public SLOptionTableView
{
    Q_OBJECT
public:
    RelationBlockListView(QWidget *parent = 0);

    //    //获取当期股票行情
    //    QVector<HZData::AllStock> stocks(){return m_dataModel->stocks();}

signals:
    void signalsItemSelected(const int& filterFlag,const QString &stockCode);
    void signalsItemRiseFallFlatChanged(const QString& bkStock,int rise,int fall,int flat);

protected slots:
    void slotsDataChanged();
    void slotsItemSeleced(const QString &stockCode);

protected:
    void OnStockCodeChange();
    void postRff();

protected:
    QString             m_selectedBkCode;
    int m_rise;
    int m_fall;
    int m_flat;
};






#endif // SLSTOCKDYNATABLEVIEW_H
