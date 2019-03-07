#include "ShortTermChangeDelegate.h"
#include <QPainter>
#include <QApplication>
#include <QDateTime>

ShortTermChangeDelegate::ShortTermChangeDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void ShortTermChangeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ShortTermData data = index.data(MYDATAROLE).value<ShortTermData>();

    //去掉Focus
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);

    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPixelSize(13);
    QFontMetrics metric(font);

    QRect rc = viewOption.rect;
    rc.adjust(14, 0, 0, 0);


    painter->setFont(font);

    painter->setPen(textColor(data.short_type));

    painter->drawText(rc, Qt::AlignVCenter, data.strTime);

    rc.adjust(metric.width(data.strTime) + 28, 0, 0, 0);

    font.setFamily("SimSun");
    font.setBold(false);
    font.setPixelSize(14);
    painter->setFont(font);

    painter->drawText(rc, Qt::AlignVCenter, data.strName);

    rc.adjust(0, 0, -95, 0);

    painter->drawText(rc, Qt::AlignVCenter | Qt::AlignRight, data.strType);

    font.setBold(true);
    font.setFamily("Arial");
    font.setPixelSize(13);
    rc.adjust(0, 0, 78, 0);

    painter->setFont(font);
    painter->drawText(rc, Qt::AlignVCenter | Qt::AlignRight, data.strVal);

}

QColor ShortTermChangeDelegate::textColor(int type) const
{
    switch (type) {
    case OrganizationEat:
    case LargeBid:
    case RocketLaunch:
    case RapidRebound:
    case LargeBuy:
    case UpperLimit:
    case OpenDownLimit:
    case OrganizationBuy:
    case BuyNew:
    {
        return m_styledWidget.riseColor();
    }

        break;

    case HighDiving:
    case AccelerateDecline:
    case LargeSell:
    case LargeAsk:
    case DownLimit:
    case OpenUpperLimit:
    case OrganizationSell:
    case OrganizationSpit:
    case SellNew:
    {
        return m_styledWidget.fallColor();
    }
        break;
    default:
        break;
    }
    return m_styledWidget.flatColor();
}
