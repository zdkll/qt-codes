#include "titlebar.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>

#include "windowcontrolbox.h"
#include "hzline.h"

TitleBar::TitleBar(QWidget *parent,QString instName)
    : BaseWidget(parent,instName),m_controlBox(0),m_isResizable(true)
{

}


TitleBar::~TitleBar()
{

}

void TitleBar::createTitleBar()
{
    m_pIconLabel = new QLabel(this);
    m_pIconLabel->setObjectName("iconlabel");

    m_pTitleLabel = new QLabel(QStringLiteral("untitled"),this);
    m_pTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pTitleLabel->setObjectName("textLabel");

    m_controlBox   =  new WindowControlBox(this,(QWidget*)this->parent(),true,true,"windowcontrolbox");
    QHBoxLayout *titleLayout = new  QHBoxLayout;

    titleLayout->addWidget(m_pIconLabel);
    titleLayout->addWidget(m_pTitleLabel,1);
    titleLayout->addStretch();
    titleLayout->addWidget(m_controlBox,0,Qt::AlignRight);

    titleLayout->setContentsMargins(4,0,4,0);
    this->setLayout(titleLayout);
}

void TitleBar::setControlFlag(const WindowControlBox::ControlFlag &controlFlag,bool enable)
{
    m_controlBox->setControlFlag(controlFlag,enable);
}

void TitleBar::setControlFlags(const WindowControlBox::ControlFlags &controlFlags)
{
    m_controlBox->setControlFlags(controlFlags);
}
void TitleBar::setWinTitle(const QString &str)
{
    m_pTitleLabel->setText(str);
}

//void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event);
//    if(!m_isResizable)
//        return ;
//    m_controlBox->SwitchExpandOrNarrow();
//}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            // m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::onMinimized()
{
    mainWndMinimized();
}

void TitleBar::onMaximinzed()
{
    mainWndMaximized();
}

void TitleBar::onClose()
{
    mainWndClose();
}

