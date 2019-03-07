/********************************************************************
created:2018.9
author: juntang.xie
purpose: 右侧盘口样式类
*********************************************************************/
#ifndef HANDICAPSTYLEDWIDGET_H
#define HANDICAPSTYLEDWIDGET_H
#include <QWidget>


class HandicapStyledWidget:public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor DESIGNABLE true)
    Q_PROPERTY(QColor downColor READ downColor WRITE setDownColor DESIGNABLE true)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)
    Q_PROPERTY(QString textFamily READ textFamily WRITE setTextFamily DESIGNABLE true)
    Q_PROPERTY(int textPixelSize READ textPixelSize WRITE setTextPixelSize DESIGNABLE true)
    Q_PROPERTY(bool textBold READ textBold WRITE setTextBold DESIGNABLE true)

    Q_PROPERTY(QColor risePriceColor READ risePriceColor WRITE setRisePriceColor DESIGNABLE true)
    Q_PROPERTY(QColor downPriceColor READ downPriceColor WRITE setDownPriceColor DESIGNABLE true)
    Q_PROPERTY(QColor volumeColor READ volumeColor WRITE setVolumeColor DESIGNABLE true)
public:
    explicit HandicapStyledWidget(QWidget *parent = nullptr);

    //上涨颜色
    QColor riseColor()const {return m_riseColor;}
    void setRiseColor(const QColor &color){m_riseColor = color;}

    //下跌颜色
    QColor downColor()const {return m_downColor;}
    void setDownColor(const QColor &color){m_downColor = color;}

    //字体颜色
    QColor textColor()const {return m_textColor;}
    void setTextColor(const QColor &color){m_textColor = color;}

    //设置字体
    QString textFamily()const {return m_textFamily;}
    void setTextFamily(const QString &fontFamily) { m_textFamily = fontFamily;}

    //设置字体大小
    int textPixelSize()const {return m_textPixelSize;}
    void setTextPixelSize(const int &pixelSize){m_textPixelSize = pixelSize;}

    //设置字体粗细
    int textBold()const {return m_textBold;}
    void setTextBold(const bool &bold){m_textBold = bold;}

    //五档买价颜色
    QColor risePriceColor()const {return m_risePriceColor;}
    void setRisePriceColor(const QColor &color){m_risePriceColor = color;}

    //五档卖价颜色
    QColor downPriceColor()const {return m_downPriceColor;}
    void setDownPriceColor(const QColor &color){m_downPriceColor = color;}

    //五档成交量颜色
    QColor volumeColor()const {return m_volumeColor;}
    void setVolumeColor(const QColor &color){m_volumeColor = color;}

private:
    QColor m_riseColor;
    QColor m_downColor;
    QColor m_textColor;
    QString m_textFamily;
    int m_textPixelSize;
    bool m_textBold;

    QColor m_risePriceColor;
    QColor m_downPriceColor;
    QColor m_volumeColor;
};

#endif // HANDICAPSTYLEDWIDGET_H
