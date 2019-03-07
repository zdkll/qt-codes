#include "infostyle.h"
#include <QMap>
#include <pubmsgdef.h>

void InfoStyle::stockClicked(const QString &code)
{
    MessageParams params;
    params.emplace(HZ_CMD, ROOT_SWITCH_PAGE);
    params.emplace(HZ_PAGE_ID,QString::number(stock_detail_page));
    params.emplace(CHILD_WND_ID, QString::number(stock_detail_page));
    params.emplace(STOCK_ID,code);
    sendMessage(ROOT_FRAME,params);
}

InfoStyle::InfoStyle(QObject *parent)
    :QObject(parent),HZObject("")
{
    m_indexFont.setFamily("SimSun");
    m_indexFont.setPixelSize(12);

    /*无效文案字体*/
    m_invalidFont = m_indexFont;

    m_numberFont.setFamily("ArialMT");
    m_numberFont.setPixelSize(14);

    m_numberFontTwo.setFamily("ArialMT");
    m_numberFontTwo.setPixelSize(12);

    m_titleFont = QFont("SimSun");
    m_titleFont.setPixelSize(14);

    m_descripFont = m_titleFont;
    m_descripFont.setPixelSize(12);

    m_customSelFont = QFont("SimSun");
    m_customSelFont.setPixelSize(10);

    m_invalidFont.setFamily("SimSun");
    m_invalidFont.setPixelSize(18);

    m_dateTextFont.setFamily("Arial");
    m_dateTextFont.setPixelSize(12);

    m_dateScrollTextFont.setFamily("SimSun");
    m_dateScrollTextFont.setPixelSize(14);

    m_dateScrollNumTextFont.setFamily("ArialBold");
    m_dateScrollNumTextFont.setPixelSize(14);

    //股份日历
    m_marketCldDetailFont = m_titleFont;
    m_mkCldItemStockNameFont = m_marketCldDetailFont;

    //idx btn (tag btn)
    m_idxBtnPens.push_back(QPen(QColor("#4f9bff"),2));
    m_idxBtnPens.push_back(QPen(QColor("#c31818"),2));
    m_idxBtnPens.push_back(QPen(QColor("#c31818"),2));
    m_idxBtnPens.push_back(QPen(QColor("#f2a123"),2));
    m_idxBtnPens.push_back(QPen(QColor("#0baa36"),2));
    m_idxBtnPens.push_back(QPen(QColor("#0baa36"),2));

    m_idxBtnBrushs.push_back(QBrush("#4f9bff"));
    m_idxBtnBrushs.push_back(QBrush("#c31818"));
    m_idxBtnBrushs.push_back(QBrush("#c31818"));
    m_idxBtnBrushs.push_back(QBrush("#f2a123"));
    m_idxBtnBrushs.push_back(QBrush("#0baa36"));
    m_idxBtnBrushs.push_back(QBrush("#0baa36"));

    m_idxBtnTxtPens.push_back(QPen(QColor("#4f9bff"),1));
    m_idxBtnTxtPens.push_back(QPen(QColor("#c31818"),1));
    m_idxBtnTxtPens.push_back(QPen(QColor("#c31818"),1));
    m_idxBtnTxtPens.push_back(QPen(QColor("#f2a123"),1));
    m_idxBtnTxtPens.push_back(QPen(QColor("#0baa36"),1));
    m_idxBtnTxtPens.push_back(QPen(QColor("#0baa36"),1));

    gIconUrls=QHash<QString,QString>{
        {"today", ":/skin/default/img/infopage/today.png"}
    };
}
