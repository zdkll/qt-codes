#include "introductionbutton.h"

#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

IntroductionButton::IntroductionButton(const QString &hintString,QWidget *parent)
    :BaseWidget(parent,"introductionbutton"),m_hintString(hintString)
{
    createWg();
}

void IntroductionButton::setHintString(const QString &hintString)
{
    m_hintString = hintString;
    m_hintLabel->setText(hintString);
}

void IntroductionButton::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(14,7,0,7);
    mainLayout->setSpacing(6);

    m_hintLabel = new QLabel(m_hintString,this);
    mainLayout->addWidget(m_hintLabel);

    // m_button = new  QPushButton(this);

    // mainLayout->addWidget(m_button);
    mainLayout->addStretch();

    // connect(m_button,&QPushButton::clicked,this, &IntroductionButton::showIntroduction);

    this->setLayout(mainLayout);
}
