#ifndef PopupDialog_H
#define PopupDialog_H
#include <QDialog>
#include <QVBoxLayout>
#include "hzcontrols_global.h"
#include "hzobject.h"
#include "frameless_helper.h"
#include "basewidget.h"

class QLabel;
class WindowControlBox;
class TitleBar;
class HZCONTROLSSHARED_EXPORT PopupDialog : public QDialog,public HZObject
{
     Q_OBJECT
public:
     explicit PopupDialog(QWidget *parent = nullptr, QString instName="");
    ~PopupDialog();

    void setWidget(QWidget *wg);


signals:
    void popWndClose();
protected:
    void createWg();

    virtual void mouseMoveEvent(QMouseEvent *event);

    TitleBar *m_pTitleBar;
    FramelessHelper *m_pHelper;
    QVBoxLayout *m_pLayout;

    QLabel                     *m_pIconLabel;
    QLabel                     *m_pTitleLabel;
    WindowControlBox *controlBox_;
};

#endif // PopupDialog_H
