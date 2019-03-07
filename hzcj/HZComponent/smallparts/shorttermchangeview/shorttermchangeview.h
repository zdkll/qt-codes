/********************************************************************
created:2018.11.13
author: juntang.xie
purpose:派生自QTableView 短线异动表格
*********************************************************************/
#ifndef ShortTermChangeView_H
#define ShortTermChangeView_H
#include <QTableView>
#include "datacontroller.h"

using namespace HZData;
class ShortTermChangeModel;
class   ShortTermChangeView:public QTableView
{
    Q_OBJECT
public:
    explicit ShortTermChangeView(QWidget *parent = nullptr);

    //显示最大行数
    int maxRowCount() const;

    //更新数据
    void upDateData(const QVector<ShortMotion> &fundVec);

    //清除数据
    void clearData();

    //列表行数
    int rowCount();

protected:

signals:
    void clickedRow(const QString strCode);

private:
    void initTableView();

private slots:
    void onRowClicked(const QModelIndex &index);

private:
    ShortTermChangeModel *m_shortTermModel;
    bool m_bSelected;
};

#endif // ShortTermChangeView_H
