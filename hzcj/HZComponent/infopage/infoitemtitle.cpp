#include "infoitemtitle.h"
#include "infodef.h"
#include "infostyle.h"
#include <QPainter>

Infoitemtitle::Infoitemtitle(int type, const QString &text, QWidget *parent)
    : QFrame(parent),m_type(type)
    ,m_titleStr(text)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    setObjectName("TitleBarFrame");
    setFixedHeight(30);
    installEventFilter(this);

    m_leftIcon = new QPushButton;
    m_leftIcon->setObjectName("LeftButtonIcon");
    m_leftIcon->setFixedSize(15, 15);

    m_titleLab = new QLabel(m_titleStr);
    m_titleLab->setObjectName("TitleTextLabel");

    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(14,0,0,0);
    hb->setSpacing(11);
    hb->addWidget(m_leftIcon);
    hb->addWidget(m_titleLab);
    setLayout(hb);
}

void Infoitemtitle::setRecordNum(int num)
{
    m_num = num;
    QString unitStr="";
    switch(m_type) {
    case FHPX_Item:
    case TFP_Item:
    case SXJJ_Item:
    case ZF_Item:
    case YJZF_Item: break;
    case MajorMeet_Item:unitStr=QStringLiteral("场");break;
    case FinancialReport_Item:unitStr=QStringLiteral("家");break;
    case EconomicData_Item:break;
    }

    m_titleLab->setText(num==-1?m_titleStr:QString("%1(%2%3)").arg(m_titleStr).arg(m_num).arg(unitStr));
    update();
}

void Infoitemtitle::setDefautBtnClicked(int id)
{
    m_defaultId = id;
}

int Infoitemtitle::currentItem() const
{
    return m_defaultId;
}

static const QVector<QString> gCountryNames = {
    QStringLiteral("全部"),
    QStringLiteral("中国"),
    QStringLiteral("美国"),
    QStringLiteral("欧盟"),
    QStringLiteral("日本")
};

void Infoitemtitle::paintEvent(QPaintEvent *e)
{
    if(m_type == EconomicData_Item) {
        QPainter p(this);
        int w = 130;
        int leftStart = width() - 5*w;
        p.setFont(gInfoStyle->m_titleFont);

        for(int i=0; i<5; ++i) {
            QRect itemRt(leftStart+i*w, 2, w, height()-4);
            drawItem(&p, itemRt, i);
            p.setPen(QColor("#191919"));
            p.drawLine(itemRt.topRight(), itemRt.bottomRight());
            if(m_defaultId == i) {
                p.setPen(QColor("#2f2f2f"));
                p.setBrush(QColor("#2f2f2f"));
                p.drawRect(itemRt);
                p.setPen(QColor("#adadad"));
                p.drawText(itemRt, Qt::AlignCenter, gCountryNames[i]);
            }
        }
    } else {
        QFrame::paintEvent(e);
    }
}

bool Infoitemtitle::eventFilter(QObject *w, QEvent *e)
{
    if(m_type == EconomicData_Item) {
        if(w == this) {
            if(e->type() == QEvent::MouseButtonPress) {
                QMouseEvent *me = static_cast<QMouseEvent*>(e);
                if(me) {
                    m_pressedPos = me->pos();
                    m_pressed = true;
                    update();
                }
            } else if(e->type() == QEvent::MouseMove) {
                QMouseEvent *me = static_cast<QMouseEvent*>(e);
                if(me) {
                    m_hoverPos = me->pos();
                    update();
                }
            }
        }
    }
    return QFrame::eventFilter(w,e);
}

void Infoitemtitle::drawItem(QPainter *p, const QRect &rt, int id)
{
    if(rt.contains(m_pressedPos) && m_pressed) {//pressed
        emit signalBtnClicked(id);
        setDefautBtnClicked(id);
        p->setPen(QColor("#2f2f2f"));
        p->setBrush(QColor("#2f2f2f"));
        p->drawRect(rt);
        p->setPen(QColor("#adadad"));
        p->drawText(rt, Qt::AlignCenter, gCountryNames[id]);
        m_pressed = false;
        update();
    } else {
        p->setPen(QColor("#222222"));
        p->setBrush(QColor("#222222"));
        p->drawRect(rt);
        p->setPen(QColor("#797979"));
        p->drawText(rt, Qt::AlignCenter, gCountryNames[id]);
    }
}
