#ifndef SLBASETABLEVIEW_H
#define SLBASETABLEVIEW_H


#include <QTableView>
#include "hzobject.h"
#include "baseitemmodel.h"
#include "viewstyleproperties.h"
#include "hzcomponent_global.h"

class BaseSortFilterProxyModel;
class BaseStockDynaProvider;
class AbstractBaseStockModel;
class  SLBaseTableview :  public QTableView,public HZObject, public ViewStyleProperties
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    SLBaseTableview(QWidget *parent = 0);
    virtual ~SLBaseTableview();

    //激活和非激活
    void afterActive();
    void afterDeactive();

    //过滤市场类型
    int filterFlag() const {return m_filterFlag;}
    virtual void  setFilterFlag(const int &filterFlag);

    void onFocusOut();

    void setAutoSelected(bool enable){m_isAutoSelected = enable;}
    bool isAutoSelected(){return m_isAutoSelected;}

signals:
    void itemEntered(const QString &stockCode);
    void itemSelected(const QString &stockCode);

protected:
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void currentChanged(const QModelIndex &current,
                        const QModelIndex &previous) Q_DECL_OVERRIDE;

protected slots:
    void slotCurrentRangeChanged();
    void slotModelReset();
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

    //子类自定义函数
    virtual void currentRangeChanged(int start,int count);

    virtual void sortIndicatorChanged(const SortField &sortField,const bool &desc);

    virtual void slotEnteredItem(const QModelIndex &index);

    virtual void modelReset(){}

    //sortType: 0 ,表示不自动排序，1表示全部自动排序
    void initTableView(AbstractBaseStockModel *model,int sortType = 0);

private:
    void initItemDelegate(const QVector<ColumnInfo> &columnInfos);

protected:
    QVector<ColumnInfo>             m_columnInfos;
    BaseStockDynaProvider         *m_stkProvider;

    AbstractBaseStockModel        *m_dataModel;
    BaseSortFilterProxyModel       *m_sortModel;
    //是否自动选中第一条
    bool                                       m_isAutoSelected;
private:
    SortIndicator                          m_sortIndicator;
    int                                          m_filterFlag;
};


#endif // SLBASETABLEVIEW_H
