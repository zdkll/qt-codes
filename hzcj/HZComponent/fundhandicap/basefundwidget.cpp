#include "basefundwidget.h"

BaseFundWidget::BaseFundWidget(QWidget *parent ,QString instName)
    :BaseWidget(parent,instName)
{

}

void BaseFundWidget::setStock(const QString &stock)
{
    stockChanged(stock);
}



