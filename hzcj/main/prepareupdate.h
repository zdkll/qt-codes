#ifndef PREPAREUPDATE_H
#define PREPAREUPDATE_H

#include <QFrame>
#include <QDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <downloadupdate.h>
#include <verinfoparse.h>
#include <framewidget.h>

namespace Ui {
class HZStartUpdateMain;
}

class HZUpdateUiCtrlDlg : public FrameLessDialog
{
    Q_OBJECT
public:
    HZUpdateUiCtrlDlg(QWidget *parent=nullptr, QString instName="PreUpdateTips_Dialog");
    void setUpdateContent(const VerInfo &info);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::HZStartUpdateMain *mUStartUi;
    QFrame *mUStartFrame;
    bool mDragWindow;
    QPoint mMousePoint;
};


class PrepareUpdate
{
public:
    static PrepareUpdate *instance();

    int checkAndUpdate(QString *errStr = 0);

private:
    void backupPacket();

    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist = true);

private:
    PrepareUpdate();
    QString m_tmpPath;
};

#endif // PREPAREUPDATE_H
