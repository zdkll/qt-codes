#ifndef HANDICAPNEWSWIDGET_H
#define HANDICAPNEWSWIDGET_H
#include "basewidget.h"
#include <QStyledItemDelegate>
#include <QAbstractTableModel>
#include <QTableView>
#include "hzinfomationdlg.h"
#include "hzinfoadapterinterface.h"
#include "newsstyledwidget.h"
#include "vlistdef.h"
#define MYDATAROLE  Qt::UserRole + 1


class HandicapNewsWidget;
class HandicapNewsDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit HandicapNewsDelegate(QObject *parent = nullptr);

private:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    NewsStyledWidget m_styledWidget;
};



class HandicapNewsModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit HandicapNewsModel(QObject *parent = nullptr);

    //返回列表行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //返回列表列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //返回索引引用的项目的给定角色下存储的数据。
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //更新列表数据
    void updateList(const QList<STVListData> &datas);

    //返回当前已有数据列表
    QList<STVListData> getRecords() const { return m_dataList;}

    /**
     * @brief clearAll
     */
    void clearAll();

private:
    void refrush();

private:
    QList<STVListData> m_dataList;
    friend class HandicapNewsWidget;
};



class   HandicapNewsView:public QTableView, public IHZInfoUpdateObserve
{
    Q_OBJECT
public:
    explicit HandicapNewsView(QWidget *parent = nullptr);


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
    HandicapNewsModel *m_newsmodel;
    friend class  HandicapNewsWidget;
};

class  HandicapNewsWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HandicapNewsWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置获取数据类型
     * @param[in] apdater 子类对象
     * @retval
     */
    void setAdapter(HZInfoAdapterInterface *apdater,const QString& titleText);
    /**
     * @brief   设置股票代码
     * @param[in] codeVec 股票代码列表
     * @retval
     */
    void setCode(const QVector<QString> &codeVec);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
private:
    void createWg();
private slots:
    void onRowClicked(const QString& id);
private:
    QString m_titleText;
    QVector<QString> m_objList;
    HandicapNewsView *m_newsview;
    HZInfoAdapterInterface *m_apdater;
};



#endif // HANDICAPNEWSWIDGET_H
