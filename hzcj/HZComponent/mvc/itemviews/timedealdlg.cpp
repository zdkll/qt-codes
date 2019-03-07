#include "timedealdlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "timedealwidget.h"
#include "httpdata.h"


///AllTimeDealDialog----
AllTimeDealDialog::AllTimeDealDialog(QWidget *parent)
    :FrameLessDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(2,0,2,2);
    m_titleBar = createTitleBar();
    mainLayout->addWidget(m_titleBar);

    m_timeDealWg = new AllTimeDealWidget(this);
    m_timeDealWg->setShowTitle(false);
    mainLayout->addWidget(m_timeDealWg);

    this->setLayout(mainLayout);

    this->resize(1000,640);
}

void AllTimeDealDialog::setStockCode(const QString &stockCode)
{
    QString stockName = Httpdata::instance()->getCodeNameByCode(stockCode);

    QString titleStr = QString("%1(%2.%3)%4").arg(stockName).arg(stockCode.mid(2))
            .arg(stockCode.left(2).toUpper()).arg(QStringLiteral("-分时成交"));
    m_titleLabel->setText(titleStr);
    m_timeDealWg->setStockCode(stockCode.toStdString(),stockName);
}

void AllTimeDealDialog::afterActive()
{
    m_timeDealWg->afterActive();
}

void AllTimeDealDialog::afterDeactive()
{
    m_timeDealWg->afterDeactive();
}

TitleBar  *AllTimeDealDialog::createTitleBar()
{
    TitleBar *titleBar = new TitleBar(this,"timedealtitlebar");

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);

    m_titleLabel = new  QLabel(this);
    hLayout->addWidget(m_titleLabel,1,Qt::AlignCenter);

    WindowControlBox *controlBox =  new WindowControlBox(this, this,true,true,"windowcontrolbox");
    hLayout->addWidget(controlBox);

    titleBar->setLayout(hLayout);

    return titleBar;
}
