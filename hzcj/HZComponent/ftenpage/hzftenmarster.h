#ifndef HZFTENMARSTER_H
#define HZFTENMARSTER_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QStackedWidget>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QMouseEvent>

#include <hzinfoadapter.h>
#include <datamodeldefs/datamodelpublic.h>
#include <datamodeldefs/stockinfodef.h>

//F10公司简介
#define F10_Company_Profile_Url "%1/f10Intro.html?stockcode=%2"

//主营构成
#define F10_Main_Industry_Url "%1/f10zygc.html?stockcode=%2"

//F10分红融资
#define F10_Bonus_Financing_Url "%1/f10Dividend.html?stockcode=%2"

//F10业绩分析
#define F10_Performance_Analysis_Url "%1/f10yjfx.html?stockcode=%2"

//行业对比
#define F10_Industry_Comparison_Url "%1/f10hydb.html?stockcode=%2"

//股东股本
#define F10_Shareholder_Equity_Url "%1/f10gdgb.html?stockcode=%2"

//机构持仓
#define F10_Institutional_Position_Url "%1/f10jgcc.html?stockcode=%2"


struct STFTenData
{
    struct TitleBar{
        QString title;
        QString url;
    };

    QVector<TitleBar> barsdata;
    QString title;
    QString stockcode;
    QString stockname;
    qreal now;//现价
    qreal change;//涨跌
    qreal chg;//涨跌幅
    qreal threechg;//三日涨幅
    qreal hs; //换手
    qreal amount ;//金额
};


class HZInfoWebPageMarster;

namespace Ui {
class HZFTenMarster;
}


class HZFTenStyle : public QObject
{
    Q_OBJECT
public:
    static HZFTenStyle *instance();

    QFont mStkNFont, mStkCFont, mOtherTxtFont, mNumFont;

    //up down no (文本样式)
    QPen mUpPen{QColor("#e62727")};
    QPen mDownPen{QColor("#0fc351")};
    QPen mNoUpDown{QColor("#cad4dc")};

    QPen mPressPn{QColor("#cad4dc")};
    QBrush mPressBr{QColor("#414141")};
    QPen mNormalPn{QColor("#cad4dc")};
    QBrush mNormalBr{QColor("#2e2e2e")};
    QPen mBorderPen{QColor("#000000")};

private:
    HZFTenStyle(QObject *parent=nullptr);
    Q_DISABLE_COPY(HZFTenStyle)

};

#define gFTenStyle HZFTenStyle::instance()


///
/// \brief The HZFTenDiscript class
///
class HZFTenDiscript : public QLabel
{
    Q_OBJECT
public:
    HZFTenDiscript(QWidget *parent=nullptr);
    ~HZFTenDiscript(){}

    void updateForUi(const STFTenData &dt);

protected:
    void paintEvent(QPaintEvent *);


private:
    STFTenData mDatas;
    QFont mSNFont, mSCFont, mNumFont, mOtherFont;
};

///
/// \brief The HZBtnGroupFrame class
///
class HZBtnGroupFrame : public QLabel
{
    Q_OBJECT
public:
    HZBtnGroupFrame(QWidget *parent=nullptr);
    ~HZBtnGroupFrame(){}

    void setDefaultItem(int index);
    void setGroupName(const QVector<STFTenData::TitleBar> &dt);

signals:
    void signalItemClicked(int index);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);

private:
    QVector<STFTenData::TitleBar> mDatas;
    QPoint mPressPos;
    bool mIsPressed{false};
    int mCurrentId{0};

    //clear
    void clearBar();
};

///
/// \brief The HZFTenMarster class
///
class HZFTenMarster : public QDialog
{
    Q_OBJECT

public:
    explicit HZFTenMarster(const QString &stockcode="", QWidget *parent = 0);
    ~HZFTenMarster();


    /**
     * @brief setStockCode
     * @param stockcode
     */
    void setStockCode(const QString &stockcode);

    /**
     * @brief updateForUi
     * @param dt
     */
    void updateForUi(const STFTenData &dt);

    /**
     * @brief showExec
     */
    void showExec(const QString &stockcode);


public slots:
    /**
     * @brief slotStackWidgetChanged stack changed
     * @param index current index
     */
    void slotStackWidgetChanged(int index);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::HZFTenMarster *ui;

    STFTenData mFunTenDt;
    HZFTenDiscript *mDiscript;
    HZBtnGroupFrame *mGrpFrame;
    HZInfoWebPageMarster *mWebView;

    bool m_dragWindow{false};
    QPoint m_mousePoint;

    QString mFTenIps{""};

    void updateStkWnd();
    void reqSnapShot(const QList<QString> &stocks);
    void reqSnapShotResponse(ReqId reqId, char *data, int num,  ErrCode err);
};

#endif // HZFTENMARSTER_H
