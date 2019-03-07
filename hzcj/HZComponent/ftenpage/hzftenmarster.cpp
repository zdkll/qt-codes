#include "hzftenmarster.h"
#include "ui_hzftenmarster.h"
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QMouseEvent>
#include <QWebEngineProfile>

#include "infowebview.h"
#include "quoteformat.h"

#include <httpdata.h>
#include <hzinfocenter.h>
#include <datacontroller.h>

const static QList<STFTenData::TitleBar> gTemplates= {
    {QStringLiteral("公司简介"), F10_Company_Profile_Url},
    {QStringLiteral("主营构成"), F10_Main_Industry_Url},
    {QStringLiteral("业绩分析"), F10_Performance_Analysis_Url},
    {QStringLiteral("行业对比"), F10_Industry_Comparison_Url},
    {QStringLiteral("股东股本"), F10_Shareholder_Equity_Url},
    {QStringLiteral("机构持仓"), F10_Institutional_Position_Url},
    {QStringLiteral("分红融资"), F10_Bonus_Financing_Url}
};


///
/// \brief HZFTenStyle::HZFTenStyle
/// \param parent
///
HZFTenStyle::HZFTenStyle(QObject *parent)
    :QObject(parent)
{
     mStkNFont.setFamily("SimSun");
     mStkNFont.setPixelSize(16);
     mStkNFont.setStyleStrategy(QFont::NoAntialias);
     mStkNFont.setBold(true);

     mStkCFont.setFamily("Arial");
     mStkCFont.setPixelSize(16);
     mStkNFont.setStyleStrategy(QFont::NoAntialias);
     mStkNFont.setBold(true);

     mOtherTxtFont.setFamily("SimSun");
     mOtherTxtFont.setPixelSize(14);

     mNumFont.setFamily("Arial");
     mNumFont.setPixelSize(14);
}

HZFTenStyle *HZFTenStyle::instance()
{
    static HZFTenStyle inst;
    return &inst;
}


///
/// \brief HZFTenDiscript::HZFTenDiscript
/// \param parent
///
HZFTenDiscript::HZFTenDiscript(QWidget *parent)
    :QLabel(parent)
{
    setFixedHeight(54);
}

void HZFTenDiscript::updateForUi(const STFTenData &dt)
{
    mDatas = dt;
    update();
}

void HZFTenDiscript::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter p(this);
    p.fillRect(rect().adjusted(0,0,0,-2), QColor("#222222"));

    QRect des(rect().adjusted(0,0,0,-2));
    int xpos = 26;
    //stock name
    p.setFont(gFTenStyle->mStkNFont);
    int stockNameW = p.fontMetrics().width(mDatas.stockname);
    des.setX(xpos);
    des.setWidth(stockNameW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, mDatas.stockname);
    xpos+=(stockNameW+10); // space = 10

    //stock code
    p.setFont(gFTenStyle->mStkCFont);
    int stockCodeW = p.fontMetrics().width(mDatas.stockcode)+5;
    des.setX(xpos);
    des.setWidth(stockCodeW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, mDatas.stockcode);
    xpos += (stockCodeW + 20); //space = 20

    //最新价
    p.setFont(gFTenStyle->mOtherTxtFont);
    bool isValid = mDatas.now != DBL_MAX;
    QString nowTitle(QStringLiteral("最新价:"));
    QString nowTxt(isValid?QString::number(mDatas.now,'f',2):"--");
    int nowW = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    QPen nowPen = isValid?(mDatas.change>0.?gFTenStyle->mUpPen:mDatas.change <0.?gFTenStyle->mDownPen:gFTenStyle->mNoUpDown):(gFTenStyle->mNoUpDown);
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;

    //涨跌
    p.setFont(gFTenStyle->mOtherTxtFont);
    isValid = mDatas.change != DBL_MAX;
    nowTitle = QStringLiteral("涨跌:");
    nowTxt = isValid?QString::number(mDatas.change,'f',2):"--";
    nowW = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    nowPen = isValid?(mDatas.change>0.?gFTenStyle->mUpPen:mDatas.change <0.?gFTenStyle->mDownPen:gFTenStyle->mNoUpDown):(gFTenStyle->mNoUpDown);
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;

    //涨跌幅
    p.setFont(gFTenStyle->mOtherTxtFont);
    isValid = mDatas.change != DBL_MAX;
    nowTitle= QStringLiteral("涨跌幅:");
    nowTxt = isValid?QString::number(mDatas.chg,'f',2)+"% ":"--";
    nowW = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    nowPen = isValid?(mDatas.chg>0.?gFTenStyle->mUpPen:mDatas.chg <0.?gFTenStyle->mDownPen:gFTenStyle->mNoUpDown):(gFTenStyle->mNoUpDown);
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;

    //3日涨跌幅
    p.setFont(gFTenStyle->mOtherTxtFont);
    isValid = mDatas.change != DBL_MAX;
    nowTitle = QStringLiteral("3日涨跌幅:");
    nowTxt = isValid?QString::number(mDatas.threechg,'f',2)+"% ":"--";
    nowW = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    nowPen = isValid?(mDatas.threechg>0.?gFTenStyle->mUpPen:mDatas.threechg <0.?gFTenStyle->mDownPen:gFTenStyle->mNoUpDown):(gFTenStyle->mNoUpDown);
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;

    //还手
    p.setFont(gFTenStyle->mOtherTxtFont);
    isValid = mDatas.change != DBL_MAX;
    nowTitle= QStringLiteral("换手:");
    nowTxt = isValid?QString::number(mDatas.hs,'f',2)+"% ":"--";
    nowW   = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    nowPen = gFTenStyle->mNoUpDown;
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;

    //金额
    p.setFont(gFTenStyle->mOtherTxtFont);
    isValid = mDatas.change != DBL_MAX;
    nowTitle=QStringLiteral("金额:");
    if(isValid) {
        nowTxt = HzQuoteFormat::StringUnitFormat::toFormatString(mDatas.amount, 2);
    } else {
        nowTxt = "--";
    }

    nowW = p.fontMetrics().width(nowTxt)+5, nowTitleW = p.fontMetrics().width(nowTitle);
    des.setX(xpos);
    des.setWidth(nowTitleW);
    p.setPen(gFTenStyle->mNormalPn);
    p.drawText(des, Qt::AlignCenter, nowTitle);
    xpos += (nowTitleW + 4);
    des.setX(xpos);
    des.setWidth(nowW);
    nowPen = gFTenStyle->mNoUpDown;
    p.setPen(nowPen);
    p.setFont(gFTenStyle->mNumFont);
    p.drawText(des, Qt::AlignCenter, nowTxt);
    xpos += (nowW + 20);//space = 20;
}

///
/// \brief HZBtnGroupFrame::HZBtnGroupFrame
/// \param parent
///
HZBtnGroupFrame::HZBtnGroupFrame(QWidget *parent)
    :QLabel(parent)
    ,mPressPos(QPoint(-1,-1))
{
    setFixedHeight(26);
}

void HZBtnGroupFrame::setDefaultItem(int index)
{
    if(mDatas.isEmpty() || index <0 || index > mDatas.count()-1) return;
    QRect rt(rect().adjusted(0,0,0,-1));
    int textW(fontMetrics().width(mDatas[index].title));
    rt.setWidth(textW+26);
    mPressPos = rt.center();
    mIsPressed = true;
}

void HZBtnGroupFrame::setGroupName(const QVector<STFTenData::TitleBar> &dt)
{
    clearBar();
    mDatas = dt;
    update();
}

void HZBtnGroupFrame::clearBar()
{
    mDatas.clear();
    update();
}

void HZBtnGroupFrame::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect().adjusted(0,0,0,-2), gFTenStyle->mNormalBr);

    p.save();
    int pos = 0;
    QRect tmpRt;
    for(int i=0; i<mDatas.count(); ++i) {
        QRect rt(rect().adjusted(0,0,0,-1));
        int textW(p.fontMetrics().width(mDatas[i].title));
        rt.adjust(pos, 0, 0, 0);
        rt.setWidth(textW+26);

        p.setPen(gFTenStyle->mNormalPn);
        p.fillRect(rt, gFTenStyle->mNormalBr);
        p.drawText(rt, Qt::AlignCenter, mDatas[i].title);
        if(i == mCurrentId)tmpRt = rt.adjusted(0,0,-1,0);
        if((rt.contains(mPressPos) && mIsPressed)) {
            mCurrentId = i;
            tmpRt = rt.adjusted(0,0,-1,0);
            emit signalItemClicked(mCurrentId);
            mIsPressed = false;
        }

        p.setPen(gFTenStyle->mBorderPen);
        p.drawLine(rt.topRight(), rt.bottomRight());
        pos += textW+26;
    }

    if((mCurrentId >= 0 && mCurrentId < mDatas.count())) {
        p.setPen(gFTenStyle->mPressPn);
        p.fillRect(tmpRt, gFTenStyle->mPressBr);
        p.drawText(tmpRt, Qt::AlignCenter, mDatas[mCurrentId].title);
    }

    p.restore();
}

void HZBtnGroupFrame::mousePressEvent(QMouseEvent *ev)
{
    mIsPressed = true;
    mPressPos = ev->pos();
    update();
}

///
/// \brief HZFTenMarster::HZFTenMarster
/// \param parent
///
HZFTenMarster::HZFTenMarster(const QString &stockcode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HZFTenMarster)
  ,mFTenIps(gHzInfoCenter->getInfoH5Ip())
  ,mWebView(new HZInfoWebPageMarster)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    mDiscript = new HZFTenDiscript;
    mDiscript->setObjectName("HZFTenDiscript");
    ui->vl_disFrame->addWidget(mDiscript);

    mGrpFrame = new HZBtnGroupFrame;
    mGrpFrame->setObjectName("HZBtnGroupFrame");
    ui->vl_btngrp->addWidget(mGrpFrame);

    connect(ui->btn_cls, &QPushButton::clicked, this, &HZFTenMarster::close);
    connect(mGrpFrame, &HZBtnGroupFrame::signalItemClicked, this, &HZFTenMarster::slotStackWidgetChanged);

    HZInfoWebPage *mWebPage = new HZInfoWebPage(PopWindow, QWebEngineProfile::defaultProfile());
    mWebView->setWebPage(mWebPage);
    ui->vl_content->addWidget(mWebView);

    resize(1000,800);

    //qDebug() << "FTen Ips:" << mFTenIps;
    setStockCode(stockcode);

//    connect(ui->pushButton, &QPushButton::clicked, [=]{
//        QString urlStr = ui->lineEdit->text();
//        if(!urlStr.isEmpty()) {
//            mWebView->setUrl(QUrl::fromUserInput(urlStr));
//        }
//    });
}

HZFTenMarster::~HZFTenMarster()
{
    delete ui;
}

void HZFTenMarster::setStockCode(const QString &stockcode)
{
    if(mFTenIps.isEmpty()) {
        qDebug() << Q_FUNC_INFO << " f10 address is invalid! ";
        return;
    }

    QList<QString> vecs;
    vecs.append(stockcode);
    reqSnapShot(vecs);

    STFTenData dt;
    dt.stockcode = stockcode;
    dt.stockname = Httpdata::instance()->getCodeNameByCode(stockcode);
    dt.title    = QString("%1(%2)").arg(dt.stockname).arg(dt.stockcode.mid(2))+"-F10";
    dt.now      = DBL_MAX;
    dt.hs       = DBL_MAX;
    dt.change   = DBL_MAX;
    dt.chg      = DBL_MAX;
    dt.amount   = DBL_MAX;
    dt.threechg = DBL_MAX;

    QVector<STFTenData::TitleBar> bars;
    for(int i=0; i<gTemplates.count(); ++i) {
        STFTenData::TitleBar tb;
        QString tmpUrl  = gTemplates[i].url;
        tb.title        = gTemplates[i].title;
        tb.url          = tmpUrl.arg(mFTenIps).arg(dt.stockcode);
        bars.push_back(tb);
    }

    dt.barsdata = bars;
    updateForUi(dt);
    updateStkWnd();
}

void HZFTenMarster::updateForUi(const STFTenData &dt)
{
    mFunTenDt = dt;
    mDiscript->updateForUi(dt);
    ui->lb_title->setText(dt.title);
}

void HZFTenMarster::showExec(const QString &stockcode)
{
    if(!isVisible())
        setVisible(true);

    setStockCode(stockcode);
    exec();
    hide();
}

void HZFTenMarster::slotStackWidgetChanged(int index)
{
    mWebView->setUrl(mFunTenDt.barsdata[index].url);
}

void HZFTenMarster::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && ui->f_titlebar->rect().contains(e->pos())) {
        m_mousePoint = e->globalPos() - pos();
        e->accept();
        m_dragWindow = true;
    }
}

void HZFTenMarster::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragWindow && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - m_mousePoint);
        e->accept();
    }
}

void HZFTenMarster::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragWindow = false;
}

void HZFTenMarster::updateStkWnd()
{
    QVector<STFTenData::TitleBar> bars(mFunTenDt.barsdata);
    mGrpFrame->setGroupName(bars);
    mGrpFrame->setDefaultItem(0);
}

void HZFTenMarster::reqSnapShot(const QList<QString> &stocks)
{
    std::list<std::string> stdlst;
    for(int i=0; i<stocks.count(); ++i) stdlst.push_back(stocks[i].toStdString());
    if(stdlst.size() == 0) return;
    HZData::ReqOptionalDynaInfo req;
    req.stock_code = stdlst;
    req.mask = HZData::Stock_Mask;
    auto fun = std::bind(&HZFTenMarster::reqSnapShotResponse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    DataController::instance()->requestOptionalDyna(req, fun);
}

void HZFTenMarster::reqSnapShotResponse(ReqId reqId, char *data, int num, ErrCode err)
{
    if(err != 0) {
        qDebug() << Q_FUNC_INFO << "error:" << err;
        return;
    }

    HZData::Stock *slStocls = (HZData::Stock *)data;
    if(slStocls) {
        for(int i=0; i<num; i++){
            QString stockcode = QString::fromStdString(slStocls[i].stock_code);
            if(stockcode == mFunTenDt.stockcode) {
                mFunTenDt.stockcode = stockcode.mid(2);
                mFunTenDt.stockname = QString::fromStdString(slStocls[i].stock_name);
                mFunTenDt.now       = slStocls[i].last_price;
                mFunTenDt.change    = slStocls[i].up_down_price;
                mFunTenDt.chg       = slStocls[i].rate;
                mFunTenDt.hs        = slStocls[i].turnover_rate;
                mFunTenDt.threechg  = slStocls[i].three_rate==INVALID_DOUBLE?INVALID_DOUBLE:slStocls[i].three_rate;
                //qDebug() << "threechg:" << mFunTenDt.threechg << slStocls[i].three_rate;
                mFunTenDt.amount    = slStocls[i].turnover;
            }
        }
    }

    mDiscript->updateForUi(mFunTenDt);
}

