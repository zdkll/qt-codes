/********************************************************************
created:2019.2.22
author: juntang.xie
purpose:搜索列表弹出窗口
*********************************************************************/
#ifndef SEACHSTOCKWIDGET_H
#define SEACHSTOCKWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"

class QPushButton;
class QLineEdit;
class SearchStockView;
class QVBoxLayout;
class SeachStockWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit SeachStockWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    void createWg();

    void searchFromServer(const QString &searchText);

    void seachrArrived(ReqId id, QVector<HZData::CodeInfo> codes, ErrCode errCode);

    void updateData(const QVector<HZData::CodeInfo> &codes);

    void clickUp();
    void clickDown();
    void clickReturn();

signals:
    void rowClicked(const QString &strCode);

private slots:
    void slotInput(const QString &strText);
    void onCloseBtnClicked();
    void onRowClicked(const QString &strCode);
private:
    QPushButton * m_closeBtn;
    QLineEdit *m_lineEdit;
    SearchStockView *m_searchView;
    QVBoxLayout *m_pMainLayout;
    QWidget *m_pWidget;
};

#endif // SEACHSTOCKWIDGET_H
