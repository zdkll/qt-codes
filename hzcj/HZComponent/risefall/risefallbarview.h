#ifndef RISEFALLBARVIEW_H
#define RISEFALLBARVIEW_H

#include <QWidget>
#include "basewidget.h"
#include "hzcomponent_global.h"
#include "risefallbarchart.h"

class  RiseFallBarView : public BaseWidget
{
    Q_OBJECT
public:
    explicit RiseFallBarView(QWidget *parent = 0);

   void setData(const HZData::StockRiseFallInfo &risefallInfo);

   void afterActive();

   void afterDeactive();

public slots:
   void setStockCode(const QString &stockCode);

protected:
   void subRiseFallInfo();
   void cancelSub();

   void onSubRiseFallInfoArrived(ReqId reqId,const HZData::StockRiseFallInfo &riseFallInfo,ErrCode);

 private:
   RiseFallBarChart       *m_risefallBarChart;
   RiseFallLegendChart *m_risefallLegendChart;
   QWidget                    *m_legendRect,*m_graphRect;
   bool                            m_actived;
   ReqId                        m_subReqId;
};

#endif // RISEFALLBARVIEW_H
