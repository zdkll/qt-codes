#ifndef LINE_H
#define LINE_H

#include <string>
#include <QPen>
#include <QPainter>
#include <QVariant>

#include "drawlinepublic.h"
#include "Ioc.hpp"
#include "kpoint.h"

class AbstractLine;

//创建线
class LineFactory
{
public:
    static LineFactory *instance();
    AbstractLine   *createLine(DrawLine::LineToolType  lineType,const QPen& pen);

private:
    LineFactory();

    std::unordered_map<DrawLine::LineToolType,std::string> m_lineMp;
    IocContainer m_ioc;
};

//线-基类
class AbstractLine : public QVector<KPoint>
{
public:
    AbstractLine(const QPen &pen = QPen());
    virtual ~AbstractLine(){}
    enum State
    {
        Normal   = 0, //正常
        Selecting = 1, //选中-移动整个线
        Revising  = 2 //修改-移动点
    };
    enum PropertyType
    {
        Pt_Text = 0,
        Pt_FontSize = 1
    };
    /*=====================================================*/
    inline void setLineType(int typeId){m_typeId = typeId;}
    inline int    typeId()const {return m_typeId;}
    //画笔
    inline void   setPen(const QPen& pen){m_pen = pen;}
    inline QPen pen() const {return m_pen;}
    //Color
    inline void setColor(const QColor& color){ m_color = color;}
    inline QColor color() const {return m_color;}

    //设置属性(文本-文字大小等)
    virtual void setProperty( PropertyType /*type*/,const QVariant& /*var*/){}
    virtual QVariant property(PropertyType /*type*/){return QVariant();}
    /*=====================================================*/

    //是否改变
    inline void setChanged(bool change){m_isChanged = change;}
    inline bool isChanged()const {return m_isChanged;}

    //状态：正常 选中 选中修改点
    inline void setState(State state){m_state = state;}
    inline State state()const{return m_state;}
    //恢复原始状态
    inline void reset();

    //点击初始化
    virtual void msPressed(const QPoint& pt);

    //点击点，选中点或线
    bool  actived(const QPoint& pt);
    inline void setActivedPoint(int idx);
    //判断鼠标是否在线或点上
    State pointAtLine(const QPoint& pt);

    //移动（线或者当前修改点） 相对位置
    inline bool msMove(const QPoint&pt,bool isPressed = true);

    //鼠标释放-判断命令是否完成(true)
    virtual inline bool msRelease(const QPoint& pt);

    virtual void paint(QPainter *pt); //绘制函数,默认按照线段绘制

    inline static void setGRect(const QRect& rect){sRect = rect; }
    inline static void setGActivedColor(const QColor& color){sActivedColor = color; }
    inline static void setGFont(const QFont& font){sFont = font; }

protected:
    friend class LineFactory;
    inline void addPoint(const QPoint& pt);//添加点
    inline int    findPoint(const QPoint& pt);//查找点

    virtual void paintPoints(QPainter *painter);//绘制点
    virtual void paintLines(QPainter *painter);

    virtual bool isSelectedBy(const QPoint& pt);
    virtual bool moveUnPressed(const QPoint& ){return false;} //非按下移动

protected:
    int        m_typeId;//线工具类型Id
    QPen   m_pen; //样式
    QColor m_color;//

    bool   m_isChanged;//标志是否被修改，修改过msRelease后需要重新定位和绘制
    State m_state;
    int     m_activedIdx = -1; //-1 表示未有激活点

    //绘制区域，所有线的区域是一样的
    static QRect sRect;
    static float    sTPeriod;//最小时间间距
    static QColor  sActivedColor;//激活颜色
    static QFont   sFont;
};

//===================================
//                  inline functions
//===================================
inline void AbstractLine::reset()
{
    this->setState(Normal);
    this->setActivedPoint(-1);
}
inline int AbstractLine::findPoint(const QPoint& pt)
{
    for(int i=this->size()-1;i>=0;i--){
        if(((*this)[i].sPt - pt).manhattanLength()<MAX_DISTANCE_VALUE)
            return i;
    }
    return -1;
}

inline void AbstractLine::addPoint(const QPoint&pt)
{
    this->append(KPoint(pt));
}
inline void AbstractLine::setActivedPoint(int idx)
{
    m_activedIdx = idx;
}

inline bool AbstractLine::msMove(const QPoint& pt,bool isPressed)
{
    if(isPressed){
        if(state() == Revising){
            if(m_activedIdx>=0)
                (*this)[m_activedIdx].sPt += pt;
        }else if(state() == Selecting)
        {
            for(int i=0;i<this->size();i++)
                (*this)[i].sPt += pt;
        }
        return true;
    }
    else
        return moveUnPressed(pt);
}

inline bool AbstractLine::msRelease(const QPoint& )
{
    if(state() == Revising){
        this->setState(Selecting);
        m_activedIdx = -1;
        return true;
    }
    return false;
}

//直线
class StraightLine: public AbstractLine
{
public:
    StraightLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);
    virtual inline bool isSelectedBy(const QPoint& pt);
};

//射线
class RayLine: public AbstractLine
{
public:
    RayLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);
    virtual inline bool isSelectedBy(const QPoint& pt);
};

//线段
class SegLine: public AbstractLine
{
public:
    SegLine(const QPen &pen):AbstractLine(pen){}
};

//水平线
class HorizLine: public AbstractLine
{
public:
    HorizLine(const QPen &pen):AbstractLine(pen){}

    virtual void msPressed(const QPoint& pt);

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//垂直线
class VertLine: public AbstractLine
{
public:
    VertLine(const QPen &pen):AbstractLine(pen){}
    virtual void msPressed(const QPoint& pt);

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//矩形
class RectLine: public AbstractLine
{
public:
    RectLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//平行线段
class ParallelLine: public AbstractLine
{
public:
    ParallelLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);

    virtual bool isSelectedBy(const QPoint& pt);
    virtual bool moveUnPressed(const QPoint& pt); //非按下移动
    virtual bool msRelease(const QPoint& );
};

//平行趋势线-直线
class ParallelTrendLine: public ParallelLine
{
public:
    ParallelTrendLine(const QPen &pen):ParallelLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//等周期线
class EqualPeriodLine: public AbstractLine
{
public:
    EqualPeriodLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//斐波那契周期-只有一个点（向时间轴正方向）
class FibonacciLine: public AbstractLine
{
public:
    FibonacciLine(const QPen &pen):AbstractLine(pen){}

    virtual void msPressed(const QPoint& pt);

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};

//黄金分割线
class GoldenSectionLine: public AbstractLine
{
public:
    GoldenSectionLine(const QPen &pen);

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);

private:
    QVector<float>  m_goldRatios;
    int      m_cursorIndex;
};

//幅度尺
class AmpScaleLine: public AbstractLine
{
public:
    AmpScaleLine(const QPen &pen):AbstractLine(pen){}

protected:
    virtual void paintLines(QPainter *pt);

    virtual bool isSelectedBy(const QPoint& pt);
};

//辅助工具===========================
//上箭头、下箭头
//文字注释
class ArrowLine: public AbstractLine
{
public:
    enum ArrowType
    {
        AT_UpArrow     = 0,
        AT_DownArrow = 1,
    };
    ArrowLine(ArrowType at ,const QPen &pen);

    virtual void msPressed(const QPoint& pt);

protected:
    virtual void paintPoints(QPainter *painter);
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);

private:
    QPolygon   createPath(ArrowType at );
    QPolygon   m_ArrowPath;
};

class UpArrowLine: public ArrowLine
{
public:
    UpArrowLine(const QPen &pen):ArrowLine(AT_UpArrow,pen){m_color = Qt::red;}
};

class DownArrowLine: public ArrowLine
{
public:
    DownArrowLine(const QPen &pen):ArrowLine(AT_DownArrow,pen){m_color = Qt::green;}
};


//文字注释
class TextNoteLine: public AbstractLine
{
public:
    TextNoteLine(const QPen &pen);

    virtual void msPressed(const QPoint& pt);
    virtual void setProperty(PropertyType, const QVariant &);
    virtual QVariant property(PropertyType);

protected:
    virtual void paintPoints(QPainter *pt); //描点在文本框中间
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);

private:
    QFont   m_font; //字体
    int         m_fontSize;
    QString m_text;
};

//标价线
class MarkPriceLine: public AbstractLine
{
public:
    MarkPriceLine(const QPen &pen):AbstractLine(pen){}

    virtual void msPressed(const QPoint& pt);

protected:
    virtual void paintLines(QPainter *pt);
    virtual bool isSelectedBy(const QPoint& pt);
};


#endif // LINE_H
