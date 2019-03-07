#ifndef BASECHART_H
#define BASECHART_H
/********************************************************************
created: 2018/4/9
author: min.cheng
purpose: 基础的绘图区域，K线/分时/指标等
*********************************************************************/
#include <qlist.h>
#include <qvariant.h>
#include <memory>
#include <qvector.h>

#include "drawable.h"
#include "chartdef.h"
#include "datakline.h"

class XAxis;
class YAxis;
class DataProvider;
class ChartTitle;

class BaseChart : public Drawable
{
    Q_OBJECT
public:
    BaseChart(ImageLayer* layer);

    ~BaseChart();

    //x轴设置和获取
    void setXAxis(std::shared_ptr<XAxis> pXAxis,bool bBindObserver=true);
    std::shared_ptr<XAxis> getXAxis();

    //y轴设置和获取
    void setYAxis(std::shared_ptr<YAxis> pYAxis);   
    std::shared_ptr<YAxis> getYAxis();

    //设置标题
    void setTitle(std::shared_ptr<ChartTitle> title);
    std::shared_ptr<ChartTitle> getTitle();

    //设置区域边界
    void setMargins(ChartMargins margins);
    ChartMargins getMargins();

    //获取屏幕显示的数据
    const DataKLine& getScreenData() const;

    //是否绘制十字交叉点
    virtual bool isDrawCrossLintPt();

    //是否绘制均线
    virtual bool isDrawAvgLine();

    //切换参数 一般用于创建完成以后
    virtual void switchParam(const QString& msg);

    //设置参数 一般用于创建完成，或只是想设置参数，而不实际发生数据请求和计算时调用
    virtual void setParam(const QString& msg);

    //获取chart主要绘图区域
    virtual QRect getChartMainArea();

    //获取X轴右边坐标区域
    virtual QRect getRightAxisArea();

    //获取X轴右边坐标区域
    virtual QRect getLeftAxisArea();

    //获取当前显示分为内的最大和最小值
    virtual void getMaxMinRange(double& max,double& min);

    //获取显示范围内的最大和最小值所在下标
    virtual void getMaxMinRangeIndex(int& maxPos,int& minPos);

    //获取整个数据（包括未显示区域）的数量
    virtual int getCacheSize();

    //重新绘制
    virtual void redraw();

    //获取格式化时间的格式
    virtual void getFormatTimeStr(QString& strFormat);

    //当主图坐标发生变化时会被调用，用于通知其他相关绘图区域
    virtual void mainChartXaixsChnage(const QString& msg);

    //返回数据提供者
    virtual DataProvider* getProvider();

    virtual void createProvider(const QString& provider);

    //获取属性 参考chartdef中关于属性值定义
    int getAttr();

    //设置属性 参考chartdef中关于属性值定义
    void setAttr(int attr);

    //设置复权标记
    void setDrType(RestorationType type);
    RestorationType getDrType();

    /**
     * @brief   数据提供者有最新数据时会被调用
     * @param[in] id 请求数据的id 返回-1 INT_MAX表示当前数据没真正发生向服务器请求，本地有数据
     * @param[in] type 请求类型 用于标示当前数据是什么类型
      */
    virtual void dataResponse(int id,int type);

    /**
     * @brief  请求数据时发生错误后被调用
     * @param[in] id 请求数据的id
     * @param[in] ncode 错误代码
     * @param[in] errReuslt 错误文本
     */
    virtual void errorResponse(int id,int ncode,const QString& errReuslt);

    /**
     * @brief 鼠标键盘事件
     */
    virtual bool mouseMoveEx(QMouseEvent *event);
    virtual bool mousePressEx(QMouseEvent *event);
    virtual bool mouseReleaseEx(QMouseEvent *event);
    virtual bool keyPressEx(QKeyEvent *event);
    virtual bool keyReleaseEx(QKeyEvent *event);
    virtual bool enterEventEx(QEvent *event);
    virtual bool leaveEventEx(QEvent *event);

    //用于界面切换后处理资源 比如数据请求等
    virtual void afterActive();
    virtual void afterDeactive();

    //更具下标获取单根数据状态 返回status 由各个子类去定义
    virtual bool getStatusFromIndex(int index,int& status);

    /**
     * @brief 获取和设置titleChart 用于通知标题重绘文本
     */
    void setTitleChart(std::shared_ptr<ChartTitle> pTitle);
    std::shared_ptr<ChartTitle> getTitleChart();

    //十字光标 左侧格式化方法
    virtual QString leftTipInfoFormat(double value);

    //十字光标右侧格式化方法
    virtual QString rightTipInfoFormat(double value);

    //十字光标Y轴设置调整 true表示设置成功，否则失败
    virtual bool adjustCrossLineYAxis(std::shared_ptr<BaseChart> crossChart,const QPoint& pos);

    //showEx()调用后，自动调用
    virtual void onShow(bool bshow);

    //resize()调用后，自动调用
    virtual void onResize();

    //立即更新界面
    void paintImmet();

    //定时刷新 非立即刷新
    void repainterOnTime();

    //设置类型
    void setType(ChartType type);

    //获取类型
    ChartType getType();

    int getId();
signals:

public slots:
    virtual void soltButtonClose();
    virtual void soltButtonParamSet();
    virtual void soltButtonPlus();
    virtual void soltButtonSub();
    virtual void soltRestorationComBoxChanged(int index);
    virtual void soltIndiSeltComBoxChanged(int index);
protected:
    void OnXaxisChange(QString msg);
protected:
    //当前屏显示数据
    DataKLine m_vKlineData;
    //区域标题
    std::shared_ptr<ChartTitle> m_title= nullptr;
    //X Y 轴坐标
    std::shared_ptr<XAxis> m_xAxis;
    std::shared_ptr<YAxis> m_yAxis;
    //边距
    ChartMargins m_margins;
    //当前屏最大最小值
    double m_chartMax;
    double m_chartmin;
    //当前屏最大最小值对应的下标
    int m_maxIndex;
    int m_minIndex;
    //属性宏
    int m_attr;
    //复权类型
    RestorationType m_drType;
    //chart类型
    ChartType m_chartType;
    //坐标刷新监听者
    int m_xaxisObserverKey = -1;
    //当前 chart id  用于外部设置，用于唯一标记当前ID
    int m_id = -1;
};

#endif // BASECHART_H
