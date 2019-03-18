/********************************************************************
created:2019/03/15
author: dk.zhang
purpose:画笔样式设置相关控件
*********************************************************************/
#ifndef PENSTYLEBUTTONS_H
#define PENSTYLEBUTTONS_H

#include <QToolButton>

//颜色选择按钮
class QLabel;
//ColorToolButton
class ColorToolButton : public QToolButton
{
    Q_OBJECT
public:
    ColorToolButton(QWidget *parent = 0);

    void   setColor(const QColor& color);
    inline QColor color(){return m_color;}

protected slots:
    void colorClicked(bool checked);
    void updateIcon();

protected:
    void resizeEvent(QResizeEvent *e);

signals:
    void colorChanged(const QColor& color);

private:
    QColor   m_color;
    QLabel *m_label;
};

//画笔设置-宽度和线型
class QButtonGroup;
class PenToolButton : public QToolButton
{
    Q_OBJECT
public:
    PenToolButton(QWidget *parent = Q_NULLPTR);
    PenToolButton(const QSize& iconSize,QWidget *parent = Q_NULLPTR);

protected slots:
    void buttonClicked(int);

protected:
    virtual void clickedItem(int ){}

    QButtonGroup *m_btnGroup;
    QMenu             *m_menu;
};

//画笔设置-宽度
class PenWidthToolButton : public PenToolButton
{
    Q_OBJECT
public:
    PenWidthToolButton(QWidget *parent = Q_NULLPTR);

    void setPenWidth(int width);
    int   penWidth()const {return m_penWidth;}

protected:
    virtual void clickedItem(int );

signals:
    void penWidthChanged(int width);

private:
    void addPenWidthItems();

private:
    QList<int>       m_lineWidths;
    int                   m_penWidth;
};

//画笔设置-线型
class PenStyleToolButton : public PenToolButton
{
    Q_OBJECT
public:
    PenStyleToolButton(QWidget *parent = Q_NULLPTR);

    void setPenStyle( Qt::PenStyle penStyle);
    Qt::PenStyle penStyle()const {return m_penStyle;}
protected:
    virtual void clickedItem(int id);

signals:
    void penStyleChanged(const Qt::PenStyle& penStyle);

private:
    void addPenStyleItems();

private:
    QList<Qt::PenStyle> m_penStyles;

    Qt::PenStyle   m_penStyle;
};


#endif // PENSTYLEBUTTONS_H
