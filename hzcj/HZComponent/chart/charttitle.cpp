#include "charttitle.h"
#include <qfontmetrics.h>
#include <qfont.h>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QScrollBar>
#include "imagelayer.h"
#include "basecanvas.h"
#include "hzcfg.h"

ChartTitle::ChartTitle(ImageLayer* layer)
    :BaseChart(layer)
{
    m_leftPadding = 11;
    m_pIndiSeltComBox = nullptr;
    m_rightPos =m_rc.right();
}

ChartTitle::~ChartTitle()
{
    for(int i =0;i < m_childWndInfo.size();i++)
    {
        m_childWndInfo[i].pWnd->deleteLater();
    }
    if(m_pIndiSeltComBox)
        m_pIndiSeltComBox->deleteLater();
    m_pIndiSeltComBox =nullptr;
    m_childWndInfo.clear();
}

void ChartTitle::updateTextItem(const std::vector<TitleTextItem> &list)
{
    BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    painter.fillRect(m_rc,pWnd->getContainerTitleBkClr());
    QPen pen;
    pen.setStyle(Qt::PenStyle::DashLine);
    pen.setWidth(1);

    QRect rc = m_rc;
    rc.setLeft(rc.left()+m_leftPadding);
    for(unsigned int i = 0;i < list.size();i++)
    {
        QFont font;
        font.setFamily(list[i].family);
        font.setPixelSize(list[i].fontSize);
        font.setBold(list[i].bBold);
        font.setStyleStrategy(QFont::NoAntialias);
        painter.setFont(font);
        int width = list[i].fixedWidth;
        if(width == INT_MAX) //没设置宽度者计算宽度
        {
            QFontMetrics fm(font);
            width = fm.boundingRect(list[i].str).width();
            if(width < 50)
                width = 50;
        }
        rc.setRight(rc.left() + width);
        if((rc.right() + list[i].nInterval) > m_rightPos)
            break;
        pen.setColor(list[i].clr);
        painter.setPen(pen);
        painter.drawText(rc,Qt::AlignLeft|Qt::AlignVCenter,list[i].str);
        rc.setLeft(rc.right() + list[i].nInterval);
    }
    painter.end();
}

void ChartTitle::mainChartXaixsChnage(const QString &msg)
{
    if(isShowEx())
    {
        QString key;
        MessageParams params;
        if(HZObject::parseMsg(msg,key,params))
        {
            if(ChartUiClear == key)
            {
                m_imageLayer->clear(m_type);
            }
        }
    }
}

QRect ChartTitle::getChartMainArea()
{
    return m_rc;
}

void ChartTitle::switchDrType()
{
//    pComBox->addItem(QStringLiteral("不复权"),QVariant((int)RestorationNone));
//    pComBox->addItem(QStringLiteral("前复权"),QVariant((int)RestorationBeforType));
//    pComBox->addItem(QStringLiteral("后复权"),QVariant((int)RestorationBackType));
    int drType = m_pReCombox->currentData().toInt();
    if(drType  == RestorationNone)
    {
        drType = RestorationBeforType;
        m_pReCombox->setCurrentIndex(1);
    }else if(drType == RestorationBeforType)
    {
        drType = RestorationBackType;
        m_pReCombox->setCurrentIndex(2);
    }else if(drType == RestorationBackType)
    {
        drType = RestorationBeforType;
        m_pReCombox->setCurrentIndex(1);
    }
    emit signalRestorationComBoxChanged(drType);
}

void ChartTitle::slotButtonClose()
{
    emit signalButtonClose();
}

void ChartTitle::slotButtonParamSet()
{
    emit signalButtonParamSet();
}

void ChartTitle::slotButtonPlus()
{
    emit signalButtonPlus();
}

void ChartTitle::slotButtonSub()
{
    emit signalButtonSub();
}

void ChartTitle::slotRestorationComBoxChanged(int index)
{
    emit signalRestorationComBoxChanged(m_pReCombox->currentData().toInt());
}

void ChartTitle::slotIndiSeltComBoxChanged(int index)
{
    emit signalIndiSeltComBoxChanged(index);
}


void ChartTitle::onCreate()
{
    createButton();
}

void ChartTitle::onResize()
{
    resizeButton();
}

void ChartTitle::createButton()
{
    ChildItemInfo item;
    QPushButton* pButton = nullptr;
    //创建指标选择按钮
    if(m_attr & ChartTitleWithIndiSelBt)
    {
        m_pIndiSeltComBox = new QComboBox(m_imageLayer->getWnd());
        m_pIndiSeltComBox->setObjectName("restoration");

        IndiIndexMap mapTab = Hzcfg::getInstance()->getIndiIndexMap();
        for(auto it = mapTab.begin();it != mapTab.end();it++)
        {
            if(it->second.drawArea != IndiDrawArea::indi_main)
            {
                m_pIndiSeltComBox->addItem(it->second.name.c_str(),it->first);
            }
        }
        m_pIndiSeltComBox->setEditable(true);
        m_pIndiSeltComBox->lineEdit()->setReadOnly(true);
        m_pIndiSeltComBox->lineEdit()->setAlignment(Qt::AlignCenter);
        m_pIndiSeltComBox->view()->horizontalScrollBar()->hide();
        m_pIndiSeltComBox->view()->verticalScrollBar()->hide();
        connect(m_pIndiSeltComBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotIndiSeltComBoxChanged(int)));
    }

    //创建前后复权
    if(m_attr & ChartTitleWithRestorationComBox)
    {
        item.widht = 50;
        item.hight = 18;
        item.leftPadding = 15;
        QComboBox* pComBox = new QComboBox(m_imageLayer->getWnd());
        pComBox->setObjectName("restoration");
        pComBox->addItem(QStringLiteral("不复权"),QVariant((int)RestorationNone));
        pComBox->addItem(QStringLiteral("前复权"),QVariant((int)RestorationBeforType));
        pComBox->addItem(QStringLiteral("后复权"),QVariant((int)RestorationBackType));
        pComBox->setEditable(true);
        pComBox->lineEdit()->setReadOnly(true);
        pComBox->lineEdit()->setAlignment(Qt::AlignCenter);
        connect(pComBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotRestorationComBoxChanged(int)));
        m_pReCombox = pComBox;
        item.pWnd = pComBox;
        m_childWndInfo.push_back(item);
    }

    //创建+按钮
    if(m_attr & ChartTitleWithPlusBt)
    {
        item = ChildItemInfo();
        pButton = new QPushButton(m_imageLayer->getWnd());
        pButton->setObjectName("plusButton");
        connect(pButton,&QPushButton::clicked,this,&ChartTitle::slotButtonPlus);
        item.pWnd = pButton;
        m_childWndInfo.push_back(item);
    }
    //创建-按钮
    if(m_attr & ChartTitleWithSubBt)
    {
        item = ChildItemInfo();
        pButton = new QPushButton(m_imageLayer->getWnd());
        pButton->setObjectName("subButton");
        connect(pButton,&QPushButton::clicked,this,&ChartTitle::slotButtonSub);
        item.pWnd = pButton;
        m_childWndInfo.push_back(item);
    }
    //创建设置按钮
    if(m_attr & ChartTitleWithSetting)
    {
        item = ChildItemInfo();
        pButton = new QPushButton(m_imageLayer->getWnd());
        pButton->setObjectName("setButton");
        connect(pButton,&QPushButton::clicked,this,&ChartTitle::slotButtonParamSet);
        item.pWnd = pButton;
        m_childWndInfo.push_back(item);
    }

    //创建关闭按钮
    if(m_attr & ChartTitleWithClose)
    {
        item = ChildItemInfo();
        pButton = new QPushButton(m_imageLayer->getWnd());
        pButton->setObjectName("hideButton");
        connect(pButton,&QPushButton::clicked,this,&ChartTitle::slotButtonClose);
        item.pWnd = pButton;
        m_childWndInfo.push_back(item);
    }
}

void ChartTitle::resizeButton()
{
    m_rightPos=m_rc.right();
    ChildItemInfo item;
    if(m_pIndiSeltComBox)
    {
       QRect rc =m_rc;
       rc.setLeft(rc.left()+10);
       rc.setRight(rc.left()+50);
       rc.setHeight(18);
       m_pIndiSeltComBox->setGeometry(rc);
       m_leftPadding = rc.right()+15;
    }
    QRect rc =m_rc;
    int midPos = rc.top() + rc.height()/2;
    int hpos = rc.right()-5;//预留5像素
    for(int i=m_childWndInfo.size()-1;i >= 0;i--)
    {
        item = m_childWndInfo[i];
        hpos -= item.leftPadding;
        rc.setRight(hpos);
        rc.setLeft(rc.right()-item.widht);
        hpos = rc.left();
        rc.setTop(midPos-item.hight/2);
        rc.setBottom(midPos + item.hight/2);
        item.pWnd->setGeometry(rc);
        item.pWnd->show();
        m_rightPos = rc.left();
    }
}
