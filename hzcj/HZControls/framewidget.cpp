#include "framewidget.h"
#include <qevent.h>
#include <QVBoxLayout>


FrameWidget::FrameWidget(BaseWidget *parent,QString instName) : BaseWidget(parent,instName)
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setMouseTracking(true); //界面拉伸需要这个属性
    m_pHelper = new FramelessHelper(this);
    m_pTitleBar = new TitleBar(this);
    m_pTitleBar->createTitleBar();
    m_pLayout = new QVBoxLayout();
    installEventFilter(m_pTitleBar);
}

void FrameWidget::create()
{
    resize(800, 600);

    //    QPalette pal(palette());
    //    pal.setColor(QPalette::Background, QColor(50, 50, 50));
    //    setAutoFillBackground(true);
    //    setPalette(pal);
    m_pLayout->addWidget(m_pTitleBar);
    m_pLayout->addStretch();
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_pLayout);


    m_pHelper->activateOn(this);  //激活当前窗体
    m_pHelper->setTitleHeight(m_pTitleBar->height());  //设置窗体的标题栏高度
    m_pHelper->setWidgetMovable(true);  //设置窗体可移动
    m_pHelper->setWidgetResizable(true);  //设置窗体可缩放
    //m_pHelper->setRubberBandOnMove(true);  //设置橡皮筋效果-可移动
    //m_pHelper->setRubberBandOnResize(true);  //设置橡皮筋效果-可缩放
}

FrameWidget::~FrameWidget()
{
    m_pHelper->removeFrom(this);
}

//FrameLessWidget------------------------------------
FrameLessWidget::FrameLessWidget(BaseWidget *parent,QString instName)
    :BaseWidget(parent,instName)
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setMouseTracking(true); //界面拉伸需要这个属性

    //无边框效果
    m_pHelper = new FramelessHelper(this);
    m_pHelper->activateOn(this);  //激活当前窗体
    m_pHelper->setWidgetMovable(true);  //设置窗体可移动
    m_pHelper->setWidgetResizable(true);  //设置窗体可缩放
    //    m_pHelper->setRubberBandOnMove(true);  //设置橡皮筋效果-可移动
    //    m_pHelper->setRubberBandOnResize(true);  //设置橡皮筋效果-可缩放
}

FrameLessWidget::~FrameLessWidget()
{
    m_pHelper->removeFrom(this);
}


FrameLessDialog::FrameLessDialog(QWidget *parent ,QString instName)
    :BaseDialog(parent,instName)
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setMouseTracking(true); //界面拉伸需要这个属性

    //无边框效果
    m_pHelper = new FramelessHelper(this);
    m_pHelper->activateOn(this);  //激活当前窗体
    m_pHelper->setWidgetMovable(true);  //设置窗体可移动
    m_pHelper->setWidgetResizable(true);  //设置窗体可缩放
}

FrameLessDialog::~FrameLessDialog()
{
    m_pHelper->removeFrom(this);
}


