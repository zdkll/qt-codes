#ifndef KLINEXAXIS_H
#define KLINEXAXIS_H
#include <QObject>
#include <QRectF>




class KlineWidget;
class DataKLine;
class KlineXaxis:public QObject
{
    Q_OBJECT
public:
    explicit KlineXaxis(QObject *parent = nullptr);

//    //计算单位数据数量对应宽度
//    void calc();

    //x轴坐标转化为对应数据下标
    int xPtxToIndex(int pt_x);

    //数据下表转化成对应x轴坐标
    int xIndexToPtx(int index);

    //窗口大小改变
    void resize(QRectF rc);

    //设置每根K线宽度
    void setSingleAreaWidth(float &width);


private:
//    int m_nItemWidth = 1;
//    int m_nTotalCount = 1;
    KlineWidget *m_parentWnd;
    DataKLine *m_data;
    QRectF m_rc;
    float m_singleAreaWidth;
};

#endif // KLINEXAXIS_H
