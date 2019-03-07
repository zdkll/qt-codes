#ifndef XAXIS_H
#define XAXIS_H
/********************************************************************
created: 2018/4/4
author: min.cheng
purpose: x坐标轴
*********************************************************************/
#include <qlist.h>
#include <qvariant.h>
#include <objbase.h>
#include <map>
#include <qrect.h>

#include "Observer.hpp"
#include "chartdef.h"

class BaseChart;
class XAxis:public QObject
{
     Q_OBJECT
public:
    XAxis(BaseChart* pchart);

    /**
     * @brief  画布X轴坐标和时间点转换
     * @param[in] pt_x 画布的X坐标
     * @retval 成功返回对应的时间点,否则返回-1
     */
    qint64 xCanvasPTxToTime(int pt_x);
    qint64 xCanvasIndexToTime(int nIndex);

    /**
     * @brief 画布X轴坐标转数据下标
     * @param[in] pt_x 画布的X坐标
     * @retval 成功返回对应的数据下标,否则返回-1
     */
    int xCanvasPTxToIndex(int pt_x);

    /**
     * @brief 根据下标获取对应的价格 （K线分时都指的是收盘价）
     * @param[in] index
     * @retval 成功返回对应的数据,否则返回0
     */
    double xGetPriceByIndex(int index);

    /**
     * @brief 数据的下标转画布的X轴坐标
     * @param[in] index
     * @retval 成功返回对应的X轴坐标,否则返回-1
     */
    int xCanvasIndexToPTx(int index,bool bMid=true);

    /**
     * @brief 设置从右开始的数据下标偏移值，初始值为0
     * @param[in] pos偏移值
     */
    void setoffset(int pos);

    /**
     * @brief 获取从右开始的数据下标偏移值，初始值为0
     */
    int getoffset();

    /**
     * @brief 获取/设置界面显示的元素个数
     */
    int getCount();


    /**
     * @brief 设置界面中单根数据的绘制宽度
     * @param[in] width
     */
    void setSingleAreaWidth(double width);

    /**
     * @brief 获取界面中单根数据的绘制宽度
     */
    double getSingleAreaWidth();

    /**
     * @brief 获取左侧开始/结束绘制的数据的时间
     * @retval
     */
    qint64 getBeginTime();
    qint64 getEndTime();

    /**
     * @brief 获取左侧开始/结束绘制的数据的下标
     * @retval
     */
    int getBeginIndex();
    int getEndIndex();

    /**
     * @brief 当数据/界面大小/偏移等发生跟新后重新计算位置和转换坐标
     * @retval 计算失败返回false，否则返回true
     */
    bool recalc(bool bReCalc=true);

    /**
     * @brief 获取绘图的区域
     */
    QRect getRc();

    /**
     * @brief 计算绘图所需数据
     */
    double calcDataSize();

    /**
     * @brief 根据知道宽度计算绘图所需数据
     */
    int calcDataSize(double fwidth);

    /**
     * @brief 设置/获取十字光标当前坐标下标位置 -1表示隐藏十字光标
     */
    void setCurrCrossXIndex(int index);
    int getCurrCrossXIndex();

    /**
     * @brief 观察者 用于监听坐标系变化 其他指标通过监听主图坐标系变化来开启数据计算
     */
    Events<XAxisDataChangeCallBack>& getObserverMgr();
    BaseChart* getChart();

    //回复默认值
    void resetDefaultVaue();
signals:

public slots:

protected:
    bool isInXaxis(const int& pt_x);
protected:
    //主图坐标变化观察者
    Events<XAxisDataChangeCallBack> m_observerMgr;
    int m_offset;     //数据的偏移位置
    int m_begin;      //显示的数据的起始位置
    int m_end;
    qint64 m_beginTime;   //显示的开始时间
    qint64 m_endTime;
    int  m_count;      //单页内显示的根数
    double m_singleAreaWidth;   //单根数据的显示宽度
    BaseChart* m_chart;
    QRect m_rc;
    int m_crossXindex;
};

#endif // XAXIS_H
