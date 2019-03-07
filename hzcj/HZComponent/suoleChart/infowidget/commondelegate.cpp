#include "commondelegate.h"

CommonDelegate::CommonDelegate(QObject *parent)
{

}

CommonDelegate::~CommonDelegate()
{

}

void CommonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //去掉Focus
            QStyleOptionViewItem viewOption(option);
            initStyleOption(&viewOption, index);
            if (option.state.testFlag(QStyle::State_HasFocus))
            {
                viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
            }
            QStyledItemDelegate::paint(painter, viewOption, index);

}
