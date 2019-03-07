#include "hzaboutdlg.h"
#include "ui_hzaboutdlg.h"

#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QFontDialog>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


HZAboutDlg::HZAboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HZAboutDlg)
  ,m_dragWindow(false)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    QVariantMap varMap;
    varMap["version"] = "1.1.2";
    varMap["productname"] = "汇正财经PC客户端";
    varMap["releasetime"] = "2018-08-17";
    varMap["versioninfo"] = "beta 20180817010102";
    setMsg(QVariant::fromValue(varMap));
    ui->lab_min_font->setStyleSheet("color:#b08324;");

    connect(ui->btn_cls, &QPushButton::clicked, this, &HZAboutDlg::close);
    connect(ui->btn_ico, &QPushButton::clicked, this, [=]{
        QFontDialog dlg;
        dlg.exec();
    });
    loadSkin();
}

HZAboutDlg::~HZAboutDlg()
{
    delete ui;
}

void HZAboutDlg::setMsg(const QVariant &variant)
{
    if(variant.isNull()) return;
    QVariantMap varMap = variant.toMap();
    QString version = varMap["version"].toString();
    QString productName = varMap["productname"].toString();
    QString releaseTime = varMap["releasetime"].toString();
    QString versionInfo = varMap["versioninfo"].toString();

    ui->lab_version->setText(version);
    ui->lab_productname->setText(productName);
    ui->lab_releasetime->setText(releaseTime);
    ui->lab_versioninfo->setText(versionInfo);
}

void HZAboutDlg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton
            && ui->frameTitle->geometry().contains(e->pos())) {
        m_mousePoint = e->pos() - ui->frameTitle->pos();
        e->accept();
        m_dragWindow = true;
    }
}

void HZAboutDlg::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - m_mousePoint);
        e->accept();
    }
}

void HZAboutDlg::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragWindow = false;
}

void HZAboutDlg::loadSkin()
{
//    QFile file(":/img/style.css");
//    if(!file.open(QIODevice::ReadOnly)){
//        qDebug() << "file open failed..." << file.errorString();
//        return;
//    }

//    setStyleSheet(file.readAll());
//    file.close();
}
