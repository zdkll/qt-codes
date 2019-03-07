#include "activitylabel.h"
#include <QMouseEvent>



ActivityLabel::ActivityLabel(const QString StrText, QWidget *parent):
    QLabel(StrText, parent)
{

}

ActivityLabel::~ActivityLabel()
{

}

void ActivityLabel::mouseReleaseEvent(QMouseEvent * ev)
{//定义鼠标左键点击事件
    if(ev->button() == Qt::LeftButton)
    {
        Q_UNUSED(ev)
            emit clicked();
    }
}

