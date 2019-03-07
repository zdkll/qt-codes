/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自QTableView 自定义新闻公告表格
*********************************************************************/
#ifndef NEWSVIEW_H
#define NEWSVIEW_H
#include <QTableView>
#include "hzinfoadapterinterface.h"

class NewsModel;
class NewsWidget;
class   NewsView:public QTableView, public IHZInfoUpdateObserve
{
    Q_OBJECT
public:
    explicit NewsView(QWidget *parent = nullptr);


    /**
     * @brief clearAll
     */
    void clearAll();

signals:
    void signalClickItem(const QString& id);

protected:
    void updateList(const QVariantMap &var);
    void paintEvent(QPaintEvent *e);

private:
    void initTableView();

signals:
    void  signalRequest();

private slots:
    void onRowClicked(const QModelIndex &index);

private:
    int mDataFlags;
    QString mTipText, mTipUrl;
    NewsModel *m_newsModel;
    friend class NewsWidget;
};

#endif // NEWSVIEW_H
