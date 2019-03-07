#ifndef HANDTABLEVIEW_H
#define HANDTABLEVIEW_H

#include <QTableView>
#include "hzobject.h"
#include "baseitemmodel.h"
#include "viewstyleproperties.h"
#include "hzcomponent_global.h"

class AbstractBaseModel;
class HandDataProvider;
class  HandTableView : public QTableView, public HZObject,public HandStyledProperities
{
    Q_OBJECT
    Q_PROPERTY(QColor riseColor READ riseColor WRITE setRiseColor)
    Q_PROPERTY(QColor plainColor READ plainColor WRITE setPlainColor)
    Q_PROPERTY(QColor fallColor READ fallColor WRITE setFallColor)

    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)
    Q_PROPERTY(QColor gridLineColor READ gridLineColor WRITE setGridLineColor)

    Q_PROPERTY(QColor bpColorFirst READ bpColorFirst WRITE setBpColorFirst)
public:
    HandTableView(QWidget *parent = 0);
    virtual ~HandTableView();

    void afterActive();
    void afterDeactive();

protected slots:
  void  slotCurrentRangeChanged();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent    *e);
    void initTableView(AbstractBaseModel *model,HandDataProvider *provider);

    virtual void initItemDelegates() = 0;
    virtual void currentRangeChanged(int ,int){}
protected:
    AbstractBaseModel    *m_dataModel;
    HandDataProvider      *m_dataProvider;
    QVector<ColumnInfo>  m_columnInfos;

    static  bool m_isInitialedStyledProperties;

private:
    int m_rowCount;
};

#endif // HANDTABLEVIEW_H
