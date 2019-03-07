/********************************************************************
created:
author: dk.zhang
purpose:板块缩略
*********************************************************************/
#ifndef SUOLUEBLOCK_H
#define SUOLUEBLOCK_H

#include "SuolueBase.h"

class SLDynaTableView;
class SuolueBlock : public SuolueBase
{
    Q_OBJECT
public:
    SuolueBlock(SuolueInfo info,QWidget *parent = 0);

    void afterActive();

    void afterDeactive();

    void onClickItem(const SuolueItem& item);

    //布局界面
    QWidget *createContentWnd();

    //创建标题区域
    virtual SuolueTitleBase* createTitleWnd();

    virtual void onFocusOut();
public slots:
    void slotItemSeleced(const QString &stockCode);
private:
      SLDynaTableView *m_bkTableView;
};

#endif // SUOLUEBLOCK_H
