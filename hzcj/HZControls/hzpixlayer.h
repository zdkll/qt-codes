#ifndef HZPIXLAYER_H
#define HZPIXLAYER_H

#include "hzcontrols_global.h"
#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include <basewidget.h>


namespace Ui {
class HZPixLayer;
}


class HZPixTopLayer : public QLabel
{
    Q_OBJECT
public:
    HZPixTopLayer(const QString &cr, QWidget* parent);


protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap mPix;
};


class HZCONTROLSSHARED_EXPORT HZPixLayer : public BaseWidget
{
    Q_OBJECT

public:
    /**
     * @brief HZPixLayer
     * @param bk 背景色url
     * @param cr 情景色url
     * @param parent
     */
    explicit HZPixLayer(const QString &bk, const QString &cr, QWidget *parent = 0);
    ~HZPixLayer();

    void afterActive();

    void paintEvent(QPaintEvent *event);
private:
    Ui::HZPixLayer *ui;
    HZPixTopLayer *mLabel;
    QPixmap mPix;

    void loadskin();
};

#endif // HZPIXLAYER_H
