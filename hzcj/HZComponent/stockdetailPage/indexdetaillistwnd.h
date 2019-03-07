#ifndef INDEXDETAILLISTWND_H
#define INDEXDETAILLISTWND_H
/********************************************************************
created:
author: min.cheng
purpose: 指数右侧窗口
*********************************************************************/
#include "blockdetaillistwnd.h"

class IndexDetailListWnd: public BlockDetaillistWnd
{
    Q_OBJECT
public:
    explicit IndexDetailListWnd(QWidget *parent = 0,QString instName="");
public slots:
    //对象改变信号 当前发生股票改变事件 内部需要判断当前窗口是否可见
    virtual void slotsObjChange(const QString& obj);
    //外面发送当前股票切换
    virtual void OnStockCodeChange();
    //排序点击
    void slotsButtonClicked(int id);

    void slotsSwitchUpDownStock(bool isUp, int step);

protected:
    NaviListView* createView();

};

#endif // INDEXDETAILLISTWND_H
