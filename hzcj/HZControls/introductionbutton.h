/********************************************************************
created:
author: dk.zhang
purpose: 介绍标题按钮，通过点击提示图标显示对应功能的介绍
*********************************************************************/
#ifndef INTRODUCTIONBUTTON_H
#define INTRODUCTIONBUTTON_H

#include <basewidget.h>

#include "hzcontrols_global.h"

class QLabel;
class QPushButton;
class  HZCONTROLSSHARED_EXPORT IntroductionButton : public  BaseWidget
{
    Q_OBJECT
public:
    IntroductionButton(const QString &hintString = QString(),QWidget *parent = 0);

    void setHintString(const QString &hintString);

signals:
    void showIntroduction();

private:
    void createWg();

    QString m_hintString;
    QLabel          *m_hintLabel;
    QPushButton *m_button;
};

#endif // INTRODUCTIONBUTTON_H
