/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef MARKETMARSTER_H
#define MARKETMARSTER_H

#include <QWidget>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QListView>
#include <QScrollArea>

#include <datacontroller.h>
#include <hzinfodata.h>

#include "infodef.h"
#include "titlebar.h"
#include "calandarbar.h"
#include "updatinterface.h"
#include "infoitemtitle.h"
#include <hzinfodata.h>
#include "basepage.h"

///
/// \brief The MarketDelegate class
///
///
class MarketView;
class MarketDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MarketDelegate(int type, MarketView *view, QObject *parent = 0);
    ~MarketDelegate();

    /**
     * @brief setSize 设置可视大小
     * @param size
     */
    void setSize(const QSize &size);

signals:
    void signalStockClicked(const QString &stockcode) /*const*/;

protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    int m_type;
    MarketView *m_view;
    mutable QSize m_size;
    mutable QPoint m_pressed;
    mutable bool m_pressFlg;
    int  m_stockW{};
};


///
/// \brief The MarketModel class
///
class MarketModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MarketModel(int type, QObject *parent=nullptr);
    ~MarketModel();

    /**
     * @brief addRecords 增加项列表
     * @param items
     */
    void addRecords(const MarketCalendar &items);

    /**
     * @brief rowCount 行总数
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief data overide parent
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief flush 重设model
     */
    void flush();

signals:
    void signalDataChanged();

private:
    int m_type;
    MarketCalendar m_records;
};

///
/// \brief The MarketView class
///
class MarketView : public QListView
{
    Q_OBJECT
public:
    MarketView(int type, QWidget *parent=nullptr);
    ~MarketView();

    /*!
     * \brief addRecord 增加项
     * \param item
     */
    void addRecord(const MarketCalendar &item);

    /**
     * @brief stipToStockDetail
     * @param stockcode
     */
    void stipToStockDetail(const QString &stockcode);


public slots:
    void slotScrollChanged(int value);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    
private:
    MarketModel *m_smartModel{nullptr};
    MarketDelegate *m_smartDelegate{nullptr};
    int m_pageId;

    void initData();
};


///
/// \brief The MarketItem class
///
class MarketItem : public QFrame
{
    Q_OBJECT
public:
    MarketItem(int type, const QString &title, QWidget *parent = 0);

    /**
     * @brief setData 设置数据
     * @param var 日历数据
     * @param num   数据数量
     */
    void setData(const MarketCalendar &var, int num);

private:
    Infoitemtitle *m_title;
    MarketView *m_marketView;
};

namespace Ui {
class MarketMaster;
}
///
/// \brief The MarketMaster class
///
class MarketMaster : public BasePage, public IUpdatePerformer//, public BasePage
{
    Q_OBJECT
public:
    explicit MarketMaster(IUpdateRequest *adapter, QWidget *parent = 0);
    ~MarketMaster();

    void startTimesReq();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater);
    void updateList(const QVariantMap &var);
    void updateTimes(const QVector<quint64> &tms);
    void updateDetails(const QVariant &var);
    void afterActive();
    void afterDeactive();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MarketMaster *ui;
    struct ItemDate{
        QString stockname;
        QString stockcode;
        qreal now;
        qreal chg;
    };

    InfoReqId m_timesReq{-1};
    IUpdateRequest *m_iURequest;
    CalandarBarMaster *m_calandar;
    MarketItem *m_FHPXPage;
    MarketItem *m_TFPPage;
    MarketItem *m_SXJJPage;
    MarketItem *m_ZFPage;
    MarketItem *m_YJZFPage;
    MarketCalendar m_MkDates;

    void startRequest(const QString &date);
    void startRequestSnapShot(const QVariant &var);
    void reqSnapShot(const QList<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
    void marketCalandarInfoTimesCallBack(const InfoRet &ret,const MarketCalandarTimes &datas);
};

#endif // MARKETMARSTER
