#include "vlistitemdelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include <QResizeEvent>
#include <QtMath>
#include <QTextLayout>
#include <QTextLine>
#include "vlistdef.h"

HZVListItemDelegate::HZVListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void HZVListItemDelegate::setWidth(int w)
{
    m_width = w;
}

void HZVListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    if(index.isValid()) {
        STVListData data = index.data(CUSTOM_ROLE_ITEM).value<STVListData>();
        QString msgId = index.data(CUSTOM_ROLE_ITEM_IID).toString();
        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item
        QRect rect = viewOption.rect;
        rect.setWidth(m_width);
        painter->save();
        if((viewOption.state.testFlag(QStyle::State_Selected)
            && viewOption.state.testFlag(QStyle::State_MouseOver)
            || msgId==data.id)) {
            painter->fillRect(option.rect, QBrush(QColor("#e9ecf5")));
        } else if(viewOption.state.testFlag(QStyle::State_MouseOver)){
            painter->fillRect(option.rect, QBrush(QColor("#e9ecf5")));
        } else {
            painter->fillRect(option.rect, QBrush(Qt::NoBrush));
        }
        painter->restore();
        painter->setPen(QColor("#dedede"));
        painter->drawLine(viewOption.rect.bottomLeft(), viewOption.rect.bottomRight());

        //绘制数据位置
        QRect rt = viewOption.rect.adjusted(VLISTITEM_LEFT_SPACE/2, VLISTITEM_TOP_SPACE, -VLISTITEM_RIGHT_SPACE, -VLISTITEM_CONTENT_HEIGHT);
        int leading = 0;
        qreal height = 0;
        QFont bodyFont(QStringLiteral("微软雅黑"));
        bodyFont.setPixelSize(14);
        painter->setFont(bodyFont);
        QString disTxt = painter->fontMetrics().elidedText(data.body, Qt::ElideRight, (rt.width()-VLISTITEM_LEFT_SPACE+3)*2);
        QTextLayout textLayout(disTxt, bodyFont);
        QTextOption opt(Qt::AlignLeft);
        opt.setWrapMode(QTextOption::WordWrap);
        textLayout.setCacheEnabled(true);
        textLayout.setTextOption(opt);
        textLayout.beginLayout();
        while (1) {
            if(height > rt.height()-VLISTITEM_CONTENT_HEIGHT)break;
            QTextLine line = textLayout.createLine();
            if (!line.isValid())  break;
            line.setLineWidth(rt.width()-VLISTITEM_LEFT_SPACE/2);
            height += leading;
            line.setPosition(QPointF(rt.x(), height));
            height += line.height();
        }
        textLayout.endLayout();
        painter->setPen(QPen(QColor("#1c1c1c")));
        textLayout.draw(painter, QPoint(rt.x(), rt.y()));
        painter->setPen(QPen(QColor("#868686")));
        bodyFont.setPixelSize(12);
        bodyFont.setBold(false);
        painter->setFont(bodyFont);
        painter->drawText(QPoint(viewOption.rect.x()+VLISTITEM_LEFT_SPACE, viewOption.rect.y()+70), data.times);
    }else {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

QSize HZVListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(m_width, VLIST_HEIGHT_SIZE);
}
