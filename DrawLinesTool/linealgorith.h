#ifndef LINEALGORITH_H
#define LINEALGORITH_H

#include <QVector>
#include <QPointF>
#include <QRect>
#include <QVector2D>

//线算法
namespace LineAlgorith
{

//线和矩形的交叉点-返回两个
QVector<QPointF>  lineIntersect(const QPoint& lpt1,const QPoint& lpt2,const QRect& rect);

//射线和矩形的交叉点-返回一个 射线方向:(lpt2 - lpt1)
QPointF  rayLineIntersect(const QPoint& lpt1,const QPoint& lpt2,const QRect& rect);

//计算点相对于线段的偏移向量
QVector2D offsetVector2D(const QPoint& lp1,const QPoint& lp2,const QPoint& pt);

//判断线段是否选中
bool isSegLineSelected(const QPoint& lp1,const QPoint& lp2,const QPoint& pt, int max_distance);

//直线是否被选中
bool isStraightLineSelected(const QPoint& lp1,const QPoint& lp2,const QPoint& pt,int max_distance);

//射线是否被选中
bool isRayLineSelected(const QPoint& lp1,const QPoint& lp2,const QPoint& pt,int max_distance);

}

#endif // LINEALGORITH_H
