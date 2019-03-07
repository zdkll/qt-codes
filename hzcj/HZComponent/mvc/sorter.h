/********************************************************************
created:
author: dk.zhang
purpose:股票数据排序
*********************************************************************/
#ifndef  SORTER_H
#define SORTER_H

#include <QVector>
#include <algorithm>
#include <functional>
#include "baseitemmodel.h"

#include "hzcomponent_global.h"

//排序基类
class    BaseSorter
{
public:
    BaseSorter();
    void setSortIndicator(SortField sortField,bool desc);

protected:
    SortField     m_sortField ;
    bool            is_desc ;
};

//行情排序
class   DynaSorter : public BaseSorter
{
public:
    DynaSorter(){}

    //SLStock 排序
    void sort(QVector<HZData::SLStock>& slStocks){
        std::sort(slStocks.begin(),slStocks.end(),std::bind(&DynaSorter::compareSlStock,this
                                                            ,std::placeholders::_1,std::placeholders::_2));
    }
    //Stock  排序
    void sort(QVector<HZData::Stock>& stocks){
        std::sort(stocks.begin(),stocks.end(),std::bind(&DynaSorter::compareStock,this
                                                        ,std::placeholders::_1,std::placeholders::_2));
    }

    //Block 排序
    void sort( QVector<HZData::Block>& blocks){
        std::sort(blocks.begin(),blocks.end(),std::bind(&DynaSorter::compareBlock,this
                                                        ,std::placeholders::_1,std::placeholders::_2));
    }

    //Index 排序
    void sort(QVector<HZData::Index>& indexs){
        qDebug()<<"sort Index,sortField:"<<m_sortField;
        std::sort(indexs.begin(),indexs.end(),std::bind(&DynaSorter::compareIndex,this
                                                        ,std::placeholders::_1,std::placeholders::_2));
    }

private:
    //比较函数
    bool compareSlStock(const HZData::SLStock &slStock1,const HZData::SLStock &slStock2);
    bool compareStock(const HZData::Stock &stock1,const HZData::Stock &stock2);
    bool compareBlock(const  HZData::Block &block1,const HZData::Block &block2);
    bool compareIndex(const HZData::Index &index1,const HZData::Index &index2);
};




#endif // SORTER_H
