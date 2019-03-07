#include "hzfinancestyleinfo.h"
#include <QFont>

//const static QVector<QPen> gPens = {
//    QPen(QColor("#254264"),1),
//    QPen(QColor("#294f7c"),1),
//    QPen(QColor("#185c9b"),1),
//    QPen(QColor("#1466B0"),1)
//};

//const static QVector<QBrush> gBrushs = {
//    QBrush(QColor("#1b222a")),
//    QBrush(QColor("#1e3142")),
//    QBrush(QColor("#1c3e63")),
//    QBrush(QColor("#1b4c84")),
//};

HZFinanceStyleInfo *HZFinanceStyleInfo::instance()
{
    static HZFinanceStyleInfo instance;
    return &instance;
}

QVector<QPen> HZFinanceStyleInfo::quarterPens()
{
    return gPens;
}

QVector<QBrush> HZFinanceStyleInfo::quarterBrushs()
{
    return gBrushs;
}

QPen HZFinanceStyleInfo::textPen() const
{
    return QPen(QColor("#529efc"),1);
}

QPen HZFinanceStyleInfo::xTicksPen() const
{
    return QPen(QColor("#858585"),1);
}

QPen HZFinanceStyleInfo::yTicksPen() const
{
    return QPen(QColor("#c9d0d6"),1);
}

QPen HZFinanceStyleInfo::baseLinePen() const
{
    return QPen(QColor("#282828"),1);
}

QPen HZFinanceStyleInfo::ellipsePen() const
{
    return QPen(QColor("#ffac00"), 2);
}

QPen HZFinanceStyleInfo::linePen() const
{
    return QPen(QColor("#f29e08"), 1);
}

QPen HZFinanceStyleInfo::incomeEllipsePen() const
{
    return QPen(QColor("#ffa200"),2);
}

QBrush HZFinanceStyleInfo::incomeEllipseBrush() const
{
    return QBrush(QColor("#191919"));
}

QPen HZFinanceStyleInfo::profitTextPen() const
{
    return QPen(QColor("#539ffd"),1);
}

QPen HZFinanceStyleInfo::profitEllipsePen() const
{
    return QPen(QColor("#4f98f6"),2);
}

QBrush HZFinanceStyleInfo::profitEllipseBrush() const
{
    return QBrush(QColor("#191919"));
}

QPen HZFinanceStyleInfo::profitLinePen() const
{
    return QPen(QColor("#4f98f6"),1);
}

QPen HZFinanceStyleInfo::profitRectPen() const
{
    return QPen(QColor("#264468"),1);
}

QBrush HZFinanceStyleInfo::profitRectBrush() const
{
    return QBrush(QColor("#192028"));
}

HZFinanceStyleInfo::HZFinanceStyleInfo()
{
    mYAxisTxFont.setFamily("ArialMT");
    mYAxisTxFont.setPixelSize(11);

    mXAxisTxFont = mYAxisTxFont;
    mTipTxFont = mYAxisTxFont;

    mNumberTxFont.setFamily("ArialMT");
    mNumberTxFont.setPixelSize(12);

    mTextFont.setFamily("SimSun");
    mTextFont.setPixelSize(12);

    gPens.push_back(QPen(QColor("#254264"),1));
    gPens.push_back(QPen(QColor("#294f7c"),1));
    gPens.push_back(QPen(QColor("#185c9b"),1));
    gPens.push_back(QPen(QColor("#1466B0"),1));

    gBrushs.push_back(QBrush(QColor("#1b222a")));
    gBrushs.push_back(QBrush(QColor("#1e3142")));
    gBrushs.push_back(QBrush(QColor("#1c3e63")));
    gBrushs.push_back(QBrush(QColor("#1b4c84")));
}
