#ifndef KLINEYAXIS_H
#define KLINEYAXIS_H
#include <QObject>
#include <QRectF>

class KlineWidget;
class DataKline;
class KlineYaxis:public QObject
{
    Q_OBJECT
public:
    explicit KlineYaxis(QObject *parent = nullptr);

    //计算比值
    void calc();

    //窗口大小改变调用
    void resize(QRectF rc);

    //y轴坐标到对应价格值的转化
    float yPtToValue(float pt_y);

    //价格到对应y轴坐标的转化
    float yValueToPt(float value);

private:
    KlineWidget *m_parentWnd; //父窗口指针
    QRectF m_rc;

    float m_maxValue = DBL_MAX;
    float m_minValue = DBL_MAX;
    float m_ratio = 1.0f;
};

#endif // KLINEYAXIS_H
