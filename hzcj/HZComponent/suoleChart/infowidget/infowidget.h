/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QWidget 首页热门资讯列表容器
*********************************************************************/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableView>
#include "tablemodel.h"

///
/// \brief The InfoWndTabView class
///
class InfoWndTabView : public QTableView
{
    Q_OBJECT
public:
    InfoWndTabView(TableModel *model, QWidget *parent=nullptr);

    void afterActive();

    void afterDeactive();
protected:
    void paintEvent(QPaintEvent *e);

private:
    int mDataFlags{0}, mNextPage{0};
    QRect mAddRect;

    QString mTipText, mTipUrl;
    TableModel *mModel{nullptr};
    HostNewsCallBack m_callback;
    friend class InfoWidget;

    void startReq(int page=0);
    void onhotNewsArrived(const InfoRet &errCode, HostNews &vec, int nRet);
};


class TableModel;
class InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();

    void afterActive();

    void afterDeactive();
protected:
    virtual void resizeEvent(QResizeEvent *event);
private:
    void initTableView();

private slots:
    void onRowClicked(const QModelIndex &index);

private:
    InfoWndTabView *table_view;
    TableModel *m_model;
};

#endif // WIDGET_H
