#include "exitdialog.h"
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>



ExitDialog::ExitDialog(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createWg();
}

void ExitDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);
    m_pHelper->setTitleHeight(230);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);


    QWidget *topWidget = new QWidget;
    topWidget->setObjectName("topwidget");

    QWidget *bottomWidget = new QWidget;
    bottomWidget->setObjectName("bottomwidget");


    topWidget->setLayout(createTitleBar());
    bottomWidget->setLayout(createBottomWidget());
    mainLayout->addWidget(topWidget);
    mainLayout->addWidget(bottomWidget);
}

QBoxLayout *ExitDialog::createTitleBar()
{
    m_controlBox = new WindowControlBox(this, this, false, false);
    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setContentsMargins(0, 7, 7, 0);
    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(m_controlBox);


    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setSpacing(0);
    pVBoxLayout->setMargin(0);

    pVBoxLayout->addLayout(pHBoxLayout);

    pVBoxLayout->addStretch();

    return pVBoxLayout;
}
#include <QDebug>
QBoxLayout *ExitDialog::createBottomWidget()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);

    QLabel *pLabel = new QLabel("确定要退出汇正财经吗？");
    pLabel->setObjectName("info");

    QCheckBox *pCheckBox = new QCheckBox;

    QPushButton *pText = new QPushButton("不再提醒");

    pCheckBox->setVisible(false);
    pText->setVisible(false);

    pText->setObjectName("remind");

    connect(pText, &QPushButton::clicked, [=]{
        if(pCheckBox->isChecked())
        {
            pCheckBox->setChecked(false);
        }
        else
        {
            pCheckBox->setChecked(true);
        }
    });



    m_pButtonBox = new QDialogButtonBox(this);

    QPushButton *pSubmit = new QPushButton;
    pSubmit->setObjectName("submit");
    QPushButton *pCancel = new QPushButton;
    pCancel->setObjectName("cancel");

    m_pButtonBox->addButton(pSubmit, QDialogButtonBox::AcceptRole);

    m_pButtonBox->addButton(pCancel, QDialogButtonBox::RejectRole);

    pCancel->setDefault(true);

    connect(pCancel, &QPushButton::clicked, [=]{

        this->close();
        qDebug() << "cancel clicked";
        emit closemainwnd(false);
    });

    connect(pSubmit, &QPushButton::clicked, [=]{

        this->close();
        emit closemainwnd(true);
    });

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setMargin(0);

//    pHBoxLayout->addSpacing(26);
//    pHBoxLayout->addWidget(pCheckBox);
//    pHBoxLayout->addWidget(pText);
//    pHBoxLayout->addSpacing(36);
    pHBoxLayout->addSpacing(130);
    pHBoxLayout->addWidget(pSubmit);
    pHBoxLayout->addSpacing(3);
    pHBoxLayout->addWidget(pCancel);
    pHBoxLayout->addStretch();

    pMainLayout->addSpacing(20);
    pMainLayout->addWidget(pLabel, 0, Qt::AlignHCenter | Qt::AlignTop);
    pMainLayout->addSpacing(16);
    pMainLayout->addLayout(pHBoxLayout);
    pMainLayout->addStretch();


    if(pCheckBox->isChecked())
    {
        emit ignoreRemind();
    }
   return pMainLayout;
}
