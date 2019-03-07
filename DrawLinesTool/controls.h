#ifndef CONTROLS_H
#define CONTROLS_H

#include <QComboBox>
#include <QToolButton>

class Controls
{
public:
    Controls();
};


class QLabel;
//自定义颜色选择ComboBox
class ColorComboBox : public QComboBox
{
    Q_OBJECT
public:
    ColorComboBox(QWidget *parent = 0);

    void setColor(const QColor& color);
    QColor color(){return m_color;}

    virtual void showPopup();
signals:
    void colorChanged(const QColor& color);

private:
    QColor   m_color;
    QLabel *m_label;
};

//PenStyle 设置ToolButton
class QButtonGroup;
class PenStyleToolButton : public QToolButton
{
    Q_OBJECT
public:
    enum PenStyleType
    {
        PenWidth = 0,//画笔宽度
        PenStyle   = 1,//画笔样式
    };
    PenStyleToolButton(PenStyleType type,QWidget *parent = Q_NULLPTR);

    void setPenWidth(int width);
    int penWidth()const {return m_penWidth;}

    void setPenStyle( Qt::PenStyle penStyle);
    Qt::PenStyle penStyle()const {return m_penStyle;}

signals:
    void penStyleChanged(const Qt::PenStyle& penStyle);
    void penWidthChanged(int width);

protected slots:
    void buttonClicked(int);

private:
    void initToolButton();
    void addPenWidthItems();
    void addPenStyleItems();

    PenStyleType    m_penStyleType;
    QButtonGroup *m_btnGroup;
    QMenu             *m_menu;

    QList<int>       m_lineWidths;
    QList<Qt::PenStyle> m_penStyles;
    int                   m_penWidth;
    Qt::PenStyle   m_penStyle;
};

#endif // CONTROLS_H
