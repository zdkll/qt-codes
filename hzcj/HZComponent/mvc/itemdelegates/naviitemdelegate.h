/********************************************************************
created:
author: dk.zhang
purpose:导航栏列表样式代理
*********************************************************************/
#ifndef NAVIITEMDELEGATE_H
#define NAVIITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QColor>
#include <QFont>

#include "mvcpublicdef.h"
#include "styleditemdelegate.h"

class NaviItemDelegate : public NoFocusStyledItemDelegate
{
public:
    NaviItemDelegate(QObject *parent = 0);

    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    static bool isInitedStyleProperties(){return m_isInitedStyleProperties;}
    static void initStyleProperties(const StyleProperties &styledProperties);

protected:
  static  QFont  m_charFont,m_digitalFont1,m_digitalFont2;

  static  QColor m_riseColor,m_fallColor,m_plainColor,m_numberColor,m_textColor;
  static bool     m_isInitedStyleProperties;
  const int       m_marginLeft,m_marginRight;
};

#endif // NAVIITEMDELEGATE_H
