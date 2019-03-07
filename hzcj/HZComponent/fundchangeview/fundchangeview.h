/********************************************************************
created:2018.10.30
author: juntang.xie
purpose:派生自QTableView 资金异动表格
*********************************************************************/
#ifndef FundChangeView_H
#define FundChangeView_H
#include <QTableView>
#include "datacontroller.h"

using namespace HZData;
class FundChangeModel;
class   FundChangeView:public QTableView
{
    Q_OBJECT
public:
    explicit FundChangeView(QWidget *parent = nullptr);

    //显示最大行数
    int maxRowCount() const;

    //更新数据
    void upDateData(const QVector<ShortMotion> &fundVec);

    //清除数据
    void clearData();

    //列表行数
    int rowCount();

    void setCurrentIndexEx(const QModelIndex &index);

protected:
//    virtual void wheelEvent(QWheelEvent *event);

private:
    void initTableView();

signals:
    void wheelMoved(bool isMax);

    void clickRow(const QString strCode);

private slots:
    void onRowClicked(const QModelIndex &index);

private:
    FundChangeModel *m_fundModel;
};

#endif // FundChangeView_H
