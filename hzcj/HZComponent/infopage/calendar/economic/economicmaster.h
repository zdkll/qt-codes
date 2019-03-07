/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef ECONOMICMASTER_H
#define ECONOMICMASTER_H

#include <QWidget>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QListView>
#include <QScrollArea>
#include <QElapsedTimer>

#include "infodef.h"
#include "infoitemtitle.h"
#include "calandarbar.h"
#include "hzinfodata.h"
#include "updatinterface.h"

#include "basepage.h"
#include <datamodeldefs/datamodelpublic.h>
#include <datamodeldefs/stockinfodef.h>

class EconomicView;

///
/// \brief The EconomicDelegate class
///
class EconomicDelegate : public QStyledItemDelegate,public HZObject
{
    Q_OBJECT
public:
    explicit EconomicDelegate(int type=0, QObject *parent = 0);
    ~EconomicDelegate();

    /**
     * @brief setSize 设置可视大小
     * @param size
     */
    void setSize(const QSize &size);

protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    QMap<QString,QString> m_countryFlags;
    mutable QSize m_size;
    mutable QPoint m_pressed;
    mutable bool m_pressFlg;
    int m_type, m_stockW{};
    QPixmap m_customPix;
};


///
/// \brief The EconomicModel class
///
class EconomicModel : public QAbstractListModel
{
    Q_OBJECT
public:
    EconomicModel(int type, QObject *parent=nullptr);
    ~EconomicModel(){}

    void setViewRect(const QRect& rc);

    /**
     * @brief addRecords 增加项列表
     * @param items
     */
    void addRecords(const QVector<STEconmicMajorMeetUi> &items);
    void addRecords(const QVector<FinancialReport::FinancialItem> &items);
    void addRecords(const QVector<STEconomicDataUi> &items);
    QVector<STEconmicMajorMeetUi> getMajorMeetRecords();
    QVector<FinancialReport::FinancialItem> getFinancialRecords();
    const QVector<STEconomicDataUi> getEconomicRecords();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void flush();

    //settime
    void setFinanceReportTime(quint32 time);

private:
    void calcFinancialReportRowAndCount(int &row, int &count) const;

signals:
    void signalDataChanged();

private:
    QVector<STEconmicMajorMeetUi> m_majors;
    QVector<FinancialReport::FinancialItem> m_financialReps;
    QVector<STEconomicDataUi> m_economicDatas;
    int m_type;
    QRect m_viewRc;
    QElapsedTimer m_time;
    quint32 m_financeReportTime{0};

    void startRequestSnapShot();
    void reqSnapShot(const QList<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
};

///
/// \brief The EconomicView class
///
class EconomicView : public QListView
{
    Q_OBJECT
public:
    EconomicView(int type, QWidget *parent=nullptr);
    ~EconomicView();

    /**
     * @brief setBtnClicked 设置按钮click
     * @param id
     */
    void setBtnClicked(int id);

    /**
     * @brief setCacheData 缓存数据
     * \note 需要的时候可以临时保存一下
    */
    void setCacheData(const QVector<STEconomicDataUi> &datas);

public slots:
    void slotScrollChanged(int value);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    friend class EconomicItem;
    EconomicModel *m_smartModel{nullptr};
    EconomicDelegate *m_smartDelegate{nullptr};
    int m_type;

    //保存临时数据
    QVector<STEconomicDataUi> m_ecoUiData;

    void initData();
};

///
/// \brief The FinancialReportView class
///
class FinancialReportView : public EconomicView
{
    Q_OBJECT
public:
    FinancialReportView(QWidget *parent=nullptr);
    ~FinancialReportView();
protected:
    void mousePressEvent(QMouseEvent *event);
};

class EconomicItem : public QFrame
{
    Q_OBJECT

public:
    EconomicItem(int type, const QString &title,QWidget *parent = 0);

    /**
     * @brief setData
     * @param var  当前页数据
     * @param time  当前日期
     */
    void setData(const QVariant &var, quint32 time=0);

private:
    int m_type;
    Infoitemtitle *m_title;
    EconomicView *m_economicView;
};

namespace Ui {
class EconomicMaster;
}

///
/// \brief The EconomicMaster class
///
class EconomicMaster : public BasePage, public IUpdatePerformer//, public
{
    Q_OBJECT
public:
    explicit EconomicMaster(IUpdateRequest *adapter, QWidget *parent = 0);
    ~EconomicMaster();

    /**
     * @brief startTimesReq
     */
    void startTimesReq();

    /**
     * @brief setAdapter
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater);

    /**
     * @brief updateList
     * @param var
     */
    void updateList(const QVariantMap &var);

    /**
     * @brief updateTimes
     * @param tms
     */
    void updateTimes(const QVector<quint64> &tms);
    void updateDetails(const QVariant &var);
    void afterActive();
    void afterDeactive();

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    Ui::EconomicMaster *ui;
    IUpdateRequest *m_iURequest;
    CalandarBarMaster *m_calandar;
    EconomicItem *m_majorMeetPage;
    EconomicItem *m_financialRepPage;
    EconomicItem *m_economicDataPage;
    InfoReqId m_timesReq{-1};

    void startRequest(const QString &date);
    void economicCalandarInfoTimesCallBack(const InfoRet &ret, const EconomicDataTimes &datas);
};

#endif // ECONOMICMASTER_H
