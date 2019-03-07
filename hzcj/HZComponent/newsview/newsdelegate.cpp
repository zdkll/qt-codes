#include "newsdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QDateTime>
#include "newsmodel.h"
NewsDelegate::NewsDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void NewsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    STVListData  data = index.data(MYDATAROLE).value<STVListData>();

    //去掉Focus
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    QFontMetrics metrics = painter->fontMetrics();

    QFont font;
//    font.setStyleStrategy(QFont::NoAntialias);
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.titlePixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.titileColor());
    QRect rc = option.rect;
    rc.adjust(12, 13, 0, 0 );
    //画标题
    painter->drawText(rc, data.body);

    QString strText = data.source + data.times;

    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.textColor());
    //画来源和时间
    painter->drawText(rc,Qt::AlignRight,strText);

    int nWidth, nHeight;
    QPixmap pix;
    QColor tagColor;
    switch (data.type) {
    case NeutralTag:       //"中性"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.zxPixmap();
        tagColor = m_styledWidget.zxColor();
        break;
    case BullTag:    //"利好"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lhPixmap();
        tagColor = m_styledWidget.lhColor();
        break;
    case BadNewsTag:    //"利空"
        nWidth = 22;
        nHeight = 14;
        pix = m_styledWidget.lkPixmap();
        tagColor = m_styledWidget.lkColor();
        break;
    case StrongBullTag:     //"强烈利好"
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllhPixmap();
        tagColor = m_styledWidget.qllhColor();
        break;
    case StrongBadNewsTag:  //"强烈利空
        nWidth = 39;
        nHeight = 14;
        pix = m_styledWidget.qllkPixmap();
        tagColor = m_styledWidget.qllkColor();
        break;
    default:
        nWidth = 0;
        nHeight = 0;
        break;
    }

    QRect rect(option.rect);



    rect.adjust(12, 23 + metrics.height(), 0, 0);

    if(UnknownTag != data.type)
    {
        //画状态
        painter->drawPixmap(rect.left(), rect.top(), nWidth, nHeight,  pix);

        rect.adjust(3, 1, 0, 0);

        font.setFamily(m_styledWidget.labelFontFamily());
        font.setPixelSize(m_styledWidget.labelPixelSize());
        painter->setFont(font);
        painter->setPen(tagColor);
        painter->drawText(rect, data.tag);

        rect.adjust(nWidth - 3 + 6, -1, 0, 0);
    }

    //画摘要
    font.setFamily(m_styledWidget.titleFontFamily());
    font.setPixelSize(m_styledWidget.textPixelSize());
    painter->setFont(font);
    painter->setPen(m_styledWidget.textColor());

    QString strSummary = painter->fontMetrics().elidedText(data.desc, Qt::ElideRight, rect.width());

    painter->drawText(rect,strSummary);
}
