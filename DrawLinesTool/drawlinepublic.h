#ifndef DRAWLINEPUBLIC_H
#define DRAWLINEPUBLIC_H

#define  MAX_DISTANCE_VALUE     5
#define  POINT_RECT_WIDTH         5

#include <QString>

namespace DrawLine
{
//画线工具类型
enum LineToolType
{
    eNoneTool = 0,
    eStraightLine =1, //直线
    eRayLine =2,  //射线
    eSegLine =3,  //线段
    eHorizLine =4,//水平线
    eVertLine =5,//垂直线
    eRectLine =6,//矩形
    eParalLine =7,//平行线段
    eParallelTrendLine =8,//平行趋势线
    eEqualPeriodLine =9,//等周期线
    eFibonacciLine =10,    //斐波那契周期
    eGoldenSectionLine =11,//黄金分割线
    eAmpScaleLine =12,        //幅度尺

    eSelect   ,//选择

    eUpArrow,//上箭头
    eDownArrow,//下箭头
    eTextNote,//文字注释
    eMarkPriceLine,//标价线

    eDelSingleLine,//删除单根画线
    eDelAllLines,//删除全部画线
    eScreenShot,//截屏
};

//画线工具信息
struct LineTool
{
    LineToolType  typeId = eNoneTool;
    QString text;
    QString icon;
    QString group;
};

struct Command
{
    void start(){running = true;}
    void end()
    {
        toolType = eNoneTool;
        running = false;
    }
    bool isRunning(){return running;}

    LineToolType toolType = eNoneTool;
    bool               running = false;
};

}


#endif // DRAWLINEPUBLIC_H
