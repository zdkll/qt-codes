#include "quotepagebaseinnerwnd.h"

#include <QDebug>
QuotePageBaseInnerWnd::QuotePageBaseInnerWnd(QWidget *parent, QString instName):
    BaseWidget(parent,instName)
{
   //connect(this,SIGNAL(signalsClickObjItem(const QString&)),this,SLOT(slotClickItem(const QString&)));
}

// void QuotePageBaseInnerWnd::slotClickItem(const QString &stockCode)
// {
//  qDebug()<<"12===========QuotePageBaseInnerWnd clickItem:"<<stockCode;
// }

