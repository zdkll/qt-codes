/********************************************************************
created:2018.10.30
author: juntang.xie
purpose:派生自QTableView 自定义龙虎榜买卖表格
*********************************************************************/
#ifndef LHBUYSELLVIEW_H
#define LHBUYSELLVIEW_H
#include <QTableView>
#include "hzinfodata.h"

class LhBuySellModel;
class LhBuySellView:public QTableView
{
    Q_OBJECT
public:
    explicit LhBuySellView(QWidget *parent = nullptr, int nType = 0);

    //更新龙虎榜数据
    void updateLhData(const StockLhbDetialListItem &data);
protected:
    virtual void resizeEvent(QResizeEvent *event);

signals:
    void clickRow(const QString &strID);
private:
    void initTableView();

private:
    LhBuySellModel* m_pLhmodel;
};

#endif // LHBUYSELLVIEW_H
