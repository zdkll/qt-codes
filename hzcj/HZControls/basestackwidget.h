#ifndef BASESTACKWIDGET_H
#define BASESTACKWIDGET_H
/********************************************************************
created:
author: min.cheng
purpose: 带有堆栈窗口的窗口
*********************************************************************/
#include <QStackedWidget>
#include "basewidget.h"
#include "hzcontrols_global.h"

class HZCONTROLSSHARED_EXPORT BaseStackWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit BaseStackWidget(QWidget *parent = nullptr, QString instName = "");

    //对象激活
    virtual void afterActive();

    //取消激活
    virtual void afterDeactive();

    int count();

    QWidget* currWidget();

    QWidget* Widget(int index);

    HZObject* currHzObject();

    void addWidget(QWidget* pWnd,HZObject* obj);

    void clearCurrWidget();

    void setCurrWidget(int index,bool bActive = true,QString stock="");

    int currWidgetIndex();

    QWidget* getStackWnd();

    void layoutStackWnd();
protected:
    //外面发送当前股票切换
    virtual void OnStockCodeChange();
private:
    QStackedWidget* m_pstackWnd;
    QVector<HZObject*> m_ObjVec;
};

#endif // BASESTACKWIDGET_H
