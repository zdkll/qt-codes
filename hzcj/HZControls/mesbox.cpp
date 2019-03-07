#include "mesbox.h"
#include "titlebar.h"
#include "windowcontrolbox.h"
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>


MesBox::MesBox(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
    :PopupDialog(parent)
{

    m_pTitleLabel->setText(title);
    m_MsgView = new MsgView(parent, text, buttons, defaultButton);

    setWidget(m_MsgView);

    connect(m_MsgView, &MsgView::btnClicked, this, &MesBox::onButtonClicked);

    m_MsgView->setFocus();
}

MesBox::~MesBox()
{

}


void MesBox::setStyle(int nStyle)
{
    m_MsgView->setStyle(nStyle);
}



void MesBox::onButtonClicked(int nRet)
{
    this->done(nRet);
}

MsgView::MsgView(QWidget *parent, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
    :QDialog(parent)
{

    QPalette pattle = this->palette();
    this->setAutoFillBackground(true);
    pattle.setColor(QPalette::Background,Qt::white);
    this->setPalette(pattle);

    resize(600, 320);
    m_pButtonBox = new QDialogButtonBox(this);;
    m_pButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)));



    setDefaultButton(defaultButton);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);

    QGridLayout *gridlayout = new QGridLayout(this);

    QHBoxLayout *hboxlayout = new QHBoxLayout(this);


    QLabel *m_lText = new QLabel(this);


    msgBtn = new QPushButton(this);

    msgBtn->setFixedSize(100, 100);

    m_lText->setText(text);
    m_lText->setWordWrap(true);

    gridlayout->addWidget(msgBtn, 0, 0);
    gridlayout->addWidget(m_lText, 0, 1);

    hboxlayout->addStretch(1);
    hboxlayout->addWidget(m_pButtonBox);
    mainlayout->addLayout(gridlayout);
    mainlayout->addLayout(hboxlayout);

    setLayout(mainlayout);

    translateUI();
    connect(m_pButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
}

void MsgView::setDefaultButton(QPushButton *button)
{

    if (!m_pButtonBox->buttons().contains(button))
        return;
    button->setDefault(true);
    button->setShortcut(Qt::Key_Enter);//设置快捷键为enter键
    button->setFocus();

}

void MsgView::setDefaultButton(QMessageBox::StandardButton button)
{
    setDefaultButton(m_pButtonBox->button(QDialogButtonBox::StandardButton(button)));
}



QMessageBox::StandardButton MsgView::standardButton(QAbstractButton *button) const
{
    return (QMessageBox::StandardButton)m_pButtonBox->standardButton(button);
}

void MsgView::setStyle(int nStyle)
{
    if(1 == nStyle)
    {
        msgBtn->setObjectName("info");
    }
    else if(2 == nStyle)
    {
        msgBtn->setObjectName("question");
    }
    else if(3 == nStyle)
    {
        msgBtn->setObjectName("warning");
    }
    else if(4 == nStyle)
    {
        msgBtn->setObjectName("error");
    }
    else
    {
        msgBtn->hide();
    }
}

void MsgView::translateUI()
{
    QPushButton *pYesButton = m_pButtonBox->button(QDialogButtonBox::Yes);
    if (pYesButton != NULL)
        pYesButton->setText(QStringLiteral("是"));

    QPushButton *pNoButton = m_pButtonBox->button(QDialogButtonBox::No);
    if (pNoButton != NULL)
        pNoButton->setText(QStringLiteral("否"));
    QPushButton *pOkButton = m_pButtonBox->button(QDialogButtonBox::Ok);
    if (pOkButton != NULL)
        pOkButton->setText(QStringLiteral("确认"));
    QPushButton *pCancelButton = m_pButtonBox->button(QDialogButtonBox::Cancel);
    if (pCancelButton != NULL)
        pCancelButton->setText(QStringLiteral("取消"));
    QPushButton *pSaveButton = m_pButtonBox->button(QDialogButtonBox::Save);
    if (pSaveButton != NULL)
        pSaveButton->setText(QStringLiteral("保存"));
    QPushButton *pSaveAllButton = m_pButtonBox->button(QDialogButtonBox::SaveAll);
    if (pSaveAllButton != NULL)
        pSaveAllButton->setText(QStringLiteral("保存所有"));
    QPushButton *pAbortButton = m_pButtonBox->button(QDialogButtonBox::Abort);
    if (pAbortButton != NULL)
        pAbortButton->setText(QStringLiteral("终止"));
    QPushButton *pRetryButton = m_pButtonBox->button(QDialogButtonBox::Retry);
    if (pRetryButton != NULL)
        pRetryButton->setText(QStringLiteral("重试"));
    QPushButton *pIgnoreButton = m_pButtonBox->button(QDialogButtonBox::Ignore);
    if (pIgnoreButton != NULL)
        pIgnoreButton->setText(QStringLiteral("忽略"));
    QPushButton *pCloseButton = m_pButtonBox->button(QDialogButtonBox::Close);
    if (pCloseButton != NULL)
        pCloseButton->setText(QStringLiteral("关闭"));

    QPushButton *pDiscardButton = m_pButtonBox->button(QDialogButtonBox::Discard);
    if (pDiscardButton != NULL)
        pDiscardButton->setText(QStringLiteral("丢弃"));
    QPushButton *pHelpButton = m_pButtonBox->button(QDialogButtonBox::Help);
    if (pHelpButton != NULL)
        pHelpButton->setText(QStringLiteral("帮助"));

    QPushButton *pApplyButton = m_pButtonBox->button(QDialogButtonBox::Apply);
    if (pApplyButton != NULL)
        pApplyButton->setText(QStringLiteral("应用"));
    QPushButton *pResetButton = m_pButtonBox->button(QDialogButtonBox::Reset);
    if (pResetButton != NULL)
        pResetButton->setText(QStringLiteral("重试"));
}

int MsgView::execReturnCode(QAbstractButton *button)
{
    int nResult = m_pButtonBox->standardButton(button);
    return nResult;
}


void MsgView::onButtonClicked(QAbstractButton *button)
{
    emit btnClicked(standardButton(button));
}



int MessageBox::showInformation(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{

    MesBox msg(parent, title, text, buttons, defaultButton);
    msg.setStyle(1);
    return msg.exec();
}

int MessageBox::showError(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    MesBox msg(parent, title, text, buttons, defaultButton);
    msg.setStyle(4);
    return msg.exec();
}

int MessageBox::showQuestion(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    MesBox msg(parent, title, text, buttons, defaultButton);
    msg.setStyle(2);
    return msg.exec();
}

int MessageBox::showWarning(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    MesBox msg(parent, title, text, buttons, defaultButton);
    msg.setStyle(3);
    return msg.exec();
}

int MessageBox::showAbout(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    MesBox msg(parent, title, text, buttons, defaultButton);
    msg.setStyle(0);
    return msg.exec();
}
