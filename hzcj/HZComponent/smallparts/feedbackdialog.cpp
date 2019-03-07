#include "feedbackdialog.h"
#include <QPushButton>
#include "hzauthenopt.h"

FeedbackDialog::FeedbackDialog(QWidget *parent):
    BaseDialog(parent),
    m_controlBox(nullptr),
    m_mainLayout(new QVBoxLayout(this)),
    m_pHelper(new FramelessHelper(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint|windowFlags());

    createWg();

    getUserInfo();
}

void FeedbackDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);
    this->setFixedSize(QSize(500, 450));

    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);
    QWidget *titleWidget = new QWidget;
    titleWidget->setObjectName("title");
    titleWidget->setFixedSize(QSize(500, 30));

    QWidget *contentWidget = new QWidget;
    contentWidget->setObjectName("content");
    contentWidget->setFixedSize(QSize(500, 420));


    QHBoxLayout *pTitleLayout = new QHBoxLayout(this);
    pTitleLayout->setSpacing(0);
    pTitleLayout->setMargin(0);
    QLabel *pTitle = new QLabel(QStringLiteral("意见反馈"));
    m_controlBox = new WindowControlBox(this, this, false, false);

    pTitleLayout->addStretch();
    pTitleLayout->addWidget(pTitle, 0, Qt::AlignCenter);
    pTitleLayout->addStretch();
    pTitleLayout->addWidget(m_controlBox, 0, Qt::AlignRight);

    titleWidget->setLayout(pTitleLayout);

    QVBoxLayout *contentLayout = new QVBoxLayout(this);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 36, 0, 28);

    m_ptextEdit = new QTextEdit;
    m_ptextEdit->setPlaceholderText(QStringLiteral("  您的反馈我们将第一时间反馈，感谢您的支持！"));
    m_ptextEdit->setFixedSize(QSize(430, 230));
    m_ptextEdit->setObjectName("content");
    m_plineEdit = new QLineEdit;
    m_plineEdit->setTextMargins(15,0,0,0);
    m_plineEdit->setPlaceholderText(QStringLiteral("QQ/邮箱/手机号"));
    m_plineEdit->setFixedSize(QSize(430, 32));
    m_plineEdit->setObjectName("userinfo");
    m_promptLabel = new QLabel(QStringLiteral("*为了联系您请留下您的联系方式"));

    QHBoxLayout *promptLayout = new QHBoxLayout;
    promptLayout->setSpacing(0);
    promptLayout->setMargin(0);
    promptLayout->addSpacing(50);
    promptLayout->addWidget(m_promptLabel);
    QPushButton *pSubmit = new QPushButton(QStringLiteral("提交"));

    connect(pSubmit, &QPushButton::clicked, this, &FeedbackDialog::onSubmitBtnClicked);
    pSubmit->setFixedSize(QSize(230, 30));

    contentLayout->addWidget(m_ptextEdit, 0, Qt::AlignCenter);
    contentLayout->addSpacing(9);
    contentLayout->addWidget(m_plineEdit, 0, Qt::AlignCenter);
    contentLayout->addSpacing(12);
    contentLayout->addLayout(promptLayout);
    contentLayout->addSpacing(25);
    contentLayout->addWidget(pSubmit, 0, Qt::AlignCenter);

    contentLayout->addStretch();

    contentWidget->setLayout(contentLayout);



    m_mainLayout->addWidget(titleWidget);
    m_mainLayout->addWidget(contentWidget);

}

void FeedbackDialog::getUserInfo()
{
    HZAuthenOpt *authenopt = HZAuthenOpt::instance();
    int nID = -1;

    HZAuthenOpt::LogStatus status = authenopt->logStatus();

    if(status == HZAuthenOpt::LogStatus::UserLogged)
    {
        nID = authenopt->queryUserProfile();
    }

    connect(authenopt, &HZAuthenOpt::commandFinished, [=](int id, int error){
        if(error == 0)
        {
            if(nID == id)
            {
                if(status == HZAuthenOpt::LogStatus::UserLogged)
                {
                    HZAuthen::UserProfile userProfile = authenopt->getUserProfile();
                    if(!userProfile.mobileNum.isEmpty())
                    {
                        m_plineEdit->setText(userProfile.mobileNum.replace(3, 4, "xxxx"));
                    }
                    else if(!userProfile.qq.isEmpty())
                    {
                        m_plineEdit->setText(userProfile.qq.replace(3, 3, "xxx"));
                    }
               }
            }
        }
    });
}

void FeedbackDialog::onSubmitBtnClicked()
{
    QString strContent = m_ptextEdit->toPlainText();
    QString strInfo = m_plineEdit->text();

    qDebug() << strContent;

    qDebug() << strInfo;
}
