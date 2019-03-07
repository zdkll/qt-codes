#ifndef CURVEGRAPHSTYLE_H
#define CURVEGRAPHSTYLE_H
/********************************************************************
created:
author: min.cheng
purpose: 曲线图绘制类   静态曲线图 非分时实时变化类型相关的
*********************************************************************/
#include "basewidget.h"

//样式表
class CurveGraphStyle: public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor m_bkClr READ getBkClr WRITE setBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_minLineClr READ getMinLineClr WRITE setMinLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_gridClr READ getGridClr WRITE setGridClr DESIGNABLE true)
    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)
    Q_PROPERTY(QColor m_titleTextClr READ getTitleTextClr WRITE setTitleTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_minAvgLineClr READ getMinAvgLineClr WRITE setMinAvgLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_gradClr READ getGradClr WRITE setGradClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineClr READ getCrossLineClr WRITE setCrossLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineRcBkClr READ getCrossLineRcBkClr WRITE setCrossLineRcBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineTextClr READ getCrossLineTextClr WRITE setCrossLineTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_axisTextClr READ getAxisTextClr WRITE setAxisTextClr DESIGNABLE true)
public:
    CurveGraphStyle(QWidget *parent);

    //背景色
    QColor getBkClr() const{return m_bkClr;}
    void setBkClr(QColor color){m_bkClr=color;}
    QColor m_bkClr = QColor("#0c1215");

    //曲线颜色
    QColor getMinLineClr() const{return m_minLineClr;}
    void setMinLineClr(QColor color){m_minLineClr=color;}
    QColor m_minLineClr = QColor(Qt::red);// QColor(54,129,227);

    //网格颜色
    QColor getGridClr() const{return m_gridClr;}
    void setGridClr(QColor color){m_gridClr=color;}
    QColor m_gridClr = QColor(Qt::blue);

    //上涨颜色
    QColor getUpClr() const{return m_upClr;}
    void setUpClr(QColor color){m_upClr=color;}
    QColor m_upClr = QColor(Qt::red);

    //下跌颜色
    QColor getDownClr() const{return m_downClr;}
    void setDownClr(QColor color){m_downClr=color;}
    QColor m_downClr = QColor(Qt::red);

    //平局颜色  （不涨不跌）
    QColor getEqualClr() const{return m_equalClr;}
    void setEqualClr(QColor color){m_equalClr=color;}
    QColor m_equalClr = QColor(Qt::red);

    //文本颜色
    QColor getTitleTextClr() const{return m_titleTextClr;}
    void setTitleTextClr(QColor color){m_titleTextClr=color;}
    QColor m_titleTextClr=QColor(102,139,139);

    //均线颜色
    QColor getMinAvgLineClr() const{return m_minAvgLineClr;}
    void setMinAvgLineClr(QColor color){m_minAvgLineClr=color;}
    QColor m_minAvgLineClr = QColor(Qt::red);// QColor(54,129,227);

    //渐变
    QColor getGradClr() const{return m_gradClr;}
    void setGradClr(QColor color){m_gradClr=color;}
    QColor m_gradClr = QColor(Qt::red);// QColor(54,129,227);

    //十字光标线条颜色
    QColor getCrossLineClr() const{return m_crossLineClr;}
    void setCrossLineClr(QColor color){m_crossLineClr=color;}
    QColor m_crossLineClr = QColor(Qt::blue);

    //十字矩形区域光标背景颜色
    QColor getCrossLineRcBkClr() const{return m_crossLineRcBkClr;}
    void setCrossLineRcBkClr(QColor color){m_crossLineRcBkClr=color;}
    QColor m_crossLineRcBkClr = QColor(Qt::red);

    //十字光标文本颜色
    QColor getCrossLineTextClr() const{return m_crossLineTextClr;}
    void setCrossLineTextClr(QColor color){m_crossLineTextClr=color;}
    QColor m_crossLineTextClr = QColor(Qt::red);

    //坐标轴颜色
    QColor getAxisTextClr() const{return m_axisTextClr;}
    void setAxisTextClr(QColor color){m_axisTextClr=color;}
    QColor m_axisTextClr = QColor(Qt::red);
};

#endif // CURVEGRAPHSTYLE_H
