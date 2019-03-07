#include "hzpixlayer.h"
#include "ui_hzpixlayer.h"
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

///
/// \brief HZPixTopLayer::HZPixTopLayer
/// \param parent
///
HZPixTopLayer::HZPixTopLayer(const QString &cr, QWidget *parent)
    :QLabel(parent)
    ,mPix(QPixmap(cr))
{

}

void HZPixTopLayer::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    int rw = width()/2;
    int rh = height()/2;
    int topwidth = mPix.size().width()/2;
    int topheight = mPix.size().height()/2;

    QPainter p(this);
    if(mPix.isNull())return;

    QRect pixRt;
    if(rw > topwidth && rh < topheight) {
        pixRt.setRect(rw-topwidth, 0, topwidth*2, rh*2);
    } else if(rw < topwidth && rh > topheight) {
        pixRt.setRect(0, rh-topheight, rw*2, topheight*2);
    } else if(rw > topwidth && rh > topheight) {
        pixRt.setRect(rw-topwidth, rh-topheight, topwidth*2, topheight*2);
    } else if(rw < topwidth && rh < topheight) {
        pixRt.setRect(0, 0, rw*2, rh*2);
    }
    p.drawPixmap(pixRt, mPix);
}



////
/// \brief HZPixLayer::HZPixLayer
/// \param parent
///
HZPixLayer::HZPixLayer(const QString &bk, const QString &cr, QWidget *parent)
    :BaseWidget(parent),
      ui(new Ui::HZPixLayer)
    ,mPix(QPixmap(bk))
    ,mLabel(new HZPixTopLayer(cr,this))
{
    ui->setupUi(this);
//    setStyleSheet(QString("background:%1;").arg(bk));
    ui->horizontalLayout->addWidget(mLabel);
    ui->horizontalLayout->setContentsMargins(0,0,0,0);
}

HZPixLayer::~HZPixLayer()
{
    delete ui;
}

void HZPixLayer::afterActive()
{
    this->update();
}

void HZPixLayer::paintEvent(QPaintEvent *event)
{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    __super::paintEvent(event);
    int rw = this->rect().width()/2;
    int rh = this->rect().height()/2;

    QPainter p(this);
    if(mPix.isNull())return;
    p.drawPixmap(rect(),mPix);
}

void HZPixLayer::loadskin()
{
}
