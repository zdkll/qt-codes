#include "shorttermchangedialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include "mincanvas.h"
#include "shorttermchangewidget.h"

ShortTermChangeDialog::ShortTermChangeDialog(QWidget *parent):
    BaseDialog(parent),
    m_pHelper(new FramelessHelper(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    createWg();
}

void ShortTermChangeDialog::createWg()
{
    m_pHelper->activateOn(this);
    m_pHelper->setWidgetResizable(false);
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(1);

    mainLayout->addWidget(createTitleBar(), 0, 0, 1, 2);

    mainLayout->addWidget(createLeftWidget(), 1, 0);

    mainLayout->setSpacing(1);
    mainLayout->addWidget(createRightWidget(), 1, 1);

}

QWidget *ShortTermChangeDialog::createTitleBar()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("title");
    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setMargin(0);
    QLabel *label = new QLabel("短线异动");

    label->setObjectName("title");
    m_controlBox = new WindowControlBox(this, this, false, false);
    pHBoxLayout->addSpacing(337);
    pHBoxLayout->addWidget(label,0 ,Qt::AlignCenter);

    pHBoxLayout->addWidget(m_controlBox, 0, Qt::AlignRight);

    widget->setLayout(pHBoxLayout);

    return widget;
}



QWidget *ShortTermChangeDialog::createLeftWidget()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("leftwidget");

    ShortTermChangeWidget *view = new ShortTermChangeWidget;
    connect(view, &ShortTermChangeWidget::clickedRow, this , &ShortTermChangeDialog::onRowClicked);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(view);

    widget->setLayout(layout);

    return widget;
}

QVector<ChartItemInfo> getChartInfo()
{
    QVector<ChartItemInfo> info;

    ChartItemInfo item;
    item.chartClass = "Minchart";
    item.type = ChartMainType;
    item.attr = MinTitleShowObjName|MinTitleShowAvgData;
    item.titleClass = "ChartTitle";
    info.push_back(item);

    item = ChartItemInfo();
    item.padding = 1;
    item.chartClass = "MinTimeXaxisChart";
    item.nHeight = TimeXaxisHeight;
    item.type = ChartTimeAxisType;
    info.push_back(item);

    item = ChartItemInfo();
    //item.margins = ChartMargins(1,1,1,1);
    item.chartClass = "CrossLineChart";
    item.type = ChartCrossType;
    item.bFillArea = true;
    info.push_back(item);

    return info;

}

QWidget *ShortTermChangeDialog::createRightWidget()
{
    m_minChart = new MinCanvas(this);
    m_minChart->setRecordSwitchPage(false);
    m_minChart->setCreateCrossDlgFlag(true);
    m_minChart->setBaseInfo(m_strCode);
    m_minChart->setChartsInfo(getChartInfo());
    m_minChart->createWg();
    return m_minChart;
}

void ShortTermChangeDialog::showEvent(QShowEvent *evnet)
{
    m_minChart->setBaseInfo(m_strCode);
    m_minChart->afterActive();
}

void ShortTermChangeDialog::hideEvent(QHideEvent *event)
{
    m_minChart->afterDeactive();
}

void ShortTermChangeDialog::onRowClicked(QString strCode)
{
    if(m_strCode != strCode)
    {
        m_strCode = strCode;
        m_minChart->switchStock(m_strCode);
    }
}
