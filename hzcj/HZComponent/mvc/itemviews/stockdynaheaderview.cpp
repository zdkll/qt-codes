#include "stockdynaheaderview.h"

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>


BaseStockDynaHeaderView::BaseStockDynaHeaderView(Qt::Orientation orientation, QWidget *parent)
    :QHeaderView(orientation,parent)
{
    this->setSectionsClickable(true);
    m_leftMargin   = 10;
    m_rightMargin = 8;
}

QRect BaseStockDynaHeaderView::getValidRect(const QRect &rect,Qt::Alignment alignment) const
{
    return  (alignment & Qt::AlignRight)?rect.adjusted(0,0,-m_rightMargin,0):
                                         (alignment & Qt::AlignLeft)?rect.adjusted(m_leftMargin,0,0,0):rect;
}


///StockDynaHeaderView--
void StockDynaHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    int logicIndex = logicalIndexAt(e->x());
    if(logicIndex<0)
        return BaseStockDynaHeaderView::mouseReleaseEvent(e);

    if(m_columnInfos[logicIndex].sortType == DisableSort)
        return ;

    return BaseStockDynaHeaderView::mouseReleaseEvent(e);
}

void  StockDynaHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QStyleOptionHeader opt;
    opt.orientation = Qt::Horizontal;
    opt.section = logicalIndex;

    this->initStyleOption(&opt);
    opt.rect = rect;

    //1 绘制单元格--------------------------
    this->style()->drawControl(QStyle::CE_HeaderSection,&opt,painter,this);

    //2 绘制文字-----------------------------
    opt.text = m_columnInfos[logicalIndex].label;
    //对齐
    Qt::Alignment alignment = Qt::AlignVCenter|m_columnInfos[logicalIndex].alignment;
    opt.textAlignment = alignment;

    //对齐后的绘制区域
    opt.rect =  getValidRect(rect,alignment);
    if(this->sortIndicatorSection() == logicalIndex && (alignment&Qt::AlignRight) && this->isSortIndicatorShown())
        opt.rect =  opt.rect.adjusted(0,0,-10,0);
    QApplication::style()->drawControl(QStyle::CE_HeaderLabel,&opt,painter,this);

    //3 排序箭头----------------------------
    if(this->sortIndicatorSection() == logicalIndex && m_columnInfos[logicalIndex].sortType != DisableSort && this->isSortIndicatorShown()){
        int textWidth = opt.fontMetrics.width(opt.text);
        if(alignment & Qt::AlignRight){
            opt.rect = QRect(opt.rect.right()+1,0,10,opt.rect.height());
        }else if(alignment & Qt::AlignLeft){
            opt.rect  = QRect(opt.rect.left()+textWidth+1,0,10,opt.rect.height());
        }else{
            opt.rect  = QRect(opt.rect.center().x()+textWidth/2+1,0,10,opt.rect.height());
        }

        opt.sortIndicator = QStyleOptionHeader::SortIndicator(this->sortIndicatorOrder());
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow,&opt,painter,this);
    }
}


///ZXStockDynaHeaderView--
void ZXStockDynaHeaderView::mouseReleaseEvent(QMouseEvent *e)
{
    int logicIndex = logicalIndexAt(e->x());
    if(m_columnInfos[logicIndex].idx == ColNo){
        //恢复到默认排序
        this->setSortIndicator(0,Qt::AscendingOrder);
        return;
    }else if(m_columnInfos[logicIndex].sortType == DisableSort)
        return;

    return BaseStockDynaHeaderView::mouseReleaseEvent(e);
}

//void  ZXStockDynaHeaderView::mouseMoveEvent(QMouseEvent *e)
//{
//    BaseStockDynaHeaderView::mouseMoveEvent(e);
//    m_curIndex =  this->logicalIndexAt(e->x());
//}

void  ZXStockDynaHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QStyleOptionHeader opt;
    opt.orientation = Qt::Horizontal;
    opt.section = logicalIndex;

    this->initStyleOption(&opt);
    opt.rect = rect;

    //1 绘制单元格--------------------------
    this->style()->drawControl(QStyle::CE_HeaderSection,&opt,painter,this);

    //绘制初始单元格背景色，待后期调整
    //    if(logicalIndex == 0){
    //        painter->setBrush(Qt::red);
    //        painter->drawRect(rect);
    //    }

    //2 绘制文字-----------------------------
    opt.text = m_columnInfos[logicalIndex].label;
    //对齐
    Qt::Alignment alignment = Qt::AlignVCenter|m_columnInfos[logicalIndex].alignment;
    opt.textAlignment = alignment;

    //对齐后的绘制区域
    opt.rect =  getValidRect(rect,alignment);
    if(this->sortIndicatorSection() == logicalIndex && (alignment&Qt::AlignRight))
        opt.rect =  opt.rect.adjusted(0,0,-10,0);
    QApplication::style()->drawControl(QStyle::CE_HeaderLabel,&opt,painter,this);

    //3 排序箭头----------------------------
    if(this->sortIndicatorSection() == logicalIndex && m_columnInfos[logicalIndex].sortType != DisableSort){
        int textWidth = opt.fontMetrics.width(opt.text);
        if(alignment & Qt::AlignRight){
            opt.rect = QRect(opt.rect.right()+1,0,10,opt.rect.height());
        }else if(alignment & Qt::AlignLeft){
            opt.rect  = QRect(opt.rect.left()+textWidth+1,0,10,opt.rect.height());
        }else{
            opt.rect  = QRect(opt.rect.center().x()+textWidth/2+1,0,10,opt.rect.height());
        }

        opt.sortIndicator = QStyleOptionHeader::SortIndicator(this->sortIndicatorOrder());
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow,&opt,painter,this);
    }
}
