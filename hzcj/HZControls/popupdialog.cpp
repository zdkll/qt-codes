#include "popupdialog.h"
#include "titlebar.h"
#include <QLabel>
#include "windowcontrolbox.h"




PopupDialog::PopupDialog(QWidget *parent, QString instName)
    :QDialog(parent),HZObject(instName),
      m_pLayout(new QVBoxLayout())
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setMouseTracking(true); //界面拉伸需要这个属性

    createWg();
}

PopupDialog::~PopupDialog()
{
    m_pHelper->removeFrom(this);
}

void PopupDialog::createWg()
{
    QWidget *widget = new QWidget(this);

    widget->setObjectName("wintitle");
    widget->setMouseTracking(true);
    m_pIconLabel = new QLabel;
    m_pIconLabel->setObjectName("iconlabel");

    m_pTitleLabel = new QLabel(QStringLiteral("untitled"));
    m_pTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pTitleLabel->setObjectName("textLabel");

    controlBox_   =  new WindowControlBox((BaseWidget*)this,(QWidget*)this,true,true,"windowcontrolbox");
    controlBox_->setControlFlags(WindowControlBox::WindowCloseButton);

    connect(controlBox_, &WindowControlBox::mainWndClose, [=](){emit popWndClose();});

    QHBoxLayout *titleLayout = new  QHBoxLayout;

    titleLayout->addWidget(m_pIconLabel);
    titleLayout->addWidget(m_pTitleLabel,1);
    titleLayout->addStretch();
    titleLayout->addWidget(controlBox_,0,Qt::AlignRight);

    titleLayout->setMargin(0);
//    titleLayout->setContentsMargins(4,0,4,0);

    widget->setLayout(titleLayout);

    m_pLayout->setSpacing(0);
    m_pLayout->setMargin(0);
//    m_pLayout->setContentsMargins(1, 1, 1, 1);

    m_pHelper = new FramelessHelper(this);
    m_pLayout->addWidget(widget);
    this->setLayout(m_pLayout);

    m_pHelper->activateOn(this);  //激活当前窗体
   // m_pHelper->setTitleHeight(m_pTitleBar->height());  //设置窗体的标题栏高度
    m_pHelper->setWidgetMovable(true);  //设置窗体可移动
    m_pHelper->setWidgetResizable(false);  //设置窗体可缩放
}

void PopupDialog::mouseMoveEvent(QMouseEvent *event)
{

}



void PopupDialog::setWidget(QWidget *wg)
{
    m_pLayout->addWidget(wg);

    m_pLayout->addStretch();
}


