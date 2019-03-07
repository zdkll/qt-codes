#ifndef QUOTEPAGEBASEINNERWND_H
#define QUOTEPAGEBASEINNERWND_H
/********************************************************************
created:
author: min.cheng
purpose: 行情内部窗口基类
*********************************************************************/
#include "basewidget.h"

//内部窗口基类
class QuotePageBaseInnerWnd:public BaseWidget
{
    Q_OBJECT
public:
    explicit QuotePageBaseInnerWnd(QWidget *parent = nullptr, QString instName = "");

    virtual ~QuotePageBaseInnerWnd(){}

    //窗口创建
    virtual void createWg(){}

    //是否有 BtChildWndType类型的子窗口
    virtual bool bShowBtChildWnd(){return true;}

    virtual void setFilterFlag(const int &filterFlag){}

    virtual void setFilterDetail(const QString &filter_detail){}

    virtual void switchChildWnd(int id,bool bactive){}

    virtual int currType(){return 0;}
signals:
    //股票点击事件
    void signalsClickObjItem(const QString& obj);
    void signalsDbClickObjItem(const QString& obj);
public slots:
    virtual void slotsSwtichChildWnd(int type){}
   // void slotClickItem(const QString &stockCode);
};
#endif // QUOTEPAGEBASEINNERWND_H
