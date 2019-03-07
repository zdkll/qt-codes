#include "zhubihandicapview.h"

#include <QBoxLayout>

#include "timedealwidget.h"
#include "pricetickview.h"

ZhubiHandicapView::ZhubiHandicapView(ZbHandType type ,QWidget *parent ,QString instName)
    :BaseWidget(parent,instName),m_zbhdType(type)
{
    createWg();
}

void ZhubiHandicapView::afterActive()
{
    m_timeDealWg->afterActive();
    m_priceTickView->afterActive();
}

void ZhubiHandicapView::afterDeactive()
{
    m_timeDealWg->afterDeactive();
    m_priceTickView->afterDeactive();
}

void ZhubiHandicapView::setStockCode(const QString &stockCode)
{
    m_timeDealWg->setStockCode(stockCode.toStdString());
    m_priceTickView->setStockCode(stockCode);
}

void ZhubiHandicapView::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);
    m_timeDealWg =  new  SLTimeDealWidget();

    m_priceTickView = new PriceTickView();

    if(m_zbhdType == ZB_Index){
        m_priceTickView->setColStretchFactor(0,0.35);
        m_priceTickView->setColStretchFactor(1,0.65);
    }

    mainLayout->addWidget(m_timeDealWg);
    mainLayout->addWidget(m_priceTickView);

    this->setLayout(mainLayout);
}
