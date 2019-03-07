/********************************************************************
created:2018/06/08
author: dk.zhang
purpose:分时成交明细表
*********************************************************************/
#ifndef TIMEDEALTABLEVIEW_H
#define TIMEDEALTABLEVIEW_H

#include <QTableView>

#include "hzcomponent_global.h"
#include "datacontroller.h"

class TimeDealModel;
//分时成交表
class   TimeDealTableView : public QTableView
{
    Q_OBJECT
public:
    TimeDealTableView(QWidget *parent =0);
    ~TimeDealTableView();

    //初始化设置显示数据
    void setData(const QVector<HZData::HZTick> &ticks);
    //清除数据
    void clearData();

    //最大显示行数
    int     maxRowCount() const;
    //实际显示行数
    int   rowCount() const;

    //设置每一列缩放因子
    void setStretchFactors(const QList<float> &factors);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    void initTableView();

    TimeDealModel    *m_dataModel;

    QList<float>          m_stretchFactors;
};




#endif // TIMEDEALTABLEVIEW_H
