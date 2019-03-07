#include "weeklydelegates.h"
#include <QPainter>
#include <QModelIndex>
#include <QtMath>
#include <QDebug>
#include <QTableView>
#include <QApplication>
#include "weeklys.h"
#include "weeklydef.h"
#include "quoteformat.h"
#include "mvcpublicdef.h"

#define Draw_Item_Value_1 QStringLiteral("每100股收益")
#define Draw_Item_Value_2 QStringLiteral("股票代码")

void WeeklyChgDelegate::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    /*if(index.column() == 0)
    {

    } else */if(index.column() == 1)
    {
        qreal curVal = qAbs(index.data(CapitalChange_Limit_Val).toDouble()),
                tMax = index.data(CapitalChange_Limit_Max).toDouble(),
                tMin = index.data(CapitalChange_Limit_Min).toDouble();

        if(tMax == -qInf() || tMin == qInf()){
            QStyledItemDelegate::paint(p, opt, index);
            return ;
        }
        QRect drawRt(opt.rect.adjusted(1,5,-1,-5));
        QColor crl = m_type == CapitalChange::CapitalChange_In?QColor("#0ec350"):QColor("#ff3f40");
        drawRt.setWidth(opt.rect.width()/tMax*curVal);//要想和设计稿上一样，只需要两个表格的数据相反排序（一方升序，另一方降序）
        p->setBrush(crl);
        p->setPen(crl);
        p->drawRect(drawRt);
    } else {
        QStyledItemDelegate::paint(p, opt, index);
    }
}

void WeeklyChgDelegate2::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);

    QPen oldPen = painter->pen();
    painter->setPen(Qt::NoPen);
    drawBackground(painter, option, index);

    if(m_type == CurUpDowns_Type && index.column() == 3)
        viewOption.rect.setWidth((option.rect.width()+painter->fontMetrics().width(Draw_Item_Value_1))/2);

    if(m_type == CurUpDowns_Type  && index.column() == 2)
    {
        //int start = (option.rect.width()-painter->fontMetrics().width(Draw_Item_Value_2))/2;
        viewOption.rect.adjust(6,0,0,0);
    }

    QColor clr = index.data(Qt::ForegroundRole).value<QColor>();
    viewOption.palette.setColor(QPalette::HighlightedText, clr);
    painter->setPen(oldPen);
    QItemDelegate::paint(painter, viewOption, index);
}

QRect WeeklyHorizonHeader::getValidRect(const QRect &rect, Qt::Alignment alignment) const
{
    return (alignment & Qt::AlignRight)?
                rect.adjusted(0,0,-m_rightMargin,0):
                (alignment & Qt::AlignLeft)?
                    rect.adjusted(m_leftMargin,0,0,0):
                    rect;
}

void WeeklyHorizonHeader::mouseReleaseEvent(QMouseEvent *e)
{
    int index = logicalIndexAt(e->x());
    if(index<0)
        return QHeaderView::mouseReleaseEvent(e);

    WeeklyListModel *mode = static_cast<WeeklyListModel*>(model());
    if(!mode->data(mode->index(0,index), Weekly_Table_Column_Sort).toBool())
        return;

    return QHeaderView::mouseReleaseEvent(e);
}

void WeeklyHorizonHeader::paintSection(QPainter *painter, const QRect &rect, int index) const
{
    QStyleOptionHeader opt;
    opt.orientation = Qt::Horizontal;
    opt.section = index;

    initStyleOption(&opt);
    opt.rect = rect;

    style()->drawControl(QStyle::CE_HeaderSection,&opt,painter,this);

    WeeklyListModel *mode = static_cast<WeeklyListModel*>(model());
    if(mode) {
        opt.text = mode->data(mode->index(0,index), Qt::DisplayRole).toString();
        opt.textAlignment = (Qt::Alignment)mode->data(mode->index(0,index), Qt::TextAlignmentRole).toInt();
        opt.rect =  getValidRect(rect,opt.textAlignment);
        if(this->sortIndicatorSection() == index && (opt.textAlignment&Qt::AlignRight) && this->isSortIndicatorShown())
            opt.rect =  opt.rect.adjusted(0,0,-10,0);
        QApplication::style()->drawControl(QStyle::CE_HeaderLabel,&opt,painter,this);
        if(this->sortIndicatorSection() == index
                && mode->data(mode->index(0,index),Weekly_Table_Column_Sort).toBool()
                && isSortIndicatorShown()){
            int textWidth = opt.fontMetrics.width(opt.text);
            if(opt.textAlignment & Qt::AlignRight){
                opt.rect = QRect(opt.rect.right()+1,0,10,opt.rect.height());
            }else if(opt.textAlignment & Qt::AlignLeft){
                opt.rect  = QRect(opt.rect.left()+textWidth+1,0,10,opt.rect.height());
            }else{
                opt.rect  = QRect(opt.rect.center().x()+textWidth/2+1,0,10,opt.rect.height());
            }

            opt.sortIndicator = QStyleOptionHeader::SortIndicator(sortIndicatorOrder());
            QApplication::style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow,&opt,painter,this);
        }
    } else {
        QHeaderView::paintSection(painter, rect, index);
    }
}
