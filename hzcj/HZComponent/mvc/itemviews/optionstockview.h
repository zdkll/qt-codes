/********************************************************************
created:
author: dk.zhang
purpose:自选股和历史浏览等确定股票的列表
*********************************************************************/
#ifndef ZXHISTSTOCKVIEW_H
#define ZXHISTSTOCKVIEW_H

#include "stockdynabaseview.h"
#include "optiondynaprovider.h"
#include "addzxstockview.h"
#include "hzcomponent_global.h"

//固定股票数的显示列表
class  OptionDynaView : public StockDynaBaseView
{
public:
    OptionDynaView(ViewType type ,QWidget *parent = 0);

protected:
    virtual void modelReset();
    int stockDynaType(){return 5;}

    void initTableView(AbstractBaseStockModel  *model,BaseStockDynaProvider *dataProvider,bool isZXStock = false,int sortType = 1);
private:
    ViewType m_viewType;
};

//自选股
class ZXDynaView : public OptionDynaView
{
    Q_OBJECT
public:
    ZXDynaView(ViewType type,QWidget *parent = 0);

signals:
    void  rowCountChanged(int rowCount);

protected:
    void keyPressEvent(QKeyEvent *e);
    ZXStockDynaProvider *m_zxStkProvider;
};

//自选股窗口，无股票时显示添加页面
class  ZXStockDynaView : public BaseWidget
{
    Q_OBJECT
public:
    ZXStockDynaView(ViewType type,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    OptionDynaView *tableView(){return m_zxStockDynaView;}

signals:
    void itemSelected(const QString &stockCode);  //选中某股票
    void itemEntered(const QString &stockCode);   //双击或者Enter进入某股票

protected slots:
    void slotRowCountChanged(int );

private:
    ZXDynaView       *m_zxStockDynaView;
    AddZXStockView  *m_addZXStockView;
};


//历史浏览列表
class  HistStockDynaView : public OptionDynaView
{
public:
    HistStockDynaView(ViewType type, QWidget *parent = 0);
};

//18个重要指数行情
class  ImpIndexDynaView : public OptionDynaView
{
public:
    ImpIndexDynaView(ViewType type = OverallView,QWidget *parent = 0);
};

//异动板块
class  HZCOMPONENTSHARED_EXPORT AblBlockDynaView : public OptionDynaView
{
public:
    AblBlockDynaView(QWidget *parent = 0);

protected:
    void modelReset();

private:
    AblBlockDynaProvider *m_dataProvider;
};




#endif // ZXHISTSTOCKVIEW_H
