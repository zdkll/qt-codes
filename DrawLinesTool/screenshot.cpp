#include "screenshot.h"

#include <QKeyEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QDesktopWidget>
#include <QFileDialog>

#include <QDebug>

ScreenShot::ScreenShot(QWidget *parent)
    :QWidget(parent),m_isPressed(false)
    ,m_fullScreen(new QPixmap())
    ,m_bgScreen(new QPixmap())
    ,m_screenRect(new ScreenRect())
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    m_screenShotToolBar = new ScreenShotToolBar(this);
    m_screenShotToolBar->setFixedSize(150,30);
    m_screenShotToolBar->hide();

    connect(m_screenShotToolBar,&ScreenShotToolBar::cmdClicked,this,&ScreenShot::slotToolBarCmd);
}

ScreenShot::~ScreenShot()
{
    delete m_fullScreen;m_fullScreen = NULL;
    delete m_bgScreen;m_bgScreen = NULL;
    delete m_screenRect;m_screenRect = NULL;
}

void ScreenShot::startScreenShot()
{
    m_screenRect->initRect();
    qApp->installEventFilter(this);
    this->showFullScreen();
}

void ScreenShot::slotToolBarCmd(int cmd)
{
    if(cmd == ScreenShotToolBar::SSTB_Save){
        //保存图片
        QString fileName = QFileDialog::getSaveFileName(this,"save file",QDir::currentPath(),tr("PNG (*.png)"));
        if(!fileName.isEmpty()){
            QPixmap retPixmap = m_fullScreen->copy(*m_screenRect);
            retPixmap.save(fileName);
            endScreenShot(true);
            this->update();
        }
    }else if(cmd == ScreenShotToolBar::SSTB_Ok){
        endScreenShot(true);
    }else if(cmd == ScreenShotToolBar::SSTB_Cancel){
        endScreenShot(false);
    }
}

bool ScreenShot::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Escape) //退出截屏
        {
            endScreenShot(false);
            return true;
        }
    }
    return QWidget::eventFilter(watched,event);
}

void ScreenShot::showEvent(QShowEvent *e)
{
    //抓取原始截屏
    QScreen *pscreen = QApplication::primaryScreen();
    *m_fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0
                                        , pscreen->size().width(), pscreen->size().height());

    QWidget::showEvent(e);

    //设置蒙层背景图片
    *m_bgScreen = *m_fullScreen;
    QPixmap pix(pscreen->size());
    pix.fill(QColor(0,0,0,100));
    QPainter pt(m_bgScreen);
    pt.drawPixmap(0,0,pix);
}

void ScreenShot::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    //绘制背景图层
    painter.drawPixmap(0,0,*m_bgScreen);

    //绘制截图区域
    if(!m_screenRect->isEmpty())
        painter.drawPixmap(*m_screenRect,m_fullScreen->copy(*m_screenRect));

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    int x = m_screenRect->x();
    int y= m_screenRect->y();
    int w= m_screenRect->width();
    int h= m_screenRect->height();
    painter.drawRect(*m_screenRect);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, QStringLiteral("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenShot::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;

    if(event->button() == Qt::LeftButton){
        hideToolBar();
        m_screenRect->setBeginPt(event->pos());
    }else if(event->button() == Qt::RightButton)
    {
        endScreenShot(false);
    }
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed){
        m_screenRect->setEndPt(event->pos());
        this->update();
    }
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    showToolBar(event->pos());
}

void ScreenShot::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_isPressed = false;
}

void ScreenShot::endScreenShot(bool success)
{
    bool ok = success;
    if(ok){
        //判断截图区域是否合适
    }
    this->hideToolBar();
    qApp->removeEventFilter(this);
    this->hide();
    emit  finishedScreenShot(ok);
}


void ScreenShot::showToolBar(const QPoint &pt)
{
    m_screenShotToolBar->move(pt.x()-m_screenShotToolBar->width(),pt.y());
    m_screenShotToolBar->show();
}


