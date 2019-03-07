/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFOMASTER_H
#define INFOMASTER_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>

#include "anncinfo.h"
#include "hotinfo.h"
#include "hzinfonews.h"
#include "multihour.h"
#include "economicmaster.h"
#include "marketmaster.h"
#include "basepage.h"
#include "infopage_export.h"

class CSubTab;
class InfoHotProvidor;
class InfoMarketCalandarProvidor;
class InfoNewsProvidor;
class InfoNultiHourProvidor;
class InfoAnncProvidor;
class InfoEconomicCalandarProvidor;
class HZINFOPAGE_EXPORT InfoMaster : public BasePage
{
    Q_OBJECT
public:
    explicit InfoMaster(QWidget *parent = 0);

    static QVector<QString> mZXStocks;//自选股列表
    static void updateOfOptionalStocks();//updateOfOptionalStocks list

    void afterActive();
    void afterDeactive();

    //处理命令消息
    void recvParam(const DetailPageParam &param,bool bCurrPage = false);

signals:
    /**
     * @brief signalZXChanged 用户登录状态发生变化，自选股列表已更新
     */
    void signalZXChanged();

public slots:
    void slotBtnGroupClicked(int id);
    void slotStockClicked(const QString& code);
    void getCustomStock();

private:
    CSubTab* m_pTab;
    QStackedWidget *m_stackWnd;
    QButtonGroup *m_btnGroup;
    HotMarster *m_hotPage;
    AnncMarster *m_anncPage;
    NewsMarster *m_newsPage;
    MultiHourMarster *m_multiHourPage;
    EconomicMaster *m_economicPage;
    MarketMaster *m_marketPage;

    InfoHotProvidor *m_infoHotData;
    InfoMarketCalandarProvidor *m_marketCalandarData;
    InfoNewsProvidor *m_infoNewsData;
    InfoNultiHourProvidor *m_multiHourData;
    InfoAnncProvidor *m_infoAnncData;
    InfoEconomicCalandarProvidor *m_econonicData;

    /**
     * @brief marketCalandarInfoTimesCallBack
     * @param datas
     */
    void marketCalandarInfoTimesCallBack(const InfoRet &ret,const MarketCalandarTimes &datas);

    /**
     * @brief economicCalandarInfoTimesCallBack
     * @param ret
     * @param datas
     */
    void economicCalandarInfoTimesCallBack(const InfoRet &ret,const EconomicDataTimes &datas);
};

#endif // INFOMASTER_H
