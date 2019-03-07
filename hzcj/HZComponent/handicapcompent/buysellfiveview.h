/********************************************************************
created:2018.9
author: juntang.xie
purpose: 买卖五档视图类
*********************************************************************/
#ifndef BUYSELLFIVEVIEW_H
#define BUYSELLFIVEVIEW_H
#include <QTableView>
#include "datacontroller.h"

class BuySellFiveModel;
class BuySellFiveView:public QTableView
{
    Q_OBJECT
public:
    explicit BuySellFiveView(int type = 0, QWidget *parent = nullptr);
    /**
     * @brief 更新数据
     * @param[in] vec 收到的数据容器
     * @retval
     */
    void updateData(const  QVector<HZData::AllStock> &vec);

private:
    void initTableView();

private:
    BuySellFiveModel *m_model;
};

#endif // BUYSELLFIVEVIEW_H
