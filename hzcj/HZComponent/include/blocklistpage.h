#ifndef BLOCKLISTPAGE_H
#define BLOCKLISTPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 板块页面
*********************************************************************/
#include "hzcomponent_global.h"
#include  "basepage.h"

class BlockListMainWnd;

class HZCOMPONENTSHARED_EXPORT  BlockListPage : public BasePage
{
Q_OBJECT
public:
    explicit BlockListPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
private:
    void createWg();
private:
    BlockListMainWnd* m_pMainWnd;
};


#endif // BLOCKLISTPAGE_H
