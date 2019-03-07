/********************************************************************
created:2019.2
author: jdxchina
purpose: 指标菜单栏
*********************************************************************/
#ifndef INDMENU_H
#define INDMENU_H

#include <QWidget>
#include <QLabel>
#include <QWidgetAction>
#include <QMenu>

namespace Ui {
class IndMenu;
}


class IndItem : public QLabel
{
    Q_OBJECT
public:
    enum{NormalInd, StaticInd, SpacialInd};
    static QVector<QString> m_items;

    Q_PROPERTY(QColor selClr READ selClr WRITE setSelClr)
    Q_PROPERTY(QColor norClr READ norClr WRITE setNorClr)
    Q_PROPERTY(QColor borderClr READ borderClr WRITE setBorderClr)

    IndItem(QWidget *parent=nullptr):QLabel(parent)
    {
        setObjectName("IndItem");
    }

    ~IndItem(){}

    void init();

    QColor selClr() const { return m_selClr;}
    void setSelClr(const QColor &clr){m_selClr = clr;}

    QColor norClr() const {return m_norClr;}
    void setNorClr(const QColor &clr) {m_norClr = clr;}

    QColor borderClr() const { return m_borderClr;}
    void setBorderClr(const QColor &clr) { m_borderClr = clr;}

signals:
    void signalTextClicked(int type, int itemIndex, const QString &text, const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *event);

private:
    QPoint m_msPos;
    QString m_selFlags;
    bool m_repaint{false};
    bool m_isComplete{true};
    QColor m_selClr/*{"#449bb2"}*/;
    QColor m_norClr/*{"#adadad"}*/;
    QColor m_borderClr{"#0f1317"};

    void flagsFromPosition()  ;
};

class IndMenuMaster : public QWidget
{
    Q_OBJECT

public:
    explicit IndMenuMaster(QWidget *parent = 0);
    ~IndMenuMaster();

    /**
     * @brief addItem 加入文本项
     * @param item
     */
    void addItem(const QString &item){ IndItem::m_items.append(item);}

    /**
     * @brief addItem 加入文本项
     * @param item
     */
    void addItem(const QStringList &item)
    {
        for(int i=0; i<item.count(); ++i) {
            IndItem::m_items.append(item[i]);
        }
    }

    /**
     * @brief insertItem 加入文本项
     * @param index
     * @param item
     */
    void insertItem(int index, const QString &item)
    {
        if(item.count() > index) {
            IndItem::m_items.insert(index, item);
        } else {
            IndItem::m_items.append(item);
        }
    }

    /**
     * @brief removeItem 移除文本项
     * @param item
     */
    void removeItem(const QString &item)
    {
        if(IndItem::m_items.contains(item)) {
            IndItem::m_items.removeAll(item);
        }
    }

signals:
    /**
     * @brief signalItemClicked 文本项点击后被触发
     * @param text
     */
    void signalItemClicked(const QString &text);

private:
    Ui::IndMenu *ui;

    IndItem *m_indItem;
    QMenu *m_indMenu{nullptr};

    void loadskin();
};

#endif // INDMENU_H
