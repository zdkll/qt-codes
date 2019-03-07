/********************************************************************
created:2019.1
author: jdxchina
purpose: 抽象数据模型定义
*********************************************************************/
#ifndef WEEKLYMODEL_H
#define WEEKLYMODEL_H

#include <QtCore>
#include <QtGlobal>
#include <QAbstractItemModel>
#include <QtMath>
#include <QFont>
#include <QColor>
#include <QVariant>
#include "weeklydef.h"
#include "quoteformat.h"


class QTableView;
static const QStringList gCurUpDowns = {
    QStringLiteral("序号"),
    QStringLiteral("股票代码"),
    QStringLiteral("股票名称"),
    QStringLiteral("每100股收益"),
    QStringLiteral("累计涨幅")
};

static const QStringList gStealingSummary = {
    QStringLiteral("股票名称"),
    QStringLiteral("累计涨幅"),
    QStringLiteral("盯盘总结")
};



/**
 * @brief The WeeklySortFilterProxyModel class
 */
class WeeklySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    WeeklySortFilterProxyModel(int type = CurWeeklys_Type,QObject *parent = nullptr)
        :QSortFilterProxyModel(parent),m_type(type)
    {}

    ~WeeklySortFilterProxyModel()
    {}

    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid()) return QVariant();
        if(m_type == CurUpDowns_Type) {
            if(role == Qt::DisplayRole) {
                if(index.column() == 0){
                    return index.row()+1;
                }
            }
        }

        return QSortFilterProxyModel::data(index,role);
    }

    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const
    {
        if(CurWeeklys_Type == m_type) {
            qint64 leftVal = left.data(CapitalChange_Limit_Val).toLongLong(),
                    rightVal = right.data(CapitalChange_Limit_Val).toLongLong();
            return leftVal < rightVal;
        } else if(CurUpDowns_Type == m_type) {
            if(left.column() == 4) {
                qreal leftVal = left.data(Weekly_Table_UpDnData).toDouble(),
                        rightVal = right.data(Weekly_Table_UpDnData).toDouble();
                return leftVal < rightVal;
            }
        } else if(StealingSummary_Type == m_type){
            if(left.column() == 1) {
                qreal leftVal = left.data(Weekly_Table_UpDnData).toDouble(),
                        rightVal = right.data(Weekly_Table_UpDnData).toDouble();
                return leftVal < rightVal;
            }
        }

        return QSortFilterProxyModel::lessThan(left,right);
    }

private:
    int m_type;
};


class WeeklyChgModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WeeklyChgModel(int type, QObject *parent=nullptr)
        :QAbstractTableModel(parent)
        ,m_type(type)
    {
        m_upFont.setFamily("Arial");
        m_upFont.setPixelSize(13);
        m_upFont.setBold(true);
        m_upFont.setStyleStrategy(QFont::NoAntialias);

        m_upFontNoBlod = m_upFont;
        m_upFontNoBlod.setBold(false);

        m_textFont.setFamily("SimSun");
        m_textFont.setPixelSize(14);
    }

    ~WeeklyChgModel()
    {

    }

    /**
     * @brief headerData 返回的表头数据
     * @param section
     * @param ori
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation ori, int role) const
    {
        if(m_type == CurUpDowns_Type) {
            if(ori == Qt::Horizontal && role == Qt::DisplayRole && section < 5) {
                return gCurUpDowns[section];
            } else if(ori == Qt::Horizontal && role == Qt::TextAlignmentRole && section < 5) {
                if(section == 2)
                    return Qt::AlignVCenter|Qt::AlignLeft;
                else
                    return Qt::AlignCenter;
            }
        } else if(m_type == StealingSummary_Type) {
            if(ori == Qt::Horizontal && role == Qt::DisplayRole && section < 3) {
                return gStealingSummary[section];
            }
        }

        return QVariant();
    }

    /**
     * @brief rowCount 行数
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent);

        if(m_type == CurWeeklys_Type) {
            return m_Datas.count();
        } else if(m_type == CurUpDowns_Type)
        {
            return m_curUpDnDatas.count();
        } else if(m_type == StealingSummary_Type)
        {
            return m_steSumDatas.count();
        }
    }

    /**
     * @brief columnCount 列数
     * @param parent
     * @return
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent);
        if(m_type == CurWeeklys_Type)
            return 3;
        else if(m_type == CurUpDowns_Type)
        {
            return 5;
        } else if(m_type == StealingSummary_Type)
        {
            return 3;
        }
    }

    /**
     * @brief data 根据用户角色返回特定数据
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid()) return QVariant();

        int r = index.row(), c = index.column();
        if(m_type == CurWeeklys_Type)
        {
            if(role == Qt::DisplayRole && !m_Datas.isEmpty() && r < m_Datas.count())
            {
                switch(c) {
                case 0:return m_Datas[r].name;
                case 2:return HzQuoteFormat::StringUnitFormat::toFormatString(m_Datas[r].amount,2);
                }
            } else if(role == CapitalChange_Limit_Max)
            {
                return m_max;
            } else if(role == CapitalChange_Limit_Min)
            {
                return m_min;
            } else if(role == CapitalChange_Limit_Val)
            {
                return m_Datas[r].amount;
            } else if(role == CapitalChange_TEST) {
                return m_Datas[r].amount;
            } else if(role == Qt::TextAlignmentRole)
            {
                if(c == 1)
                {
                    return Qt::AlignRight | Qt::AlignVCenter;
                } else if(c == 2)
                {
                    return Qt::AlignRight|Qt::AlignVCenter;
                }
            } else if(role == Qt::FontRole)
            {
                return (c == 2) ? m_upFontNoBlod : m_textFont;
            }
        } else if(m_type == CurUpDowns_Type) {
            if(role == Qt::DisplayRole && !m_curUpDnDatas.isEmpty() && r < m_curUpDnDatas.count())
            {
                switch(c) {
                case 0:return r+1;
                case 1:{
                    QString tmpStockCode(m_curUpDnDatas[r].stockcode);
                    tmpStockCode.replace("sz", "");
                    tmpStockCode.replace("sh", "");
                    return tmpStockCode;
                }
                case 2:return m_curUpDnDatas[r].stockname;
                case 3:return QString::number(m_curUpDnDatas[r].profit / 1000, 'f',2) + QStringLiteral("元");
                case 4:return QString::number(m_curUpDnDatas[r].ups,'f',2) + "%";
                }
            } else if(role == Qt::TextAlignmentRole)
            {
                switch(c) {
                case 0:
                case 1:return Qt::AlignCenter;
                case 2:return Qt::AlignVCenter | Qt::AlignLeft;
                case 3:return Qt::AlignVCenter | Qt::AlignRight;
                case 4:return Qt::AlignCenter;
                }
            } else if(role == Qt::FontRole)
            {
                return (c != 2) ? m_upFont : m_textFont;
            } else if(role == Qt::ForegroundRole)
            {
                double ups = m_curUpDnDatas[r].ups;
                switch(c) {
                case 0:
                case 1:
                case 2:
                    return QColor("#cad4dc");
                case 3:
                    return QVariant(ups>0.?QColor("#e62727"): ups <0.?QColor("#0aaa38"):QColor("#cad4dc"));
                case 4:
                    return QVariant(ups>0.?QColor("#e62727"): ups <0.?QColor("#0aaa38"):QColor("#cad4dc"));
                }
            }else if(role == Qt::BackgroundRole)
            {
                return QColor("#222222");
            } else if(role == CapitalChange_ModelIndex)
            {
                return this->index(r,c);
            }else if(role == Weekly_Table_UpDnData)
            {
                return m_curUpDnDatas[r].ups;
            } else if(role == Weekly_Table_StockCode)
            {
                return m_curUpDnDatas[r].stockcode;
            }
        } else if(m_type == StealingSummary_Type)//盯盘总结
        {
            if(role == Qt::DisplayRole && !m_steSumDatas.isEmpty() && r < m_steSumDatas.count())
            {
                switch(c) {
                case 0:return m_steSumDatas[r].stockname;
                case 1:return QString::number(m_steSumDatas[r].ups,'f',2) + "%";
                case 2:return m_steSumDatas[r].des;
                }
            } else if(role == Qt::TextAlignmentRole)
            {
                return (c!=2)?Qt::AlignCenter : Qt::AlignLeft | Qt::AlignVCenter;
            } else if(role == Qt::FontRole)
            {
                return (c==1)?m_upFont:m_textFont;
            } else if(role == Qt::ForegroundRole)
            {
                qreal ups = m_steSumDatas[r].ups;
                return (c==1) ? (ups>0?QColor("#e62727"): ups <0?QColor("#0aaa38"):QColor("#cad4dc"))
                           : QColor("#cad4dc");
            } else if(role == Qt::BackgroundRole)
            {
                return QColor("#222222");
            } else if(role == Weekly_Table_UpDnData)
            {
                return m_steSumDatas[r].ups;
            }else if(role == Weekly_Table_StockCode)
            {
                return m_steSumDatas[r].stockcode;
            }
        }

        return QVariant();
    }

    /**
     * @brief setValue 设置数据
     * @param chgs
     */
    void setValue(const QVariant &chgs)
    {
        if(m_type == CurWeeklys_Type)
        {
            m_Datas = chgs.value<QVector<STCapitalChange>>();
            m_max = LONG64_MIN,m_min=LONG64_MAX;
            for(int i=0; i<m_Datas.count(); ++i)
            {
                m_max = qMax(m_Datas[i].amount, m_max);
                m_min = qMin(m_Datas[i].amount, m_min);
            }
        } else if(m_type == CurUpDowns_Type)
        {
            m_curUpDnDatas = chgs.value<QVector<STCurUpDowns>>();
        } else if(m_type == StealingSummary_Type)
        {
            m_steSumDatas = chgs.value<QVector<STStealingSummary>>();
        }

        beginResetModel();
        endResetModel();
    }

    /**
     * @brief getLimitValue 获取最大最小值
     * @param max
     * @param min
     * @return 返回true，表示极限值是有意义的
     */
    bool getLimitValue(qlonglong &max, qlonglong &min)
    {
        if(m_max == LONG64_MIN || m_min == LONG64_MAX)
            return false;

        max = m_max;
        min = m_min;
        return true;
    }

    int getChangeType() const
    {
        return m_type;
    }

private:
    int m_type;
    QFont m_upFont, m_upFontNoBlod, m_textFont;
    QVector<STCapitalChange> m_Datas;
    QVector<STCurUpDowns> m_curUpDnDatas;
    QVector<STStealingSummary> m_steSumDatas;
    qlonglong m_max,m_min;
};

/**
 * @brief The WeeklyListModel class 抽象一维数组数据
 */
class WeeklyListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WeeklyListModel(int type,QObject *parent=nullptr):QAbstractTableModel(parent),m_type(type)
    {}

    ~WeeklyListModel()
    {}

    int rowCount(const QModelIndex &parent=QModelIndex()) const
    {
        Q_UNUSED(parent);
        return 1;
    }

    int columnCount(const QModelIndex &parent=QModelIndex()) const
    {
        Q_UNUSED(parent);
        if(m_type == CurUpDowns_Type) {
            return 5;
        } else if( m_type == StealingSummary_Type) {
            return 3;
        }
    }

    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const
    {
        Q_UNUSED(row);
        Q_UNUSED(parent);
        return createIndex(0,column);
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if(!index.isValid()) return QVariant();
        int r = index.row(), c = index.column();

        if(m_type == CurUpDowns_Type) {
            if(role == Qt::DisplayRole && m_datas.count() == columnCount()) {
                return m_datas[c];
            } else if(role == Qt::TextAlignmentRole) {
                switch(c) {
                case 0:
                case 1:return Qt::AlignCenter;
                case 2:return Qt::AlignVCenter | Qt::AlignLeft;
                case 3:return Qt::AlignVCenter | Qt::AlignRight;
                case 4:return Qt::AlignCenter;
                }
            } else if(role == Weekly_Table_Column_Sort){
                switch(c) {
                case 0:return false;
                case 1:return false;
                case 2:return false;
                case 3:return false;
                case 4:return true;
                }
            }
        } else if(m_type == StealingSummary_Type) {
            if(role == Qt::DisplayRole && m_datas.count() == columnCount()) {
                return m_datas[c];
            } else if(role == Qt::TextAlignmentRole) {
                switch(c) {
                case 0:
                case 1:return Qt::AlignCenter;
                case 2:return Qt::AlignVCenter | Qt::AlignLeft;
                }
            } else if(role == Weekly_Table_Column_Sort) {
                switch(c){
                case 0:return false;
                case 1:return true;
                case 2:return false;
                }
            }
        }

        return QVariant();
    }

    void setValue(const QStringList &val)
    {
        m_datas = val;
        beginResetModel();
        endResetModel();
    }

private:
    int m_type;
    QStringList m_datas;
};
#endif // WEEKLYMODEL_H
