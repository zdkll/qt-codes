#include "linealgorith.h"

#include <QLineF>

//线和矩形的交叉点
QVector<QPointF> LineAlgorith::lineIntersect(const QPoint& lpt1,const QPoint& lpt2,const QRect& rect)
{
    QPoint p(rect.bottomRight()-rect.topLeft());
    int length =  p.manhattanLength();

    QVector2D vec1(lpt1-lpt2);
    vec1.normalize();
    QPoint p1 = lpt1 + (vec1*length).toPoint();

    QVector2D vec2(lpt2-lpt1);
    vec2.normalize();
    QPoint p2 = lpt2 + (vec2*length).toPoint();

    QLineF line(p1,p2);

    QLineF line1 =QLineF(rect.topLeft(),rect.topRight());
    QLineF line2 =QLineF(rect.topRight()+QPoint(0,1),rect.bottomRight());
    QLineF line3 =QLineF(rect.bottomLeft()+QPoint(1,0),rect.bottomRight()-QPoint(1,0));
    QLineF line4 =QLineF(rect.topLeft()+QPoint(0,1),rect.bottomLeft());

    QPointF pt1, pt2, pt3, pt4;
    QVector<QPointF> pts;
    bool bi1 = QLineF::BoundedIntersection == line.intersect(line1, &pt1);
    if(bi1)
        pts.append(pt1);
    bool bi2 = QLineF::BoundedIntersection == line.intersect(line2, &pt2);
    if(bi2)
        pts.append(pt2);
    bool bi3 = QLineF::BoundedIntersection == line.intersect(line3, &pt3);
    if(bi3)
        pts.append(pt3);
    bool bi4 = QLineF::BoundedIntersection == line.intersect(line4, &pt4);
    if(bi4)
        pts.append(pt4);

    return pts;
}

//射线和矩形的交叉点
QPointF LineAlgorith::rayLineIntersect(const QPoint& lpt1,const QPoint& lpt2,const QRect& rect)
{
    QPoint p(rect.bottomRight()-rect.topLeft());
    int length =  p.manhattanLength();

    QVector2D vec(lpt2-lpt1);
    vec.normalize();
    QPoint p2 = lpt2 + (vec*length).toPoint();
    QLineF line(lpt1,p2);

    QLineF line1 =QLineF(rect.topLeft(),rect.topRight());
    QLineF line2 =QLineF(rect.topRight(),rect.bottomRight());
    QLineF line3 =QLineF(rect.bottomLeft(),rect.bottomRight());
    QLineF line4 =QLineF(rect.topLeft(),rect.bottomLeft());

    QPointF pt;
    bool bi1 = QLineF::BoundedIntersection == line.intersect(line1, &pt);
    if(bi1)
        return pt;
    bool bi2 = QLineF::BoundedIntersection == line.intersect(line2, &pt);
    if(bi2)
        return pt;
    bool bi3 = QLineF::BoundedIntersection == line.intersect(line3, &pt);
    if(bi3)
        return pt;
    bool bi4 = QLineF::BoundedIntersection == line.intersect(line4, &pt);
    if(bi4)
        return pt;
    return pt;
}

QVector2D LineAlgorith::offsetVector2D(const QPoint& lp1,const QPoint& lp2,const QPoint& pt)
{
    QVector2D vec1 = QVector2D(lp2 - lp1);
    QVector2D vec2 = QVector2D(pt  - lp1);

    float length = QVector2D::dotProduct(vec1,vec2)/vec1.length();
    QVector2D vec = length*vec1.normalized();

    return (vec2 - vec);
}

bool LineAlgorith::isSegLineSelected(const QPoint &lp1, const QPoint &lp2, const QPoint &pt,int max_distance)
{
    QVector2D vec11 = QVector2D(lp2-lp1);
    QVector2D vec12 = QVector2D(pt -lp1);
    QVector2D vec21 = QVector2D(lp1-lp2);
    QVector2D vec22 = QVector2D(pt -lp2);

    if(QVector2D::dotProduct(vec11,vec12)>=0 && QVector2D::dotProduct(vec21,vec22)>=0){
        if(QVector2D(pt).distanceToLine(QVector2D(lp1),vec11.normalized()) < max_distance)
            return true;
    }
    return false;
}

bool LineAlgorith::isStraightLineSelected(const QPoint &lp1, const QPoint &lp2, const QPoint &pt,int max_distance)
{
    QVector2D vec11 = QVector2D(lp2-lp1);

    if(QVector2D(pt).distanceToLine(QVector2D(lp1),vec11.normalized()) < max_distance)
        return true;
    return false;
}

bool LineAlgorith::isRayLineSelected(const QPoint &lp1, const QPoint &lp2, const QPoint &pt,int max_distance)
{
    QVector2D vec11 = QVector2D(lp2-lp1);
    QVector2D vec12 = QVector2D(pt -lp1);

    if(QVector2D::dotProduct(vec11,vec12)>=0)
        if(QVector2D(pt).distanceToLine(QVector2D(lp1),vec11.normalized()) < max_distance)
            return true;
    return false;
}
