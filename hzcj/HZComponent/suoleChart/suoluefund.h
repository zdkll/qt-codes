/********************************************************************
created:
author: dk.zhang
purpose:资金缩略图
*********************************************************************/
#ifndef SUOLUEFUND_H
#define SUOLUEFUND_H

#include "SuolueBase.h"
#include "fundview.h"

class SuolueFund : public SuolueBase
{
    Q_OBJECT
public:
    SuolueFund(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    void onClickItem(const SuolueItem& item);

    //布局界面
    QWidget *createContentWnd();

    virtual void onFocusOut();

    //创建标题区域
    virtual SuolueTitleBase* createTitleWnd();
public slots:
    void slotItemSeleced(const QString &stockCode);
private:
   SLFundNetputView *m_fundTableView;
};

#endif // SUOLUEFUND_H
