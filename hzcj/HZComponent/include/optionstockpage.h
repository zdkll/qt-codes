#ifndef OPTIONSTOCKPAGE_H
#define OPTIONSTOCKPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 自选股页面
*********************************************************************/

#include "hzcomponent_global.h"
#include  "basepage.h"
class NewsWidget;
class HZAnncsInfo;
class QuotePanelWnd;
class OptionMainWnd;
class HZCOMPONENTSHARED_EXPORT OptionStockPage : public BasePage
{
     Q_OBJECT
public:
    explicit OptionStockPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
private slots:
    //合约切换
    void slotsSwitchObj(const QString& obj);
    //跳转到分时详情页面（个股分时，板块分时，指数分时分别处理）
    void slotsSwtichDetailPage(const QString& obj);
private:
    void createWg();
private:
    OptionMainWnd* m_leftWidget;
    QuotePanelWnd *m_rightWidget;
};

#endif // OPTIONSTOCKPAGE_H
