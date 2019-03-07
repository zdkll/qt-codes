#ifndef MINLINEHISDLG_H
#define MINLINEHISDLG_H
/********************************************************************
created:
author: 谢
purpose: 历史分时
*********************************************************************/
#include "popupdialog.h"
#include "popupwindow.h"
#include "mincanvas.h"

/**
 * @brief 分时缩略图类
 */
class MinHisCanvas: public MinCanvas
{
    Q_OBJECT
public:
    explicit MinHisCanvas(QWidget *parent = 0):MinCanvas(parent){}
};

class MinLineHisWidget:public PopupWindow
{
    Q_OBJECT
public:
    explicit MinLineHisWidget(QWidget *parent = nullptr, QString instname = "");
    //设置股票，名称，历史分时的时间
    void setObject(const QString& strCode, const QString& strName,qint64 nTime,qint64 reqTime);
private:
    void createWg();
    void showMsg();
private:
    MinHisCanvas* m_pMline;
};
#endif // MINLINEHISDLG_H
