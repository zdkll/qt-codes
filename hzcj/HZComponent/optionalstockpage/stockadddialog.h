/********************************************************************
created: 2018.9.28
author: juntang.xie
purpose: 股票添加对话框
*********************************************************************/
#ifndef STOCKADDDIALOG_H
#define STOCKADDDIALOG_H
#include "basewidget.h"
#include "frameless_helper.h"
#include "windowcontrolbox.h"
#include "hzcomponent_global.h"
#pragma execution_character_set("utf-8")

class HZCOMPONENTSHARED_EXPORT StockAddDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit StockAddDialog(QWidget *parent = nullptr);
    ~StockAddDialog();
private:
    void createWg();
    QWidget* createTitle();
    QWidget* createBody();
signals:
    void finished();
private:
    FramelessHelper *m_pHelper;
    WindowControlBox *m_controlBox;
    bool m_bState;
};

#endif // STOCKADDDIALOG_H
