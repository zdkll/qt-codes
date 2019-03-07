#ifndef BASECANVAS_H
#define BASECANVAS_H
/********************************************************************
created:   2018/4/4
author: min.cheng
purpose: 所有画布基类
*********************************************************************/
#include <QWidget>
#include <QMap>
#include <memory>
#include <map>
#include <unordered_map>

#include "basewidget.h"
#include "chartdef.h"
#include "datamodeldef.h"
#include "chartcrossdlg.h"

class ImageLayer;
class BaseChart;
class ChartContainer;
class BaseChart;
class XAxis;
class IndicatorChart;
typedef std::unordered_map<int,std::shared_ptr<ChartContainer>> ContainersMap;

class BaseCanvas : public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor m_bkClr READ getBkClr WRITE setBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_containerBorderClr READ getContainerBorderClr WRITE setContainerBorderClr DESIGNABLE true)
    Q_PROPERTY(QColor m_containerTitleTextClr READ getContainerTitleTextClr WRITE setContainerTitleTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_containerTitleBkClr READ getContainerTitleBkClr WRITE setContainerTitleBkClr DESIGNABLE true)
    Q_PROPERTY(QString m_containerTitleFontFamily READ getContainerTitleFontFamily WRITE setContainerTitleFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_containerTitleFontSize READ getContainerTitleFontSize WRITE setContainerTitleFontSize DESIGNABLE true)

    Q_PROPERTY(QString m_crossLineFontFamily READ getCrossLineFontFamily WRITE setCrossLineFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_crossLineFontSize READ getCrossLineFontSize WRITE setCrossLineFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineTextClr READ getCrossLineTextClr WRITE setCrossLineTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineRcBkClr READ getCrossLineRcBkClr WRITE setCrossLineRcBkClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineClr READ getCrossLineClr WRITE setCrossLineClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineClostPtClr READ getCrossLineClostPtClr WRITE setCrossLineClostPtClr DESIGNABLE true)
    Q_PROPERTY(QColor m_crossLineAvgPtClr READ getCrossLineAvgPtClr WRITE setCrossLineAvgPtClr DESIGNABLE true)

    Q_PROPERTY(QString m_xaxisFontFamily READ getXaxisFontFamily WRITE setXaxisFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_xaxisFontSize READ getXaxisFontSize WRITE setXaxisFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_xaxisTextClr READ getXaxisTextClr WRITE setXaxisTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_gridClr READ getGridClr WRITE setGridClr DESIGNABLE true)
    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)
    Q_PROPERTY(QString m_yaxisFontFamily READ getYaxisFontFamily WRITE setYaxisFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_yaxisFontSize READ getYaxisFontSize WRITE setYaxisFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_yaxisTextClr READ getYaxisTextClr WRITE setYaxisTextClr DESIGNABLE true)
    Q_PROPERTY(QColor m_xaxisScaleClr READ getXaxisScaleClr WRITE setXaxisScaleClr DESIGNABLE true)
public:
    explicit BaseCanvas(QWidget *parent = 0);

    ~BaseCanvas();

     //容器区域背景色
     QColor getBkClr() const{return m_bkClr;}
     void setBkClr(QColor color){m_bkClr=color;}
     QColor m_bkClr = QColor("#0c1215");

     //容器边框颜色
     QColor getContainerBorderClr() const{return m_containerBorderClr;}
     void setContainerBorderClr(QColor color){m_containerBorderClr=color;}
     QColor m_containerBorderClr =  QColor(119,136,153);

     //容器对应的title背景颜色
     QColor getContainerTitleBkClr() const{return m_containerTitleBkClr;}
     void setContainerTitleBkClr(QColor color){m_containerTitleBkClr=color;}
     QColor m_containerTitleBkClr=QColor(102,139,139);

     //容器对应的title颜色
     QColor getContainerTitleTextClr() const{return m_containerTitleTextClr;}
     void setContainerTitleTextClr(QColor color){m_containerTitleTextClr=color;}
     QColor m_containerTitleTextClr=QColor(102,139,139);


     //容器对应的title字体样式
     QString getContainerTitleFontFamily() const{return m_containerTitleFontFamily;}
     void setContainerTitleFontFamily(QString family){m_containerTitleFontFamily=family;}
     QString m_containerTitleFontFamily = "Arial";

     //容器对应的title字体高度
     int getContainerTitleFontSize() const{return m_containerTitleFontSize;}
     void setContainerTitleFontSize(int size){m_containerTitleFontSize=size;}
     int m_containerTitleFontSize = 8;

     //十字光标字体样式
     QString getCrossLineFontFamily() const{return m_crossLineFontFamily;}
     void setCrossLineFontFamily(QString family){m_crossLineFontFamily=family;}
     QString m_crossLineFontFamily = "Arial";

     //十字光标字体高度
     int getCrossLineFontSize() const{return m_crossLineFontSize;}
     void setCrossLineFontSize(int size){m_crossLineFontSize=size;}
     int m_crossLineFontSize = 8;

     //十字光标文本颜色
     QColor getCrossLineTextClr() const{return m_crossLineTextClr;}
     void setCrossLineTextClr(QColor color){m_crossLineTextClr=color;}
     QColor m_crossLineTextClr = QColor(Qt::red);

     //十字矩形区域光标背景颜色
     QColor getCrossLineRcBkClr() const{return m_crossLineRcBkClr;}
     void setCrossLineRcBkClr(QColor color){m_crossLineRcBkClr=color;}
     QColor m_crossLineRcBkClr = QColor(Qt::red);

     //十字光标线条颜色
     QColor getCrossLineClr() const{return m_crossLineClr;}
     void setCrossLineClr(QColor color){m_crossLineClr=color;}
     QColor m_crossLineClr = QColor(Qt::blue);

     //十字光标线条与收盘价交叉点颜色
     QColor getCrossLineClostPtClr() const{return m_crossLineClostPtClr;}
     void setCrossLineClostPtClr(QColor color){m_crossLineClostPtClr=color;}
     QColor m_crossLineClostPtClr = QColor(Qt::blue);

     //十字光标线条与均价交叉点颜色
     QColor getCrossLineAvgPtClr() const{return m_crossLineAvgPtClr;}
     void setCrossLineAvgPtClr(QColor color){m_crossLineAvgPtClr=color;}
     QColor m_crossLineAvgPtClr = QColor(Qt::blue);

     //X轴字体样式
     QString getXaxisFontFamily() const{return m_xaxisFontFamily;}
     void setXaxisFontFamily(QString family){m_xaxisFontFamily=family;}
     QString m_xaxisFontFamily = "Arial";

     //X轴字体高度
     int getXaxisFontSize() const{return m_xaxisFontSize;}
     void setXaxisFontSize(int size){m_xaxisFontSize=size;}
     int m_xaxisFontSize = 8;

     //X时间轴文本颜色
     QColor getXaxisTextClr() const{return m_xaxisTextClr;}
     void setXaxisTextClr(QColor color){m_xaxisTextClr=color;}
     QColor m_xaxisTextClr = QColor(Qt::red);

     //网格颜色
     QColor getGridClr() const{return m_gridClr;}
     void setGridClr(QColor color){m_gridClr=color;}
     QColor m_gridClr = QColor(Qt::blue);

     //上涨颜色
     QColor getUpClr() const{return m_upClr;}
     void setUpClr(QColor color){m_upClr=color;}
     QColor m_upClr = QColor(Qt::red);

     //下跌颜色
     QColor getDownClr() const{return m_downClr;}
     void setDownClr(QColor color){m_downClr=color;}
     QColor m_downClr = QColor(Qt::red);

     //平局颜色  （不涨不跌）
     QColor getEqualClr() const{return m_equalClr;}
     void setEqualClr(QColor color){m_equalClr=color;}
     QColor m_equalClr = QColor(Qt::red);

     //Y轴字体样式
     QString getYaxisFontFamily() const{return m_yaxisFontFamily;}
     void setYaxisFontFamily(QString family){m_yaxisFontFamily=family;}
     QString m_yaxisFontFamily = "Arial";

     //Y轴字体高度
     int getYaxisFontSize() const{return m_yaxisFontSize;}
     void setYaxisFontSize(int size){m_yaxisFontSize=size;}
     int m_yaxisFontSize = 8;

     //Y轴文本颜色
     QColor getYaxisTextClr() const{return m_yaxisTextClr;}
     void setYaxisTextClr(QColor color){m_yaxisTextClr=color;}
     QColor m_yaxisTextClr = QColor(Qt::red);


     //X轴刻度颜色
     QColor getXaxisScaleClr() const{return m_xaxisScaleClr;}
     void setXaxisScaleClr(QColor color){m_xaxisScaleClr=color;}
     QColor m_xaxisScaleClr = QColor(Qt::red);


     //设置区域图层信息
     void setChartsInfo(const QVector<ChartItemInfo>& info);

     //窗口创建函数，相关属性设置好以后需要调用
     void createWg();

     QVector<int> addAttachCharts(const QVector<ChartItemInfo>& chartsInfo);

     void removeAttachCharts(const QVector<int>& idList,bool bResize = false);

     //立即刷新界面 图层重新叠加
     void paintImmet();

     //定时刷新 非立即刷新
     void repainterOnTime();

     //激活窗口
     virtual void afterActive();

     //取消窗口激活
     virtual void afterDeactive();

     virtual ChartCrossDlg* createCrossDlg();

     //设置是否显示十字光标
     void showCrossLine(bool bShow);

     //搬到当前是否显示了十字光标
     bool isShowCrossLine();

     //隐藏和显示区域
     void hideChart(bool bHide,int type);

     //处理十字光标信息小窗
     void proceCrossWndInfoChange(const HZData::HZKline& info,bool bEndIndex,bool bKeyEvent);

     //设置是否创建十字小窗
     void setCreateCrossDlgFlag(bool bFlag);

     //支持双击回车切换
     void setEnterAndDbClickSwitchKlineMin(bool bEnable);

     //切换前后复权
     void switchDrType();

     //设置复权标记
     void setDrType(RestorationType type);
     RestorationType getDrType();

     virtual void setAttachIndi(int indi);

     virtual int getAttachIndi();
signals:
     //十字光标移动后发送信号  info表示K线信息  bEndIndex表示是否是最后一根K线  bKeyEvent为true表示通过左右箭头移动的，否则是鼠标移动事件发送的。
     void signalCrossWndInfoChange(const HZData::HZKline& info,bool bEndIndex,bool bKeyEvent);
     // 双击信号  info 当前X轴所在K线数据 bEndIndex当前是否是最后一根，
     void signalDbClicked(const HZData::HZKline& info,bool bEndIndex,bool bInCandle=false);
      //內部选择指标区域变化后，用于通知外部，比如跳转指标TAB的状态等
     void signalAttachIndiSeletedChange(int id);
     //通知外部，除权发生变化
     void signalDrTypeChanged(int type,bool bCtrl);
public slots:
     //定时任务，目前用于定时重绘机制，没有数据更新时部分发生重绘事件
     void onTimerOut();
     //处理附图指标变化
     virtual void switchAttachIndicator(int id);
     //除权变化
     void slotsDrTypeChanged(int id);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void showEvent(QShowEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual QRect getWndRc();
    virtual void OnAreaSeletedChange();
    virtual void OnCreateEx();
    virtual QRect getContainerAreaRc(QResizeEvent *event = nullptr);

     //获取当前选中的附图，可能为空
    IndicatorChart* getSlectedIndiChart();
private:
    //重新计算区域
    void resize();
    //计算chart区域
    QVector<QRect> calcChartRc(const QVector<ChartItemInfo>& info);
    //创建chart
    std::shared_ptr<BaseChart> createChart(QRect rc,const ChartItemInfo& info,std::shared_ptr<XAxis> pXAxis);
    //调整选择项
    void adjustSelectedArea(const QPoint& pos);
    //调整十字光标当前Y轴坐标
    void adjustCrossLineYAxis(const QPoint& pos);
    //调整十字小窗
    void adjustCrossDlg(const QPoint& currPos);
protected:
    struct ChartData
    {
        std::shared_ptr<BaseChart> pChart;
        ChartItemInfo info;
    };
    //图层管理
    ImageLayer* m_imgLayer=nullptr;
    //定时器通过判断来刷新
    bool m_repaint;
    //区域信息
    QVector<ChartItemInfo>  m_chartsInfo;
    //可选图层映射
    QVector<ChartData> m_sletModelchartVec;
    //所有图层映射
    QVector<std::shared_ptr<BaseChart>> m_chartVec;
    std::shared_ptr<BaseChart> m_mainChart=nullptr;
    std::shared_ptr<BaseChart> m_crossChart=nullptr;
    std::shared_ptr<BaseChart> m_selectedChart=nullptr;
    //十字光标
    ChartCrossDlg* m_crossDlg=nullptr;
    bool m_bCrossIsShow = false;
    bool m_bFlag=false;
    //回车，双击切换K线分时
    bool m_bSwitchKlinAnMinEnable = true;
};

#endif // BASECANVAS_H
