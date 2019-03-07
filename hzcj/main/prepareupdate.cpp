#include "prepareupdate.h"
#include "ui_hzstartupdatemain.h"

#define  UpdateParam_File "/updateparam.txt"

#include <QDebug>
#include <QProcess>
#include <QUrl>
#include <QMessageBox>
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
#include <hzcjinfo.h>
#include <hzcfg.h>
#include <verinfoparse.h>
#include <hzlog.h>
#include <hzuserdatamgr.h>

///
/// \brief HZUpdateUiCtrlDlg::HZUpdateUiCtrlDlg
/// \param parent
///
HZUpdateUiCtrlDlg::HZUpdateUiCtrlDlg(QWidget *parent, QString instName)
    :FrameLessDialog(parent, instName)
    ,mUStartUi(new Ui::HZStartUpdateMain)
    ,mUStartFrame(new QFrame)
    ,mDragWindow(false)
{
    setObjectName("HZUpdateUiCtrlDlg");
    m_pHelper->setWidgetResizable(false);
    m_pHelper->setWidgetResizable(false);
    m_pHelper->setTitleHeight(this->height());

    mUStartUi->setupUi(mUStartFrame);
    mUStartFrame->setObjectName("UpdateStartFrameMain");

    setLayout(new QHBoxLayout);
    layout()->addWidget(mUStartFrame);
    layout()->setContentsMargins(0,0,0,0);
    setContentsMargins(0,0,0,0);

    connect(mUStartUi->btn_start_left, &QPushButton::clicked, this, &HZUpdateUiCtrlDlg::reject);
    connect(mUStartUi->btn_start_right, &QPushButton::clicked, this, &HZUpdateUiCtrlDlg::accept);

    resize(510,310);
}

void HZUpdateUiCtrlDlg::setUpdateContent(const VerInfo &info)
{
    mUStartUi->lab_start_version->setText(QString("V%1").arg(info.updateVersion));
    mUStartUi->tb_start_msg->setText(info.desc);//descript
}

void HZUpdateUiCtrlDlg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void HZUpdateUiCtrlDlg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void HZUpdateUiCtrlDlg::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - mMousePoint);
        e->accept();
    }
}

void HZUpdateUiCtrlDlg::mouseReleaseEvent(QMouseEvent *e)
{
    mDragWindow = false;
}


///
/// \brief PrepareUpdate::PrepareUpdate
///
PrepareUpdate::PrepareUpdate()
{
    m_tmpPath = HzUserDataMgr::getInstance()->getAllUsersDir()+"/tmp";
}

PrepareUpdate *PrepareUpdate::instance()
{
    static PrepareUpdate prepareUpdate;
    return &prepareUpdate;
}

int PrepareUpdate::checkAndUpdate(QString *errStr)
{
    //清除缓存目录
    QString tmpFile = m_tmpPath+"/hzcj";
    QDir dir(tmpFile);
    if(dir.exists())
        dir.removeRecursively();

    HZ_AUTO_ELAPSED_TIMER;

    bool ok = false;
    //1 下载版本文件
    DownloadUpdate dowloader;
    QString verInfoUrl = Hzcfg::getInstance()->serverInfo().updateInfoUrl;
    QString verionfile = dowloader.downloadBlocked(QUrl(verInfoUrl),&ok,DownloadUpdate::VersionInfo);
    if(!ok){
        if(errStr)
            *errStr = "Get version file failed.";
        return -1;
    }
   qDebug()<<"downloaded verinfo";
    //2 解析版本文件
    VerInfoParse verInfoParse;
    verInfoParse.setVersionFile(verionfile);
    int updateMode = verInfoParse.updateType();

    //判断版本号
    if(verInfoParse.versionInfo().updateVersion == PRODUCT_VERSION) return 1;
    //判断渠道号
    int channelNo = verInfoParse.versionInfo().channel;
    if(PRODUCT_CHANNEL != channelNo && channelNo != 0)
        return 1;

    switch(updateMode)
    {
    case VerInfoParse::NoUpdate:break;
    case VerInfoParse::OptionalUpdate:
    {
        HZUpdateUiCtrlDlg updlg;
        updlg.setUpdateContent(verInfoParse.versionInfo());
        if(updlg.exec() == QDialog::Rejected) {
            return 1;
        }
    }
    case VerInfoParse::RequredUpdate:
    {
        //准备更新,将整个程序移到tmp目录
        backupPacket();

        //保存参数
        QString paramFile = m_tmpPath+UpdateParam_File;
        QFile file(paramFile);
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream out(&file);
        out << verionfile << endl;
        out << qApp->applicationDirPath() << endl;
        file.flush();
        file.close();
        //版本文件和更新目录
        QString exec_path = m_tmpPath+"/hzcj/HZUpdate.exe";
        exec_path.prepend("\"");
        exec_path.append("\"");
        QProcess::startDetached(exec_path);
        return 0;
    }
    default: break;
    }

    return 1;
}


void PrepareUpdate::backupPacket()
{
    HZ_AUTO_ELAPSED_TIMER;
    QString source_dir = qApp->applicationDirPath();
    QString dest_dir    =  HzUserDataMgr::getInstance()->getAllUsersDir()+"tmp/hzcj";
    qDebug()<<"move"<<source_dir<<"to"<<dest_dir;
    QDir dir(dest_dir);
    if(dir.exists())
        dir.removeRecursively();
    copyDirectoryFiles(source_dir,dest_dir);
}

//拷贝文件夹
bool PrepareUpdate::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    HZ_AUTO_ELAPSED_TIMER;
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
