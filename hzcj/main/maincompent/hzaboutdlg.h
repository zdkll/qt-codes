/********************************************************************
created: 2018,8,22
author: xuzhehu
purpose: 关于对话框
*********************************************************************/

#ifndef HZABOUTDLG_H
#define HZABOUTDLG_H

#include <QDialog>

namespace Ui {
class HZAboutDlg;
}

class HZAboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HZAboutDlg(QWidget *parent = 0);
    ~HZAboutDlg();

    /**
     * @brief setMsg 设置消息
     * @param variant
     */
    void setMsg(const QVariant &variant);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::HZAboutDlg *ui;
    bool m_dragWindow;
    QPoint m_mousePoint;

    void loadSkin();
};

#endif // HZABOUTDLG_H
