/********************************************************************
created: 2018,10,18
author: xuzhehu
purpose:
*********************************************************************/
#ifndef MULTIHOUR_H
#define MULTIHOUR_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include <QAbstractListModel>
#include <QListView>
#include <QFrame>
#include <QTimer>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QPushButton>

#include "basepage.h"
#include "updatinterface.h"
#include "infodef.h"


///
/// \brief The MultiHourDelegate class
///
class MultiHourDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MultiHourDelegate(QObject *parent = 0);
    ~MultiHourDelegate();

    /**
     * @brief setSize 设置可视大小
     * @param size
     */
    void setSize(const QSize &size);

protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    mutable QSize mSize;
    void drawDay(QPainter *painter, const QRect &rect, quint32 date) const;
    void drawTime(QPainter *painter, const QRect &rect, const QString &text, bool isHover) const;
};


///
/// \brief The MultiHourModel class
///
class MultiHourModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MultiHourModel(QObject *parent=nullptr);
    ~MultiHourModel();

    /**
     * @brief addRecords 增加项列表
     * @param items
     */
    void addRecords(const QList<STMultiHour> &items);

    void appendRecords(const QList<STMultiHour> &items);

    /**
     * @brief addRecord 增加项
     * @param item
     */
    void addRecord(const STMultiHour &item);

    /**
     * @brief insertRecord 在指定idx中插入项
     * @param item
     * @param idx
     */
    void insertRecord(const STMultiHour &item, int idx=0);

    /**
     * @brief recordByIndex 通过索引index查找当前值
     * @param index
     * @return
     */
    QVariant recordByIndex(int index);

    /**
     * @brief resetModel
     */
    void resetModel();

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

    /**
     * @brief getRecores 返回model中所有项
     * @return
     */
    QList<STMultiHour> getRecores() const;

private:
    QList<STMultiHour> m_records;
};

///
/// \brief The MultiHourView class
///
class MultiHourView : public QListView
{
    Q_OBJECT
public:
    MultiHourView(int page, QWidget *parent=nullptr);
    ~MultiHourView();

    /*!
     * \brief addRecord 增加项
     * \param item
     */
    void addRecord(const STMultiHour &item);

    /*!
     * \brief addRecords
     * \param item
     */
    void addRecords(const QList<STMultiHour> &item);

    /**
     * @brief resetModel
     */
    void resetModel();

    /**
     * @brief setData
     * @param data
     */
    void setData(const QVariantMap &data);


public slots:
    void slotScrollChanged(int value);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    bool m_isPressed{false};
    int m_dataFlags{0};
    QPoint m_pressPos;
    QRect m_addRect;
    QString m_tipUrl, m_tipText;

    MultiHourModel *m_smartModel{nullptr};
    MultiHourDelegate *m_smartDelegate{nullptr};
    int m_pageId;
};

///
/// \brief The MultiHourMarster class
///
class MultiHourMarster : public BasePage, public IUpdatePerformer//, public
{
    Q_OBJECT

public:
    MultiHourMarster(IUpdateRequest *adapter, QWidget *parent=nullptr);
    ~MultiHourMarster();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater);
    void updateList(const QVariantMap& var);
    void updateDetails(const QVariant& var);

    /**
     * @brief afterActive
     */
    void afterActive();
    void afterDeactive();

signals:
    void signalScrollValue(int value);
    void signalReStartRequest();

protected:
    void resizeEvent(QResizeEvent *event);
    void hideEvent(QHideEvent *event);

private:
    IUpdateRequest *m_iURequest;
    MultiHourView *m_panelView;
    QScrollBar *m_vScrollBar;
    QFrame *m_ctrlFrame;
    QPushButton *m_autoRefresh, *m_refresh;
    bool m_refreshFlag{false};//0-auto
    QButtonGroup *m_btnGrps;
    int m_timerId{0};
    QTimer m_reqTimer;
};
#endif // MULTIHOUR_H
