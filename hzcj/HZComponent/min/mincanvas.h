#ifndef MINCANVAS_H
#define MINCANVAS_H
/********************************************************************
created:
author: min.cheng
purpose: 分时窗口类
*********************************************************************/
#include "basecanvas.h"
#include "chartdef.h"

class Minchart;
class MinCanvas: public BaseCanvas
{
    Q_PROPERTY(QColor m_minChartBottomLineClr READ getMinChartBottomLineClr WRITE setMinChartBottomLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_minLineClr READ getMinLineClr WRITE setMinLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_minAvgLineClr READ getMinAvgLineClr WRITE setMinAvgLineClr DESIGNABLE true)
    Q_PROPERTY(QString m_daiesFontFamily READ getDaiesFontFamily WRITE setDaiesFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_daiesFontSize READ getDaiesFontSize WRITE setDaiesFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_daiesTextClr READ getDaiesTextClr WRITE setDaiesTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_daiesGridLineClr READ getDaiesGridLineClr WRITE setDaiesGridLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_daiesTopHLineClr READ getDaiesTopHLineClr WRITE setDaiesTopHLineClr DESIGNABLE true)


    //异动相关
    Q_PROPERTY(QColor m_abNormalLineClr READ getAbNormalLineClr WRITE setAbNormalLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_abNormalRcBkClr READ getAbNormalRcBkClr WRITE setAbNormalRcBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_abNormalPtBkClr READ getAbNormalPtBkClr WRITE setAbNormalPtBkClr DESIGNABLE true)

    Q_PROPERTY(QString m_abNormalFontFamily READ getAbNormalFontFamily WRITE setAbNormalFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_abNormalFontSize READ getAbNormalFontSize WRITE setAbNormalFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_abNormalTextClr READ getAbNormalTextClr WRITE setAbNormalTextClr DESIGNABLE true)
    Q_OBJECT
public:
    explicit MinCanvas(QWidget *parent = 0);

////////////////////QSS自定义样式接口/////////////////////////////////////
    //分时图底部边框线颜色
    QColor getMinChartBottomLineClr() const{return m_minChartBottomLineClr;}
    void setMinChartBottomLineClr(QColor color){m_minChartBottomLineClr=color;}
    QColor m_minChartBottomLineClr = QColor(Qt::red);

    //分时线颜色
    QColor getMinLineClr() const{return m_minLineClr;}
    void setMinLineClr(QColor color){m_minLineClr=color;}
    QColor m_minLineClr = QColor(Qt::red);// QColor(54,129,227);
    //分时均线颜色
    QColor getMinAvgLineClr() const{return m_minAvgLineClr;}
    void setMinAvgLineClr(QColor color){m_minAvgLineClr=color;}
    QColor m_minAvgLineClr = QColor(Qt::red);// QColor(54,129,227);

    //多日分时字体样式
    QString getDaiesFontFamily() const{return m_daiesFontFamily;}
    void setDaiesFontFamily(QString family){m_daiesFontFamily=family;}
    QString m_daiesFontFamily = "Arial";

    //多日分时字体高度
    int getDaiesFontSize() const{return m_daiesFontSize;}
    void setDaiesFontSize(int size){m_daiesFontSize=size;}
    int m_daiesFontSize = 11;

    //多日分时字体颜色
    QColor getDaiesTextClr() const{return m_daiesTextClr;}
    void setDaiesTextClr(QColor color){m_daiesTextClr=color;}
    QColor m_daiesTextClr = QColor(Qt::red);

    //多日分时网格线颜色
    QColor getDaiesGridLineClr() const{return m_daiesGridLineClr;}
    void setDaiesGridLineClr(QColor color){m_daiesGridLineClr=color;}
    QColor m_daiesGridLineClr = QColor(22,30,35);

    //多日分时顶部水平线颜色
    QColor getDaiesTopHLineClr() const{return m_daiesTopHLineClr;}
    void setDaiesTopHLineClr(QColor color){m_daiesTopHLineClr=color;}
    QColor m_daiesTopHLineClr = QColor(22,30,35);


    //异动边框和线条颜色
    QColor getAbNormalLineClr() const{return m_abNormalLineClr;}
    void setAbNormalLineClr(QColor color){m_abNormalLineClr=color;}
    QColor m_abNormalLineClr = QColor("#6B7B8B");

    //异动矩形框背景色
    QColor getAbNormalRcBkClr() const{return m_abNormalRcBkClr;}
    void setAbNormalRcBkClr(QColor color){m_abNormalRcBkClr=color;}
    QColor m_abNormalRcBkClr = QColor("#535E69");

    //异动价格点背景色
    QColor getAbNormalPtBkClr() const{return m_abNormalPtBkClr;}
    void setAbNormalPtBkClr(QColor color){m_abNormalPtBkClr=color;}
    QColor m_abNormalPtBkClr = QColor("#4497e2");


    //异动文本样式
    QString getAbNormalFontFamily() const{return m_abNormalFontFamily;}
    void setAbNormalFontFamily(QString family){m_abNormalFontFamily=family;}
    QString m_abNormalFontFamily = "宋体";

    //异动字体高度
    int getAbNormalFontSize() const{return m_abNormalFontSize;}
    void setAbNormalFontSize(int size){m_abNormalFontSize=size;}
    int m_abNormalFontSize = 12;

    //异动字体颜色
    QColor getAbNormalTextClr() const{return m_abNormalTextClr;}
    void setAbNormalTextClr(QColor color){m_abNormalTextClr=color;}
    QColor m_abNormalTextClr = QColor("#ffffff");
    ////////////////////QSS自定义样式END/////////////////////////////////////
    //基本数据设置接口 不立即切换股票
    void setBaseInfo(const QString& obj,int day=-1);

    //立即切换合约
    void switchStockEx(const QString& obj);

    //切换合约，切换历史分时天数
    void switchStock(const QString& obj,int daysIndex=-1);

    //跳转多图
    void adjustMultip(int num);

    //激活
    virtual void afterActive();

    //系统事件
    virtual void keyPressEvent(QKeyEvent *event);

    //创建十字小窗
    ChartCrossDlg* createCrossDlg();

    void setAttachIndi(int indi);

    int getAttachIndi();

    void setRecordSwitchPage(bool bRecord);
signals:

public slots:
    //处理附图指标变化
    void switchAttachIndicator(int id);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    QString getBaseCmd(const QString& obj,int day);
    QString getIndiCmd(int id);
    void postCrossWndInfo(bool bKeyEvent);
    void getIndiIdList(int& mainId,std::vector<int>& attachIndiList);
    Minchart* getMinChart();
protected:
    QString m_obj;
    int m_day;
    IndiIndexMap m_indiMap;
    bool m_recordPage = true;
};

#endif // MINCANVAS_H
