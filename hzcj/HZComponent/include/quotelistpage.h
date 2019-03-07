#ifndef QUOTELISTPAGE_H
#define QUOTELISTPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 行情页
*********************************************************************/

#include "hzcomponent_global.h"
#include  "basepage.h"

class QuoteListPageMainWnd;
class QuoteListPanelWnd;
class HZCOMPONENTSHARED_EXPORT  QuoteListPage : public BasePage
{
Q_OBJECT
public:
    explicit QuoteListPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
private slots:
    //股票点击事件
    void slotClickObjItem(const QString& obj);
    void slotDbClickObjItem(const QString& obj);
private:
    void createWg();
    //创建左侧列表窗口
    QWidget* createLeftWnd();
    //创建右侧行情盘口窗口
    QWidget* createRightWnd();
private:
    QuoteListPageMainWnd* m_leftWnd;
    QuoteListPanelWnd* m_rightWnd;
};

#endif // QUOTELISTPAGE_H
