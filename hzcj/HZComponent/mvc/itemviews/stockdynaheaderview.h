/********************************************************************
created:2018/06
author: dk.zhang
purpose:行情列表水平自定义水平表头
*********************************************************************/
#ifndef STOCKDYNAHEADERVIEW_H
#define STOCKDYNAHEADERVIEW_H

#include <QHeaderView>
#include "../itemmodels/baseitemmodel.h"

//行情列表表头基类
class BaseStockDynaHeaderView : public QHeaderView
{
public:
    BaseStockDynaHeaderView(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);

    //param[in] 列信息结构体列表，由model读取配置文件加载
   virtual void setColumnInfos(const QVector<ColumnInfo> &columnInfos){m_columnInfos = columnInfos;}
    // 分别设置列左右的Margin 边距
    void setLeftMargin(const int &margin){m_leftMargin = margin;}
    void setRightMargin(const int &margin){m_rightMargin = margin;}

protected:
    QRect getValidRect(const QRect &rect,Qt::Alignment alignment) const;

protected:
    QVector<ColumnInfo>   m_columnInfos;
    int                                 m_leftMargin,m_rightMargin;
};



//自定义行情列表水平表头
class StockDynaHeaderView : public BaseStockDynaHeaderView
{
    Q_OBJECT
public:
    StockDynaHeaderView(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR)
        :BaseStockDynaHeaderView(orientation,parent){}

protected:
    void mouseReleaseEvent(QMouseEvent *e);

    //自定义绘制函数
    void  paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
};


//自定义自选股水平表头,实现点击表头首列实现初始排序
class ZXStockDynaHeaderView : public BaseStockDynaHeaderView
{
public:
    ZXStockDynaHeaderView(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR)
        :BaseStockDynaHeaderView(orientation,parent),m_curIndex(-1){}

    void setColumnInfos(const QVector<ColumnInfo> &columnInfos){
        m_columnInfos = columnInfos;
        m_columnInfos[0].label = QStringLiteral("初始");
    }

protected:
    void mouseReleaseEvent(QMouseEvent *e);

//    void mouseMoveEvent(QMouseEvent *e);

    //自定义绘制函数
    void  paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
   int m_curIndex;
};


#endif // STOCKDYNAHEADERVIEW_H
