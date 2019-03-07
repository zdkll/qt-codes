#include "hzupdateuictrl.h"
#include "ui_hzupdateuictrl.h"
#include "ui_hzstartupdate.h"
#include "ui_hzendupdate.h"
#include "ui_hzupdating.h"

#include <QFile>
#include <QDebug>

HZUpdateUiCtrl::HZUpdateUiCtrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HZUpdateUiCtrl)
  ,mUStartUi(new Ui::HZStartUpdate)
  ,mUMidUi(new Ui::HZUpdating)
  ,mUEndUi(new Ui::HZEndUpdate)
  ,mUStartFrame(new QFrame)
  ,mUMidFrame(new QFrame)
  ,mUEndFrame(new QFrame)
  ,mStackWdt(new QStackedWidget)
{
    ui->setupUi(this);
    mUStartUi->setupUi(mUStartFrame);
    mUMidUi->setupUi(mUMidFrame);
    mUEndUi->setupUi(mUEndFrame);

    mUStartFrame->setObjectName("UpdateStartFrame");
    mUMidFrame->setObjectName("UpdateMidFrame");
    mUEndFrame->setObjectName("UpdateEndFrame");
    mStackWdt->setObjectName("UpdateStackWidget");
    ui->horizontalLayout->addWidget(mStackWdt);
    mStackWdt->insertWidget(StartUpdate, mUStartFrame);
    mStackWdt->insertWidget(MidUpdate, mUMidFrame);
    mStackWdt->insertWidget(EndUpdate, mUEndFrame);

    connect(mUStartUi->btn_start_right, &QPushButton::clicked, this, &HZUpdateUiCtrl::signalBtnClickedForEnsure);
    connect(mUStartUi->btn_start_left, &QPushButton::clicked, this, &HZUpdateUiCtrl::signalBtnClickedForCancle);
    connect(mUMidUi->btn_mid_cancle, &QPushButton::clicked, this, &HZUpdateUiCtrl::signalAbortFinished);
    connect(mUEndUi->btn_end_over, &QPushButton::clicked, [=]{
        emit signalBtnClickedForUpdateOver(mUEndUi->cbox_end_sel->isChecked());
    });

    loadskin();
}

HZUpdateUiCtrl::~HZUpdateUiCtrl()
{
    delete ui;
}

void HZUpdateUiCtrl::initProgressVaraint(int min, int max, int curVar)
{
    mUMidUi->pg_mid_progress->setRange(min,max);
    mUMidUi->pg_mid_progress->setValue(curVar);
}

void HZUpdateUiCtrl::setUpdateContent(const VerInfo &info)
{
//    qDebug() << Q_FUNC_INFO
//             << "update version:"   << info.updateVersion
//             << "update type:"      << info.updateType
//             << "channel:"          << info.channel
//             << "particalFrom:"     << info.partialFrom
//             << "release Time:"     << info.releaseTime
//             << "desc:"             << info.desc
//             << "details url:"      << info.detialsUrl
//             << "complete patch:"   << info.completePatch.type << info.completePatch.url << info.completePatch.size
//             << "optional patch:"   << info.optionalPatch.type << info.optionalPatch.url << info.optionalPatch.size;

    mUStartUi->lab_start_version->setText(QString("V%1").arg(info.updateVersion));
    mUStartUi->tb_start_msg->setText(info.desc);//descript
}

void HZUpdateUiCtrl::setProgressInfomation(int progress, const QString &content)
{
    if(progress != -1) mUMidUi->pg_mid_progress->setValue(progress);
    if(!content.isEmpty()) mUMidUi->lab_mid_update_tips->setText(content);
}

void HZUpdateUiCtrl::slotUpdatePage(int index)
{
    if(mStackWdt->count() <= index) return;
    mStackWdt->setCurrentIndex(index);
}

void HZUpdateUiCtrl::loadskin()
{
    QFile file(":/img/style.css");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    this->setStyleSheet(file.readAll());
    file.close();
}
