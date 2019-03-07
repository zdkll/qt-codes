/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自BaseWidget   资金异动图表窗体
*********************************************************************/
#ifndef FUNDCHANGECHARTWIDGET_H
#define FUNDCHANGECHARTWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
using namespace HZData;

struct FundChangeDataUint
{
    QString strName = "";   //下方文本
    QString strValue = "";  //要显示的数值
    float value = 0.0f;     //计算后的数值
    QColor borderColor;     //矩形边框颜色
    QColor brushColor;      //矩形填充颜色
    QColor numColor;        //上方数字颜色
};

typedef QVector<FundChangeDataUint> FundChangeSet;

struct FundChangeData
{
    FundChangeSet fundSet;
    float maxValue;
};

class FundChartStyledWidget:public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor DESIGNABLE true)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)
    Q_PROPERTY(QColor riseNumColor READ riseNumColor WRITE setRiseNumColor DESIGNABLE true)
    Q_PROPERTY(QColor fallNumColor READ fallNumColor WRITE setFallNumColor DESIGNABLE true)
    Q_PROPERTY(QColor riseBorderColor READ riseBorderColor WRITE setRiseBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor fallBorderColor READ fallBorderColor WRITE setFallBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor riseBrushColor READ riseBrushColor WRITE setRiseBrushColor DESIGNABLE true)
    Q_PROPERTY(QColor fallBrushColor READ fallBrushColor WRITE setFallBrushColor DESIGNABLE true)
public:
    explicit FundChartStyledWidget(QWidget *parent = nullptr):BaseWidget(parent){}

    //分割线颜色
    QColor lineColor() const{return m_lineColor;}
    void setLineColor(const QColor &color){m_lineColor = color;}

    //下方文本颜色
    QColor textColor() const{return m_textColor;}
    void setTextColor(const QColor &color){m_textColor = color;}

    //上涨数字颜色
    QColor riseNumColor() const{return m_riseNumColor;}
    void setRiseNumColor(const QColor &color){m_riseNumColor = color;}

    //下跌数字颜色
    QColor fallNumColor() const{return m_fallNumColor;}
    void setFallNumColor(const QColor &color){m_fallNumColor = color;}

    //上涨边框颜色
    QColor riseBorderColor() const{return m_riseBorderColor;}
    void setRiseBorderColor(const QColor &color){m_riseBorderColor = color;}

    //下跌边框颜色
    QColor fallBorderColor() const{return m_fallBorderColor;}
    void setFallBorderColor(const QColor &color){m_fallBorderColor = color;}

    //上涨矩形填充颜色
    QColor riseBrushColor() const{return m_riseBrushColor;}
    void setRiseBrushColor(const QColor &color){m_riseBrushColor = color;}

    //下跌矩形填充颜色
    QColor fallBrushColor() const{return m_fallBrushColor;}
    void setFallBrushColor(const QColor &color){m_fallBrushColor = color;}

private:
    QColor m_lineColor;
    QColor m_textColor;
    QColor m_riseNumColor;
    QColor m_fallNumColor;
    QColor m_riseBorderColor;
    QColor m_fallBorderColor;
    QColor m_riseBrushColor;
    QColor m_fallBrushColor;
};

/**
  * @brief 资金流入图表
  */
class FundChangeChart:public BaseWidget
{
    Q_OBJECT
public:
    explicit FundChangeChart(QWidget *parent = nullptr);

     ~FundChangeChart();

    void initData();

    void updateData(char *data, int num);


protected:
    virtual void paintEvent(QPaintEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

private:
    void draw();

private:
    QStringList m_strLists;
    FundChangeData m_fundChangeData;
    QFont m_numFont;
    QFont m_textFont;


    FundChartStyledWidget m_styledWidget;

    QPixmap *m_pix;

    QList<INT64> m_netputList;
};



/**
  * @brief 资金异动图表主窗体
  */
class QLabel;
class FundChangeChartWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit FundChangeChartWidget(QWidget *parent = nullptr);

    //激活对象
    virtual void afterActive();
    //取消激活
    virtual void afterDeactive();

private:
    void createWg();

    void startSub();

    void cancelSub();


    void onDataArrived(ReqId id, char *data, int num, ErrCode errCode);


private:
    QLabel *m_titleLabel;

    FundChangeChart *m_fundChangeChart;

    ReqId m_nReq;
};

#endif // FUNDCHANGECHARTWIDGET_H
