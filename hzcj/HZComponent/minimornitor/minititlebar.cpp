#include "minititlebar.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QVariant>


MiniTitleBar::MiniTitleBar(BaseWidget *parent):
    BaseWidget(parent),
    m_bPlay(true)
{
    createWg();
}

void MiniTitleBar::createWg()
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(14, 0, 0, 0);
    pLayout->setSpacing(0);
    QLabel *label = new QLabel(QStringLiteral("MINI盯盘"));

    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QPushButton *playBtn = new QPushButton(QStringLiteral(""));
    connect(playBtn, &QPushButton::clicked, this, &MiniTitleBar::onPlayBtnClicked);
    playBtn->setProperty("on", m_bPlay);
    QPushButton *pMaxBtn = new QPushButton(QStringLiteral(""));
    QPushButton *pMinBtn = new QPushButton(QStringLiteral(""));
    QPushButton *pCloseBtn = new QPushButton(QStringLiteral(""));
    playBtn->setObjectName("playBtn");
    pMaxBtn->setObjectName("maxBtn");
    pMinBtn->setObjectName("minBtn");
    pCloseBtn->setObjectName("closeBtn");
    connect(pMinBtn, &QPushButton::clicked, [this]{emit minMizeClicked();});
    connect(pCloseBtn, &QPushButton::clicked, [this]{emit clickClose();});
    connect(pMaxBtn, &QPushButton::clicked, [this]{emit maxiMize();});
    pLayout->addWidget(label);
    pLayout->addStretch();
    pLayout->addWidget(playBtn);
    pLayout->addSpacing(11);
    pLayout->addWidget(pMaxBtn);
    pLayout->addWidget(pMinBtn);
    pLayout->addWidget(pCloseBtn);

}

void MiniTitleBar::onPlayBtnClicked()
{
    if(!m_bPlay)
    {
        m_bPlay = true;
    }
    else
    {
       m_bPlay = false;
    }
    emit playBtnClicked();
    QPushButton *playBtn = qobject_cast<QPushButton *>(sender());
    playBtn->setProperty("on", m_bPlay);
    playBtn->style()->unpolish(playBtn);
    playBtn->style()->polish(playBtn);
    playBtn->update();
}
