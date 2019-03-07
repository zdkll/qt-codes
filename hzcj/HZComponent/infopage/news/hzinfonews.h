/********************************************************************
created: 2018,10,16
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZINFONEWS_H
#define HZINFONEWS_H

#include <QWidget>
#include <QLabel>
#include <QPen>
#include <QBrush>
#include <QHash>
#include <QPair>
#include <QAbstractListModel>
#include <QListView>
#include <QStyledItemDelegate>
#include <QWheelEvent>

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


class MyDialog : public QFrame
{
    Q_OBJECT
public:
    MyDialog(QWidget *parent=nullptr);

signals:
    void signalClicked(const QString &text);
    void signalColorClicked(const QString &text);
private:
    QLineEdit *edit, *edit2;
    QPushButton *btn, *btn2;
};



///
/// \brief The HZInFoNewsModel class
///
class HZInFoNewsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    HZInFoNewsModel(QObject *parent=nullptr);
    void addRecords(const QList<STOptionalNews> &items);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void resetModel();

    /*!
     * \brief updateChgs 全量更新涨幅
     * \param var
     */
    void updateChgs(const QVariant &var);
    void flush();
    QList<STOptionalNews> getRecores() const;
    QList<QString> getStockCodes(int start, int end);

private:
    QList<STOptionalNews> m_records;
};

///
/// \brief The HZInFoNewsItemDelegate class
///
class HZInFoNewsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HZInFoNewsItemDelegate(QObject *parent = 0);
    ~HZInFoNewsItemDelegate(){}

     void setWidth(int w);
protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    mutable int m_width;
    QPixmap m_customPix;
};


///
/// \brief The HZInfoNewsView class
///
class HZInfoNewsView : public QListView
{
    Q_OBJECT
public:
    HZInfoNewsView(QWidget *parent=nullptr);
    ~HZInfoNewsView();

    void setData(const QVariantMap &data);
    void resetModel();

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *event);

signals:
    void signalReStartRequest(int max);
    void signalItemClicked(const QUrl &url);
    void signalAddCustomStock();//add custom stock
    void signalDateChanged(const QUrl &url);

private:
    int m_scrollVal{0}, m_reqId{}, m_dataFlags{0};
    bool m_isPressed{false};
    QPoint m_pressPos;
    QRect m_addRect;
    QString m_tipText, m_tipUrl;
    HZInFoNewsModel *m_listModel;
    HZInFoNewsItemDelegate *m_listDelegate;
    friend class NewsMarster;

    void startRequestSnapShot();
    void reqSnapShot(const QVector<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
};

///
/// \brief The NewsMarster class
///
class NewsMarster : public BasePage, public IUpdatePerformer//, public
{
    Q_OBJECT

public:
    enum{ZXG_NEWS, ZX_NEWS};
    NewsMarster(IUpdateRequest *adapter, int type=ZX_NEWS, QWidget *parent = 0);
    ~NewsMarster();

    /**
     * @brief setAdapter 设置更新对象
     * @param apdater
     */
    void setAdapter(IUpdateRequest *apdater){}
    void updateDetails(const QVariant &var){}
    void updateList(const QVariantMap &var);
    void afterActive();
    void afterDeactive();


public slots:
    void slotZXStockClicked(const QString &stockcode);

protected:
//    bool eventFilter(QObject *watched, QEvent *event);

private:
    int m_type;
    QString m_stockcode;//自选股资讯新闻
    IUpdateRequest *m_iURequest;
    IndexReqButton *m_indexBtn;
    HZInfoNewsView *m_leftView;
    HZInfoWebPageMarster *m_webView;
    HZInfoWebPage *m_webPage;
    MyDialog *dlg;

    void reqDatas();
    bool isRequest();
};


#endif // HZINFONEWS_H
