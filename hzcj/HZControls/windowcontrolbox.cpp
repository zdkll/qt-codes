#include "windowcontrolbox.h"
#include "ui_windowcontrolbox.h"
#include <QDesktopWidget>

WindowControlBox::WindowControlBox(QWidget* parent,QWidget* controlWindow,bool showMinButton,bool showMaxButton,QString instName)
    :BaseWidget(parent,instName),
      ui(new Ui::WindowControlBox),
      controlWindow_(controlWindow),
      showMinButton_(showMinButton),
      showMaxButton_(showMaxButton),
      isMaximized_(false),
      m_controlFlags(WindowDefaultFlags)
{
    ui->setupUi(this);
    ui->minBtn->setObjectName("minBtn");
    ui->expandBtn->setObjectName("expandBtn");
    ui->closeBtn->setObjectName("closeBtn");

    ui->minBtn->setFocusPolicy(Qt::NoFocus);
    ui->expandBtn->setFocusPolicy(Qt::NoFocus);
    ui->closeBtn->setFocusPolicy(Qt::NoFocus);

    ui->hBoxLayout->setMargin(0);
    ui->hBoxLayout->setSpacing(1);
    ui->hBoxLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    connect(ui->minBtn, &QPushButton::clicked, this, &WindowControlBox::OnMinimized);
    connect(ui->expandBtn, &QPushButton::clicked, this, &WindowControlBox::OnMaximinzed);
    connect(ui->closeBtn, &QPushButton::released, this, &WindowControlBox::OnClose);
    setControlFlag(WindowMaximizeButton, showMaxButton_);
    setControlFlag(WindowMinimizeButton, showMinButton_);
    updateControlFlags();
}

WindowControlBox::~WindowControlBox()
{
	if (ui)
	{
		delete ui;
		ui = nullptr;
	}
}

void WindowControlBox::SwitchExpandOrNarrow()
{
    //SwitchExpandOrNarrow(isMaximized_);
    OnMaximinzed();
}

void WindowControlBox::setControlFlag(const ControlFlag &controlFlag,bool enable)
{
    if(enable)
        m_controlFlags |= controlFlag;
    else
        m_controlFlags &= ~controlFlag;

    updateControlFlags();
}
void WindowControlBox::setControlFlags(const ControlFlags &controlFlags)
{
    m_controlFlags = controlFlags;
    updateControlFlags();
}

void WindowControlBox::SwitchExpandOrNarrow(bool isMaximized)
{
    if(isMaximized)
        ui->expandBtn->setObjectName("expandBtn");
    else
        ui->expandBtn->setObjectName("narrowBtn");

    ui->expandBtn->setStyle(ui->expandBtn->style());
}

void WindowControlBox::updateControlFlags()
{
    ui->minBtn->setVisible(m_controlFlags&WindowMinimizeButton);
    ui->expandBtn->setVisible(m_controlFlags&WindowMaximizeButton);
    ui->closeBtn->setVisible(m_controlFlags&WindowCloseButton);
}

void WindowControlBox::OnMinimized()
{
    if(controlWindow_)
    {
        controlWindow_->showMinimized();
    }
    mainWndMinimized();
}

void WindowControlBox::OnMaximinzed()
{
    if(controlWindow_)
    {
        SwitchExpandOrNarrow(isMaximized_);
        if(isMaximized_)
        {
            isMaximized_ = false;
            controlWindow_->showNormal();
        }
        else
        {
            isMaximized_ = true;
            controlWindow_->showMaximized();
            controlWindow_->setGeometry(QApplication::desktop()->availableGeometry());
            mainWndMaximized();
        }
    }
}

void WindowControlBox::onRestoreWindow()
{
    if(isMaximized_)
    {
        controlWindow_->showMaximized();
    }
    else
    {
       controlWindow_->showNormal();
    }
}

void WindowControlBox::updateMaximize(bool isMaximized)
{
    isMaximized_ = isMaximized;
    SwitchExpandOrNarrow(!isMaximized_);
}

void WindowControlBox::OnClose()
{
    if(controlWindow_){
        mainWndClose();
        controlWindow_->close();
    }

}
