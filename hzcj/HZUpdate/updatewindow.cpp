#include "updatewindow.h"
#include <QApplication>
#include <QLabel>
#include <QDebug>
#include <QThread>
#include <QCloseEvent>
#include <quazip/JlCompress.h>
#include <verinfoparse.h>

#include "updater.h"
#include "hzupdateuictrl.h"
#include "hzuserdatamgr.h"


//UpdateWindow---------------------------
HZUpdateWindow::HZUpdateWindow(QWidget *parent)
    :QDialog(parent)
    ,m_updater(new Updater)
    ,mUpdateUi(new HZUpdateUiCtrl)
    ,m_thread(new QThread(this))
    ,mDragWindow(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint|this->windowFlags());
    this->setObjectName("updatewindow");
    m_tmpPath  =HzUserDataMgr::getInstance()->getAllUsersDir()+"/tmp";
    //页面布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(mUpdateUi);
    setLayout(mainLayout);
    setContentsMargins(0,0,0,0);

    m_updater->moveToThread(m_thread);

    connect(mUpdateUi, &HZUpdateUiCtrl::signalAbortFinished, this, &HZUpdateWindow::cancelUpdate);//终止完成
    connect(mUpdateUi, &HZUpdateUiCtrl::signalBtnClickedForUpdateOver, this, &HZUpdateWindow::slotUpdateFinished);//更新完成按钮点击

    connect(m_updater, &Updater::signalProgress, this, &HZUpdateWindow::slotProgress);//本地处理进度内容
    connect(m_updater, &Updater::signalVerInfo, this, &HZUpdateWindow::slotVerInfo);//本地处理更新内容

    connect(this, &HZUpdateWindow::signalUpdateUi, mUpdateUi, &HZUpdateUiCtrl::slotUpdatePage);//更新页面
    connect(this, &HZUpdateWindow::signalStart, m_updater, &Updater::startUpdate);

    resize(510,310);
}

HZUpdateWindow::~HZUpdateWindow()
{
    if(m_updater)
        delete m_updater;
    if(m_thread->isRunning()){
        m_thread->quit();
        m_thread->wait();
    }
}

void HZUpdateWindow::startUpdate(const QString &verfile,const QString &updateDir)
{
    //初始化起始页面信息
    mUpdateUi->initProgressVaraint(0, Total_Progress, 0);

    m_updateDir = updateDir;
    mVerfile    = verfile;

    //开始更新
    slotEnsure();
}

void HZUpdateWindow::closeEvent(QCloseEvent *e)
{
    if(!m_updater->isStoped()){
        QMessageBox::warning(NULL,QStringLiteral("正在更新")
                             ,QStringLiteral("正在更新程序，请取消或者等待结束。"));
        return e->ignore();
    }
    return  e->accept();
}

void HZUpdateWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void HZUpdateWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - mMousePoint);
        e->accept();
    }
}

void HZUpdateWindow::mouseReleaseEvent(QMouseEvent *e)
{
    mDragWindow = false;
}

void HZUpdateWindow::cancelUpdate()
{
    mUpdateUi->setProgressInfomation(-1, "正在取消...");
    m_updater->cancel();

    //等待更新中止
    while(1){
        if(m_updater->isStoped()){
            break;
        }
        WaitSec(0.1);
    }

    //清除缓存
    if(m_updater->updateStep()>=Updater::Downloaded) {//清除下载包
        QDir dir(m_tmpPath);
        dir.remove("hzcj_update.zip");
    }

    if(m_updater->updateStep()==Updater::ExtraCompress) {//恢复安装目录
        QDir dir(m_updateDir);
        dir.removeRecursively();
        copyDirectoryFiles(m_tmpPath+"/hzcj",m_updateDir);
    }
    mUpdateUi->setProgressInfomation(-1, "更新已经取消");

    qApp->quit();
}

/*!
 * \brief HZUpdateWindow::slotEnsure
 * \details 点击立即更新时，切换页面并启动更新
 */
void HZUpdateWindow::slotEnsure()
{
    emit signalUpdateUi(MidUpdate);
    m_thread->start();
    emit signalStart(mVerfile, m_updateDir);
}

/*!
 * \brief HZUpdateWindow::slotUpdateOver 更新完成时，判断checkbox是否选中，选中则启动主程；否则退出界面
 * \param isRun checkbox 是否选中
 */
void HZUpdateWindow::slotUpdateFinished(bool isRun)
{
    m_updateDir.append("/shhzcjd.exe");
    m_updateDir.prepend("\"");
    m_updateDir.append("\"");
    if(isRun) QProcess::startDetached(m_updateDir);
    qApp->quit();
}

void HZUpdateWindow::slotProgress(int progress, QString context)
{
    mUpdateUi->setProgressInfomation(progress, context);
    if(Finshed_Progress == progress){
        m_thread->quit();
        m_thread->wait();
        //暂时不删除更新包，留测试
        QDir dir(m_tmpPath);
        dir.remove("hzcj_update.zip");
        emit signalUpdateUi(EndUpdate);
    }
}

void HZUpdateWindow::slotVerInfo(const VerInfo &verInfo)
{
    mUpdateUi->setUpdateContent(verInfo);
}


//拷贝文件夹
bool HZUpdateWindow::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}



