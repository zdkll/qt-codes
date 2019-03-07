#ifndef DATACENTERPAGE_H
#define DATACENTERPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 数据中心
*********************************************************************/
#include "hzcomponent_global.h"
#include  "basepage.h"
#include "hzinfocenter.h"
class DataCenterPageMainWnd;
class HZCOMPONENTSHARED_EXPORT DataCenterPage: public BasePage
{
Q_OBJECT
public:
    explicit DataCenterPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);

    void onNetIoReady(bool ready);
private:
    DataCenterPageMainWnd* m_mainChildWnd;
};

#endif // DATACENTERPAGE_H
