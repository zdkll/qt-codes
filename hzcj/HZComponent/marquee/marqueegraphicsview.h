#ifndef MARQUEEGRAPHICSVIEW_H
#define MARQUEEGRAPHICSVIEW_H
/********************************************************************
created:
author: min.cheng
purpose: 跑马灯视图
*********************************************************************/
#include <QGraphicsView>
#include "componentdef.h"
class MarqueePanel;
class MarqueeGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(QString m_fontFamily READ getFontFamily WRITE setFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_fontSize READ getFontSize WRITE setFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_selectedTextClr READ getSelectedTextClr WRITE setSelectedTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_normalTextClr READ getNormalTextClr WRITE setNormalTextClr DESIGNABLE true)
public:
    //文本字体
    QString getFontFamily() const{return m_fontFamily;}
    void setFontFamily(QString family){m_fontFamily=family;}
    QString m_fontFamily = "Arial";

    //文本大小
    int getFontSize() const{return m_fontSize;}
    void setFontSize(int size){m_fontSize=size;}
    int m_fontSize = 11;

    //选中颜色
    QColor getSelectedTextClr() const{return m_selectedTextClr;}
    void setSelectedTextClr(QColor color){m_selectedTextClr=color;}
    QColor m_selectedTextClr = QColor(Qt::red);

    //正常文本颜色
    QColor getNormalTextClr() const{return m_normalTextClr;}
    void setNormalTextClr(QColor color){m_normalTextClr=color;}
    QColor m_normalTextClr = QColor(Qt::red);
public:
    MarqueeGraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
    void clickItem(const QString& id);
protected:
    virtual void enterEvent(QEvent *event);

    virtual void leaveEvent(QEvent *event);

    //virtual void paintEvent(QPaintEvent *e);
private:
    MarqueePanel* m_panel;
};

#endif // MARQUEEGRAPHICSVIEW_H
