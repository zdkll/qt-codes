#ifndef CROSSINFODLG_H
#define CROSSINFODLG_H

#include <QDialog>
#include <QWidget>
#include "hzcontrols_global.h"

class QStackedWidget;
class QWidget;
class QListView;
class QVBoxLayout;
class QLabel;
class HZCONTROLSSHARED_EXPORT VBoxWidget : public QWidget
{
public:
    VBoxWidget(QWidget *parent = 0);

    void addLabel();
    void addLabel(const QString &text);
    void addValLabel();
    void addValLabel(const QString &text);

    void setValblText(const int &index,const QString &valStr);

private:
    QVBoxLayout          *layout;
    QVector<QLabel *>  m_labels;
};
class HZCONTROLSSHARED_EXPORT CrossInfoDlg : public QDialog
{
public:
    CrossInfoDlg(QWidget *parent = 0);

    VBoxWidget *widgetAt(const int &index);

    void setCurrentIndex(const int &index);
protected:
    //调整位置，根据父窗口
    void adjustPosition();
private:
    void createDlg();

private:
    QStackedWidget *m_statckWg;
    QWidget              *m_parentWg;

};

#endif // CROSSINFODLG_H
