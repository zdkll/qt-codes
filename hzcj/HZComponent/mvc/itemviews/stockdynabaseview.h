#ifndef STOCKDYNABASEVIEW_H
#define STOCKDYNABASEVIEW_H

#include "stockdynaprovider.h"
#include "frozentableview.h"
#include "viewstyleproperties.h"

#include "../itemmodels/baseitemmodel.h"
#include "hzcomponent_global.h"

#include "hzobject.h"

class  BaseSortFilterProxyModel;

//行情列表全图基类显示窗口
class  HZCOMPONENTSHARED_EXPORT StockDynaBaseView : public FrozenView,public HZObject, public ViewStyleProperties
{
    Q_OBJECT
    //声明属性
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)

public:
    StockDynaBaseView(const FrozenFlag &frozenFlag,QWidget *parent = 0);

    virtual ~StockDynaBaseView();

    // 列表激活和非激活
    void afterActive();
    void afterDeactive();


    //设置filterFlag 和filterDetail
    virtual void setFilterFlag(const int &filterFlag);
    virtual void setFilterDetail(const QString &filter_detail);
    virtual void setFilterInfo(const int &filterFlag,const QString &filter_detail);

    QString filterDetail(){return m_filterDetail;}

    virtual void onFocusOut();

    // 获取列表的filterFlag 和 filterDetail
    int filterFlag() const {return m_filterFlag;}
    QString filterDetail() const { return m_filterDetail;}

    //获取列表当前码表
    virtual QVector<HZData::StockCode> stockTable(){return QVector<HZData::StockCode>();}
signals:
    void itemSelected(const QString &stockCode);  //选中某股票
    void itemEntered(const QString &stockCode);   //双击或者Enter进入某股票

protected slots:
    void slotSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void slotCurrentRangeChanged();
    void slotModelReset();

    virtual void slotCurrentItem(const QModelIndex &index);//选中
    virtual void slotEnteredItem(const QModelIndex &index);//双击或者enter 进入

protected:
    void showEvent(QShowEvent *e);

    void keyPressEvent(QKeyEvent *e);

    virtual int stockDynaType(){return 0;}

    //isZXStock：是否是自选股,SortType:0，不自动排序，1全部自动排序
    void initTableView(AbstractBaseStockModel  *model,BaseStockDynaProvider *dataProvider,bool isZXStock = false,int sortType  = 0);

    //子类实现接口
    virtual void sortIndicatorChanged(const SortField & ,const bool & );
    virtual void modelReset(){}
    virtual void currentRangeChanged(int  start ,int  count );

    virtual void selectedItem(const QModelIndex &index);
    virtual void enteredItem(const QModelIndex &index);

private:
    void initItemDelegate();

protected:
    AbstractBaseStockModel      *m_dataModel;
    BaseSortFilterProxyModel     *m_sortModel;

    QVector<ColumnInfo>           m_columnInfos;

    //统一的数据管理
    BaseStockDynaProvider       *m_stkProvider;
private:
    SortIndicator      m_sortIndicator;

    int         m_filterFlag;
    QString m_filterDetail;
};


#endif // STOCKDYNABASEVIEW_H
