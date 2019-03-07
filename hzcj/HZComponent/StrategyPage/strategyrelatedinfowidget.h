/********************************************************************
created:2019.03.04
author: juntang.xie
purpose: 策略相关资讯窗口
*********************************************************************/
#ifndef StrategyRelatedInfowidget_H
#define StrategyRelatedInfowidget_H
#include "basewidget.h"
#include <QStyledItemDelegate>
#include <QAbstractTableModel>
#include <QTableView>
#include "newsstyledwidget.h"
#include "hzinfocenter.h"
#define MYDATAROLE  Qt::UserRole + 1


class StrategyRelatedInfowidget;
class StrategyRelatedInfoDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StrategyRelatedInfoDelegate(QObject *parent = nullptr);

private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    NewsStyledWidget m_styledWidget;
};


Q_DECLARE_METATYPE(TopicInvestNews::STNewList)
class StrategyRelatedInfoModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StrategyRelatedInfoModel(QObject *parent = nullptr);

    //返回列表行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //返回列表列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //返回索引引用的项目的给定角色下存储的数据。
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


    //更新列表数据
    void updateList(const QVector<TopicInvestNews::STNewList> &vec);


    /**
     * @brief clearAll
     */
    void clearAll();

private:
    void refrush();

private:
    QVector<TopicInvestNews::STNewList> m_lists;
    friend class StrategyRelatedInfowidget;
};


class   StrategyRelatedInfoView:public QTableView
{
    Q_OBJECT
public:
    explicit StrategyRelatedInfoView(QWidget *parent = nullptr);


    /**
     * @brief clearAll
     */
    void clearAll();

protected:
    void updateList(const QVector<TopicInvestNews::STNewList> &vec, int nCode);
    void paintEvent(QPaintEvent *e);

private:
    void initTableView();

signals:
    void  signalRequest();

private:
    int mDataFlags;
    QString mTipText, mTipUrl;
    StrategyRelatedInfoModel *m_newsmodel;
    friend class  StrategyRelatedInfowidget;
};

class  StrategyRelatedInfowidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit StrategyRelatedInfowidget(QWidget *parent = nullptr);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();

    void setNextPage(int nPage);

    void requestTopicNews(int topic_id);

private:
    void createWg();

    void onTopicInvestNews(const InfoRet& ret,const TopicInvestNews& data);

    void requestNews();
private:
    QString m_titleText;
    QVector<QString> m_objList;
    StrategyRelatedInfoView *m_newsview;
    int m_nReq = -1;
    int m_nTopic_id;
    int m_nNextPage = -1;
};



#endif // StrategyRelatedInfowidget_H
