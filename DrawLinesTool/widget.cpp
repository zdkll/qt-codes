#include "widget.h"
#include "ui_widget.h"

#include <QResizeEvent>
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //移动到点到线需要修改鼠标状态
    this->setMouseTracking(true);

    m_linesDrawer = new LinesDrawer(this);

    this->installEventFilter(m_linesDrawer);

    this->resize(800,400);
    m_drawLinePanel = new DrawLinePanel(this);
    m_drawLinePanel->setFixedSize(180,420);

    m_drawLinePanel->show();

    m_linesDrawer->penChanged(m_drawLinePanel->pen());
    m_linesDrawer->setLineTools(m_drawLinePanel->lineTools());
    //命令先过滤一道
    connect(m_drawLinePanel,&DrawLinePanel::startCommand,this,&Widget::startCommand);
    connect(m_drawLinePanel,&DrawLinePanel::penChanged,m_linesDrawer,&LinesDrawer::penChanged);

    connect(m_linesDrawer,&LinesDrawer::finishedCurCommand,m_drawLinePanel,&DrawLinePanel::finishedCommand);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::startCommand(const DrawLine::Command& cmd)
{
    //命令过滤
    m_linesDrawer->beginCommand(cmd);
}

void Widget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter pt(this);
    pt.fillRect(this->rect(),QColor("#222222"));

    pt.drawPixmap(0,0,m_linesDrawer->pixmap());
    pt.end();
}

