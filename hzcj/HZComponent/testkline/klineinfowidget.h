#ifndef KLINEINFOWIDGET_H
#define KLINEINFOWIDGET_H
#include "basewidget.h"
#include "frameless_helper.h"

class QLabel;
class KlineInfoWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit KlineInfoWidget(QWidget *parent = nullptr);

private:
    void createWg();

private:
    FramelessHelper *m_pHelper;

    QLabel *m_dateLabel; //= new QLabel(QStringLiteral("日 期"));
    QLabel *m_openLabel; // = new QLabel(QStringLiteral("开 盘"));
    QLabel *m_highLabel; // = new QLabel(QStringLiteral("最 高"));
    QLabel *m_lowLabel; // = new QLabel(QStringLiteral("最 低"));
    QLabel *m_closeLabel; // = new QLabel(QStringLiteral("收 盘"));
    QLabel *m_riseLabel; // = new QLabel(QStringLiteral("涨 跌"));
    QLabel *m_riseRateLabel; // = new QLabel(QStringLiteral("涨 幅"));
    QLabel *m_volumeLabel; // = new QLabel(QStringLiteral("成交量"));
    QLabel *m_moneyLabel; // = new QLabel(QStringLiteral("金 额"));
    QLabel *m_ampLabel; // = new QLabel(QStringLiteral("振 幅"));
    QLabel *m_changeLabel; // = new QLabel(QStringLiteral("换 手"));
};

#endif // KLINEINFOWIDGET_H
