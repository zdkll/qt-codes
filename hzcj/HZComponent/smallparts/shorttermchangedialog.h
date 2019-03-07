/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自BaseDialog 短线异动界面
*********************************************************************/
#ifndef SHORTTERMCHANGEDIALOG_H
#define SHORTTERMCHANGEDIALOG_H
#include <QBoxLayout>
#include "hzcomponent_global.h"
#include "basewidget.h"
#include "frameless_helper.h"
#include "windowcontrolbox.h"

#pragma execution_character_set("utf-8")

class MinCanvas;
class HZCOMPONENTSHARED_EXPORT ShortTermChangeDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit ShortTermChangeDialog(QWidget *parent = nullptr);

private:
    void createWg();

    QWidget* createTitleBar();
    QWidget* createLeftWidget();
    QWidget* createRightWidget();

    virtual void showEvent(QShowEvent * evnet);
    virtual void hideEvent(QHideEvent *event);
private slots:
    void onRowClicked(QString strCode);
private:
    MinCanvas* m_minChart;
    FramelessHelper *m_pHelper;
    WindowControlBox *m_controlBox;
    QString m_strCode;
};

#endif // SHORTTERMCHANGEDIALOG_H
