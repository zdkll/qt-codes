/********************************************************************
created:2018/08/07
author: dk.zhang
purpose: 资金列表自定义表头
*********************************************************************/
#ifndef FUNDHEADERVIEW_H
#define FUNDHEADERVIEW_H

#include <QTableView>
#include "../itemmodels/baseitemmodel.h"
#include "frozentableview.h"
#include "viewstyleproperties.h"

class FundHeaderItemModel;
class FundHeaderDelegate;
class FundHeaderView : public FrozenColTableView,public CustomHeaderStyledProperities
{
    Q_OBJECT
    Q_PROPERTY(QPixmap upPixmap READ upPixmap WRITE setUpPixmap)
    Q_PROPERTY(QPixmap downPixmap READ downPixmap WRITE setDownPixmap)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor)

public:
    // @param[in]  水平滚动条
    // @param[in]  对应的控制表格
    FundHeaderView(QScrollBar *hscrollBar,QTableView *tableView,QWidget *parent = 0);

    // @brief 设置排序列和排序方式(升序或者降序)
    void setSortIndicator(const int &logicalIndex,const Qt::SortOrder &sortOrder);

    // @brief 设置列宽
    void setColumnWidth(int column, int width);

    //param[in]  列信息结构体列表，从表格Model中读取配置文件得到
    void setColumnInfos(const QVector<ColumnInfo> &columnInfos);

    //@brief 表头起始主列数，分合并的列
    void setHeaderColCount(const int &count);

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
    FundHeaderItemModel       *m_model;
    FundHeaderDelegate         *m_headerDelegate;
    QMap<int,Qt::SortOrder>    m_sortMap;
    QTableView                        *m_tableView;
    int                                       m_sortIndex;
    QVector<ColumnInfo>         m_columnInfos;

    bool                m_msPressed;
    int                   m_resizeColumn;  //当前缩放的列
    int                   m_headerColCount; //开头单列的行数

    bool               m_isInitedProperties;
};

#endif // FUNDHEADERVIEW_H
