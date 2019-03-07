#ifndef SIMIKVIEW_H
#define SIMIKVIEW_H

#include <QTableView>
#include "basewidget.h"
#include "viewstyleproperties.h"
#include "hzcomponent_global.h"
#include "simikmodel.h"

class SimiKHeaderView;
class SimiKHeaderItemModel;
class SortFilterProxyModel;
class QScrollBar;

//相似K线表格
class SimiKTableView : public QTableView,public ViewStyleProperties
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    SimiKTableView(QWidget *parent = Q_NULLPTR);

    void afterActive();
    void afterDeactive();

    void setHeaderView(SimiKHeaderView *headerView){m_headerView = headerView;}
    AbstractBaseModel*   model(){return m_model;}
    SortFilterProxyModel* sortModel(){return m_sortModel;}

signals:
    void expandSimiKline(bool);

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

protected slots:
    void clickedIndex(QModelIndex);

private:
    void hideAllRowExcept(int row);
    void showAllRow();
    void initItemDelegate();

    SimiKModel  *m_model;
    SortFilterProxyModel *m_sortModel;
    QVector<ColumnInfo> m_columnInfos;
    SimiKHeaderView       *m_headerView;

    //新增几个样式 ：相似度和查看相似K线相关样式

};

class SmKWidget :public BaseWidget
{
   public:
SmKWidget(QWidget *parent = 0):BaseWidget(parent){}
protected:
void paintEvent(QPaintEvent *e);
};

//控件
class HZCOMPONENTSHARED_EXPORT SimiKView : public BaseWidget, public ViewStyleProperties
{
    Q_OBJECT
public:
    SimiKView(QWidget *parent = 0);

    void afterActive();
    void afterDeactive();

protected slots:
    void slotExpandKline(bool expand);
    void slotVertBarRangeChanged(int,int);

private:
    void initWg();
    void initTableView();

    //K线图
    BaseWidget          *m_kWidget;

    SimiKHeaderView  *m_headerView;
    SimiKTableView     *m_tableView;
    QScrollBar            *m_vertScrollbar;
};


#endif // SIMIKVIEW_H
