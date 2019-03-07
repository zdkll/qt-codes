/********************************************************************
created:  2018.9
author: juntang.xie
purpose: MiniMornitorWnd 迷你盯盘主窗口
*********************************************************************/
#ifndef MINIMORNITORWND_H
#define MINIMORNITORWND_H
#include "basewidget.h"
#include "frameless_helper.h"
#include "hzcomponent_global.h"



class SelfStockRollPanel;
class MiniNaveBar;
class SelfStockPanel;
class MarketChangePanel;
class StockRollStatusBar;
class QStackedWidget;
class HZCOMPONENTSHARED_EXPORT MiniMornitorWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit MiniMornitorWnd(BaseWidget *parent = 0,QString instName="");

    /**
     * @brief 对象激活
     */
    virtual void afterActive();

    /**
     * @brief 取消激活
     */
    virtual void afterDeactive();

private:
    void createWg();
    void setPositon();
    void buildConnect();
    std::list<std::string> getStockLists();

signals:
    void maxiMize();
private:
    SelfStockRollPanel *m_pRollPanel;
    MiniNaveBar *m_pNavbar;
    SelfStockPanel *m_pSelfStockPanel;
    MarketChangePanel *m_pMarketPanel;
    StockRollStatusBar *m_pRollStatusBar;
    QStackedWidget *m_pStackedWidget;
    bool m_isActive;
    FramelessHelper *m_pHelper;

};

#endif // MINIMORNITORWND_H
