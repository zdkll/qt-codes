/********************************************************************
created:
author: dk.zhang
purpose:添加自选股窗口
*********************************************************************/
#ifndef ADDZXSTOCKVIEW_H
#define ADDZXSTOCKVIEW_H

#include "basewidget.h"

#include "hzcomponent_global.h"

class StockAddDialog;
class  AddZXStockView : public BaseWidget
{
    Q_OBJECT
public:
    //为了区分自选股列表和其他自选股列表的背景色，使用不同颜色添加按钮，设置属性
    enum ZXViewType
    {
        ZXStockView = 0,
        OtherZXView = 1,
    };

    AddZXStockView(ZXViewType type = ZXStockView,QWidget *parent = 0);

signals:
    void  addFinshed();

protected slots:
    void slotAddStock();

private:
  void createWg();

//  StockAddDialog  *m_addStockDlg;
};

#endif // ADDZXSTOCKVIEW_H
