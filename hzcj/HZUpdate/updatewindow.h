/********************************************************************
created:2018/05
author: dk.zhang
purpose:更新主界面
*********************************************************************/
#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>
#include <QMouseEvent>

#include <basewidget.h>
#include <quazip/zip.h>
//#include <authenpublic.h>

class HZUpdateUiCtrl;
class QThread;
class Updater;
struct VerInfo;
class HZUpdateWindow : public QDialog,public HZObject
{
    Q_OBJECT

public:
    HZUpdateWindow(QWidget *parent = 0);
    ~HZUpdateWindow();

    /**
     * @brief 开始更新
     * @param[in] 更新版本信息文件
     * @param[in] 更新目标程序路径
     * @retval
     */
    void startUpdate(const QString &verfile,const QString &updateDir);

signals:
    void signalUpdateUi(int index);
    void signalStart(const QString &verfile,const QString &updateDir);

protected:
    void closeEvent(QCloseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

protected slots:
    void slotProgress(int progress, QString context );//刷新当前更新进度信息
    void slotVerInfo(const VerInfo &verInfo); //设置版本信息
    void cancelUpdate();  //取消更新
    void slotEnsure();       //立即更新
    //更新完成，是否启动
    void slotUpdateFinished(bool isRun);

private:
     bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist = true);

    Updater *m_updater;
    QThread  *m_thread;

    HZUpdateUiCtrl *mUpdateUi;
    QString m_updateDir, mVerfile;

    bool mDragWindow;
    QPoint mMousePoint;
    QString m_tmpPath;
};
#endif // UPDATEWINDOW_H
