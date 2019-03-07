/********************************************************************
created:   2018/05/21
author: dk.zhang
purpose:自定义表格，支持固定显示前几列
*********************************************************************/
#ifndef FREEZETABLEVIEW_H
#define FREEZETABLEVIEW_H

#include "hzcontrols_global.h"
#include <QTableView>


class HZCONTROLSSHARED_EXPORT FreezeTableView : public QTableView
{
    Q_OBJECT
public:
    FreezeTableView(QWidget *parent = Q_NULLPTR);
    FreezeTableView(QAbstractItemModel * model,QWidget *parent = Q_NULLPTR);
    ~FreezeTableView();

    void setObjectName(const QString &objName);
    void setShowGrid(const bool &show);

    void setModel(QAbstractItemModel *model);
    void setFreezeColumn(const int &column);

    void setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior);
    void setSelectionMode(QAbstractItemView::SelectionMode mode);

    void setColumnWidth(int column, int width);

    void setVerticalSetcionSize(int size);

    void setSortingEnabled(bool enabled);

    void setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate) ;

    void setColumnHidden(int column, bool hide);

    void setSortIndicator(const int &column,const Qt::SortOrder &order);

    //隐藏显示表头
    void setShowHorizontalHeader(const bool &enable);
    void setShowVerticalHeader(const bool &enable);

    QTableView *subTableView(){return frozenTableView;}

    void setVerticalScrollMode(ScrollMode mode);

    void setHorizontalScrollMode(ScrollMode mode);

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;
    void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible) Q_DECL_OVERRIDE;

    QAbstractItemModel  *m_model;
private:
    void initTable();
    void initFromModel(QAbstractItemModel *model);

    void updateFrozenTable();
    void updateFrozenTableGeometry();

private slots:
    void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
    void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

    void sortIndicatorChanged(int,Qt::SortOrder);

private:
    QTableView *frozenTableView;
    int                m_freezeColumn; //固定的列数
};

#endif // FREEZETABLEVIEW_H
