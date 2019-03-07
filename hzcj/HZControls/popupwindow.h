#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H
#include <QWidget>
#include <QVBoxLayout>
#include "hzcontrols_global.h"
#include "hzobject.h"
#include "frameless_helper.h"

class TitleBar;
class HZCONTROLSSHARED_EXPORT PopupWindow : public QWidget,public HZObject
{
     Q_OBJECT
public:
     explicit PopupWindow(QWidget* parent=nullptr,QString instName="");
    ~PopupWindow();

    void setWidget(QWidget *wg);

protected:
     void createWg();

    TitleBar *m_pTitleBar;
    FramelessHelper *m_pHelper;
    QVBoxLayout *m_pLayout;
};

#endif // POPUPWINDOW_H
