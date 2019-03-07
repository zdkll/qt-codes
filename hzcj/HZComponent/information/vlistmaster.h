/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QAbstractListModel>
#include <QListView>
#include <QTimer>

#include "vlistitemdelegate.h"
#include "vlistdef.h"
#include "hzinfoadapterinterface.h"
#include "hzinfoadapter.h"
#include "infomation_export.h"
#include "infowebview.h"

///
/// \brief The HZVListModel class
///
class HZVListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    HZVListModel(QObject *parent=nullptr);

    /**
     * @brief addRecords 增加项列表
     * @param items
     */
    void addRecords(const QList<STVListData> &items);

    /**
     * @brief addRecord 增加项
     * @param item
     */
    void addRecord(const STVListData &item);

    /**
     * @brief insertRecord 插入项
     * @param item
     * @param idx
     */
    void insertRecord(const STVListData &item, int idx=0);

    /**
     * @brief recordByIndex 通过index查找项
     * @param index
     * @return
     */
    QVariant recordByIndex(int index);

    /**
     * @brief setMsgId
     * @param id
     */
    void setMsgId(const QString &id);

    /**
     * @brief rowCount 返回列表总数
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief data  重新父类data
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief flush 重启model
     */
    void flush();

    /**
     * @brief getRecores 获取所有项数据
     * @return
     */
    QList<STVListData> getRecores() const;

    void clearAll();
private:
    QList<STVListData> m_records;
    QString m_msgId{""};
};


///
/// \brief The HZVListView class
///
class HZVListView : public QListView
{
    Q_OBJECT
public:
    HZVListView(QWidget *parent=nullptr);

    /**
     * @brief setSelected 选择
     * @param index
     */
    void setSelected(const QString &index);

    /**
     * @brief setData
     * @param data
     */
    void setData( const QVariantMap &data);

    /**
     * @brief clearAll
     */
    void clearAll();

signals:
    void signalRequest(int max);
    void signalClicked(const QModelIndex &index);


protected:
    void paintEvent(QPaintEvent *e);

private:
    int m_dataFlags;
    QString m_tipText, m_tipUrl;
    HZVListModel *m_listModel;
    HZVListItemDelegate *m_listDelegate;
    friend class HZVListMaster;
};

/// \brief The HZVListMaster class
///
class INFOMATION_EXPORT HZVListMaster : public QWidget, public IHZInfoUpdateObserve
{
    Q_OBJECT

public:
    explicit HZVListMaster(QWidget *parent = 0);
    ~HZVListMaster();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(HZInfoAdapterInterface *apdater);
    void updateList(const QVariantMap &data);
    void clearAll();
    void clearWeb();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    HZInfoAdapterInterface *m_adapter;
    HZVListView *m_vListView;
    HZInfoWebPageMarster *m_webView{nullptr};
//    HZInfoWebPage *mWebPage;

    void init();
};

#endif // MAINWINDOW_H
