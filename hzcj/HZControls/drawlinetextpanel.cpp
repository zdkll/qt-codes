#include "drawlinetextpanel.h"
#include <qfontmetrics.h>
#include <QPainter>
#include <QDebug>

DrawLineTextPanel::DrawLineTextPanel(QWidget *parent):BaseWidget(parent)// : BaseWidget(parent)
{
}

void DrawLineTextPanel::addItem(const DrawLineTextItem &item,bool bUpdate)
{
    if(item.itemName.isEmpty() || indexOfItem(item.itemName) != -1)
        return;
    InnerItem innerItem;
    innerItem.item = item;
    QFontMetrics fm(item.font);
    innerItem.nWidth = fm.boundingRect(item.text).width();
    m_itemList.push_back(innerItem);
    if(bUpdate)
        this->update();
}

void DrawLineTextPanel::addItems(const QVector<DrawLineTextItem> &items, bool bUpdate)
{
    for(int i=0;i < items.size();i++)
    {
        addItem(items[i],false);
    }
    if(bUpdate)
        this->update();
}

void DrawLineTextPanel::remove(const QString &name,bool bUpdate)
{
    int index  = indexOfItem(name);
    if(index != -1)
    {
        m_itemList.erase(m_itemList.begin()+index);
        if(bUpdate)
            update();
    }
}

void DrawLineTextPanel::removeAll(bool bUpdate)
{
    m_itemList.clear();
    if(bUpdate)
        this->update();
}

void DrawLineTextPanel::setItem(const DrawLineTextItem &item,bool bUpdate)
{
    int index  = indexOfItem(item.itemName);
    if(index != -1)
    {
        m_itemList[index].item = item;
        QFontMetrics fm(item.font);
        m_itemList[index].nWidth = fm.boundingRect(item.text).width();
        if(bUpdate)
//            this->update();
            repaint();
    }
}

DrawLineTextItem DrawLineTextPanel::getItem(const QString &name)
{
    DrawLineTextItem item;
    int index  = indexOfItem(name);
    if(index != -1)
    {
        item = m_itemList[index].item;
    }
    return item;
}

int DrawLineTextPanel::indexOfItem(const QString &name)
{
    int  index = -1;
    for(int i =0;i < m_itemList.size();i++)
    {
        if(m_itemList[i].item.itemName == name)
        {
            index = i;
            break;
        }
    }
    return index;
}

void DrawLineTextPanel::paintEvent(QPaintEvent *e)
{
   BaseWidget ::paintEvent(e);
    redraw();
}

void DrawLineTextPanel::resizeEvent(QResizeEvent *event)
{
    BaseWidget::resizeEvent(event);
    update();
}


void DrawLineTextPanel::redraw()
{
    QRect rc(0,0,this->geometry().width(),this->geometry().height());
    QRect tempRc = rc;
    tempRc.setRight(tempRc.left());
    QPainter dc(this);
    for(int i = 0;i < m_itemList.size();i++)
    {
        dc.setFont(m_itemList[i].item.font);
        dc.setPen(m_itemList[i].item.pen);
        tempRc.setLeft(tempRc.right()+m_itemList[i].item.left_padding);
        tempRc.setRight(tempRc.left()+m_itemList[i].nWidth);
        dc.drawText(tempRc,Qt::AlignLeft|Qt::AlignVCenter,m_itemList[i].item.text);
    }
}
