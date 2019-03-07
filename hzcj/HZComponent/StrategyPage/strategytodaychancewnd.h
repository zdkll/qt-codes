/********************************************************************
created:
author: min.cheng  juntang.xie
purpose: 用于策略今日机会列表展示
*********************************************************************/
#ifndef STRATEGYTODAYCHANCEWND_H
#define STRATEGYTODAYCHANCEWND_H


#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QListView>

#include "basewidget.h"
#include "hzinfodata.h"
#include "quoteformat.h"

enum ChanceType
{
    TodayChanceType = 0,
    LongTermChanceType
};

class StrategyTodayChanceWnd;
class ViewTipsPanel;
class QLabel;
//数据代理
class TodayChanceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TodayChanceDelegate(StrategyTodayChanceWnd* pStyle,QObject *parent = 0);
    ~TodayChanceDelegate();
protected:
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    StrategyTodayChanceWnd* m_style;
};

struct TodayChanceDynaData
{
    QString stockName;
    QString stockCode;
    QString strRate;
    HzQuoteFormat::QuoteItem rateItem;
};

struct TodayChanceItem
{
    QString topic_id;
    QString codeName;
    QString strTime;
    QString desc;
    QVector<TodayChanceDynaData> relationStocks;
};
Q_DECLARE_METATYPE(TodayChanceItem)

//模型
//class TodayChanceModel : public QAbstractListModel
//{
//    Q_OBJECT
//public:
//    TodayChanceModel(int type, QObject *parent=nullptr);
//    ~TodayChanceModel();

//    int rowCount(const QModelIndex &parent = QModelIndex()) const;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//};

//今日机会窗口
class DynaDataProvider;
class StrategyTodayChanceWnd :public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QString m_codeNameFontFamily READ getCodeNameFontFamily WRITE setCodeNameFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_codeNameFontSize READ getCodeNameFontSize WRITE setCodeNameFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_codeNameTextClr READ getCodeNameTextClr WRITE setCodeNameTextClr DESIGNABLE true)

    Q_PROPERTY(QString m_descTextFontFamily READ getDescTextFontFamily WRITE setDescTextFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_descTextFontSize READ getDescTextFontSize WRITE setDescTextFontSize DESIGNABLE true)
    Q_PROPERTY(QColor m_descTextTextClr READ getDescTextTextClr WRITE setDescTextTextClr DESIGNABLE true)

    Q_PROPERTY(QString m_dynaTextFontFamily READ getDynaTextFontFamily WRITE setDynaTextFontFamily DESIGNABLE true)
    Q_PROPERTY(int m_dynaTextFontSize READ getDynaTextFontSize WRITE setDynaTextFontSize DESIGNABLE true)

    Q_PROPERTY(QColor m_upClr READ getUpClr WRITE setUpClr DESIGNABLE true)
    Q_PROPERTY(QColor m_downClr READ getDownClr WRITE setDownClr DESIGNABLE true)
    Q_PROPERTY(QColor m_equalClr READ getEqualClr WRITE setEqualClr DESIGNABLE true)

    Q_PROPERTY(QColor m_itemSelectedClr READ getItemSelectedClr WRITE setItemSelectedClr DESIGNABLE true)
    Q_PROPERTY(QColor m_itemHoverClr READ getItemHoverClr WRITE setItemHoverClr DESIGNABLE true)

    Q_PROPERTY(QColor m_itemBorderClr READ getItemBorderClr WRITE setItemBorderClr DESIGNABLE true)
public:
    explicit StrategyTodayChanceWnd(QWidget *parent = 0, ChanceType type = TodayChanceType);

    void setDynaProvider(DynaDataProvider* pProvider);

    void afterActive();

    void afterDeactive();

    void setTitleText(const QString &strText);

    void selectFirstRow();

    void clearSelect();

    //策略名称
    QString getCodeNameFontFamily() const{return m_codeNameFontFamily;}
    void setCodeNameFontFamily(QString family){m_codeNameFontFamily=family;}
    QString m_codeNameFontFamily = "Arial";

    //策略名称字体高度
    int getCodeNameFontSize() const{return m_codeNameFontSize;}
    void setCodeNameFontSize(int size){m_codeNameFontSize=size;}
    int m_codeNameFontSize = 11;

    //策略名称文本颜色
    QColor getCodeNameTextClr() const{return m_codeNameTextClr;}
    void setCodeNameTextClr(QColor color){m_codeNameTextClr=color;}
    QColor m_codeNameTextClr = QColor(Qt::red);

    //描述文本字体
    QString getDescTextFontFamily() const{return m_descTextFontFamily;}
    void setDescTextFontFamily(QString family){m_descTextFontFamily=family;}
    QString m_descTextFontFamily = "Arial";

    //描述文本字体高度
    int getDescTextFontSize() const{return m_descTextFontSize;}
    void setDescTextFontSize(int size){m_descTextFontSize=size;}
    int m_descTextFontSize = 11;

    //描述文本颜色
    QColor getDescTextTextClr() const{return m_descTextTextClr;}
    void setDescTextTextClr(QColor color){m_descTextTextClr=color;}
    QColor m_descTextTextClr = QColor(Qt::red);


    //行情文本字体
    QString getDynaTextFontFamily() const{return m_dynaTextFontFamily;}
    void setDynaTextFontFamily(QString family){m_dynaTextFontFamily=family;}
    QString m_dynaTextFontFamily = "Arial";

    //行情文本字体高度
    int getDynaTextFontSize() const{return m_dynaTextFontSize;}
    void setDynaTextFontSize(int size){m_dynaTextFontSize=size;}
    int m_dynaTextFontSize = 11;

    //上涨颜色
    QColor getUpClr() const{return m_upClr;}
    void setUpClr(QColor color){m_upClr=color;}
    QColor m_upClr = QColor(Qt::red);

    //下跌颜色
    QColor getDownClr() const{return m_downClr;}
    void setDownClr(QColor color){m_downClr=color;}
    QColor m_downClr = QColor(Qt::red);

    //平局颜色  （不涨不跌）
    QColor getEqualClr() const{return m_equalClr;}
    void setEqualClr(QColor color){m_equalClr=color;}
    QColor m_equalClr = QColor(Qt::red);

    //代理选中颜色
    QColor getItemSelectedClr() const{return m_itemSelectedClr;}
    void setItemSelectedClr(QColor color){m_itemSelectedClr=color;}
    QColor m_itemSelectedClr = QColor(Qt::red);

    //代理高亮颜色
    QColor getItemHoverClr() const{return m_itemHoverClr;}
    void setItemHoverClr(QColor color){m_itemHoverClr=color;}
    QColor m_itemHoverClr = QColor(Qt::red);

    //代理边框颜色
    QColor getItemBorderClr() const{return m_itemBorderClr;}
    void setItemBorderClr(QColor color){m_itemBorderClr=color;}
    QColor m_itemBorderClr = QColor(Qt::red);
signals:
    void signalsClickId(QString topic_id,QString codeName);
    void updateReady();
public slots:
    void slotsTimer();
    void slotsClicked(const QModelIndex &index);
private:
    void onTopicInfoCallBack(const InfoRet& ret,const TopicInvest& data);
    void createWg();
    QWidget* createListView();
    void cancleTopicReq();
private:
    DynaDataProvider* m_pProvider = nullptr;
    int m_reqTopicId = -1;
    QListView* m_pTodyChanceView = nullptr;
    ViewTipsPanel* m_pInfoTipsPanel = nullptr;
    QLabel *m_pTitleLabel;
    QVector<TodayChance> m_chanceDataVec;
    ChanceType m_chanceType;
};

#endif // STRATEGYTODAYCHANCEWND_H
