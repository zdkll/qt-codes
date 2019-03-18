#include "penstylebuttons.h"

#include <QBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QButtonGroup>
#include <QMenu>
#include <QPen>
#include <QPainter>

#define  Icon_Width     60
#define  Icon_Height    20

//ColorToolButton---
//ColorToolButton---
ColorToolButton::ColorToolButton(QWidget *parent )
    :QToolButton(parent)
    ,m_color(Qt::white)
{
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setPopupMode(QToolButton::MenuButtonPopup);

    m_label = new QLabel(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(3,3,3,3);
    hLayout->addWidget(m_label);
    hLayout->addSpacing(14);//右侧subcontrol 留出空间
    this->setLayout(hLayout);

    this->setColor(m_color);

    connect(this,&QToolButton::clicked,this,&ColorToolButton::colorClicked);
}

void ColorToolButton::setColor(const QColor& color)
{
    m_color = color;
    updateIcon();
}

void ColorToolButton::colorClicked(bool )
{
    QColor color = QColorDialog::getColor(m_color,this,QStringLiteral("选择颜色"));
    if(! color.isValid()) return;
    if(color != m_color){
        m_color = color;
        this->setColor(m_color);
        emit colorChanged(m_color);
    }
}

void ColorToolButton::updateIcon()
{
    QPixmap pix(m_label->size());
    pix.fill(m_color);
    m_label->setAutoFillBackground(true);
    m_label->setPixmap(pix);
}

void ColorToolButton::resizeEvent(QResizeEvent *e)
{
    QToolButton::resizeEvent(e);
    updateIcon();
}



///PenStyleToolButton---
PenToolButton::PenToolButton(QWidget *parent)
    :QToolButton(parent)
{
    this->setIconSize(QSize(Icon_Width,Icon_Height));
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setPopupMode(QToolButton::MenuButtonPopup);

    m_menu = new QMenu(this);
    this->setMenu(m_menu);
    m_btnGroup = new QButtonGroup(this);
    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));
}

PenToolButton::PenToolButton(const QSize &iconSize, QWidget *parent)
    :QToolButton(parent)
{
    this->setIconSize(iconSize);
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setPopupMode(QToolButton::MenuButtonPopup);

    m_menu = new QMenu(this);
    this->setMenu(m_menu);
    m_btnGroup = new QButtonGroup(this);
    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));
}



void PenToolButton::buttonClicked(int id)
{
    m_menu->hide();
    clickedItem(id);
    this->setIcon(m_btnGroup->button(id)->icon());
}




///PenWidthToolButton----
PenWidthToolButton::PenWidthToolButton(QWidget *parent)
    :PenToolButton(parent)
{
    addPenWidthItems();
}

void PenWidthToolButton::setPenWidth(int width)
{
    int index =  m_lineWidths.indexOf(width);
    if(index>=0){
        m_penWidth = width;
        this->setIcon(m_btnGroup->button(index)->icon());
    }
}

void PenWidthToolButton::clickedItem(int id)
{
    m_penWidth = m_lineWidths[id];
    emit penWidthChanged(m_lineWidths[id]);
}

void PenWidthToolButton::addPenWidthItems()
{
    m_lineWidths<<1<<2<<3<<4;
    QPen pen;
    pen.setColor(Qt::black);

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setSpacing(0);
    menuLayout->setContentsMargins(0,0,0,0);
    for(int i=0;i<m_lineWidths.size();i++){
        QPixmap pix(this->iconSize());
        pix.fill(Qt::white);
        QPainter pt(&pix);
        pen.setWidth(m_lineWidths[i]);
        pt.setPen(pen);
        pt.drawLine(0,(iconSize().height()-m_lineWidths[i])/2,iconSize().width(),(iconSize().height()-m_lineWidths[i])/2);

        QToolButton *toolBtn = new QToolButton;
        toolBtn->setIcon(QIcon(pix));
        toolBtn->setIconSize(this->iconSize());
        menuLayout->addWidget(toolBtn);
        m_btnGroup->addButton(toolBtn,i);
    }

    m_penWidth = m_lineWidths[0];
    m_menu->setLayout(menuLayout);
    this->setIcon(m_btnGroup->button(0)->icon());
}


///PenStyleToolButton
PenStyleToolButton::PenStyleToolButton(QWidget *parent )
    :PenToolButton(parent)
{
    addPenStyleItems();
}

void PenStyleToolButton::setPenStyle(Qt::PenStyle penStyle)
{
    int index =  m_penStyles.indexOf(penStyle);
    if(index>=0){
        m_penStyle =  penStyle;
        this->setIcon(m_btnGroup->button(index)->icon());
    }
}

void PenStyleToolButton::clickedItem(int id)
{
    m_penStyle = m_penStyles[id];
    emit penStyleChanged(m_penStyles[id]);
}

void PenStyleToolButton::addPenStyleItems()
{
    m_penStyles<<Qt::SolidLine<<Qt::DashLine<<Qt::DotLine<<Qt::DashDotLine
              <<Qt::DashDotDotLine;
    QPen pen;
    pen.setColor(Qt::black);

    QVBoxLayout *menuLayout = new QVBoxLayout;
    menuLayout->setContentsMargins(0,0,0,0);
    menuLayout->setSpacing(0);
    for(int i=0;i<m_penStyles.size();i++){
        QPixmap pix(iconSize());
        pix.fill(Qt::white);
        QPainter pt(&pix);
        pen.setStyle(m_penStyles[i]);
        pt.setPen(pen);
        pt.drawLine(0,(iconSize().height()-1)/2,iconSize().width(),(iconSize().height()-1)/2);

        QToolButton *toolBtn = new QToolButton;
        toolBtn->setIcon(QIcon(pix));
        toolBtn->setIconSize(iconSize());
        menuLayout->addWidget(toolBtn);

        m_btnGroup->addButton(toolBtn,i);
    }
    m_penStyle = m_penStyles[0];
    m_menu->setLayout(menuLayout);
    this->setIcon(m_btnGroup->button(0)->icon());
}

