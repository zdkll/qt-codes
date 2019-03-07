#ifndef TIMEDEALDLG_H
#define TIMEDEALDLG_H

#include "hzcomponent_global.h"
#include "framewidget.h"
class AllTimeDealWidget;
class TitleBar;
class QLabel;
//分时成交明细表对话框-自定义无边框
class HZCOMPONENTSHARED_EXPORT AllTimeDealDialog : public FrameLessDialog
{
    Q_OBJECT
public:
    AllTimeDealDialog(QWidget *parent = 0);

    void setStockCode(const QString &stock);
    void afterActive();
    void afterDeactive();

private:
    TitleBar  *createTitleBar();

    TitleBar  *m_titleBar;

    QLabel   *m_titleLabel;

    AllTimeDealWidget *m_timeDealWg;
};


#endif // TIMEDEALDLG_H
