#include "line.h"

#include <assert.h>
#include <QVector2D>
#include <QDebug>

#include "linealgorith.h"

///LineFactory---
LineFactory::LineFactory()
{
    m_ioc.RegisterInherit<AbstractLine,StraightLine,const QPen&>("StraightLine");
    m_ioc.RegisterInherit<AbstractLine,RayLine,const QPen&>("RayLine");
    m_ioc.RegisterInherit<AbstractLine,SegLine,const QPen&>("SegLine");
    m_ioc.RegisterInherit<AbstractLine,HorizLine,const QPen&>("HorizLine");
    m_ioc.RegisterInherit<AbstractLine,VertLine,const QPen&>("VertLine");
    m_ioc.RegisterInherit<AbstractLine,RectLine,const QPen&>("RectLine");
    m_ioc.RegisterInherit<AbstractLine,ParallelLine,const QPen&>("ParallelLine");
    m_ioc.RegisterInherit<AbstractLine,ParallelTrendLine,const QPen&>("ParallelTrendLine");
    m_ioc.RegisterInherit<AbstractLine,EqualPeriodLine,const QPen&>("EqualPeriodLine");
    m_ioc.RegisterInherit<AbstractLine,FibonacciLine,const QPen&>("FibonacciLine");
    m_ioc.RegisterInherit<AbstractLine,GoldenSectionLine,const QPen&>("GoldenSectionLine");
    m_ioc.RegisterInherit<AbstractLine,AmpScaleLine,const QPen&>("AmpScaleLine");

    m_ioc.RegisterInherit<AbstractLine,UpArrowLine,const QPen&>("UpArrowLine");
    m_ioc.RegisterInherit<AbstractLine,DownArrowLine,const QPen&>("DownArrowLine");
    m_ioc.RegisterInherit<AbstractLine,TextNoteLine,const QPen&>("TextNoteLine");
    m_ioc.RegisterInherit<AbstractLine,MarkPriceLine,const QPen&>("MarkPriceLine");

    m_lineMp = std::unordered_map<DrawLine::LineToolType,std::string>(
    {{DrawLine::eStraightLine,"StraightLine"}
     ,{DrawLine::eRayLine,"RayLine"}
     ,{DrawLine::eSegLine,"SegLine"}
     ,{DrawLine::eHorizLine,"HorizLine"}
     ,{DrawLine::eVertLine,"VertLine"}
     ,{DrawLine::eRectLine,"RectLine"}
     ,{DrawLine::eParalLine,"ParallelLine"}
     ,{DrawLine::eParallelTrendLine,"ParallelTrendLine"}
     ,{DrawLine::eEqualPeriodLine,"EqualPeriodLine"}
     ,{DrawLine::eFibonacciLine,"FibonacciLine"}
     ,{DrawLine::eGoldenSectionLine,"GoldenSectionLine"}
     ,{DrawLine::eAmpScaleLine,"AmpScaleLine"}
     ,{DrawLine::eUpArrow,"UpArrowLine"}
     ,{DrawLine::eDownArrow,"DownArrowLine"}
     ,{DrawLine::eTextNote,"TextNoteLine"}
     ,{DrawLine::eMarkPriceLine,"MarkPriceLine"}});

}

LineFactory *LineFactory::instance()
{
    static    LineFactory lineFactory;
    return &lineFactory;
}

AbstractLine *LineFactory::createLine(DrawLine::LineToolType lineType,const QPen& pen)
{
    qDebug()<<"create line:"<<lineType;
    auto it = m_lineMp.find(lineType);
    if(it != m_lineMp.end()){
        AbstractLine *bline = m_ioc.Resolve<AbstractLine,const QPen&>(it->second,pen);
        if(bline)
            bline->setLineType(lineType);
        return bline;
    }
    return Q_NULLPTR;
}


///AbstractLine---
QRect  AbstractLine::sRect = QRect();
float     AbstractLine::sTPeriod = 5.0f;
QColor AbstractLine::sActivedColor = Qt::white;
QFont  AbstractLine::sFont = QFont();
AbstractLine::AbstractLine(const QPen& pen)
    :m_pen(pen)
    ,m_isChanged(false)
    ,m_state(Normal)
    ,m_activedIdx(-1)
{

}

void AbstractLine::msPressed(const QPoint &pt)
{
    this->addPoint(pt);
    this->addPoint(pt);

    //该点激活修改
    this->actived(pt);
}

bool AbstractLine::actived(const QPoint &pt)
{
    //遍历判断是否是选中某点
    m_activedIdx = findPoint(pt);
    if(m_activedIdx>=0){
        this->setState(AbstractLine::Revising);
        return true;
    }

    //判断是否选中
    bool selected = isSelectedBy(pt);
    if(selected){
        this->setState(AbstractLine::Selecting);
        return true;
    }
    return false;
}

AbstractLine::State AbstractLine::pointAtLine(const QPoint &pt)
{
    //遍历判断是否是选中某点
    if(findPoint(pt)>=0)
        return AbstractLine::Revising;

    //判断是否选中
    if(isSelectedBy(pt))
        return AbstractLine::Selecting;

    return AbstractLine::Normal;
}

void AbstractLine::paint(QPainter *pt)
{
    //绘制描点-只有选中的时候才绘制
    if(this->state() == Selecting){
        QPen pen = m_pen;
        pen.setColor(sActivedColor);
        pt->setPen(pen);

        paintPoints(pt);
    }
    //绘制线---
    if(this->state() == Revising){
        QPen pen = m_pen;
        pen.setColor(sActivedColor);
        pt->setPen(pen);
    }else
        pt->setPen(m_pen);
    paintLines(pt);
}

void AbstractLine::paintPoints(QPainter *painter)
{
    if(this->state() != Normal){
        painter->setBrush(sActivedColor);
        QPoint pt;
        for(int i=0;i<this->size();i++){
            pt = (*this)[i].sPt+QPoint(-2,-2);
            painter->drawRect(QRect(pt.x(),pt.y(),POINT_RECT_WIDTH,POINT_RECT_WIDTH));
        }
        painter->setBrush(Qt::NoBrush);
    }
}

void AbstractLine::paintLines(QPainter *painter)
{
    for(int i=0;i<this->size();i+=2){
        painter->drawLine((*this)[i].sPt,(*this)[i+1].sPt);
    }
}

//线段类型可以使用此判断方法
bool AbstractLine::isSelectedBy(const QPoint &pt)
{
    assert(this->size() >=2);
    return  LineAlgorith::isSegLineSelected((*this)[0].sPt,(*this)[1].sPt,pt,MAX_DISTANCE_VALUE);
}



///StraightLine--
void StraightLine::paintLines(QPainter *pt)
{
    QVector<QPointF> pts = LineAlgorith::lineIntersect((*this)[0].sPt,(*this)[1].sPt,sRect);
    pt->drawLines(pts);
}

bool StraightLine::isSelectedBy(const QPoint &pt)
{
    assert(this->size() >=2);
    return  LineAlgorith::isStraightLineSelected((*this)[0].sPt,(*this)[1].sPt,pt,MAX_DISTANCE_VALUE);
}


///RayLine---
void RayLine::paintLines(QPainter *painter)
{
    QPointF pt = LineAlgorith::rayLineIntersect((*this)[0].sPt,(*this)[1].sPt,sRect);
    painter->drawLine((*this)[0].sPt,pt);
}

bool RayLine::isSelectedBy(const QPoint &pt)
{
    assert(this->size() >=2);
    return  LineAlgorith::isRayLineSelected((*this)[0].sPt,(*this)[1].sPt,pt,MAX_DISTANCE_VALUE);
}


///HorizLine---
void HorizLine::msPressed(const QPoint &pt)
{
    //只有一个点
    this->addPoint(pt);
    //该点激活修改
    this->actived(pt);
}

void HorizLine::paintLines(QPainter *painter)
{
    painter->drawLine(sRect.left(),(*this)[0].sPt.y(),sRect.right(),(*this)[0].sPt.y());
}

bool HorizLine::isSelectedBy(const QPoint &pt)
{
    return abs((*this)[0].sPt.y()-pt.y())<MAX_DISTANCE_VALUE;
}


///VertLine---
void VertLine::msPressed(const QPoint &pt)
{
    //只有一个点
    this->addPoint(pt);
    //该点激活修改
    this->actived(pt);
}

void VertLine::paintLines(QPainter *painter)
{
    painter->drawLine((*this)[0].sPt.x(),sRect.top(),(*this)[0].sPt.x(),sRect.bottom());
}

bool VertLine::isSelectedBy(const QPoint &pt)
{
    return abs((*this)[0].sPt.x()-pt.x())<MAX_DISTANCE_VALUE;
}


///RectLine---
void RectLine::paintLines(QPainter *painter)
{
    painter->drawRect(QRect((*this)[0].sPt, (*this)[1].sPt));
}

bool RectLine::isSelectedBy(const QPoint &pt)
{
    int x1,x2,y1,y2;
    QPoint p1 = (*this)[0].sPt;
    QPoint p2 = (*this)[1].sPt;
    p1.x()<=p2.x()?(x1=p1.x(),x2=p2.x()):(x1=p2.x(),x2=p1.x());
    p1.y()<=p2.y()?(y1=p1.y(),y2=p2.y()):(y1=p2.y(),y2=p1.y());

    if(x1<=pt.x() && pt.x()<=x2)
    {
        if(abs(pt.y() - y1) < MAX_DISTANCE_VALUE || abs(pt.y() - y2) < MAX_DISTANCE_VALUE)
            return true;
    }
    if(y1<=pt.y() && pt.y()<=y2)
    {
        if(abs(pt.x() - x1) < MAX_DISTANCE_VALUE || abs(pt.x() - x2) < MAX_DISTANCE_VALUE)
            return true;
    }
    return false;
}



///ParallelLine---
void ParallelLine::paintLines(QPainter *painter)
{
    //0 1 基准线段
    painter->drawLine((*this)[0].sPt,(*this)[1].sPt);

    //判断是否有第三个点,画第二条平行线
    if(this->size()>2){
        //偏移向量
        QVector2D offsetVec = LineAlgorith::offsetVector2D((*this)[0].sPt,(*this)[1].sPt,(*this)[2].sPt);
        painter->drawLine((*this)[0].sPt + offsetVec.toPoint(),(*this)[1].sPt + offsetVec.toPoint());
    }
}


bool ParallelLine::isSelectedBy(const QPoint &pt)
{
    //分别判断两条线段是否被选中
    bool isSelected = LineAlgorith::isSegLineSelected((*this)[0].sPt,(*this)[1].sPt,pt, MAX_DISTANCE_VALUE);
    if(isSelected) return true;

    if(this->size()>2){
        //偏移向量
        QVector2D offsetVec = LineAlgorith::offsetVector2D((*this)[0].sPt,(*this)[1].sPt,(*this)[2].sPt);
        return LineAlgorith::isSegLineSelected((*this)[0].sPt + offsetVec.toPoint(),(*this)[1].sPt + offsetVec.toPoint()
                ,pt, MAX_DISTANCE_VALUE);
    }
    return false;
}

bool ParallelLine::moveUnPressed(const QPoint &pt)
{
    if(m_activedIdx>=0 && state() == Revising)
        (*this)[m_activedIdx].sPt += pt;
    return true;
}

bool ParallelLine::msRelease(const QPoint &pt)
{
    //添加第三个点,并修改当前修改点为第三个点
    if(this->size()<3){
        this->addPoint(pt);
        this->setActivedPoint(2);
    }else if(state() == Revising){
        this->setState(Selecting);
        m_activedIdx = -1;
        return true;
    }
    return false;
}


///ParallelTrendLine---
void ParallelTrendLine::paintLines(QPainter *painter)
{
    //0 1 基准线段
    QVector<QPointF> pts = LineAlgorith::lineIntersect((*this)[0].sPt,(*this)[1].sPt,sRect);
    painter->drawLines(pts);

    //判断是否有第三个点,画第二条平行线
    if(this->size()>2){
        //偏移向量
        QVector2D offsetVec = LineAlgorith::offsetVector2D((*this)[0].sPt,(*this)[1].sPt,(*this)[2].sPt);
        pts = LineAlgorith::lineIntersect((*this)[0].sPt + offsetVec.toPoint(),(*this)[1].sPt + offsetVec.toPoint(),sRect);
        painter->drawLines(pts);
    }
}


bool ParallelTrendLine::isSelectedBy(const QPoint &pt)
{
    //分别判断两条线段是否被选中
    bool isSelected = LineAlgorith::isStraightLineSelected((*this)[0].sPt,(*this)[1].sPt,pt, MAX_DISTANCE_VALUE);
    if(isSelected) return true;

    if(this->size()>2){
        //偏移向量
        QVector2D offsetVec = LineAlgorith::offsetVector2D((*this)[0].sPt,(*this)[1].sPt,(*this)[2].sPt);
        return LineAlgorith::isStraightLineSelected((*this)[0].sPt + offsetVec.toPoint(),(*this)[1].sPt + offsetVec.toPoint()
                ,pt, MAX_DISTANCE_VALUE);
    }
    return false;
}


///EqualPeriodLine---
void EqualPeriodLine::paintLines(QPainter *painter)
{
    //绘制第一根基准线
    int x = (*this)[0].sPt.x();
    painter->drawLine(x,0,x,sRect.height());

    int dx = (*this)[1].sPt.x() - (*this)[0].sPt.x();
    if(abs(dx)<sTPeriod) return;

    x += dx;
    while(0<x && x<sRect.width()){
        painter->drawLine(x,0,x,sRect.height());
        x += dx;
    }
}

bool EqualPeriodLine::isSelectedBy(const QPoint &pt)
{
    //绘制第一根基准线
    int x = (*this)[0].sPt.x();
    if(abs(pt.x() -x)<MAX_DISTANCE_VALUE) return true;

    int dx = (*this)[1].sPt.x() - (*this)[0].sPt.x();
    if(abs(dx)<sTPeriod) return false;

    x += dx;
    while(0<x && x<sRect.width()){
        if(abs(pt.x() -x)<MAX_DISTANCE_VALUE)
            return true;
        x += dx;
    }
    return  false;
}


///FibonacciLine---
void FibonacciLine::msPressed(const QPoint &pt)
{
    this->addPoint(pt);
    //该点激活修改
    this->actived(pt);
}

void FibonacciLine::paintLines(QPainter *painter)
{
    int n1 = 0,n2 = 1,n = n1+n2;

    int x = (*this)[0].sPt.x()+n*sTPeriod;
    while(x<sRect.width()){
        painter->drawLine(x,0,x,sRect.height());
        n1 = n2;
        n2 = n;
        n   = n1 + n2;

        x = (*this)[0].sPt.x()+n*sTPeriod;
    }
}

bool FibonacciLine::isSelectedBy(const QPoint &pt)
{
    int n1 = 0,n2 = 1,n = n1+n2;

    int x = (*this)[0].sPt.x()+n*sTPeriod;
    while(x<sRect.width()){
        if(abs(pt.x() - x)<MAX_DISTANCE_VALUE)
            return true;
        n1 = n2;
        n2 = n;
        n   = n1 + n2;

        x = (*this)[0].sPt.x()+n*sTPeriod;
    }
    return false;
}


///GoldenSectionLine--
GoldenSectionLine::GoldenSectionLine(const QPen &pen)
    :AbstractLine(pen)
{
    m_goldRatios = {0.f,0.236f,0.382f,0.5f,0.618f,0.809f,1.0f,1.38f,1.61f,2.0f,2.38f,2.61f};
    m_cursorIndex = 6;
}

void GoldenSectionLine::paintLines(QPainter *painter)
{
    QPoint p1 = (*this)[0].sPt;
    QPoint p2 = (*this)[1].sPt;

    int x = p1.x();
    painter->drawLine(p1,QPoint(sRect.width(),p1.y()));
    painter->drawLine(x,p2.y(),sRect.width(),p2.y());

    int uint_y = p2.y() - p1.y();
    QPen pen = m_pen;
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    for(int i=0;i<m_goldRatios.size();i++)
        if(i != 0 && i != m_cursorIndex){
            int y = p1.y()+qRound(m_goldRatios[i]*uint_y);
            painter->drawLine(x,y,sRect.width(),y);
        }
}

bool GoldenSectionLine::isSelectedBy(const QPoint &pt)
{
    QPoint p1 = (*this)[0].sPt;
    QPoint p2 = (*this)[1].sPt;

    int x = p1.x();
    int uint_y = p2.y() - p1.y();
    if(x<pt.x() && pt.y()<sRect.width())
        for(int i=0;i<m_goldRatios.size();i++){
            int y = p1.y()+qRound(m_goldRatios[i]*uint_y);
            if(abs(pt.y() -y)<MAX_DISTANCE_VALUE)
                return true;
        }
    return false;
}


///AmpScaleLine---
void AmpScaleLine::paintLines(QPainter *painter)
{
    QPoint p1 = (*this)[0].sPt;
    QPoint p2 = (*this)[1].sPt;
    painter->drawLine(p1,QPoint(p2.x(),p1.y()));
    painter->drawLine(QPoint(p1.x(),p2.y()),p2);

    QPen pen =  m_pen;
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawLine((p1.x()+p2.x())/2,p1.y(),(p1.x()+p2.x())/2,p2.y());
}

bool AmpScaleLine::isSelectedBy(const QPoint &pt)
{
    int x1,x2,y1,y2;
    QPoint p1 = (*this)[0].sPt;
    QPoint p2 = (*this)[1].sPt;
    (p1.x()<p2.x())?(x1 = p1.x(),x2=p2.x()):(x1 = p2.x(),x2=p1.x());
    (p1.y()<p2.y())?(y1= p1.y(),y2=p2.y()):(y1= p2.y(),y2=p1.y());

    float mid_x  = float(x1+x2)/2.0f;
    if(x1<pt.x() && pt.x()<x2){
        if(abs(p1.y() - pt.y())<MAX_DISTANCE_VALUE || abs(p2.y() - pt.y())<MAX_DISTANCE_VALUE)
            return true;
        else if(y1<pt.y() && pt.y()<y2)
            if(abs(mid_x-pt.x())<MAX_DISTANCE_VALUE)
                return true;
    }
    return false;
}

///ArrowLine--
ArrowLine::ArrowLine(ArrowLine::ArrowType at, const QPen &pen)
    :AbstractLine(pen)
{

    //创建箭头Path
    m_ArrowPath = createPath(at);
}

void ArrowLine::msPressed(const QPoint &pt)
{
    this->addPoint(pt);
    this->actived(pt);
}

void ArrowLine::paintPoints(QPainter *painter)
{
    QRect rect(-4,0,8,12);
    painter->drawRect(rect.translated((*this)[0].sPt));
}

void ArrowLine::paintLines(QPainter *painter)
{
    painter->setBrush(m_color);
    painter->setPen(QPen(m_color));
    QPolygon path = m_ArrowPath.translated((*this)[0].sPt);
    painter->drawPolygon(path);
    painter->setBrush(Qt::NoBrush);
}

bool ArrowLine::isSelectedBy(const QPoint &pt)
{
    QRect rect = m_ArrowPath.translated((*this)[0].sPt).boundingRect();
    return rect.contains(pt);
}

QPolygon  ArrowLine::createPath(ArrowType at)
{
    QVector<QPoint> pts;
    if(at == AT_UpArrow){
        int data[][2] = {{0,0},{-4,4},{-1,4},{-1,12}
                         ,{1,12},{1,4},{4,4}};
        for(int i=0;i<7;i++)
            pts.append(QPoint(data[i][0],data[i][1]));
    }else{
        int data[][2] =  {{0,12},{4,8},{1,8},{1,0}
                          ,{-1,0},{-1,8},{-4,8}};
        for(int i=0;i<7;i++)
            pts.append(QPoint(data[i][0],data[i][1]));
    }
    return QPolygon(pts);
}

///TextNoteLine----
TextNoteLine::TextNoteLine(const QPen &pen)
    :AbstractLine(pen)
{
    m_font = QFont();
    m_fontSize = 14;
    m_color = Qt::red;
}

void TextNoteLine::msPressed(const QPoint &pt)
{
    this->addPoint(pt);
    this->actived(pt);
}

void TextNoteLine::setProperty(AbstractLine::PropertyType type, const QVariant &var)
{
    if(type == Pt_Text){
        m_text        = var.toString();
    }else if(type == Pt_FontSize){
        m_fontSize = var.toInt();
    }
}

QVariant TextNoteLine::property(AbstractLine::PropertyType type)
{
    if(type == Pt_Text){
        return m_text;
    }else if(type == Pt_FontSize){
        return m_fontSize;
    }
    return QVariant();
}

void TextNoteLine::paintPoints(QPainter *painter)
{
    m_font = sFont;
    m_font.setPixelSize(m_fontSize);
    QFontMetrics fontMetrix(m_font);

    int textwidth = fontMetrix.width(m_text);
    int textHeight = fontMetrix.height();

    QRect rect(-1,-textHeight/2,textwidth+5,textHeight);
    painter->drawRect(rect.translated((*this)[0].sPt));
}

void TextNoteLine::paintLines(QPainter *painter)
{
    QPoint pt = (*this)[0].sPt;
    painter->save();
    painter->setPen(m_color);

    QFontMetrics fontMetrix(m_font);
    int textHeight = fontMetrix.height();

    painter->setFont(m_font);
    painter->drawText(pt.x(),pt.y()+textHeight/2-1,m_text);

    painter->restore();
}

bool TextNoteLine::isSelectedBy(const QPoint &pt)
{
    QPoint p0 = (*this)[0].sPt;

    QFontMetrics fontMetrix(m_font);
    int textHeight = fontMetrix.height();
    int textWidth = fontMetrix.width(m_text);

    QRect rect(p0.x()-1,p0.y()-textHeight/2,textWidth+5,textHeight);
    return rect.contains(pt);
    return false;
}

///MarkPriceLine---
void MarkPriceLine::msPressed(const QPoint &pt)
{
    this->addPoint(pt);
    //该点激活修改
    this->actived(pt);
}

void MarkPriceLine::paintLines(QPainter *painter)
{
    QPoint pt = (*this)[0].sPt;
    painter->drawLine(pt,QPoint(sRect.width(),pt.y()));
}

bool MarkPriceLine::isSelectedBy(const QPoint &pt)
{
    QPoint p= (*this)[0].sPt;
    if(p.x() <pt.x() && pt.x()<sRect.width())
        if(abs(pt.y()-p.y())<MAX_DISTANCE_VALUE) return true;
    return false;
}



