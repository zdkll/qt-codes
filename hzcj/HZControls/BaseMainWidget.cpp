#include "BaseMainWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QStyleOption>
#include <QDesktopWidget>


CBaseMainWidget::CBaseMainWidget(QWidget *parent,QString instName, QRect &clickableRect) :
    BaseWidget(parent,instName),
    m_ClickableRect(clickableRect),
    m_IsDraggable(false),
    m_IsStretchable(false),
    m_IsMaximized(false),
    m_CursorStyle(eCursorNormal)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setMouseTracking(true); //界面拉伸需要这个属性
}

CBaseMainWidget::~CBaseMainWidget()
{

}

void CBaseMainWidget::mousePressEvent(QMouseEvent *event)
{
    if(0 == m_ClickableRect.width())
        m_ClickableRect.setWidth(this->width());

    if(0 == m_ClickableRect.height())
        m_ClickableRect.setHeight(this->height());

    if (eCursorNormal != m_CursorStyle)
        m_IsStretchable = true;

    if(!m_IsStretchable && isPosInClickableRect(event->pos())
            && (event->button() == Qt::LeftButton))
    {
        m_IsDraggable = true;
        m_MovePosition = event->pos();
    }

    BaseWidget::mousePressEvent(event);
}

void CBaseMainWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint deltaPos = event->globalPos() - m_MovePosition;
    if (m_IsDraggable && (deltaPos.manhattanLength() > QApplication::startDragDistance()))
    {
        if(m_IsMaximized)
            switchNormalMaximize();

        move(deltaPos);
        m_MovePosition = event->globalPos() - this->pos();
    }

    if(!m_IsStretchable)
        m_CursorStyle = static_cast<ECursorStyle>(setCursorStyle(event->pos()));
    else
        resizeWidget(event->globalPos());

    BaseWidget::mouseMoveEvent(event);
}

void CBaseMainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_IsDraggable = false;
    m_IsStretchable = false;
    m_CursorStyle = static_cast<ECursorStyle>(setCursorStyle(event->pos()));

    BaseWidget::mouseReleaseEvent(event);
}

void CBaseMainWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(isPosInClickableRect(event->pos()))
        switchNormalMaximize();

    BaseWidget::mouseDoubleClickEvent(event);
}

void CBaseMainWidget::paintEvent(QPaintEvent *event)
{
   QStyleOption opt;
   opt.init(this);
   QPainter painter(this);
   style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    BaseWidget::paintEvent(event);
}

void CBaseMainWidget::OnShowMaximized()
{
    switchNormalMaximize();
}

void CBaseMainWidget::switchNormalMaximize()
{
    if(m_IsMaximized)
    {
        m_IsMaximized = false;
        setWidgetGeometry(m_NormalLocation);
    }
    else
    {
        m_IsMaximized = true;
        m_NormalLocation = this->geometry();
        QDesktopWidget *desk = QApplication::desktop();
        setWidgetGeometry(desk->availableGeometry()); //不遮挡任务栏
    }
}

bool CBaseMainWidget::isPosInClickableRect(const QPoint &point)
{
    if((point.x() > m_ClickableRect.x()) && (point.x() < m_ClickableRect.width())
            &&  (point.y() > m_ClickableRect.y()) && (point.y() < m_ClickableRect.height()))
        return true;

    return false;
}

int CBaseMainWidget::setCursorStyle(const QPoint& point)
{
    int width = this->width();
    int height = this->height();
    ECursorStyle cursorStyle = eCursorNormal;

    bool isleftEdge = (point.x() <= StretchableEdge) ? true : false;
    bool isTopEdge = (point.y() <= StretchableEdge) ? true : false;
    bool isRightEdge = (width - point.x() <= StretchableEdge) ? true : false;
    bool isBottomEdge = (height - point.y() <= StretchableEdge) ? true : false;

    if(isleftEdge && isTopEdge)
    {
        setCursor(Qt::SizeFDiagCursor);
        cursorStyle = eCursorLeftTop;
    }
    else if(isRightEdge && isBottomEdge)
    {
        setCursor(Qt::SizeFDiagCursor);
        cursorStyle = eCursorRightBottom;
    }
    else if(isRightEdge && isTopEdge)
    {
        setCursor(Qt::SizeBDiagCursor);
        cursorStyle = eCursorRightTop;
    }
    else if(isleftEdge && isBottomEdge)
    {
        setCursor(Qt::SizeBDiagCursor);
        cursorStyle = eCursorLeftBottom;
    }
    else if(isleftEdge)
    {
        setCursor(Qt::SizeHorCursor);
        cursorStyle = eCursorLeft;
    }
    else if(isTopEdge)
    {
        setCursor(Qt::SizeVerCursor);
        cursorStyle = eCursorTop;
    }
    else if(isRightEdge)
    {
        setCursor(Qt::SizeHorCursor);
        cursorStyle = eCursorRight;
    }
    else if(isBottomEdge)
    {
        setCursor(Qt::SizeVerCursor);
        cursorStyle = eCursorBottom;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        cursorStyle = eCursorNormal;
    }

    return cursorStyle;
}

void CBaseMainWidget::resizeWidget(const QPoint& point)
{
    QRect origRect = this->geometry();
    int left = origRect.left();
    int top = origRect.top();
    int right = origRect.right();
    int bottom = origRect.bottom();
    switch (m_CursorStyle)
    {
    case eCursorLeft:
        left = point.x();
        break;
    case eCursorRight:
        right = point.x();
        break;
    case eCursorTop:
        top = point.y();
        break;
    case eCursorBottom:
        bottom = point.y();
        break;
    case eCursorLeftTop:
        left = point.x();
        top = point.y();
        break;
    case eCursorRightBottom:
        right = point.x();
        bottom = point.y();
        break;
    case eCursorRightTop:
        right = point.x();
        top = point.y();
        break;
    case eCursorLeftBottom:
        left = point.x();
        bottom = point.y();
        break;
    default:
        break;
    }

    QRect newRect(QPoint(left, top), QPoint(right, bottom));
    if (newRect.isValid())
    {
        if (this->minimumWidth() > newRect.width())
        {
            if (left != origRect.left())
                newRect.setLeft(origRect.left());
            else
                newRect.setRight(origRect.right());
        }

        if (this->minimumHeight() > newRect.height())
        {
            if (top != origRect.top())
                newRect.setTop(origRect.top());
            else
                newRect.setBottom(origRect.bottom());
        }

        setWidgetGeometry(newRect);
    }
}

void CBaseMainWidget::setWidgetGeometry(const QRect& rect)
{
    bool widthFlag = (this->width() == m_ClickableRect.width()) ? true : false;
    bool heightFlag = (this->height() == m_ClickableRect.height()) ? true : false;

    this->setGeometry(rect);

    if(widthFlag)
        m_ClickableRect.setWidth(this->width());

    if(heightFlag)
        m_ClickableRect.setHeight(this->height());
}
