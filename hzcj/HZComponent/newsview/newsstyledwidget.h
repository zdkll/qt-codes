/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自BaseWidget 自定义样式属性窗体
*********************************************************************/
#ifndef NEWSSTYLEDWIDGET_H
#define NEWSSTYLEDWIDGET_H
#include "basewidget.h"


class NewsStyledWidget:public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor titileColor READ titileColor WRITE setTitileColor DESIGNABLE true)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)
    Q_PROPERTY(QString titleFontFamily READ titleFontFamily WRITE setTitleFontFamily DESIGNABLE true )
    Q_PROPERTY(int titlePixelSize READ titlePixelSize WRITE setTitlePixelSize DESIGNABLE true)
    Q_PROPERTY(int textPixelSize READ textPixelSize WRITE setTextPixelSize DESIGNABLE true)
    Q_PROPERTY(QString labelFontFamily READ labelFontFamily WRITE setLabelFontFamily DESIGNABLE true)
    Q_PROPERTY(int labelPixelSize READ labelPixelSize WRITE setLabelPixelSize DESIGNABLE true)
    Q_PROPERTY(QColor lhColor READ lhColor WRITE setLhColor DESIGNABLE true)
    Q_PROPERTY(QColor lkColor READ lkColor WRITE setLkColor DESIGNABLE true)
    Q_PROPERTY(QColor qllhColor READ qllhColor WRITE setQllhColor DESIGNABLE true)
    Q_PROPERTY(QColor qllkColor READ qllkColor WRITE setQllkColor DESIGNABLE true)
    Q_PROPERTY(QColor zxColor READ zxColor WRITE setZxColor DESIGNABLE true)
    Q_PROPERTY(QPixmap lhPixmap READ lhPixmap WRITE setLhPixmap DESIGNABLE true)
    Q_PROPERTY(QPixmap lkPixmap READ lkPixmap WRITE setLkPixmap DESIGNABLE true)
    Q_PROPERTY(QPixmap qllhPixmap READ qllhPixmap WRITE setQllhPixmap DESIGNABLE true)
    Q_PROPERTY(QPixmap qllkPixmap READ qllkPixmap WRITE setQllkPixmap DESIGNABLE true)
    Q_PROPERTY(QPixmap zxPixmap READ zxPixmap WRITE setZxPixmap DESIGNABLE true)

public:
    explicit NewsStyledWidget(QWidget *parent = nullptr);
    //标题颜色
    QColor titileColor() const { return m_titileColor;}
    void setTitileColor(const QColor &color){m_titileColor = color;}
    //内容颜色
    QColor textColor() const { return m_textColor;}
    void setTextColor(const QColor &color){m_textColor = color;}
    //设置标题字体
    QString titleFontFamily() const{return m_titleFontFamily;}
    void setTitleFontFamily(const QString &FontFamily){m_titleFontFamily = FontFamily;}

    //设置标题字体大小
    int titlePixelSize() const {return m_titlePixelSize;}
    void setTitlePixelSize(const int &pixelSize){m_titlePixelSize = pixelSize;}

    //设置内容字体大小
    int textPixelSize() const {return m_textPixelSize;}
    void setTextPixelSize(const int &pixelSize){m_textPixelSize = pixelSize;}

    //设置标签字体
    QString labelFontFamily() const{return m_labelFontFamily;}
    void setLabelFontFamily(const QString &FontFamily){m_labelFontFamily = FontFamily;}

    //设置标签字体大小
    int labelPixelSize() const {return m_labelPixelSize;}
    void setLabelPixelSize(const int &pixelSize){m_labelPixelSize = pixelSize;}

    //利好颜色
    QColor lhColor() const { return m_lhColor;}
    void setLhColor(const QColor &color){m_lhColor = color;}

    //利空颜色
    QColor lkColor() const { return m_lkColor;}
    void setLkColor(const QColor &color){m_lkColor = color;}

    //强烈利好颜色
    QColor qllhColor() const { return m_qllhColor;}
    void setQllhColor(const QColor &color){m_qllhColor = color;}

    //强烈利空颜色
    QColor qllkColor() const { return m_qllkColor;}
    void setQllkColor(const QColor &color){m_qllkColor = color;}

    //中性颜色
    QColor zxColor() const { return m_zxColor;}
    void setZxColor(const QColor &color){m_zxColor = color;}
    //利好图片
    QPixmap lhPixmap() const {return m_lhPixmap;}
    void setLhPixmap(const QPixmap &pixmap){m_lhPixmap = pixmap;}

    //利空图片
    QPixmap lkPixmap() const {return m_lkPixmap;}
    void setLkPixmap(const QPixmap &pixmap){m_lkPixmap = pixmap;}

    //强烈利好图片
    QPixmap qllhPixmap() const {return m_qllhPixmap;}
    void setQllhPixmap(const QPixmap &pixmap){m_qllhPixmap = pixmap;}

    //强烈利空图片
    QPixmap qllkPixmap() const {return m_qllkPixmap;}
    void setQllkPixmap(const QPixmap &pixmap){m_qllkPixmap = pixmap;}

    //中性图片
    QPixmap zxPixmap() const {return m_zxPixmap;}
    void setZxPixmap(const QPixmap &pixmap){m_zxPixmap = pixmap;}

private:
    QColor m_titileColor;
    QColor m_textColor;
    QString m_titleFontFamily;
    int m_titlePixelSize;
    int m_textPixelSize;
    QString m_labelFontFamily;
    int m_labelPixelSize;
    QColor m_lhColor;
    QColor m_lkColor;
    QColor m_qllhColor;
    QColor m_qllkColor;
    QColor m_zxColor;
    QPixmap m_lhPixmap;
    QPixmap m_lkPixmap;
    QPixmap m_qllhPixmap;
    QPixmap m_qllkPixmap;
    QPixmap m_zxPixmap;

};

#endif // NEWSSTYLEDWIDGET_H
