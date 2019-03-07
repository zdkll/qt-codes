/********************************************************************
created:
author: dk.zhang
purpose:
*********************************************************************/
#ifndef SIMIKHEADERVIEW_H
#define SIMIKHEADERVIEW_H

#include <QTableView>
#include "baseitemmodel.h"
#include "viewstyleproperties.h"


class SimiKHeaderItemModel;
class SimiKHeaderDelegate;
class SimiKHeaderView : public QTableView , public CustomHeaderStyledProperities
{
    Q_OBJECT
    Q_PROPERTY(QPixmap upPixmap READ upPixmap WRITE setUpPixmap)
    Q_PROPERTY(QPixmap downPixmap READ downPixmap WRITE setDownPixmap)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor)

public:
    SimiKHeaderView(QTableView *tableView,QWidget *parent = 0);

    void setColumnInfos(const QVector<ColumnInfo> &columnInfos);

    void setSortIndicator(const int &logicalIndex,const Qt::SortOrder &sortOrder);

    void setColumnWidth(int column, int width);

    //设置每种股票的列数 中间 为相似度
    void setStockColumnCount(int count = 4);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *){}
    void showEvent(QShowEvent *e);

private:
    void initItemDelegate();
    void initItemDelegateProperties();

private:
    QTableView                      *m_tableView;
    SimiKHeaderItemModel     *m_model;
    SimiKHeaderDelegate        *m_headerDelegate;
    QMap<int,Qt::SortOrder>    m_sortMap;
    int                                       m_sortIndex;
    QVector<ColumnInfo>         m_columnInfos;

    bool m_isInitedProperties = false;
    bool                m_msPressed;
    int                   m_resizeColumn;  //当前缩放的列
    const int         cstk_columnCount;
};

#endif // SIMIKHEADERVIEW_H
