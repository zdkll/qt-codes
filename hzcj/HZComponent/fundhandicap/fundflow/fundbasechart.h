#ifndef BASECHART_H
#define BASECHART_H

#include <QWidget>
#include "fundflowdef.h"

class FundBaseChart : public QWidget
{
    Q_OBJECT
public:
    explicit FundBaseChart(QWidget *parent = 0);
    virtual ~FundBaseChart(){}

    void draw();
    void reDraw();

protected:
  void  paintEvent(QPaintEvent *e);

  virtual void drawPixmap(QPixmap *pix) = 0;

   virtual void paintOthers(QPainter *){}

protected:
    QSize                 m_size;
    PaintAction        m_paintAction;
    QPixmap           m_pixmap;
};

#endif // BASECHART_H
