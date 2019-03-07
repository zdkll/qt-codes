#include "basepage.h"

#include <QTabWidget>
#include <QBoxLayout>
#include <QEvent>

BasePage::BasePage(QWidget *parent,QString instName)
    :BaseWidget(parent,instName),parent_(parent)
{

}

void BasePage::recvParam(const DetailPageParam &param,bool bCurrPage)
{

}

void BasePage::afterActive()
{

}

void BasePage::afterDeactive()
{

}



