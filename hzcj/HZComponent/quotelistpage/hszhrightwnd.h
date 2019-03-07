/********************************************************************
created:
author: dk.zhang
purpose:行情沪深综合 右侧页面
*********************************************************************/
#ifndef HSZHRIGHTWND_H
#define HSZHRIGHTWND_H

#include "basewidget.h"
#include "suoluewidget.h"

#include "subtab.h"

#include "sltableview.h"

//四种类型
enum HszhRankType
{
    Total_Rank            = 0, //总涨幅排行
    ThreeMin_RiseFall = 1,//三分钟涨幅
    FiveDay_RiseFall   = 2,//5日涨幅
    FromYear_RiseFall = 3,//年初至今涨幅
};

//涨跌排行信息
struct  RiseFallRankInfo
{
    RiseFallRankInfo(){}
    RiseFallRankInfo(int filter,ColumnIdx idx,bool desc)
        :filterFlag(filter),colIdx(idx),is_desc(desc){}
    ColumnIdx colIdx;  //字段，判断加载哪个model
    int  filterFlag; //filter 分类
    bool is_desc; //升序还是降序
};

struct HszhRankPageInfo
{
    HszhRankType rankType;
    QString            tiltle;
};

class ZXStockDynaView;
class QStackedWidget;
class HszhRiseFallView;
class HszhRankPage;
class HszhRightWnd : public BaseWidget
{
    Q_OBJECT
public:
    HszhRightWnd(QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

protected slots:
    void  currentTabChanged(int id);

    void slotDbClickItem(const QString &stockCode);

private:
    void createWg();
    QWidget *createTopWg();
    QWidget *createBottomWg();

    QVector<HszhRankPageInfo>   m_hszhRankPages;
    QStackedWidget   *m_stackedWidget;
    ZXStockDynaView *m_zxTableView;
};

//沪深涨幅排行StackedWidget,分：涨幅排行和3 ，5，年初至今涨幅
class  HszhRankPage : public BaseWidget
{
    Q_OBJECT
public:
    HszhRankPage(const HszhRankType &type = Total_Rank,QWidget *parent = 0);
    void afterActive();
    void afterDeactive();

signals:
    void signalDbClickObjItem(const QString &stockCode);

private:
    QVector<RiseFallRankInfo> getRankInfoByType(const HszhRankType &type);
    QList<HszhRiseFallView *> createViews(const HszhRankType &type);

    HszhRankType                   m_rankType;
    QList<HszhRiseFallView *> m_riseFallViews;
};


class HSZHTableView;
//每个列表Tilte +TabView
class HszhRiseFallView : public BaseWidget
{
    Q_OBJECT
public:
    HszhRiseFallView(RiseFallRankInfo rankInfo, QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

signals:
    void  signalDbClickObjItem(const QString &stockCode);

private:
    void createWg();
    HSZHTableView   *createTableView(ColumnIdx idx);
    SortField               sortField(ColumnIdx idx);
    QString getTitle(const RiseFallRankInfo& rankInfo);

    CSubTab             *m_csTab;
    HSZHTableView   *m_tableView;
    RiseFallRankInfo m_rankInfo;
};

#endif // HSZHRIGHTWND_H
