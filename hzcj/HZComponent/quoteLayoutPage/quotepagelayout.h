#ifndef QUOTEPAGELAYOUT_H
#define QUOTEPAGELAYOUT_H
/********************************************************************
created:
author: min.cheng
purpose: 用于行情个股，板块，指数详情的页面展示
*********************************************************************/
#include <QObject>
#include <QWidget>
#include "basewidget.h"

class StockDetailBaseWnd;
class QuotePanelWnd;
class QuotePageLayout:public QObject
{
    Q_OBJECT
public:
    QuotePageLayout(BaseWidget* pOwnerWnd,QObject *parent=Q_NULLPTR);

    //添加布局窗口
    void layout(StockDetailBaseWnd* midWnd,StockDetailBaseWnd* midBottomWnd,StockDetailBaseWnd* leftWnd,QuotePanelWnd* rightWnd);
signals:
public slots:
    //隐藏/显示左侧窗口
    void slotsHideLeftWnd(bool bHide);
    //隐藏/显示中间底部窗口
    void slotsHideMidBottomWnd(bool bHide);
    //隐藏/显示右侧窗口
    void slotsHideRightWnd(bool bHide);
private:
    StockDetailBaseWnd* m_midWnd;  //中上部  非固定高度，非固定宽度
    StockDetailBaseWnd* m_midBottomWnd;//中底部 固定高度 非固定宽度
    StockDetailBaseWnd* m_leftWnd; //左 固定宽度
    QuotePanelWnd* m_rightWnd;//右 固定宽度
    BaseWidget* m_ownerWnd;
};

#endif // QUOTEPAGELAYOUT_H
