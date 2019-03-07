#ifndef QUOTELISTINNERWND_H
#define QUOTELISTINNERWND_H
#include <QMap>
#include "quotepagebaseinnerwnd.h"
#include "hzchildwndmgr.h"
#include "datamodeldefs/datamodelpublic.h"
class StockDynaView;
class ImpIndexDynaView;

class FundFlowView;
class OptionFundFlowView;
struct QuoteListInnerWndItem
{
    QWidget* pWnd = nullptr;
    HZObject* pObj = nullptr;
};

//子窗口类型
enum  QuoteListBtChildWndType
{
    QuoteLisBtBaseQuoteListType = 0,
    QuoteLisBtFundFlowerFlowType,
    QuoteLisBtDDEType,
};


class HszhRightWnd;
class HszhLeftWnd;

//列表相关
class QuoteListInnerWnd : public QuotePageBaseInnerWnd
{
    Q_OBJECT
public:
    explicit QuoteListInnerWnd(QWidget *parent = nullptr, QString instName = "");

    //窗口创建
    void createWg();

    void switchChildWnd(int id,bool bactive=false);

    //取消激活
    void afterDeactive();

    void afterActive();

    int currType();
signals:

public slots:
    virtual void slotsSwtichChildWnd(int type);

protected:
    //创建基础行情列表
    virtual QuoteListInnerWndItem createBaseQuoteList();
    //创建资金流向列表
    virtual QuoteListInnerWndItem createFundFlowList();
    //创建DDE决策
    virtual QuoteListInnerWndItem createDDEList();

    //创建基础行情列表
    ChildWndItem createBaseQuoteListEx();
    //创建资金流向列表
    ChildWndItem createFundFlowListEx();
    //创建DDE决策
    ChildWndItem createDDEListEx();

private:
    //子页面类型和下标的映射  key==页面类型 value=m_pstackWnd窗口下标
    QMap<int,int> m_typeIndexMap;
    //子窗口统一管理
    HzChildWndMgr<QuoteListInnerWnd>* m_pMainChildWndMgr = nullptr;
};




//沪深综合
class HszhInnerWnd: public QuotePageBaseInnerWnd
{
    Q_OBJECT
public:
    explicit HszhInnerWnd(QWidget *parent = nullptr, QString instName = "");

    void createWg();

    void afterActive();
    void afterDeactive();
    void setStockCode(const QString &stock);


    bool bShowBtChildWnd(){return false;}

private:

    HszhRightWnd *m_rightWnd;
    HszhLeftWnd *m_leftWnd;
};

//个股行情列表
class StockListWnd:public QuoteListInnerWnd
{
    Q_OBJECT
public:
    explicit StockListWnd(QWidget *parent = nullptr, QString instName = "");

    void setFilterFlag(const int &filterFlag);

    void setFilterDetail(const QString &filter_detail);
protected:
    //创建基础行情列表
    virtual QuoteListInnerWndItem createBaseQuoteList();
    //创建资金流向列表
    virtual QuoteListInnerWndItem createFundFlowList();
    //创建DDE决策
    virtual QuoteListInnerWndItem createDDEList();
private:
    int m_filterFlag = HZData::MarketAll;
    QString m_filter_detail;
    StockDynaView *m_pBaseView = nullptr;
    FundFlowView   *m_fundView = nullptr;
};


//指数:重要指数
class ImpIndexListWnd:public QuoteListInnerWnd
{
    Q_OBJECT
public:
    explicit ImpIndexListWnd(QWidget *parent = nullptr, QString instName = "");

protected:
    //创建基础行情列表
    virtual QuoteListInnerWndItem createBaseQuoteList();
    //创建资金流向列表
    virtual QuoteListInnerWndItem createFundFlowList();
    //创建DDE决策
    virtual QuoteListInnerWndItem createDDEList();
private:
    ImpIndexDynaView    *m_pBaseView = nullptr;
    OptionFundFlowView *m_fundView = nullptr;
};


//指数:全部指数
class IndexListWnd:public QuoteListInnerWnd
{
    Q_OBJECT
public:
    explicit IndexListWnd(QWidget *parent = nullptr, QString instName = "");

    void setFilterFlag(const int &filterFlag);

    void setFilterDetail(const QString &filter_detail);
protected:
    //创建基础行情列表
    virtual QuoteListInnerWndItem createBaseQuoteList();
    //创建资金流向列表
    virtual QuoteListInnerWndItem createFundFlowList();
    //创建DDE决策
    virtual QuoteListInnerWndItem createDDEList();
private:
    int m_filterFlag = HZData::IndexInner;
    StockDynaView          *m_pBaseView = nullptr;
    OptionFundFlowView *m_fundView = nullptr;
};



#endif // QUOTELISTINNERWND_H
