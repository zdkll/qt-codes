/********************************************************************
created:2018/07/16
author: dk.zhang
purpose:新版固定行列表格控件
*********************************************************************/

#ifndef FROZENTABLEVIEW_H
#define FROZENTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QStack>
#include <QWidget>
#include "hzcontrols_global.h"

/**
 * @brief 固定前几行、列表格基类
 */
class HZCONTROLSSHARED_EXPORT FrozenBaseTableView : public QTableView
{
    Q_OBJECT
public:
    FrozenBaseTableView(QWidget *parent = 0);

    /**
     * @brief 设置固定列数
     * @param[in] 列数
     */
    virtual void setFrozenColCount(const int &){}
    /**
     * @brief 设置固定行数
     * @param[in] 行数
     */
    virtual  void setFrozenRowCount(const int &){}

    virtual void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy){m_vertScrollBarPolicy = policy;}

    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy){m_horizScrollBarPolicy = policy;}

    int lastRow(){return this->rowAt(this->viewport()->height());}

signals:
    void currentItemChanged(const QModelIndex &index);

protected:
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;

protected slots:
    virtual void  currentChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
    Qt::ScrollBarPolicy m_horizScrollBarPolicy,m_vertScrollBarPolicy;
};


/**
 * @brief 固定列表格类
 */
class  HZCONTROLSSHARED_EXPORT FrozenColTableView : public FrozenBaseTableView
{
    Q_OBJECT
public:
    /**
     * @brief   构造函数
     * @param[in] 横向滚动条
     * @retval
     */
    FrozenColTableView(QScrollBar *hscrollBar,QWidget *parent = 0);
    ~FrozenColTableView(){}

    /**
     * @brief  设置Model
     * @param[in] model 对象指针
     * @retval
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

    /**
     * @brief  设置固定列数
     * @param[in] 固定列数
     * @retval
     */
    void setFrozenColCount(const int &count) Q_DECL_OVERRIDE;

    virtual void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy){ QTableView::setVerticalScrollBarPolicy(policy);}

protected slots:
    void resizeEvent(QResizeEvent *e)  Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *e)  Q_DECL_OVERRIDE;

    void columnResized(int column, int oldWidth, int newWidth);

    virtual void slotModelReset();

    void slotHorizBarValueChanged(const int &value);

protected:
    QAbstractItemModel *m_model;

    void initModel(QAbstractItemModel *model);

private:
    int columnCount() const{return m_columnCount;}

    int rowCount() const {return 0;}

    void updateHorizScrollBar();

    int m_frozenColumn;
    int m_columnCount;

    QVector<int>                m_columnWidths;

    QStack<int>                 m_hiddenColumns;

    QScrollBar                 *m_horizBar;
};


/**
 * @brief 同时固定行和列表格类
 */
class   HZCONTROLSSHARED_EXPORT FrozenTableView : public FrozenColTableView
{
    Q_OBJECT
public:
    /**
     * @brief   构造函数
     * @param[in] 横向滚动条
     * @param[in] 纵向滚动条
     * @retval
     */
    FrozenTableView(QScrollBar *hscrollBar,QScrollBar *vscrollBar,QWidget *parent = 0);
    ~FrozenTableView(){}

    /**
     * @brief    设置Model
     * @param[in] model 对象指针
     * @retval
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

    /**
     * @brief    设置固定行数
     * @param[in] 固定行数
     * @retval
     */
    void setFrozenRowCount(const int &count) Q_DECL_OVERRIDE;

protected slots:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *e)  Q_DECL_OVERRIDE;

    void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;

   virtual void slotModelReset();

private slots:
    void slotVertBarValueChanged(const int &value);

private:
    int rowCount() const  {return m_rowCount;}

    void updateVertScrollBar();

    int m_frozenRow;
    int m_rowCount;

    QScrollBar                 *m_vertBar;

    QStack<int>               m_hiddenRows;
};


class  HZCONTROLSSHARED_EXPORT  FrozenView : public QWidget
{
    Q_OBJECT
public:
    enum FrozenFlag{
        ColFrozen        = 0x01,
        ColRowFrozen = 0x02 | 0x01
    };
    FrozenView(const FrozenFlag &frozenFlag,QWidget *parent = 0);

    /**
     * @brief 获取显示窗口对应的表格
     * @retval 表格对象
     */
    FrozenBaseTableView   *tableView(){return m_tableView;}

    QScrollBar *horizontalScrollBar();
    QScrollBar *verticalScrollBar();

    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy);

    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy);

    FrozenFlag frozenFlag()const {return m_frozenFlag;}

private:
    void initWg();

    FrozenBaseTableView *m_tableView;

    FrozenFlag                m_frozenFlag;
    QScrollBar                *m_horizScrollbar,*m_vertScrollbar;
};
#endif // FROZENTABLEVIEW_H
