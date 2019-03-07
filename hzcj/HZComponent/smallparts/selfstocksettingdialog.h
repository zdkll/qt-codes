/********************************************************************
created:2019.2.22
author: juntang.xie
purpose:搜索对话框
*********************************************************************/
#ifndef SELFSTOCKSETTINGDIALOG_H
#define SELFSTOCKSETTINGDIALOG_H
#include <QBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QMenu>
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "frameless_helper.h"


struct TableData
{
    QString strFirCol;
    QString strSecCol;
    QString strThrCol;
};

class HZCOMPONENTSHARED_EXPORT SelfStockSettingDialog:public BaseDialog
{
    Q_OBJECT
public:
    explicit SelfStockSettingDialog(QWidget *parent = nullptr);


protected:
    virtual bool eventFilter(QObject *obj, QEvent *eve);


    virtual void
    mousePressEvent(QMouseEvent *event);



private slots:
    void on_moveUp_clicked();

    void on_moveDown_clicked();

    void on_remove_clicked();

    void on_add_clicked();

    void onRowClicked(const QString &strCode);

    void on_submit_clicked();

    void on_cancel_clicked();

private:
    void createWg();

    QBoxLayout *createTitleBar();

    QBoxLayout *createBody();

    void initTableWidget();

    void initMenu();

    void moveRow(QTableWidget *pTable, int currentRow, int toRow);

    void setTextEx(int nCount);

    void swapData(int currentRow, int toRow);


private:
    FramelessHelper *m_pHelper;
    QLabel *m_pInfo;
    QTableWidget* m_tableWidget;
    QMenu *m_menu;
    QVector<TableData> m_dataVec;
    QVector<TableData> m_oldVec;
    QStringList m_addList;
    QFont m_firFont;
    QFont m_SecFont;
};

#endif // SELFSTOCKSETTINGDIALOG_H
