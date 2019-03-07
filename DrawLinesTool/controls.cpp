#include "controls.h"

#include <QDebug>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QPainter>
#include <QMenu>
#include <QButtonGroup>

Controls::Controls()
{

}


//QComboBox---
ColorComboBox::ColorComboBox(QWidget *parent )
    :QComboBox(parent)
    ,m_color(Qt::white)
{
    m_label = new QLabel(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(3,3,3,3);
    hLayout->addWidget(m_label);
    hLayout->addSpacing(14);
    this->setLayout(hLayout);
    this->setColor(Qt::red);
}

void ColorComboBox::setColor(const QColor& color)
{
    QPixmap pix(m_label->size());
    pix.fill(color);
    m_label->setAutoFillBackground(true);
    m_label->setPixmap(pix);
}

void ColorComboBox::showPopup()
{
    QColor color = QColorDialog::getColor(m_color,this,QStringLiteral("选择颜色"));
    if(! color.isValid()) return;
    if(color != m_color){
        m_color = color;
        this->setColor(m_color);
        emit colorChanged(m_color);
    }
}


///PenStyleToolButton---
PenStyleToolButton::PenStyleToolButton(PenStyleType type,QWidget *parent)
    :QToolButton(parent),m_penStyleType(type)
{
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setPopupMode(QToolButton::MenuButtonPopup);

    this->setIconSize(QSize(56,18));
    initToolButton();
}

void PenStyleToolButton::setPenWidth(int width)
{
    int index =  m_lineWidths.indexOf(width);
    if(index>=0){
        m_penWidth = width;
        this->setIcon(m_btnGroup->button(index)->icon());
    }
}

void PenStyleToolButton::setPenStyle(Qt::PenStyle penStyle)
{
    int index =  m_penStyles.indexOf(penStyle);
    if(index>=0){
        m_penStyle =  penStyle;
        this->setIcon(m_btnGroup->button(index)->icon());
    }
}

void PenStyleToolButton::buttonClicked(int id)
{
    m_menu->hide();
    if(m_penStyleType ==  PenWidth){
        m_penWidth = m_lineWidths[id];
        emit penWidthChanged(m_lineWidths[id]);
    }
    else{
        m_penStyle = m_penStyles[id];
        emit penStyleChanged(m_penStyles[id]);
    }
    this->setIcon(m_btnGroup->button(id)->icon());
}

void PenStyleToolButton::initToolButton()
{
    m_btnGroup = new QButtonGroup(this);
    if(m_penStyleType ==  PenWidth)
        addPenWidthItems();
    else
        addPenStyleItems();

    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));
}

void PenStyleToolButton::addPenWidthItems()
{
    m_lineWidths<<1<<2<<4;
    QSize size(36,10);
    QPen pen;
    pen.setColor(Qt::black);
    m_menu = new QMenu(this);
    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setSpacing(0);
    for(int i=0;i<m_lineWidths.size();i++){
        QPixmap pix(size);
        pix.fill(Qt::white);
        QPainter pt(&pix);
        pen.setWidth(m_lineWidths[i]);
        pt.setPen(pen);
        pt.drawLine(0,(size.height()-m_lineWidths[i])/2,size.width(),(size.height()-m_lineWidths[i])/2);

        QToolButton *toolBtn = new QToolButton;
        toolBtn->setIcon(QIcon(pix));
        toolBtn->setIconSize(QSize(36,12));
        menuLayout->addWidget(toolBtn);
        m_btnGroup->addButton(toolBtn,i);
    }
    m_penWidth = m_lineWidths[0];
    m_menu->setLayout(menuLayout);
    this->setIcon(m_btnGroup->button(0)->icon());

    this->setMenu(m_menu);
}

void PenStyleToolButton::addPenStyleItems()
{
    m_penStyles<<Qt::SolidLine<<Qt::DashDotLine<<Qt::DashLine;
    QSize size(36,10);
    QPen pen;
    pen.setColor(Qt::black);
    m_menu = new QMenu(this);
    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setSpacing(0);
    for(int i=0;i<m_penStyles.size();i++){
        QPixmap pix(size);
        pix.fill(Qt::white);
        QPainter pt(&pix);
        pen.setStyle(m_penStyles[i]);
        pt.setPen(pen);
        pt.drawLine(0,(size.height()-1)/2,size.width(),(size.height()-1)/2);

        QToolButton *toolBtn = new QToolButton;
        toolBtn->setIcon(QIcon(pix));
        toolBtn->setIconSize(QSize(36,12));
        menuLayout->addWidget(toolBtn);

        m_btnGroup->addButton(toolBtn,i);
    }
    m_penStyle = m_penStyles[0];
    m_menu->setLayout(menuLayout);
    this->setIcon(m_btnGroup->button(0)->icon());

    this->setMenu(m_menu);
}




