/********************************************************************
created:2018.11/26
author: juntang.xie
purpose: 自定义菜单按钮类
*********************************************************************/
#ifndef QListButton_H
#define QListButton_H
#include <QPushButton>


class QListButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)

public:
    QListButton(QWidget *parent = nullptr);

    QColor textColor()const{return m_textColor;}

    void setTextColor(const QColor &color){m_textColor = color;}


    void updateText(const QString &strText, bool bChecked = false);

protected:

    virtual void
    paintEvent(QPaintEvent *event);



private:
    QString m_strText;

    QColor m_textColor;

    QPixmap m_pix;
};

#endif // QListButton_H
