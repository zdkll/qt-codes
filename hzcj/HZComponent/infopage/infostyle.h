/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFOSTYLE_H
#define INFOSTYLE_H

#include <QtCore>

#include <QPen>
#include <QBrush>
#include <QFont>
#include <QObject>
#include <hzobject.h>

#include "infodef.h"

///
/// \brief The HotInfoStyle class
///
class InfoStyle : public QObject, public HZObject
{
    Q_OBJECT
public:
    static InfoStyle *instance()
    {
        static InfoStyle inst;
        return &inst;
    }

    //up down no (文本样式)
    QPen m_upPen{QColor("#e62727")};
    QPen m_downPen{QColor("#0fc351")};
    QPen m_noUpDown{QColor("#cad4dc")};

    //up down no (外边框样式)
    QPen m_upBrPen{QColor("#9a1f1f")};
    QPen m_downBrPen{QColor("#127f3a")};
    QPen m_noUpDownBrPen{QColor("#797979")};

    //up down no (填充样式)
    QBrush m_upBrush{QColor("#350c0f")};
    QBrush m_downBrush{QColor("#0b2e18")};
    QBrush m_noUpDownBrush{QColor("#2f2f2f")};

    //font
    QFont m_indexFont, m_titleFont, m_descripFont,
        m_filterFont, m_numberFont, m_numberFontTwo,m_customSelFont,
        m_invalidFont;

    //日历按钮
    QFont m_dateTextFont;
    QColor m_dateSelClr{QColor("#ffffff")};
    QColor m_dateArrowSelClr{QColor("#4f9bff")};

    //日历滚动栏
    QFont m_dateScrollTextFont,m_dateScrollNumTextFont;
    QColor m_dateScrollClr{"#adadad"};
    QColor m_dateScrollSelClr{"#ffffff"};


    //股份日历
    QColor m_calandarPen{"#222222"};
    QFont m_marketCldDetailFont, m_boardFont;
    QFont m_mkCldItemStockNameFont;
    QPen m_mkCldItemStockNamePen{QColor("#ffffff")};
    QPen m_marketCldDetailPen{QColor("#feffff")};
    QBrush m_marketCldDetailBrush{QColor("#191919")};

    QPen m_mkCldItemHoverPen{QColor("#3366a6")};
    QBrush m_mkCldItemHoverBrush{QColor("#1b406b")};
    QPen m_mkCldItemNormalPen{QColor("#414141")};
    QBrush m_mkCldItemNormalBrush{QColor("#222222")};

    //无效文案画笔
    QPen m_invalidPen{QColor("#858585")};
    QBrush m_invalidBrush{QColor("#191919")};//#333333

    //pen
    QPen m_iBtnNormalBorderPen{QPen(QColor("#414141"),2)};
    QPen m_iBtnHoverBorderPen{QPen(QColor("#509cfe"),2)};
    QPen m_iBtnNormalTextPen{QColor("#cad4de")};
    QPen m_iBtnHoverTextPen{QColor("#509cfe")};

    QPen m_hLinePen{QColor("#414141")};
    QPen m_titlePen{QColor("#fffffd")};
    QPen m_descriptPen{QColor("#aeaeae")};
    QPen m_customSelPen{QColor("#4f9aff")};

    //鼠标悬停
    QBrush m_itemMousePressBrush{QColor("#191919")};
    QBrush m_itemMouseHoverBrush{QColor("#191919")};
    QBrush m_itemNormalBrush{QColor("#222222")};

    //web background
    QColor m_webPopWndBg{QColor("#ffffff")};
    QColor m_webInsertWndBg{QColor("#191919")};
    QHash<QString,QString> gIconUrls;//文本与国家图标之间的映射

    //index btn  QMap<int,STBtnStyle> tagStyles; tag标签与其样式之间的映射
    QVector<QPen> m_idxBtnPens;
    QVector<QBrush> m_idxBtnBrushs;
    QVector<QPen> m_idxBtnTxtPens;
    QBrush m_idxBtnTxtBrush{QColor("#191919")};

    /*7*24*/
    QFont m_multiTitleFont;

    /*经济日历 重大会议*/
    QPen m_meetTitle{QColor("#cad4dc")};
    QColor m_mllipsePen{"#adadad"};
    QBrush m_ellipseBrush{QColor("#0e0e0e")};

    void stockClicked(const QString &code);

signals:
    /**
     * @brief signalStockClicked 跳转详情页
     * @param code
     */
    void signalStockClicked(const QString &code);

    /**
     * @brief signalWebAboutBlank 异常情况，请求浏览器数据
     *  about:blank
     */
    void signalWebAboutBlank();

    /**
     * @brief signalAddCustomStock 更新自选股缓存列表
     */
    void signalUpdateCustomStockCache();

    /**
     * @brief signalFinaoReportDetail 财报/重大会议详情页
     * @param url
     */
    void signalFinaoReportDetail(const QString &title, const QString &url);



private:
    InfoStyle(QObject *parent=nullptr);
};

#define gInfoStyle InfoStyle::instance()
#endif // INFOSTYLE_H
