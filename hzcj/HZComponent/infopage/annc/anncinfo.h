/********************************************************************
created: 2018,10,16
author: xuzhehu
purpose:
*********************************************************************/
#ifndef ANNCINFO_H
#define ANNCINFO_H

#include <QWidget>
#include <QLabel>
#include <QPen>
#include <QBrush>
#include <QHash>
#include <QPair>
#include <QAbstractListModel>
#include <QListView>
#include <QStyledItemDelegate>

#include <hzinfoadapterinterface.h>
#include <hzinfoadapter.h>
#include <datamodeldefs/datamodelpublic.h>
#include <datamodeldefs/stockinfodef.h>

#include "basepage.h"
#include "updatinterface.h"
#include "indexreqbutton.h"
#include "infodef.h"
#include "infostyle.h"
#include "infowebview.h"

///
/// \brief The AnncModel class
///
class AnncModel : public QAbstractListModel
{
    Q_OBJECT
public:
    AnncModel(QObject *parent=nullptr);
    void addRecords(const QList<STAnnc> &items);
    void addRecord(const STAnnc &item);
    void insertRecord(const STAnnc &item, int idx=0);
    QVariant recordByIndex(int index);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void flush();
    void updateChgs(const QVariant &var);
    void resetModel();
    QList<STAnnc> getRecores() const;
    QList<QString> getStockCodes(int start, int end);

private:
    QList<STAnnc> m_records;
};


class AnncItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AnncItemDelegate(QObject *parent = 0);
    ~AnncItemDelegate(){}

     void setWidth(int w);
protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    mutable int m_width;
    QPixmap m_customPix;
};


///
/// \brief The AnncView class
///
class AnncView : public QListView
{
    Q_OBJECT
public:
    AnncView(QWidget *parent=nullptr);

    void setData(const QVariantMap &data);
    void setSelected(const QString &index);
    void resetModel();
protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *event);

signals:
    void signalReStartRequest(int max);
    void signalItemClicked(const QUrl &url);
    void signalAddCustomStock();
    void signalDateChanged(const QUrl &url);


private:
    bool m_isPressed{false};
    int m_dataFlags{0}, m_scrollVal{0};
    QPoint m_pressPos;
    QRect m_addRect;
    QString m_tipUrl, m_tipText;
    AnncModel *m_listModel;
    AnncItemDelegate *m_listDelegate;
    friend class AnncMarster;

    void startRequestSnapShot();
    void reqSnapShot(const QVector<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
};


///
/// \brief The AnncMarster class
///
class AnncMarster : public BasePage, public IUpdatePerformer//, public
{
    Q_OBJECT

public:
    explicit AnncMarster(IUpdateRequest *adapter, QWidget *parent = 0);
    ~AnncMarster();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater){}
    void updateList(const QVariantMap &var);
    void updateDetails(const QVariant &var);
    void afterActive();
    void afterDeactive();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    IUpdateRequest *m_iURequest;
    IndexReqButton *m_indexBtn;
    AnncView *m_leftView;
    HZInfoWebPageMarster *m_webView;
    HZInfoWebPage *m_webPage;

    void reqDatas();
    bool isRequest();
};

#endif // ANNCINFO_H
