#ifndef COMPONENTDEF_H
#define COMPONENTDEF_H
/********************************************************************
created:
author: min.cheng
purpose: 导出相关页面的公共结构定义文件
*********************************************************************/\
//跑马灯数据项
struct MarqueeDataItem
{
    QString id;
    QString title;
    qint64 time;
    QString url;
};

//跑马灯详情数据
struct MarqueeDetail
{

};


#endif // COMPONENTDEF_H
