/********************************************************************
created:2018/08/07
author: dk.zhang
purpose: 所有的资金列表
*********************************************************************/
#ifndef FUNDVIEW_H
#define FUNDVIEW_H

#include <QWidget>
#include "fundbaseview.h"
#include "fundprovider.h"

#include "hzcomponent_global.h"

//板块\个股\指数 资金
class   FundFlowView : public FundBaseView
{
    Q_OBJECT
public:
    FundFlowView(MoneyFlowType type,QWidget *parent = 0);
};

//板块及其成分股
class  BKStockFundFlowView : public FundBaseView
{
public:
    BKStockFundFlowView(QWidget *parent = 0);

protected:
    void modelReset();
};

//固定股票数资金:自选/历史浏览/重要资金
class  HZCOMPONENTSHARED_EXPORT  OptionFundFlowView : public FundBaseView
{
public:
    enum OptFundType
    {
        OFT_ZXStock  = 0, //自选
        OFT_HistStock = 1,//历史
        OFT_ImpIndex = 2 //重要指数
    };
    OptionFundFlowView(OptFundType type,QWidget *parent = 0);

protected:
    void           keyPressEvent(QKeyEvent *e);
    virtual void modelReset();
    OptionFundProvider  *createDataProvider(FundBaseModel *model,OptFundType type);

private:
OptFundType m_optFundType;
OptionFundProvider *m_optFundProvider;
};


//特殊-首页资金净流入列表,左右固定，上下拖动
class  SLFundNetputView : public QTableView,public HZObject, public ViewStyleProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    SLFundNetputView(QWidget *parent = 0);

    void  afterActive();
    void  afterDeactive();

    void setSortOrder(bool desc);

    void setStockCount(int count);

    void onFocusOut();

    void setFilterFlag(int filterFlag);
signals:
    void itemSelected(const QString &);
    void itemEntered(const QString &);

protected:
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void currentChanged(const QModelIndex &current,
                        const QModelIndex &previous) Q_DECL_OVERRIDE;

protected slots:
    void slotEnteredItem(const QModelIndex &index);

private:
    void initTableView();
    void initItemDelegate(const QVector<ColumnInfo> &columnInfos);

private:
    FundBaseModel          *m_dataModel;
    SortFilterProxyModel   *m_sortModel;
    SLStockFundProvider  *m_fundProvider;

    QVector<ColumnInfo> m_columnInfos;
    int                               m_rowCount;
    QPair<int,int>              m_rowRange;
};

#endif // FUNDVIEW_H
