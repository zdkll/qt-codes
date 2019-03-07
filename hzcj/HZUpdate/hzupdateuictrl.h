/********************************************************************
created: 2018/08
author: zhehu.xu
purpose: //更新UI
*********************************************************************/
#ifndef HZUPDATEUICTRL_H
#define HZUPDATEUICTRL_H

#include <QWidget>
#include <QtWidgets>
#include <QFrame>
#include <verinfoparse.h>

enum {StartUpdate=0, MidUpdate, EndUpdate};

namespace Ui {
class HZUpdateUiCtrl;
class HZEndUpdate;
class HZStartUpdate;
class HZUpdating;
}

class HZUpdateUiCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit HZUpdateUiCtrl(QWidget *parent = 0);
    ~HZUpdateUiCtrl();

    void initProgressVaraint(int min, int max, int curVar);
    void setUpdateContent(const VerInfo &info);

    /*!
     * \brief setProgressInfomation 设置进度信息
     * \param progress 进度条值
     * \param content 更新内容
     */
    void setProgressInfomation(int progress, const QString &content);

signals:
    void signalBtnClickedForEnsure();
    void signalBtnClickedForCancle();
    void signalAbortFinished();//终止完成
    void signalBtnClickedForUpdateOver(bool isRun);

public slots:
    void slotUpdatePage(int index);

private:
    Ui::HZUpdateUiCtrl *ui;
    Ui::HZStartUpdate *mUStartUi;
    Ui::HZUpdating  *mUMidUi;
    Ui::HZEndUpdate *mUEndUi;
    QFrame *mUStartFrame, *mUEndFrame, *mUMidFrame;
    QStackedWidget *mStackWdt;

    void loadskin();
};

#endif // HZUPDATEUICTRL_H
