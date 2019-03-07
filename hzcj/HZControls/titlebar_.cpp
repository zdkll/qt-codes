#include "titlebar.h"
#include "ui_TitleBar.h"
#include <QHBoxLayout>
#include "windowcontrolbox.h"
#include <QStyleOption>
#include <QPainter>

TitleBar::TitleBar(QWidget *parent,QString instName,BaseWidget* midWidget) :
   BaseWidget(parent,instName),
   midWidget_(midWidget),
   ui(new Ui::titlebar),
   m_bPressed(false),
   controlBox_(new WindowControlBox(this,parent,true,true))
{
    ui->setupUi(this);
    ui->iconLabel->setObjectName("iconlabel");
    ui->textLabel->setObjectName("textLabel");
    QHBoxLayout* leftLayout = new QHBoxLayout;
    leftLayout->addWidget(ui->iconLabel);
    leftLayout->addWidget(ui->textLabel);
    leftLayout->setMargin(0);
    leftLayout->setSpacing(10);
    leftLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout* cLayout = new QHBoxLayout;
    if(midWidget_)
    {
         cLayout->addWidget(midWidget_);
         cLayout->setAlignment(midWidget_,Qt::AlignJustify);
    }
    cLayout->addStretch();

    QHBoxLayout* rightLayout = new QHBoxLayout;
    rightLayout->setMargin(0);
    rightLayout->addWidget(controlBox_);
    controlBox_->setFixedWidth(90);
    rightLayout->setAlignment(controlBox_,Qt::AlignRight | Qt::AlignVCenter);


    QHBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(10, 0, 10, 0);
    titleLayout->addLayout(leftLayout);
    titleLayout->addLayout(cLayout);
    titleLayout->addLayout(rightLayout);
    titleLayout->setSpacing(0);
    titleLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setLayout(titleLayout);

    connect(controlBox_, &WindowControlBox::mainWndMinimized, this, &TitleBar::OnMinimized);
    connect(controlBox_, &WindowControlBox::mainWndMaximized, this, &TitleBar::OnMaximinzed);
    connect(controlBox_, &WindowControlBox::mainWndClose, this, &TitleBar::OnClose);
    //connect(m_pPushBtn,SIGNAL(clicked(bool)),parentWidget(),SLOT(CloseParent()));
    //connect(m_pPushBtn,SIGNAL(clicked(bool)),parentWidget(),SLOT(this-parentWidget()->close()));
}

void TitleBar::SetTitleWnd(const QString &str)
{
    ui->textLabel->setText(str);
}

void TitleBar::OnMinimized()
{
    mainWndMinimized();
}

void TitleBar::OnMaximinzed()
{
    mainWndMaximized();
}

void TitleBar::OnClose()
{
    mainWndClose();
}


void TitleBar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    controlBox_->SwitchExpandOrNarrow();
}
