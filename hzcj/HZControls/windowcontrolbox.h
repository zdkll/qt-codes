#ifndef WINDOWCONTROLBOX_H
#define WINDOWCONTROLBOX_H

#include <QWidget>
#include "hzcontrols_global.h"
#include "basewidget.h"

namespace Ui {
class WindowControlBox;
}
class HZCONTROLSSHARED_EXPORT WindowControlBox : public BaseWidget
{
    Q_OBJECT
public:
    enum ControlFlag
    {
        WindowMinimizeButton = 0x01,
        WindowMaximizeButton = 0x02,
        WindowMinMaxButtons =  WindowMinimizeButton|WindowMaximizeButton,
        WindowCloseButton      = 0x08,
        WindowDefaultFlags      = WindowMinMaxButtons|WindowCloseButton
    };
    Q_DECLARE_FLAGS(ControlFlags,ControlFlag)

    explicit WindowControlBox(QWidget* parent,QWidget* controlWindow,bool showMinButton = true,bool showMaxButton = true,QString instName="");
    virtual ~WindowControlBox();
    void SwitchExpandOrNarrow();

    void setControlFlag(const ControlFlag &controlFlag,bool enable = true);
    void setControlFlags(const ControlFlags &controlFlags);

    void OnMinimized();

    void OnMaximinzed();

    void onRestoreWindow();

    void updateMaximize(bool isMaximized);

    virtual void OnClose();
signals:
    void mainWndMinimized();
    void mainWndMaximized();
    void mainWndClose();
public slots:

private:
//    void OnClose();
    void SwitchExpandOrNarrow(bool isMaximized);

    void updateControlFlags();
private:
    bool isMaximized_;
    bool showMinButton_;      //是否显示最小化按钮
    bool showMaxButton_;      //是否显示最大化按钮
    QWidget* controlWindow_;  //传入可被控制的Window,如果未传入，则会激活信号
    Ui::WindowControlBox* ui;

    ControlFlags m_controlFlags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WindowControlBox::ControlFlags)
#endif // WINDOWCONTROLBOX_H
