/********************************************************************
created:2019.2.22
author: juntang.xie
purpose:搜索列表视图
*********************************************************************/
#ifndef SEARCHSTOCKVIEW_H
#define SEARCHSTOCKVIEW_H
#include <QTableView>
#include "searchstockmodel.h"


class SearchStockView:public QTableView
{
    Q_OBJECT
public:
    explicit SearchStockView(QWidget *parent = nullptr);

    void upDateData(QList<SearchStockData> &list);

//protected:
//    virtual void   keyPressEvent(QKeyEvent *e);


private:
    void initTableView();

private slots:
    void onRowClicked(const QModelIndex &index);

signals:
    void rowClicked(const QString &strCode);

private:
    SearchStockModel *m_model;
};

#endif // SEARCHSTOCKVIEW_H
