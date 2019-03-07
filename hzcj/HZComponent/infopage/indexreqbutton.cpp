#include "indexreqbutton.h"
#include <QtWidgets>
#include <QtCore>
#include <QPen>

///
/// \brief IndexReqButton::IndexReqButton
/// \param parent
///
IndexReqButton::IndexReqButton(QWidget *parent)
    :QFrame(parent)
    ,m_idxBtn(new IndexBtnGroup)
{
    setMouseTracking(true);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setContentsMargins(0,0,0,0);
    hl->addWidget(m_idxBtn);
    setLayout(hl);
    setContentsMargins(2,0,2,0);

    setStyleSheet("IndexReqButton{background:#222222;border-bottom:1px solid black;}");

    connect(m_idxBtn, &IndexBtnGroup::signalIndexChanged, this, &IndexReqButton::signalIndexChanged);
    resize(490, 1000);
}

void IndexReqButton::setSelect(int index)
{
    m_idxBtn->setSelect(index);
}

int IndexReqButton::selected() const
{
    return m_idxBtn->selected();
}

void IndexReqButton::tabPressed(int start)
{
    m_idxBtn->tabPressed(start);
}

void IndexReqButton::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QFrame::paintEvent(e);
}

///
/// \brief IndexBtnGroup::IndexBtnGroup
/// \param parent
///
IndexBtnGroup::IndexBtnGroup(QWidget *parent)
    :QLabel(parent)
    ,m_pressedPos(QPoint(-1,-1))
    ,m_filterIndex(0)
{
    tabPressed(-1);
}

void IndexBtnGroup::setSelect(int index)
{
    ///![02]
    m_filterIndex = index;
    emit signalIndexChanged(index);
}

int IndexBtnGroup::selected() const{  return m_filterIndex;}

/**
 * @brief IndexBtnGroup::tabPressed
 * @param start
 * TAB键切换
 */
void IndexBtnGroup::tabPressed(int start)
{
    static int index=0;
    if(-1 == start){
        index = start+1;
    } else {
        index++;
        if(index >= HZINFOITEM_INDEX_CNT){
            index = 0;
        }
    }

    setSelect(index);
}

void IndexBtnGroup::mousePressEvent(QMouseEvent *ev)
{
    ev->ignore();
    m_pressedPos = ev->pos();
    m_isPressed = true;

    m_pix = QPixmap(size());
    m_pix.fill(Qt::transparent);
    drawPix();

    /*去除多次请求*/
    for(int i=0; i<m_rects.count(); ++i) {
        if(m_rects[i].contains(m_pressedPos)){
            setSelect(m_filterIndex);
            update();
            break;
        }
    }
}

void IndexBtnGroup::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QRect lineBtm(rect());
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
    painter.drawPixmap(lineBtm, m_pix);
    //__super::paintEvent(e);
}

void IndexBtnGroup::resizeEvent(QResizeEvent *event)
{
    m_pix = QPixmap(event->size());
    m_pix.fill(Qt::transparent);
    drawPix();
}

void IndexBtnGroup::drawPix()
{
    QPainter p(&m_pix);
    p.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
    int xPos = HZINFOITEM_LEFT_SPACE;
    QRect tmpRt;
    p.setFont(gInfoStyle->m_indexFont);
    m_rects.clear();
    for(int i=0; i<HZINFOITEM_INDEX_CNT; ++i) {
        int w = fontMetrics().width(gTagTexts[i])+HZINFOITEM_TEXT_MARGIN*2;
        QRect rt(xPos, HZINFOITEM_OFFSET_Y,  w, HZINFOITEM_INDEXBTN_H);
        m_rects.append(rt);
        if(i == m_filterIndex)tmpRt = rt;
        if((rt.contains(m_pressedPos) && m_isPressed)) {
            m_filterIndex = i;
            tmpRt = rt;
        }

        p.setPen(QPen(QColor("#414141"),2));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(rt, HZINFOITEM_BUTTON_RADIUS, HZINFOITEM_BUTTON_RADIUS);
        p.drawText(rt, Qt::AlignCenter, gTagTexts[i==0?6:i]);
        xPos += (w + HZINFOITEM_DISTANCE_LINE);
        if(i!=HZINFOITEM_INDEX_CNT-1) {
            p.setPen(gInfoStyle->m_iBtnNormalBorderPen);
            p.drawLine(rt.x()+rt.width()+2, HZINFOITEM_OFFSET_Y+rt.height()/2, xPos, HZINFOITEM_OFFSET_Y+rt.height()/2);
        }
    }

    if((m_filterIndex >= 0 || m_filterIndex < HZINFOITEM_INDEX_CNT)) {
        p.setPen(gInfoStyle->m_idxBtnPens[m_filterIndex]);
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(tmpRt, HZINFOITEM_BUTTON_RADIUS, HZINFOITEM_BUTTON_RADIUS);
        p.drawText(tmpRt, Qt::AlignCenter, gTagTexts[m_filterIndex==0?6:m_filterIndex]);
    }
}

