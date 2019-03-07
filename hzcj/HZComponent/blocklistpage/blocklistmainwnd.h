#ifndef BLOCKLISTMAINWND_H
#define BLOCKLISTMAINWND_H
/********************************************************************
created:
author: min.cheng
purpose: 板块列表右侧主窗口
*********************************************************************/
#include <QMap>
#include "hzchildwndmgr.h"
#include "basewidget.h"
class QLabel;
class StockDynaView;
class BKAndStockView;
class FundFlowView;
class BKStockFundFlowView;
class DetailPageParam;
class AblBlockDynaView;
struct BlockListInnerWndItem
{
    QWidget* pWnd = nullptr;
    HZObject* pObj = nullptr;
};

enum BlockListWndType
{
    BkListCategoryType = 0,
    BkListChildType
};
class BlockListCategoryWnd;
class BlockListWnd;
class PlateHandicapWidget;
class BkListBottomNavigator;
class BlockListMainWnd: public BaseWidget
{
    Q_OBJECT
public:
    explicit BlockListMainWnd(QWidget *parent = 0,QString instName="");

    void createWg();

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
public slots:
    //列表双击事件
    void slotDbClickItem(const QString& stockCode);
    //板块分类导航
    void slotBkCategoryNavChange(int filterFlag);
    //板块切换信号
    void slotBkNavChange(int filterFlag,const QString& bkCode);
protected:
    //创建右侧窗口
    QWidget* createRightPanelWnd();
    //创建分类导航窗口
    ChildWndItem createBlockListCategoryWnd();
    //创建板块列表窗口
    ChildWndItem createBlockListWnd();

private slots:
    void onBkCodeChanged(const QString& bkCode);
    void onStockCodeChanged(const QString& bkCode);
protected:
    BlockListCategoryWnd* m_pCategoryWnd = nullptr;
    BlockListWnd* m_pChildWnd = nullptr;
    PlateHandicapWidget* m_rightHandicap = nullptr;
    //板块导航
    BkListBottomNavigator* m_bkListNaibar = nullptr;
    int m_filterFlag = 103;
    QMap<int,QString> m_filterStockMap;
    bool m_bRecodFlag = true;

    //子页面管理器
    HzChildWndMgr<BlockListMainWnd>* m_pMainChildWndMgr = nullptr;
};

//概念 行业 板块分类
class BlockListCategoryWnd: public BaseWidget
{
    Q_OBJECT
public:
    explicit BlockListCategoryWnd(QWidget *parent = 0,QString instName="");
    void setFilterFlag(const int &filterFlag);
    void afterDeactive();
    void afterActive();
signals:
    //板块数据列表单击事件
    void signalListCategoryCodeChange(const QString& bkCode);
    //成分股列表单击事件
    void signalStockCodeChange(const QString& stockCode);
public slots:
    void slotsTabCurrentBtnChanged(int nId);
    void slotsHideBottomWnd(bool bHide);

    //分类表事件
    void slotsCategoryWndItemSelected(const QString &stockCode);  //选中某股票
    void slotsCategoryWndEntered(const QString &stockCode);   //双击或者Enter进入某股票

    //当前成分股列表
    void slotsChildWndItemSelected(const QString &stockCode);  //选中某股票
    void slotsChildWndEntered(const QString &stockCode);   //双击或者Enter进入某股票
protected:
    void createWg();
    //创建基础行情列表
    ChildWndItem createBaseQuoteList();
    //创建资金流向列表
    ChildWndItem createFundFlowList();
    //异动板块列表
    ChildWndItem createAbnormalBkList();
    //创建成分股类别
    QWidget* createChildWnd();
    //创建按钮按钮和成分股描述文本标题栏
    QWidget* createBottomBar();
    //根据板块ID格式标题描述
    QString foramtTitleTextFromBkCode(int filter,const QString& code);
protected:
    QLabel* m_childListDesc;
    StockDynaView* m_pBaseView = nullptr;
    FundFlowView*  m_fundView = nullptr;
    StockDynaView* m_bkStockDynaView=nullptr;
    AblBlockDynaView        *m_ablBlockDynaView = nullptr;

    int m_filterFlag;

    //子页面管理器
    HzChildWndMgr<BlockListCategoryWnd>* m_pMainChildWndMgr = nullptr;
};

//板块列表
class BlockListWnd: public BaseWidget
{
    Q_OBJECT
public:
    explicit BlockListWnd(QWidget *parent = 0,QString instName="");
    void setFilterInfo(const int &filterFlag,const QString &filter_detail);

    //取消激活
    void afterDeactive();

    void afterActive();
signals:
    //成分股列表单击事件
    void signalStockCodeChange(const QString& stockCode);
public slots:
    void slotsTabCurrentBtnChanged(int nId);
    //分类表事件
    void slotsBlockListWndItemSelected(const QString &stockCode);  //选中某股票
    void slotsBlockListWndEntered(const QString &stockCode);   //双击或者Enter进入某股票
protected:
    void createWg();
    //根据板块ID格式标题描述
    QString foramtTitleTextFromBkCode(int filter,const QString& code);

    //创建基础行情列表
    ChildWndItem createBaseQuoteList();
    //创建资金流向列表
    ChildWndItem createFundFlowList();

protected:
    int m_filterFlag;
    QString m_filter_detail;
    QLabel                          *m_childListDesc;
    BKStockFundFlowView *m_fundView = nullptr;
    BKAndStockView          *m_pBaseView=nullptr;
   //子页面管理器
    HzChildWndMgr<BlockListWnd>* m_pMainChildWndMgr = nullptr;
};
#endif // BLOCKLISTMAINWND_H
