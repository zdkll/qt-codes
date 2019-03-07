#include "strategytitlewnd.h"
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QStyle>
#include "strategydynaprovider.h"
#include "plateinfodlg.h"
StrategyTitleWnd::StrategyTitleWnd(QWidget *parent) : BaseWidget(parent),
    m_pNameLabel(new QLabel),
    m_pZyxImgLabel(new QLabel),
    m_pCxxImgLabel(new QLabel),
    m_pContentLael(new QLabel)
{
    createWg();
}

void StrategyTitleWnd::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(28, 38, 0, 0);

    QHBoxLayout *pTopLayout = new QHBoxLayout;
    pTopLayout->setSpacing(0);
    pTopLayout->setMargin(0);

    pTopLayout->addWidget(m_pNameLabel);
    pTopLayout->addSpacing(40);
    pTopLayout->addWidget(new QLabel(QStringLiteral("重要性：")));
    pTopLayout->addWidget(m_pZyxImgLabel);
    pTopLayout->addSpacing(20);
    pTopLayout->addWidget(new QLabel(QStringLiteral("持续性：")));
    pTopLayout->addWidget(m_pCxxImgLabel);
    pTopLayout->addStretch();

    mainLayout->addLayout(pTopLayout);
    mainLayout->addSpacing(20);
    QLabel *label = new QLabel(QStringLiteral("板块介绍："));
    label->setObjectName("content");
    mainLayout->addWidget(label);
//    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_pContentLael);
    m_pContentLael->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    mainLayout->addSpacing(15);
    QPushButton *pViewBtn = new QPushButton(QStringLiteral("查看详情"));
    mainLayout->addWidget(pViewBtn);
    mainLayout->addStretch();

    connect(pViewBtn, &QPushButton::clicked, [&]{
        qDebug() << "11111111111";
        PlateInfoDlg dlg;
        dlg.updatedata(m_deTailData);
        dlg.exec();
    });
    setObjName();
    updateText();
}

void StrategyTitleWnd::updateText()
{
    if(m_deTailData.detail.topic_name.isEmpty()){
        m_pNameLabel->setText(QStringLiteral("--"));
    }
    else{
        m_pNameLabel->setText(m_deTailData.detail.topic_name);
    }
    if(m_deTailData.detail.intro.isEmpty()){
        m_pContentLael->setText(QStringLiteral("--"));
    }
    else{
        QFontMetrics metrics = m_pContentLael->fontMetrics();

        QString strText = metrics.elidedText(m_deTailData.detail.intro, Qt::ElideRight, this->width() - 28);

        m_pContentLael->setText(strText);
    }


    //	重要性
//    if(m_deTailData.detail.significance == "0")
//    {
//        m_pZyxImgLabel->setProperty("type", "0");
//    }
//    else if(m_deTailData.detail.significance == "1")
//    {
//        m_pZyxImgLabel->setProperty("type", "1");
//    }
//    else if(m_deTailData.detail.significance == "2")
//    {
//        m_pZyxImgLabel->setProperty("type", "2");
//    }
//    else if(m_deTailData.detail.significance == "3")
//    {
//        m_pZyxImgLabel->setProperty("type", "3");
//    }
//    else if(m_deTailData.detail.significance == "4")
//    {
//        m_pZyxImgLabel->setProperty("type", "4");
//    }
//    else if(m_deTailData.detail.significance == "5")
//    {
//        m_pZyxImgLabel->setProperty("type", "5");
//    }
//    else
//    {
//        m_pZyxImgLabel->setProperty("type", "0");
//    }
    //	重要性
    m_pZyxImgLabel->setProperty("type", m_deTailData.detail.significance);
    m_pZyxImgLabel->style()->unpolish(m_pZyxImgLabel);
    m_pZyxImgLabel->style()->polish(m_pZyxImgLabel);
    m_pZyxImgLabel->update();

    //	持续性
//    if(m_deTailData.detail.sustainability == "0")
//    {
//        m_pCxxImgLabel->setProperty("type", "0");
//    }
//    else if(m_deTailData.detail.sustainability == "1")
//    {
//        m_pCxxImgLabel->setProperty("type", "1");
//    }
//    else if(m_deTailData.detail.sustainability == "2")
//    {
//        m_pCxxImgLabel->setProperty("type", "4");
//    }
//    else if(m_deTailData.detail.sustainability == "3")
//    {
//        m_pCxxImgLabel->setProperty("type", "3");
//    }
//    else if(m_deTailData.detail.sustainability == "4")
//    {
//        m_pCxxImgLabel->setProperty("type", "4");
//    }
//    else if(m_deTailData.detail.sustainability == "5")
//    {
//        m_pCxxImgLabel->setProperty("type", "5");
//    }
//    else
//    {
//        m_pCxxImgLabel->setProperty("type", "0");
//    }

    //	持续性
    m_pCxxImgLabel->setProperty("type", m_deTailData.detail.sustainability);
    m_pCxxImgLabel->style()->unpolish(m_pCxxImgLabel);
    m_pCxxImgLabel->style()->polish(m_pCxxImgLabel);
    m_pCxxImgLabel->update();

}

void StrategyTitleWnd::setObjName()
{
    m_pNameLabel->setObjectName("title");
    m_pContentLael->setObjectName("content");
    m_pZyxImgLabel->setObjectName("zyx");
    m_pCxxImgLabel->setObjectName("cxx");
}

void StrategyTitleWnd::setDynaProvider(DynaDataProvider *pProvider)
{
    m_pProvider = pProvider;
}

void StrategyTitleWnd::updatedata(const TopicInvestDetail &data)
{
    m_deTailData = data;
    updateText();
}

void StrategyTitleWnd::resizeEvent(QResizeEvent *e)
{
    BaseWidget::resizeEvent(e);

    QFontMetrics metrics = m_pContentLael->fontMetrics();

    QString strText = metrics.elidedText(m_deTailData.detail.intro, Qt::ElideRight, this->width() - 28);

    m_pContentLael->setText(strText);

}

