#include "popupwindow.h"
#include "titlebar.h"


PopupWindow::PopupWindow(QWidget* parent,QString instName)
    :QWidget(parent),HZObject(instName)
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    this->setMouseTracking(true); //界面拉伸需要这个属性

    createWg();
}

PopupWindow::~PopupWindow()
{

}

void PopupWindow::createWg()
{
    m_pTitleBar = new TitleBar((BaseWidget *)this);
    m_pTitleBar->createTitleBar();
    m_pLayout = new QVBoxLayout();
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(1, 1, 1, 1);

    m_pHelper = new FramelessHelper(this);
    m_pLayout->addWidget(m_pTitleBar);
    this->setLayout(m_pLayout);

    m_pHelper->activateOn(this);  //激活当前窗体
    m_pHelper->setTitleHeight(m_pTitleBar->height());  //设置窗体的标题栏高度
    m_pHelper->setWidgetMovable(true);  //设置窗体可移动
    m_pHelper->setWidgetResizable(true);  //设置窗体可缩放
    //m_pHelper->setRubberBandOnMove(true);  //设置橡皮筋效果-可移动
    //m_pHelper->setRubberBandOnResize(true);  //设置橡皮筋效果-可缩放
}

void PopupWindow::setWidget(QWidget *wg)
{
    m_pLayout->addWidget(wg);
}


