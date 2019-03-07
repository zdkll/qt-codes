/********************************************************************
created:2018.10.24
author: juntang.xie
purpose: 板块底部导航
*********************************************************************/
#ifndef DROWDOWNLISTWIDGET_H
#define DROWDOWNLISTWIDGET_H
#define ITEMWIDTH 88
#define ITEMHEIGHT 23
#include "basewidget.h"
#include "httpdata.h"

#pragma execution_character_set("utf-8")

class QPushButton;
class QTableWidget;
class DrowDownListWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit DrowDownListWidget(QWidget *parent = nullptr);
    /**
     * @brief 设置菜单标题
     * @param[in] strText 标题内容
     * @retval
     */

    void setTitle(const QString &strText);
    /**
     * @brief 设置弹出列表数据
     * @param[in] vec 列表显示的数据
     * @retval
     */
    void setData(const QVector<BlockFormatDataItem> &vec);

private:
    void createWg();
    void initTableWiget();
    void setTableWidget();

signals:
    void clicked();
    void cellClicked(QString strName, QString strCode);

private slots:
    void onCellClicked(int row, int column);

private:
    QPushButton *m_pBtn;
    QTableWidget *m_tableWiget;
    QVector<BlockFormatDataItem> m_itemVec;
};

#endif // DROWDOWNLISTWIDGET_H
