#ifndef PLATEINFODLG_H
#define PLATEINFODLG_H
#include "basewidget.h"
#include "frameless_helper.h"
#include "windowcontrolbox.h"
#include "hzinfocenter.h"

class QTextEdit;
class QLabel;
class PlateInfoDlg:public BaseDialog
{
    Q_OBJECT
public:
    explicit PlateInfoDlg(QWidget *parent = nullptr);

    void updatedata(const TopicInvestDetail& data);

private:
    void createWg();
    QWidget* createTitle();
    QWidget* createBody();

private:
    FramelessHelper *m_pHelper;
    TopicInvestDetail m_deTailData;
    QTextEdit * m_contentLabel;
    QLabel *m_pTitle;
};

#endif // PLATEINFODLG_H
