#ifndef RENDER_H
#define RENDER_H
/********************************************************************
created: 2018/4/17
author: min.cheng
purpose: 指标绘图功能实现
*********************************************************************/
#include <qpainter.h>
#include <memory>

#include "chartdef.h"
#include "datamodeldef.h"
#include "IndiInterface.h"

class XAxis;
class YAxis;
struct IndiRspResult;
struct IndiResul;

struct UpDownEqualClr
{
    QColor upClr;
    QColor downClr;
    QColor equalClr;
};

class Render
{
public:
    //指标绘图
    static void drawIndicator(QPainter& dc,const IndiRspResult& data,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);

    //插值算法
    //static int interpolationKline(const DataKLine& input,int num,double interval,DataKLine& output);
private:
    //彩色棒状绘制指标
    static void IndiColorStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //线条绘制指标
    static void IndiLineStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //分时柱状绘制指标
    static void IndiMinVolStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //K线柱状绘制指标 VOL
    static void IndiKlineVolStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //指标实体柱绘制 柱状颜色外部指定
    static void IndiEntityRectStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //筹码分布集分绘制
    static void IndiCmfbStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //两点区间绘制
    static void IndiCptxRedGreenStick(QPainter &dc,const IndiResult& resultStyle,QVector<MemBlock*> blockList,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //绘制买卖点
    static void IndiBuySellStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //绘制多空线
    static void IndiDuoKongStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);
    //绘制圆圈
    static void IndiCircleStick(QPainter &dc,const IndiResult& resultStyle,MemBlock* pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr);

};

#endif // RENDER_H
