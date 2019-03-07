#ifndef SLRISEFALLWIDGET_H
#define SLRISEFALLWIDGET_H

#include "sltableview.h"

class  QSignalMapper;
//沪深A排行窗口
class   SLRiseFallWidget : public BaseWidget
{
    Q_OBJECT
public:
    SLRiseFallWidget(QWidget *parent = 0,QString instName="");

    void afterActive();

    void afterDeactive();

    void onFocusOut();

signals:
    void currentStockChanged();

protected slots:
    void slotCurrentViewChanged(int );
public slots:
    void slotItemSeleced(const QString &stockCode);
private:
    void createWg();

    SLTableView *createHSTableView(int flag,int sortColumn,Qt::SortOrder sortOrder);

    QLayout   *createLineLayout();

    QLayout *createColumnLayout(int flag,int column);

    QList<SLTableView*>   m_tableViews;
    QSignalMapper                *m_signalMapper;
    int                                     m_currenIdx;
    bool                                  m_focused;
};


#endif // SLRISEFALLWIDGET_H
