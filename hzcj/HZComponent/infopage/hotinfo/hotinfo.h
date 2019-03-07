/********************************************************************
created: 2018,10,16
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HOTINFO_H
#define HOTINFO_H

#include <QWidget>
#include <QLabel>
#include <QPen>
#include <QBrush>
#include <QHash>
#include <QPair>
#include <QAbstractListModel>
#include <QListView>
#include <QStyledItemDelegate>

#include "basepage.h"
#include "indexreqbutton.h"
#include "infodef.h"
#include "infostyle.h"
#include "updatinterface.h"
#include "infowebview.h"

///
/// \brief The HotInfoModel class
///
class HotInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    HotInfoModel(QObject *parent=nullptr);
    void addRecords(const QList<STHotInfoData> &items);
    void addRecord(const STHotInfoData &item);
    void insertRecord(const STHotInfoData &item, int idx=0);
    QVariant recordByIndex(int index);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void flush();
    void resetModel();
    QList<STHotInfoData> getRecores() const;

private:
    QList<STHotInfoData> m_records;
};

///
/// \brief The HotItemDelegate class
///
class HotItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HotItemDelegate(QObject *parent = 0);
    ~HotItemDelegate(){}

     void setWidth(int w);
protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    mutable int m_width;
    mutable QPoint m_pressed;
    mutable bool m_pressFlg;
    QPixmap m_customPix;
};


///
/// \brief The HotView class
///
class HotView : public QListView
{
    Q_OBJECT
public:
    HotView(QWidget *parent=nullptr);

    void setSelected(const QString &index);
    void setData(const QVariantMap &data);
    void resetModel();

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

signals:
    void signalDateChanged(const QUrl &url);
    void signalReStartRequest(int max);
    void signalItemClicked(const QUrl &url);

private:
    bool m_isPressed{false};
    QPoint m_pressPos;
    QRect m_addRect;
    int m_dataFlags{0};
    QString m_tipText, m_tipUrl;
    HotInfoModel *m_listModel;
    QItemSelectionModel *m_selectModel;
    HotItemDelegate *m_listDelegate;
    friend class HotMarster;
};


///
/// \brief The HotMarster class
///
class HotMarster : public BasePage, public IUpdatePerformer
{
    Q_OBJECT

public:
    explicit HotMarster(IUpdateRequest *iadapter, QWidget *parent = 0);
    ~HotMarster();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater);
    void updateList(const QVariantMap& var);
    void updateDetails(const QVariant& var){Q_UNUSED(var);}
    void afterActive();
    void afterDeactive();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    IUpdateRequest *m_iURequest;
    IndexReqButton *m_indexBtn;
    HotView *m_leftView;
    HZInfoWebPageMarster *m_webView;
    HZInfoWebPage *m_webPage;
};

#endif // HOTINFO_H
