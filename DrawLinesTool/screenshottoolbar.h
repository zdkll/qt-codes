#ifndef SCREENSHOTTOOLBAR_H
#define SCREENSHOTTOOLBAR_H

#include <QToolBar>

class ScreenShotToolBar : public QToolBar
{
    Q_OBJECT
public:
    //指令
    enum SSTBCommand
    {
        SSTB_Save = 0,//保存
        SSTB_Ok    = 1,//确定
        SSTB_Cancel = 2,//取消
    };
    ScreenShotToolBar(QWidget *parent = Q_NULLPTR);

signals:
    void cmdClicked(int sbCommand);//SSTBCommand

protected slots:
    void slotActionTriggered(QAction *action);

protected:
    void paintEvent(QPaintEvent *e);

private:
   void createToolBar();

};

#endif // SCREENSHOTTOOLBAR_H
