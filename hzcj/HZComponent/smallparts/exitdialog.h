/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自BaseDialog 退出提示框
*********************************************************************/
#ifndef EXITDIALOG_H
#define EXITDIALOG_H
#include <QBoxLayout>
#include <QDialogButtonBox>
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "frameless_helper.h"
#include "windowcontrolbox.h"
#pragma execution_character_set("utf-8")

class HZCOMPONENTSHARED_EXPORT ExitDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit ExitDialog(QWidget *parent = nullptr);
private:
    void createWg();

    QBoxLayout* createTitleBar();
    QBoxLayout* createBottomWidget();

signals:
    void ignoreRemind();
    void closemainwnd(bool bClose);

private:
    FramelessHelper *m_pHelper;
    WindowControlBox *m_controlBox;
    QDialogButtonBox *m_pButtonBox;
};

#endif // EXITDIALOG_H
