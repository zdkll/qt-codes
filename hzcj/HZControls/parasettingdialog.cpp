#include "parasettingdialog.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QDebug>
#include <QSplitter>
#include <QFormLayout>
#include <QButtonGroup>
#include <QColorDialog>
#include <QListWidget>
#include <QMenu>
#include <QPixmap>
#include <QPainter>


PenStyleToolButton::PenStyleToolButton(QWidget *parent )
    :QToolButton(parent),
     m_ndefaultWidth(0)
{

}

void PenStyleToolButton::createWg()
{
    QPixmap pix0(80,12);
    pix0.fill(Qt::transparent);
    QPainter painter0;
    painter0.begin(&pix0);
    QPen pen0;
    pen0.setWidth(m_ndefaultWidth);
    pen0.setStyle(Qt::SolidLine);
    pen0.setColor(Qt::white);
    painter0.setPen(pen0);
    painter0.drawLine(0, 5, 80, 5);
    painter0.end();
    this->setIconSize(QSize(80,12));
    this->setIcon(QIcon(pix0));

    this->setToolButtonStyle(Qt::ToolButtonIconOnly);

    this->setPopupMode(QToolButton::MenuButtonPopup);



    QMenu *menu = new QMenu;

    this->setMenu(menu);

    QVBoxLayout *menuLayout = new QVBoxLayout;

    m_defaultBtn = new QToolButton;

    m_defaultBtn->setObjectName("menubtn1");

    QToolButton *mbtn2 = new QToolButton;
    mbtn2->setObjectName("menubtn2");

    QToolButton *mbtn3 = new QToolButton;
    mbtn3->setObjectName("menubtn3");



    connect(m_defaultBtn, &QToolButton::clicked,  [=](){menu->hide(); this->setIcon(m_defaultBtn->icon()); emit setLineWidth(1);});
    connect(mbtn2, &QToolButton::clicked,  [=](){menu->hide(); this->setIcon(mbtn2->icon()); emit setLineWidth(2);});
    connect(mbtn3, &QToolButton::clicked,  [=](){menu->hide(); this->setIcon(mbtn3->icon()); emit setLineWidth(3);});

    m_defaultBtn->setIconSize(QSize(80,12));
    m_defaultBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap pix(80,12);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(0,5,80,5);
    painter.end();
    m_defaultBtn->setIcon(QIcon(pix));



    mbtn2->setIconSize(QSize(80,12));
    mbtn2->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap pix1(80,12);
    pix1.fill(Qt::transparent);
    painter.begin(&pix1);
    pen.setWidth(2);
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(0,5,80,5);
    painter.end();
    mbtn2->setIcon(QIcon(pix1));


    mbtn3->setIconSize(QSize(80,12));
    mbtn3->setToolButtonStyle(Qt::ToolButtonIconOnly);
    QPixmap pix2(80,12);
    pix2.fill(Qt::transparent);
    painter.begin(&pix2);
    pen.setWidth(3);
    pen.setColor(Qt::white);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(0,5,80,5);
    painter.end();
    mbtn3->setIcon(QIcon(pix2));


    menuLayout->addWidget(m_defaultBtn);
    menuLayout->addWidget(mbtn2);
    menuLayout->addWidget(mbtn3);

    menuLayout->setContentsMargins(1,1,1,1);
    menuLayout->setSpacing(0);
    menu->setLayout(menuLayout);
}

void PenStyleToolButton::setDefalutWidth(int nWidth)
{
    m_ndefaultWidth = nWidth;

    createWg();
}

void PenStyleToolButton::setDefalutBtn()
{
    this->setIcon(m_defaultBtn->icon());
}


ParaSettingDialog::ParaSettingDialog(QWidget *parent, QString instname):
    PopupDialog(parent, instname),
    m_pGridLayout(new QGridLayout),
    m_pButtonGroup(new QButtonGroup(this)),
    m_strIndi("")
{
    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onButtonGroupClicked(int)));
    connect(this, &ParaSettingDialog::popWndClose, this, &ParaSettingDialog::onWndClose);
    m_inidItemList = Hzcfg::getInstance()->getIndiMap();

}

void ParaSettingDialog::createWg()
{
    if(!m_inidItemList.empty())
    {
        if(!m_strIndi.isEmpty())
        {
            m_itemInfo = m_inidItemList[m_strIndi.toStdString()];
            m_defaultIndiInfo = m_itemInfo;
        }
    }

    m_pTitleLabel->setText(QString::fromStdString(m_itemInfo.screenname));

    m_pTitleLabel->setFixedHeight(30);
    m_pHelper->setTitleHeight(30);

    m_pTitleLabel->setObjectName("parasetting");


    QWidget *mainWidget = new QWidget;

    mainWidget->setObjectName("mainWidget");

    mainWidget->setFixedWidth(518);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QHBoxLayout *pHlayout  = new QHBoxLayout;

    pHlayout->setContentsMargins(39, 0, 0, 0);

    pHlayout->setSpacing(0);

    QLabel *lab1 = new QLabel(QStringLiteral("参数名称"));
    QLabel *lab2 = new QLabel(QStringLiteral("参数值"));
    QLabel *lab3 = new QLabel(QStringLiteral("指标线"));
    QLabel *lab4 = new QLabel(QStringLiteral("颜色"));
    QLabel *lab5 = new QLabel(QStringLiteral("线宽"));
    lab1->setFixedWidth(lab1->fontMetrics().width(lab1->text()));
    pHlayout->addWidget(lab1);
    pHlayout->addSpacing(39);
    lab2->setFixedWidth(lab2->fontMetrics().width(lab2->text()));
    pHlayout->addWidget(lab2);
    pHlayout->addSpacing(46);
    lab3->setFixedWidth(lab3->fontMetrics().width(lab3->text()));
    pHlayout->addWidget(lab3);
    pHlayout->addSpacing(52);
    lab4->setFixedWidth(lab4->fontMetrics().width(lab4->text()));
    pHlayout->addWidget(lab4);
    pHlayout->addSpacing(82);
    lab5->setFixedWidth(lab5->fontMetrics().width(lab5->text()));
    pHlayout->addWidget(lab5);

    pHlayout->addStretch();

    QWidget *wg1 = new QWidget;

    wg1->setObjectName("topwidget");

    wg1->setFixedHeight(40);

    wg1->setLayout(pHlayout);


    QWidget *wg2 = new QWidget;



    wg2->setFixedWidth(480);
    wg2->setObjectName("centralwidget");


    m_pGridLayout->setContentsMargins(20, 8, 11, 8);

    m_pGridLayout->setSpacing(5);

    for(int i = 0; i < m_itemInfo.params.size(); i++)
    {
        QSpinBox *sp = new QSpinBox;

        connect(sp, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),this, &ParaSettingDialog::onvalueChanged);

        sp->setSingleStep(1);
        sp->setRange(m_itemInfo.params[i].minValue, m_itemInfo.params[i].maxValue);
        sp->setValue(m_itemInfo.params[i].value);

        m_pGridLayout->addWidget(new QLabel(QString::fromStdString(m_itemInfo.params[i].des)), i, 0, 1, 1, Qt::AlignLeft);

        m_pGridLayout->addWidget(sp, i, 1, 1, 1, Qt::AlignLeft);

        m_splist.push_back(sp);
    }


    for(int i = 0; i < m_itemInfo.results.size(); i++)
    {
        QWidget *widget = new QWidget;
        widget->setObjectName("colorwidget");
        m_colorWglist.push_back(widget);
        QFormLayout *formLayout = new QFormLayout;
        formLayout->setMargin(0);
        formLayout->setSpacing(0);

        QLabel *label = new QLabel;
        label->setStyleSheet("background:" + m_itemInfo.results[i].clrMap[0].name());
        QPushButton *colorBtn = new QPushButton;
        addButton(colorBtn, i);
        formLayout->addRow(label, colorBtn);
        widget->setLayout(formLayout);

        PenStyleToolButton *toolButton = new PenStyleToolButton;
        toolButton->setObjectName("styletoolBtn");
        connect(toolButton, &PenStyleToolButton::setLineWidth, this, &ParaSettingDialog::onsetLineWidth);
        m_toolBtnlist.push_back(toolButton);
        toolButton->setDefalutWidth(m_itemInfo.results[i].linewidth);
//        AlignHCenter
        m_pGridLayout->addWidget(new QLabel(QString::fromStdString(m_itemInfo.results[i].screenname)), i, 2, 1, 1, Qt::AlignLeft);
        m_pGridLayout->addWidget(widget, i, 3, 1, 1, Qt::AlignLeft);
        m_pGridLayout->addWidget(toolButton, i, 4, 1, 1, Qt::AlignLeft);
    }


    QSplitter *splitter = new QSplitter(Qt::Vertical,0);

    splitter->setFixedHeight(12);

    QSplitter *splitterbottom = new QSplitter(Qt::Vertical,0);

    splitterbottom->setFixedHeight(12);

    QWidget *wg3 = new QWidget;

    wg3->setObjectName("bottomwidget");

    QHBoxLayout *playout  = new QHBoxLayout;
    playout->setContentsMargins(0,0,20, 0);

    playout->addStretch();

    QPushButton *restBtn = new QPushButton(QStringLiteral("恢复默认值"));
    restBtn->setObjectName("resetbtn");
    QPushButton *saveBtn = new QPushButton(QStringLiteral("保存"));

    connect(restBtn, &QPushButton::clicked, this, &ParaSettingDialog::onrestBtnClicked);
    connect(saveBtn, &QPushButton::clicked, this, &ParaSettingDialog::onsaveBtnClicked);

    saveBtn->setObjectName("savebtn");
    playout->addWidget(restBtn);
    playout->addWidget(saveBtn);



    wg1->setLayout(pHlayout);
    wg2->setLayout(m_pGridLayout);
    wg3->setLayout(playout);

    mainLayout->addWidget(wg1);
    mainLayout->addWidget(wg2, 0, Qt::AlignHCenter);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(wg3);
    mainLayout->addWidget(splitterbottom);
    mainWidget->setLayout(mainLayout);


    setWidget(mainWidget);
}

void ParaSettingDialog::setIndi(QString strIndi)
{
    m_strIndi = strIndi;
    createWg();
}

void ParaSettingDialog::addButton(QPushButton *btn, const int id)
{
    m_pButtonGroup->addButton(btn, id);
}



void ParaSettingDialog::onrestBtnClicked()
{
    for(int i = 0; i < m_splist.size(); i++)
    {
        m_splist[i]->setValue(m_defaultIndiInfo.params[i].value);
    }

    for(int i = 0; i < m_colorWglist.size(); i++)
    {
        QLabel *label = qobject_cast<QLabel*>((m_colorWglist[i]->layout()->itemAt(0)->widget()));
        label->setStyleSheet("background:" + m_defaultIndiInfo.results[i].clrMap[0].name());
    }

    for(int i = 0; i < m_toolBtnlist.size(); i++)
    {
        m_toolBtnlist[i]->setDefalutBtn();
    }

    m_itemInfo = m_defaultIndiInfo;
    Hzcfg::getInstance()->setIndiInfo(m_defaultIndiInfo);
    emit valueChanged(true);
}

void ParaSettingDialog::onsaveBtnClicked()
{
    Hzcfg::getInstance()->setIndiInfo(m_itemInfo);
    this->close();
}

void ParaSettingDialog::onButtonGroupClicked(int nId)
{
    QLabel *label = qobject_cast<QLabel*>((m_colorWglist[nId]->layout()->itemAt(0)->widget()));


    if(nullptr == label)
        return;

    QPoint   point = label->mapToGlobal(QPoint(0, label->height()));

    QColorDialog *colordlg = new QColorDialog(this);

    colordlg->move(point.x(), point.y());

    colordlg->setWindowFlags(Qt::FramelessWindowHint | colordlg->windowFlags());

    if(QDialog::Rejected == colordlg->exec())
        return;
    QColor color = colordlg->currentColor();

    label->setStyleSheet("background:" + color.name());

    if(m_itemInfo.results.empty())
        return;
    m_itemInfo.results[nId].clrMap[0] = color;

    qDebug() << "color = " << color;

    Hzcfg::getInstance()->setIndiInfo(m_itemInfo);
    emit valueChanged(true);
}

void ParaSettingDialog::onsetLineWidth(int nWidth)
{
    PenStyleToolButton *sender = qobject_cast<PenStyleToolButton *>(this->sender());

    if(nullptr == sender)
        return;

    for(int i = 0; i < m_toolBtnlist.size(); i++)
    {
        if(sender == m_toolBtnlist[i])
        {
            qDebug() << "nWidth = " << nWidth;

            if(m_itemInfo.results.empty())
                return;

            m_itemInfo.results[i].linewidth = nWidth;
            Hzcfg::getInstance()->setIndiInfo(m_itemInfo);

            emit  valueChanged(true);
            break;
        }
    }
}

void ParaSettingDialog::onvalueChanged(int nValue)
{
    QSpinBox *spinBox = qobject_cast<QSpinBox*>(this->sender());

    if(nullptr == spinBox)
        return;

    for(int i = 0; i < m_splist.size(); i++)
    {
        if(spinBox == m_splist[i])
        {
            qDebug() << "nValue = " << nValue;

            if(m_itemInfo.params.empty())
                return;
            m_itemInfo.params[i].value = nValue;
            Hzcfg::getInstance()->setIndiInfo(m_itemInfo);
            emit  valueChanged(true);
            break;
        }
    }
}

void ParaSettingDialog::onWndClose()
{
    onrestBtnClicked();
}


void ParaSettingDialog::test()
{
    qDebug() << "11111111111111";
}


