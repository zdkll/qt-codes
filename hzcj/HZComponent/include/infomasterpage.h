#ifndef INFOMASTERPAGE_H
#define INFOMASTERPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 资讯页面
*********************************************************************/
#include "hzcomponent_global.h"
#include  "basepage.h"
#include "infomaster.h"

class HZCOMPONENTSHARED_EXPORT InfoMasterPage: public BasePage
{
Q_OBJECT
public:
    explicit InfoMasterPage(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void recvParam(const DetailPageParam &param,bool bCurrPage = false);
private:
    InfoMaster *mInfoMarster;
};

#endif // INFOMASTERPAGE_H
