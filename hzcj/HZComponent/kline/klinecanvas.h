#ifndef KLINECANVAS_H
#define KLINECANVAS_H
/********************************************************************
created:
author: min.cheng
purpose: K线窗口
*********************************************************************/
#include <QWidget>
#include <map>
#include <QVector>
#include "basecanvas.h"
#include "chartdef.h"

class KlineChart;
class KlineCanvas : public BaseCanvas
{
    Q_OBJECT
    Q_PROPERTY(QString m_maxMinPriceFontFamily READ getMaxMinPriceFontFamily WRITE setMaxMinPriceFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_maxMinPriceFontSize READ getMaxMinPriceFontSize WRITE setMaxMinPriceFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_maxMinPriceTextClr READ getMaxMinPriceTextClr WRITE setMaxMinPriceTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_maxMinPriceLineClr READ getMaxMinPriceLineClr WRITE setMaxMinPriceLineClr DESIGNABLE true)
public:
    //最高/最低价格字体样式
    QString getMaxMinPriceFontFamily() const{return m_maxMinPriceFontFamily;}
    void setMaxMinPriceFontFamily(QString family){m_maxMinPriceFontFamily=family;}
    QString m_maxMinPriceFontFamily = "Arial";

    //最高/最低价格字体高度
    int getMaxMinPriceFontSize() const{return m_maxMinPriceFontSize;}
    void setMaxMinPriceFontSize(int size){m_maxMinPriceFontSize=size;}
    int m_maxMinPriceFontSize = 11;

    //最高/最低价格文本颜色
    QColor getMaxMinPriceTextClr() const{return m_maxMinPriceTextClr;}
    void setMaxMinPriceTextClr(QColor color){m_maxMinPriceTextClr=color;}
    QColor m_maxMinPriceTextClr = QColor(Qt::red);

    //最高/最低价格箭头颜色
    QColor getMaxMinPriceLineClr() const{return m_maxMinPriceLineClr;}
    void setMaxMinPriceLineClr(QColor color){m_maxMinPriceLineClr=color;}
    QColor m_maxMinPriceLineClr = QColor(Qt::blue);

    explicit KlineCanvas(QWidget *parent = 0);

    //设置股票和周期 不立即切換
    void setBaseInfo(const QString& obj,int period);

    //切換股票和週期
    void switchStock(const QString& obj,int period);

    //系统事件
    virtual void keyPressEvent(QKeyEvent *event);

    //创建十字小窗
    ChartCrossDlg* createCrossDlg();

    void setAttachIndi(int indi);

    int getAttachIndi();

    void switchTemplate(const QVector<ChartItemInfo>& attachIndiList,int mainIndi = -1);

    void removeSrvIndi();
signals:
public slots:
    //处理周期变化
    void switchPeriod(int period);
    //处理主图指标变化
    void switchMainIndicator(int id);
    //处理附图指标变化
    void switchAttachIndicator(int id);
protected slots:

protected:
    //处理系统事件
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void OnCreateEx();
    virtual void paintEvent(QPaintEvent *event);
    void getIndiIdList(int& mainId,std::vector<int>& attachIndiList);
protected:
    KlineChart* getKlineChart();
    void postCrossWndInfo(bool bKeyEvent);
    QString getBaseCmd(const QString& obj,int period,int mainIndiId = INT_MAX);
    QString getIndiCmd(int id);
protected:
    bool m_press;
    QString m_obj;
    int m_period;
    IndiIndexMap m_indiMap;
    int m_mainIndiId;
    //汇正特色指标
    QVector<int> m_templateIdList;
};

#endif // KLINECANVAS_H
