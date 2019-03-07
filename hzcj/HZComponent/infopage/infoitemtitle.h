/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INFOITEMTITLE_H
#define INFOITEMTITLE_H

#include <QFrame>
#include <QtWidgets>
#include "infopage_export.h"



/**
 * @brief The Infoitemtitle class
 */
class HZINFOPAGE_EXPORT Infoitemtitle : public QFrame
{
    Q_OBJECT
public:
    explicit Infoitemtitle(int type, const QString &text="", QWidget *parent = 0);

    void setRecordNum(int num);
    void setDefautBtnClicked(int id);
    int currentItem() const;

protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void signalBtnClicked(int id);

public slots:


private:
//    class BtnGroup: public QFrame
//    {
//        Q_OBJECT
//    public:
//        BtnGroup()
//    };

    QString m_titleStr{""};
    QPushButton *m_leftIcon;
    QLabel *m_titleLab;
    int m_num, m_type, m_defaultId{0};
    QPoint m_pressedPos, m_hoverPos;
    bool m_pressed{false};

    void drawItem(QPainter *p, const QRect &rt, int id);
};

#endif // INFOITEMTITLE
