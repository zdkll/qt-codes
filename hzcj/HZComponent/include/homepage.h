#ifndef HOMEPAGE_H
#define HOMEPAGE_H
/********************************************************************
created:
author: min.cheng
purpose: 首页页面实现，主要负责创建缩略图，布局相关。
*********************************************************************/
#include <QVector>
#include "hzcomponent_global.h"
#include "basepage.h"

#include <QDebug>
class SuolueBase;
class QSignalMapper;
class HZCOMPONENTSHARED_EXPORT HomePage:public BasePage
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = 0,QString instName="");

    ~HomePage();

    virtual void afterActive();

    virtual void afterDeactive();

protected slots:
    void  slotCurrentStockViewChanged(int);

private:
    void createWg();
private:
   QVector<SuolueBase*>  m_slWndList;
   QSignalMapper             *m_signalMapper;
   int                                  m_currentViewIdx;
};

#endif // HOMEPAGE_H
