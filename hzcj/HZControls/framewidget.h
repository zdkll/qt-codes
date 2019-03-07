#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include "hzcontrols_global.h"
#include "basewidget.h"
#include  "titlebar.h"

#include "frameless_helper.h"

class  QVBoxLayout;
class HZCONTROLSSHARED_EXPORT FrameWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit FrameWidget(BaseWidget *parent = 0,QString instName="");
    virtual void create();
    ~FrameWidget();

signals:

public slots:

protected:
    FramelessHelper *m_pHelper;
    TitleBar                 *m_pTitleBar;
    QVBoxLayout      *m_pLayout;
};


class HZCONTROLSSHARED_EXPORT FrameLessWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit FrameLessWidget(BaseWidget *parent = 0,QString instName="");
    virtual void createWg(){}
    ~FrameLessWidget();

signals:

public slots:

protected:
    FramelessHelper *m_pHelper;

};

class HZCONTROLSSHARED_EXPORT FrameLessDialog : public BaseDialog
{
    Q_OBJECT
public:
    explicit FrameLessDialog(QWidget *parent = 0,QString instName="");

    ~FrameLessDialog();

signals:

public slots:

protected:
    FramelessHelper *m_pHelper;

};

#endif // FRAMEWIDGET_H
