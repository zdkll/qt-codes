/********************************************************************
created: 2018.9.29
author: juntang.xie
purpose: 历史框视图
*********************************************************************/
#ifndef HISTORYBOXVIEW_H
#define HISTORYBOXVIEW_H
#include <QTableView>


class HistoryBoxView:public QTableView
{
    Q_OBJECT
public:
    explicit HistoryBoxView(QWidget *parent = nullptr);

private:
    void initTableView();

};

#endif // HISTORYBOXVIEW_H
