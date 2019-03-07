#include "plateinfodlg.h"
#include <QBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFormLayout>
#include <QScrollArea>
#include "scrollwidget.h"

PlateInfoDlg::PlateInfoDlg(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this)),
    m_contentLabel(new QTextEdit),
    m_pTitle(new QLabel)
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createWg();
//    m_contentLabel->setEnabled(false);
//    m_contentLabel->setDisabled(true);
    m_contentLabel->setFocusPolicy(Qt::NoFocus);
}

void PlateInfoDlg::updatedata(const TopicInvestDetail &data)
{
    m_deTailData = data;

//    QString strTag;

//    for(int i = 0; i < 30; i++)
//    {
//        strTag += data.detail.intro;
//    }

//    QString str = QString("<p style=\" margin-top:5px; margin-bottom:5px; margin-left:0px; margin-right:0px; line-height:26px; -qt-block-indent:0; text-indent:50px;\">%1</p>").arg(strTag);

    QString str = QString("<p style=\" margin-top:5px; margin-bottom:5px; margin-left:0px; margin-right:0px; line-height:26px; -qt-block-indent:0; text-indent:28px;\">%1</p>").arg(data.detail.intro);

    m_pTitle->setText(data.detail.topic_name);

    m_contentLabel->setHtml(str);

//    m_contentLabel->insertHtml(data.detail.intro);
}

void PlateInfoDlg::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(0);

    mainLayout->addWidget(createTitle());

    mainLayout->addWidget(createBody());
}

QWidget *PlateInfoDlg::createTitle()
{
    QWidget *widget = new QWidget;

    widget->setObjectName("title");

    WindowControlBox *controlBox = new WindowControlBox(this, this, false, false);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(600);
    layout->addWidget(new QLabel(QStringLiteral("板块介绍")), 0, Qt::AlignCenter);

    layout->addStretch();

    layout->addWidget(controlBox);

    widget->setLayout(layout);

    return widget;
}


QWidget *PlateInfoDlg::createBody()
{
    QWidget *widget = new QWidget;

    widget->setObjectName("body");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(32, 18, 57 ,0);

    layout->addWidget(m_pTitle);
    m_pTitle->setObjectName("title");
    layout->addSpacing(26);

    QFormLayout* form = new QFormLayout;
    form->setSpacing(0);
    form->setMargin(0);
    form->addRow(new QLabel(QStringLiteral("·")), new QLabel(QStringLiteral("板块介绍")));

    form->itemAt(0)->widget()->setObjectName("dot");

    layout->addLayout(form);

    layout->addSpacing(12);
    layout->addWidget(m_contentLabel);

    widget->setLayout(layout);

    return widget;
}
