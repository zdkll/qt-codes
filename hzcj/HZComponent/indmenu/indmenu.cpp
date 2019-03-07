#include "indmenu.h"
#include "ui_indmenu.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QResizeEvent>

#define TEXT_ITEM_MARGIN 4 * 2

static const QStringList gStaticBtnText = {
    QStringLiteral("更多"),
    //QStringLiteral("模板")
};


static const QStringList gText = {
    QStringLiteral("指标"),
    QStringLiteral("恢复默认"),
    QStringLiteral("成交量"),
    QStringLiteral("均线"),
    QStringLiteral("资金博弈"),
    QStringLiteral("指标12"),
    QStringLiteral("恢复默认12"),
    QStringLiteral("成交量12"),
    QStringLiteral("均线12"),
    QStringLiteral("资金博弈12"),
};

QVector<QString> IndItem::m_items;
void IndItem::init()
{
    m_msPos = QPoint(-1,-1);
    m_selFlags = "";
    m_isComplete = false;
}

void IndItem::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    QPainter p(this);
    int staticLen = 0;
    int len = 0, xpos=0, leftWdt=0, scaleIndex = 0;

    for(const QString &text:gStaticBtnText)
        staticLen += (p.fontMetrics().width(text)+TEXT_ITEM_MARGIN);

    leftWdt = width() - staticLen;
    for(int i=0; i<m_items.count(); ++i)
    {
        QString text(m_items[i]);
        len = p.fontMetrics().width(text) + TEXT_ITEM_MARGIN;
        QRect rt(xpos, 0, len, height());
        if(xpos < leftWdt && xpos + len < leftWdt) {
            if(rt.contains(m_msPos) && !m_repaint)
                emit signalTextClicked(NormalInd,i,m_items[i], mapToGlobal(rt.bottomRight()));
            p.setPen((rt.contains(m_msPos) && m_selFlags == m_items[i]) || (m_selFlags == m_items[i]) ? m_selClr : m_norClr);
            p.drawText(rt, Qt::AlignCenter, text);
            p.setPen(m_borderClr);
            p.drawLine(rt.topRight()+QPoint(0,4), rt.bottomRight()-QPoint(0,4));
            xpos += len;
            m_isComplete = true;
        } else {
            scaleIndex = i;
            m_isComplete = false;
            break;
        }
    }

    if(!m_isComplete) {
        QRect rightRt(xpos, 0, width()-xpos, height());
        for(int i = 0; i< gStaticBtnText.count(); ++i)
        {
            int len = p.fontMetrics().width(gStaticBtnText[i])+TEXT_ITEM_MARGIN;
            rightRt.setWidth(len);
            if(rightRt.contains(m_msPos) && !m_repaint)
                emit signalTextClicked(StaticInd, scaleIndex, gStaticBtnText[i], mapToGlobal(rightRt.bottomRight()));
            p.setPen((rightRt.contains(m_msPos) && m_selFlags == gStaticBtnText[i]) || (m_selFlags == gStaticBtnText[i]) ? m_selClr : m_norClr);
            p.drawText(rightRt, Qt::AlignCenter, gStaticBtnText[i]);
            p.setPen(m_borderClr);
            p.drawLine(rightRt.topRight()+QPoint(0,4), rightRt.bottomRight()-QPoint(0,4));
            xpos += len;
            rightRt.setX(xpos);
        }
    }
}

void IndItem::mousePressEvent(QMouseEvent *e)
{
    e->ignore();
    m_msPos = e->pos();
    flagsFromPosition();
    m_repaint = false;
    update();
}

void IndItem::resizeEvent(QResizeEvent *event)
{
    event->ignore();
    m_repaint = true;
}

void IndItem::flagsFromPosition()
{
    int staticLen = 0;
    int len = 0, xpos=0, leftWdt=0, scaleIndex = 0;

    for(const QString &text:gStaticBtnText)
        staticLen += (fontMetrics().width(text)+TEXT_ITEM_MARGIN);

    leftWdt = width() - staticLen;
    for(int i=0; i<m_items.count(); ++i)
    {
        QString text(m_items[i]);
        len = fontMetrics().width(text) + TEXT_ITEM_MARGIN;
        QRect rt(xpos, 0, len, height());
        if(xpos < leftWdt && xpos + len < leftWdt)
        {
            if(rt.contains(m_msPos)){
                m_selFlags = text;
                break;
            }
            xpos += len;
            m_isComplete = true;
        }else {
            m_selFlags = text;
        }
    }
}


///
/// \brief IndMenu::IndMenu
/// \param parent
///
IndMenuMaster::IndMenuMaster(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndMenu)
{
    ui->setupUi(this);
    setFixedHeight(22);
    m_indMenu = new QMenu(this);
    m_indItem = new IndItem;
    ui->horizontalLayout->addWidget(m_indItem);

    ui->horizontalLayout->setStretch(0,1);
    addItem(gText);

    connect(m_indMenu, &QMenu::triggered, [=](QAction *action){
        emit signalItemClicked(action->text());
    });
    connect(m_indItem, &IndItem::signalTextClicked, [=](int type, int itemIndex, const QString &text, const QPoint &pos){
        if(gStaticBtnText.contains(text)) {
            m_indItem->init();
            m_indMenu->clear();
            switch(type) {
            case IndItem::SpacialInd:break;
            case IndItem::NormalInd: break;
            case IndItem::StaticInd:{
                int cnt = IndItem::m_items.count();
                for(int i=itemIndex; i<cnt; ++i)
                    m_indMenu->addAction(IndItem::m_items[i]);
            }break;
            }
            m_indMenu->exec(pos);
        }
        else {
            emit signalItemClicked(text);
        }
    });

    loadskin();
}

IndMenuMaster::~IndMenuMaster()
{
    delete ui;
    if(m_indMenu)
    {
        delete m_indMenu;
    }
}

void IndMenuMaster::loadskin()
{
    QFile file(":/qss/indmenu.qss");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << Q_FUNC_INFO << file.errorString();
        file.close();
        return;
    }

    setStyleSheet(file.readAll());
    file.close();
}

