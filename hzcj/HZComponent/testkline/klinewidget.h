#ifndef KLINEWIDGET_H
#define KLINEWIDGET_H
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "datacontroller.h"

#define     XAXISSEPARATE 9   //x等分数
#define     YAXISPERHEIGHT 25.0 //y轴高度
#define     YAXISWIDTH   60.0 //y轴坐标宽度

#define     XAXISHEIGHT  60.0 //x轴坐标高度

#define KLINESINGLEWIDTH  10.0   //k线宽度

#define KLINEMAXWIDTH 50.0   //k线最大宽度

#define KLINEMINWIDTH 1.0f  //最小K线宽度




struct ReqInfoEx
{
    int noffset = 0;        //偏移量
    int nCount = 0;         //当前显示根数
    bool bEndPoint = false;
};

class KlineXaxis;
class KlineYaxis;
class KlineInfoWidget;
class HZCOMPONENTSHARED_EXPORT KlineWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit KlineWidget(QWidget *parent = nullptr);
    ~KlineWidget();


    virtual void afterActive();

    DataKLine *getKlineData();

    void getMaxMinRange(float &max, float &min);

//    void startReq();

protected:
    virtual void paintEvent(QPaintEvent *e);

//    virtual void showEvent(QShowEvent *event);

    virtual void resizeEvent(QResizeEvent *e);

    virtual void mouseMoveEvent(QMouseEvent *e);



    virtual void mousePressEvent(QMouseEvent *e);

    virtual void mouseReleaseEvent(QMouseEvent *e);

    virtual void keyPressEvent(QKeyEvent *e);

    void drawGrid(QPainter *painter);
    void drawYAxis(QPainter *painter);
    void drawKline(QPainter *painter);
    void drawCross(QPainter *painter);
    void drawYAxisPoint(QPainter *painter);
    void drawXAxis(QPainter *painter);

private:
    QRectF getDrawRect();
    void startReq();

    void reqKline(HZData::ReqKlineInfo reqInfo);

    void onKlineDataArrived(ReqId id ,DataKLine *dataKline, ErrCode errCode);

    void dragLeft(int noffset);
    void dragRight(int noffset);

    void keyUp();
    void keyDown();

    void mergeData(DataKLine *src);

    void getScreenData();

private slots:
    void onTimeOut();

private:

    DataKLine *m_cacheData;     //缓存数据

    DataKLine* m_klineData;     //当前显示数据
    int m_period;
    bool m_bFlg = true;

    int m_pressPosX = -1;

    ReqInfoEx m_reqInfo;

    int m_noffset;

    float m_singleKlineWidth;

    QPointF m_mousePt;

    KlineXaxis *m_xAxis;
    KlineYaxis *m_yAxis;

    float m_maxValue;
    float m_minValue;

    bool m_isDown = false;

    bool m_isEnd = false;

    KlineInfoWidget *m_infoWidget;

};

#endif // KLINEWIDGET_H
