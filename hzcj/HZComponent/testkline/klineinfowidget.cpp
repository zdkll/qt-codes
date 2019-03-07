#include "klineinfowidget.h"
#include <QLabel>
#include <QVBoxLayout>

KlineInfoWidget::KlineInfoWidget(QWidget *parent):
    BaseWidget(parent),
    m_pHelper(new FramelessHelper(this)),
    m_dateLabel(new QLabel("--")),
    m_openLabel(new QLabel("--")),
    m_highLabel(new QLabel("--")),
    m_lowLabel(new QLabel("--")),
    m_closeLabel(new QLabel("--")),
    m_riseLabel(new QLabel("--")),
    m_riseRateLabel(new QLabel("--")),
    m_volumeLabel(new QLabel("--")),
    m_moneyLabel(new QLabel("--")),
    m_ampLabel(new QLabel("--")),
    m_changeLabel(new QLabel("--"))
{

//    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint |windowFlags());
    m_pHelper->setWidgetResizable(false);
    m_pHelper->activateOn(this);
    this->setFixedSize(60, 345);
    m_pHelper->setTitleHeight(345);
    this->setWindowOpacity(0.9);

    createWg();
}

void KlineInfoWidget::createWg()
{
    QLabel *titleLabel = new QLabel(QStringLiteral("【可拖动】"));
    QLabel *dateLabel = new QLabel(QStringLiteral("日 期"));
    QLabel *openLabel = new QLabel(QStringLiteral("开 盘"));
    QLabel *highLabel = new QLabel(QStringLiteral("最 高"));
    QLabel *lowLabel = new QLabel(QStringLiteral("最 低"));
    QLabel *closeLabel = new QLabel(QStringLiteral("收 盘"));
    QLabel *riseLabel = new QLabel(QStringLiteral("涨 跌"));
    QLabel *riseRateLabel = new QLabel(QStringLiteral("涨 幅"));
    QLabel *volumeLabel = new QLabel(QStringLiteral("成交量"));
    QLabel *moneyLabel = new QLabel(QStringLiteral("金 额"));
    QLabel *ampLabel = new QLabel(QStringLiteral("振 幅"));
    QLabel *changeLabel = new QLabel(QStringLiteral("换 手"));

    titleLabel->setObjectName("title");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(2);
    mainLayout->setMargin(2);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(dateLabel);
    mainLayout->addWidget(m_dateLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(openLabel);
    mainLayout->addWidget(m_openLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(highLabel);
    mainLayout->addWidget(m_highLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(lowLabel);
     mainLayout->addWidget(m_lowLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(closeLabel);
     mainLayout->addWidget(m_closeLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(riseLabel);
    mainLayout->addWidget(m_riseLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(riseRateLabel);
    mainLayout->addWidget(m_riseRateLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(volumeLabel);
    mainLayout->addWidget(m_volumeLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(moneyLabel);
    mainLayout->addWidget(m_moneyLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(ampLabel);
    mainLayout->addWidget(m_ampLabel, 0, Qt::AlignRight);
    mainLayout->addWidget(changeLabel);
    mainLayout->addWidget(m_changeLabel, 0, Qt::AlignRight);

}
