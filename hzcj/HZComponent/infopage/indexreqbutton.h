/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef INDEXREQBUTTON_H
#define INDEXREQBUTTON_H

#include <QLabel>
#include "infodef.h"
#include "infostyle.h"
#include "infopage_export.h"
#include <QPixmap>


/**
 * @brief The IndexBtnGroup class 左侧按钮组
 */
class IndexBtnGroup : public QLabel
{
    Q_OBJECT
public:
    IndexBtnGroup(QWidget *parent=nullptr);

    /**
     * @brief setSelect 设置选中
     * @param index
     */
    void setSelect(int index);

    /**
     * @brief selected 当前选中
     * @return
     */
    int selected() const;

    /**
     * @brief tabPressed tab键按下，tab标签加1
     * @param start 起始位置； -1 表示从0开始
     */
    void tabPressed(int start=-1);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *event);

signals:
    void signalIndexChanged(int index);

private:
    int m_filterIndex{0};
    QVector<QRect> m_rects;
    QPoint m_pressedPos, m_hoverPos;
    bool m_isPressed{false};
    QPixmap m_pix;

    void drawPix();
};



///
/// \brief The IndexReqButton class
///
class HZINFOPAGE_EXPORT IndexReqButton : public QFrame
{
    Q_OBJECT
public:
    IndexReqButton(QWidget *parent=nullptr);
    ~IndexReqButton(){}

    /**
     * @brief setSelect
     * @param index
     */
    void setSelect(int index);

    /**
     * @brief selected
     * @return
     */
    int selected() const;

    /**
     * @brief tabPressed tab键按下，tab标签加1
     * @param start 起始位置； -1 表示从0开始
     */
    void tabPressed(int start=-1);

    /**
     * @brief paintEvent
     * @param e
     */
    void paintEvent(QPaintEvent *e);

signals:
    void signalIndexChanged(int index);

private:
    IndexBtnGroup *m_idxBtn;
};

#endif // INDEXREQBUTTON_H
