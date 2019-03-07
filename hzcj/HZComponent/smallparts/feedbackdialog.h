/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自BaseDialog 意见反馈界面
*********************************************************************/
#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H
#include <QBoxLayout>
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "windowcontrolbox.h"
#include "frameless_helper.h"
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#pragma execution_character_set("utf-8")


class HZCOMPONENTSHARED_EXPORT FeedbackDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit FeedbackDialog(QWidget *parent = nullptr);

private:
    void createWg();
    void getUserInfo();

private:
    WindowControlBox *m_controlBox;
    QVBoxLayout *m_mainLayout;
    FramelessHelper *m_pHelper;

    QTextEdit *m_ptextEdit;
    QLineEdit *m_plineEdit;
    QLabel *m_promptLabel;

private slots:
    void onSubmitBtnClicked();
};

#endif // FEEDBACKDIALOG_H
