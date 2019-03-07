#include "hzrelogindlg.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QStyleOption>

#include "packetprocess.h"
#include "datacontroller.h"
#include "hzobject.h"
#include "pubmsgdef.h"

HZReloginDlg::HZReloginDlg(QWidget *parent)
    : QDialog(parent)
  ,mDragWindow(false)
{
    setObjectName("HZReloginDlg");
    setFixedSize(300,200);
    setWindowFlags(Qt::FramelessWindowHint);

    //注册回调
    OnRecvUserKick kickFun = std::bind(&HZReloginDlg::startKick, this, std::placeholders::_1);
    gHZDataController->subUserKickMsg(kickFun);

    mTitleFrame = new QFrame;
    mTitleFrame->setObjectName("KickTitle");
    mTitleFrame->setLayout(new QHBoxLayout);

    mTitleFont.setFamily("SimSun");
    mTitleFont.setPixelSize(14);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(mTitleFrame);

    QVBoxLayout *childVl = new QVBoxLayout;
    childVl->setSpacing(0);

    QLabel *label = new QLabel("您已被迫下线");
    label->setAlignment(Qt::AlignHCenter);
    childVl->addWidget(label);

    label = new QLabel("请重新登录或关闭！");
    label->setAlignment(Qt::AlignHCenter);
    childVl->addWidget(label);
    vLayout->addLayout(childVl);

    mBtnReLogin = new QPushButton("重新登录");
    mBtnClose = new QPushButton("退出");

    mBtnReLogin->setFixedSize(100,25);
    mBtnClose->setFixedSize(100,25);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(mBtnReLogin);
    hLayout->addWidget(mBtnClose);

    vLayout->setSpacing(12);
    vLayout->addLayout(hLayout);

    QSpacerItem *bottonItem = new QSpacerItem(0,18,QSizePolicy::Expanding,QSizePolicy::Fixed);
    vLayout->addSpacerItem(bottonItem);
    setLayout(vLayout);

    connect(mBtnReLogin, &QPushButton::clicked, this, &HZReloginDlg::slotBtnClicked);
    connect(mBtnClose, &QPushButton::clicked, this, &HZReloginDlg::slotBtnClicked);
    connect(this, &HZReloginDlg::accepted, this, &HZReloginDlg::hide);
    connect(this, &HZReloginDlg::rejected, this, &HZReloginDlg::hide);

    loadSkin();
}

void HZReloginDlg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void HZReloginDlg::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - mMousePoint);
        e->accept();
    }
}

void HZReloginDlg::mouseReleaseEvent(QMouseEvent *e)
{
    mDragWindow = false;
}

/*!
 * \brief HZReloginDlg::startKick 实现互踢
 * \details
 *   其他机器登陆后，当前登陆机器会收到服务器踢人通知。
     首先断开本次网络连接
     弹框提示用户 重新登陆 或退出（UI可以试用自绘的messageBox控件）
     退出： 关闭进程，退出程序
     重新登陆： 启用登陆流程
 */
void HZReloginDlg::startKick(const HZData::UserLoginKickInfo &kickInfo)
{
    qDebug() << Q_FUNC_INFO << "uid:" << kickInfo.uid << "dev:" <<  kickInfo.device_type << "des:" << QString::fromStdString(kickInfo.kick_desc);
    gHZDataController->disconnect();
    exec();
}

void HZReloginDlg::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QDialog::paintEvent(e);

    QRect rt(rect());
    rt.setHeight(30);
    p.fillRect(rt, QColor("#2f2f2f"));
    p.setPen(QColor("#ffffff"));
    p.setFont(mTitleFont);
    p.drawText(rt, Qt::AlignCenter, QStringLiteral("系统提示"));
}

void HZReloginDlg::slotBtnClicked()
{
    if(sender() == mBtnReLogin) {
        accepted();
        std::map<QString,QString> msg;
        QString path = ROOT_FRAME;
        path.append("//");
        path.append(LogIn_Dialog);
        msg.emplace(HZ_CMD,SHOW_LOGIN_DIALOG);
        sendMessage(path,msg,false);
    } else if(sender() == mBtnClose) {
        rejected();
        qApp->quit();
    }
}

void HZReloginDlg::loadSkin()
{
//    QFile file("kickdlg.qss");
//    if(!file.open(QIODevice::ReadOnly)) {
//        qDebug() << file.errorString();
//        return;
//    }

//    setStyleSheet(file.readAll());
//    file.close();
}
