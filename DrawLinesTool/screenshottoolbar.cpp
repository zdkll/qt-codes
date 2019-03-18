#include "screenshottoolbar.h"

#include <QPainter>
#include <QActionGroup>

ScreenShotToolBar::ScreenShotToolBar(QWidget *parent)
    :QToolBar(parent)
{
    //
    createToolBar();

    connect(this,&ScreenShotToolBar::actionTriggered,this,&ScreenShotToolBar::slotActionTriggered);
}

void ScreenShotToolBar::slotActionTriggered(QAction *action)
{
    emit cmdClicked(action->data().toInt());
}

void ScreenShotToolBar::paintEvent(QPaintEvent *)
{
    QPainter pt(this);
    pt.fillRect(this->rect(),Qt::white);
}

void ScreenShotToolBar::createToolBar()
{
    QAction *saveAct = this->addAction(QStringLiteral("保存"));
    saveAct->setData(SSTB_Save);

    QAction *okAct = this->addAction(QStringLiteral("确定"));
    okAct->setData(SSTB_Ok);

    QAction *cancelAct = this->addAction(QStringLiteral("取消"));
    cancelAct->setData(SSTB_Cancel);
}
