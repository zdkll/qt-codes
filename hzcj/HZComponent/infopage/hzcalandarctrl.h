/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef HZCALANDARCTRLCTRL_H
#define HZCALANDARCTRLCTRL_H

#include <QWidget>
#include <QDate>
#include <QFrame>
#include <QLabel>
#include <QDate>
#include <QPushButton>
#include <QStyle>
#include "infopage_export.h"

namespace Ui {
class HZCalanderCtrl;
}

class CalandarDisplay : public QFrame
{
    Q_OBJECT
public:
    enum MonthFlags{
        PreMonth,
        CurrentMonth,
        NextMonth,
    };

    CalandarDisplay(QWidget *parent=nullptr);

    /**
     * @brief setCurrentMonthDate 设置当前月
     * @param num
     */
    void setCurrentDate(const QDate &date);

    /**
     * @brief setCurrentClickDate 设置当前时间
     * \note 当前时间如果是无效的，会从有效列表中找出首个的日期并选中
     */
    void setCurrentClickDate(quint64 time);

    /**
     * @brief disabledTime
     * @param time
     */
    void disabledTime(quint64 time);

    /**
     * @brief disabledTimes
     * @param times
     */
    void disabledTimes(const QVector<quint64> &times);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void signalDateClicked(quint64 time);
    void signalDatePressed(quint64 time);

private:
    typedef struct STCalandarVernierInfo
    {
        struct STDateInfo
        {
            int day;
            quint64 dayTime;
            bool enabled;
        };

        int premonthEnd;
        int nextmonthStart;
        QVector<STDateInfo> data;
    }CldPrivate;

    quint64 m_defaultTime{0};
    QVector<quint64> m_validTms;
    QRect m_invalidRt;
    CldPrivate m_verInfo;
    QPoint m_press, m_hover;
    bool m_isPressed{false}, m_gridEnabed{false},m_defaultClicked{false};

    bool findOf(const QPoint &pt);
    QPoint pointOf(int order);
    void drawGrid(QPainter *p);
    void drawWeekBar(QPainter *p, const QRect &rt, int col);
    void drawItem(QPainter *p, const QRect &rt, int row, int col);
};

/**
 * @brief The HZCalanderCtrl class
 */
class HZINFOPAGE_EXPORT HZCalanderCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit HZCalanderCtrl(bool hidetoday=false, QWidget *parent = 0);
    ~HZCalanderCtrl();

    /**
     * @brief addInvalidTimes 增加无效时间列表
     * @param times 天数的时间戳
     */
    void addValidTimes(const QVector<quint64> &times);

    /**
     * @brief setDate
     * @param time
     */
    void setDate(quint64 time);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    /**
     * @brief signalDateClicked 抛出用户点击信号
     * @param time
     */
    void signalDateClicked(quint64 time);
    void signalDatePressed(quint64 time);


public slots:
    void slotDateChanged();

private:
    Ui::HZCalanderCtrl *ui;
    QDate m_date, m_startTime, m_endTime;
    CalandarDisplay *m_display;
    QVector<quint64> m_cacheTimes;
    QVector<quint32> m_timeLst;
};




#endif // HZCALANDARCTRLCTRL_H
