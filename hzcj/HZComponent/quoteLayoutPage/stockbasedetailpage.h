#ifndef STOCKBASEDETAILPAGE_H
#define STOCKBASEDETAILPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 板块，个股，指数详情页基础类
*********************************************************************/
#include <QMap>
#include "hzcomponent_global.h"
#include "basepage.h"
#include "basestackwidget.h"
#include "hzftenmarster.h"

class QuotePageLayout;
class StockDetailKlineMinWnd;
class QuoteDetailPanelWnd;

class StockBaseDetailListWnd;
class StockBaseDetailInfoWnd;
class CSubTab;
class QToolButton;

struct DetailWndItem
{
    StockBaseDetailListWnd* leftWnd;
    StockBaseDetailInfoWnd* infoBottomWnd;
    int index = 0;
};
class HZCOMPONENTSHARED_EXPORT StockBaseDetailPage : public BasePage
{
    Q_OBJECT
public:
    explicit StockBaseDetailPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam& param,bool bCurrPage = false);

    void onMessage(const QString& senderPath,const MessageParams& params);

    void onNetIoReady(bool ready);
protected slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    virtual void slotsLeftObjChange(const QString& obj);
    virtual void slotsBottomObjChange(const QString& obj);
    //切换子页面
    virtual void slotsTablSwitch(int id);
    //隐藏/显示左侧窗口
    void slotsHideLeftWnd(bool bHide);
    //隐藏/显示中间底部窗口
    void slotsHideMidBottomWnd(bool bHide);
    //隐藏/显示右侧窗口
    void slotsHideRightWnd(bool bHide);
    //滚动和上下PAGE切换合约
    void slotsWheelAndUpDownPageChangeStock(const QString& obj,bool bUp,int step);
    //F10点击
    void slotsF10Click();
protected:
    virtual StockBaseDetailListWnd* createLeftListWnd();
    virtual StockBaseDetailInfoWnd* createBottomWnd();
    void OnStockCodeChange();
    void createWg();
    //创建标题
    QWidget *createTitle();
    //左右列表切换相关
    void createListAndInfoWnd();
    bool changeLeftListWndByStock(const QString& stock,bool bActive);
    bool changeBottomInfoByStock(const QString& stock,bool bActive);
    void changeTabItemByWnd(StockBaseDetailInfoWnd* midBottomWnd);
    void proceF10Visual(const QString& stock);
    void keyPressEvent(QKeyEvent *event);
protected:
    QuotePageLayout* m_pageLayout=nullptr;
    StockDetailKlineMinWnd* m_midTopWnd=nullptr;
    QuoteDetailPanelWnd* m_rightPanel=nullptr;
    StockBaseDetailListWnd* m_leftWnd=nullptr;
    StockBaseDetailInfoWnd* m_midBottomWnd=nullptr;
    QMap<int,DetailWndItem> m_pageIdMapToChildWnd;

    //左侧列表
    BaseStackWidget* m_BottomWndStackWnd=nullptr;
    BaseStackWidget* m_leftWndStackWnd=nullptr;
    //标题tab
    CSubTab* m_table = nullptr;
    QWidget* m_bottomTitleBar = nullptr;
    QToolButton* m_titleHideBt = nullptr;
    QPushButton* m_f10Bt = nullptr;
    HZFTenMarster fTenDlg;
};

#endif // STOCKBASEDETAILPAGE_H
