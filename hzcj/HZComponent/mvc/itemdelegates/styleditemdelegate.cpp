#include "styleditemdelegate.h"

QColor StockStyledItemDelegate::m_riseColor      = Qt::red;
QColor StockStyledItemDelegate::m_plainColor = Qt::white;
QColor StockStyledItemDelegate::m_fallColor      =  Qt::green;
QColor StockStyledItemDelegate::m_normalColor = Qt::white;

QFont   StockStyledItemDelegate::m_digitalFont = QFont("Arial");
QFont   StockStyledItemDelegate::m_charFont = QFont("Arial");

bool StockStyledItemDelegate::m_isInitedStyleProperties = false;

///NormalStyledItemDelegate-----
QColor NormalStyledItemDelegate::m_riseColor      = Qt::red;
QColor NormalStyledItemDelegate::m_plainColor = Qt::white;
QColor NormalStyledItemDelegate::m_fallColor      =  Qt::green;
QColor NormalStyledItemDelegate::m_normalColor = Qt::white;

QFont   NormalStyledItemDelegate::m_digitalFont = QFont("Arial");
QFont   NormalStyledItemDelegate::m_charFont = QFont("Arial");

bool      NormalStyledItemDelegate::m_isInitedStyleProperties = false;

///HandStyledItemDelegate--
QFont   HandStyledItemDelegate::m_digitalFont = QFont("Arial");
QFont   HandStyledItemDelegate::m_charFont    = QFont("Arial");
bool      HandStyledItemDelegate::m_isInitedStyleProperties = false;

QColor  HandStyledItemDelegate::m_riseColor   = QColor("#ff0000");
QColor  HandStyledItemDelegate::m_plainColor = QColor("#cad4dc");
QColor  HandStyledItemDelegate::m_fallColor    = QColor("#0fc351");
QColor  HandStyledItemDelegate::m_normalColor = QColor("#cad4dc");

QColor  HandStyledItemDelegate::m_gridLineColor= QColor("#2e2e2e"); //网格线颜色
QColor  HandStyledItemDelegate::m_bpColorFirst=QColor("#0fc351"); //其他备用颜色

///NoFocusStyledItemDelegate-----
void NoFocusStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;
    if(option.state & QStyle::State_HasFocus)
        myOption.state ^= QStyle::State_HasFocus;
    return QStyledItemDelegate::paint(painter,myOption,index);
}


////StockStyledItemDelegate----
StockStyledItemDelegate::StockStyledItemDelegate(QObject *parent)
    :NoFocusStyledItemDelegate(parent)
    ,m_alignment(Qt::AlignVCenter|Qt::AlignRight)
    ,m_rightMargin(Col_Margin),m_leftMargin(Col_Margin)
{

}

void StockStyledItemDelegate::initStyleProperties(const StyleProperties &properties)
{
    m_riseColor = properties.riseColor;
    m_plainColor = properties.plainColor;
    m_fallColor = properties.fallColor;
    m_normalColor = properties.normalColor;

    m_digitalFont.setPixelSize(13);
    m_digitalFont.setBold(true);
    m_digitalFont.setStyleStrategy(QFont::NoAntialias);

    m_charFont.setPixelSize(14);

    m_isInitedStyleProperties = true;
}


///HandStyledItemDelegate---
void HandStyledItemDelegate::initStyleProperties(const StyleProperties &properties)
{
    m_riseColor   = properties.riseColor;
    m_plainColor = properties.plainColor;
    m_fallColor    = properties.fallColor;
    m_normalColor = properties.normalColor;
    m_gridLineColor = properties.gridLineColor;
    m_bpColorFirst = properties.bpColorFirst;
    //字体固定
    m_digitalFont.setPixelSize(13);
}

///NormalStyledItemDelegate---
void NormalStyledItemDelegate::initStyleProperties(const StyleProperties &properties)
{
    m_riseColor   = properties.riseColor;
    m_plainColor = properties.plainColor;
    m_fallColor    = properties.fallColor;
    m_normalColor = properties.normalColor;

    m_isInitedStyleProperties = true;
}




