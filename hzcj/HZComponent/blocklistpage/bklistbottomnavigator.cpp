#include "bklistbottomnavigator.h"
#include <QPainter>
#include <QHBoxLayout>
#include "mytoolbutton.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QButtonGroup>
#include <QToolButton>
#include <QBoxLayout>
#include <QMenu>
#include <QDebug>
#include <QList>

#include "httpdata.h"
#include "datamodeldef.h"
#include "drowdownlistwidget.h"
BkListBottomNavigator::BkListBottomNavigator(QWidget *parent, QString instName):
    BaseWidget(parent,instName),
    m_hyDownListWidget(new DrowDownListWidget),
    m_gnDownListWidget(new DrowDownListWidget),
    m_areaDownListWidget(new DrowDownListWidget),
    m_toolBtnMore(new MyToolButton),
    m_toolBtnMore1(new MyToolButton),
    m_toolBtnMore2(new MyToolButton),
    m_hyBtn(new QToolButton),
    m_gnBtn(new QToolButton),
    m_areaBtn(new QToolButton),
    m_hyWidget(new QWidget),
    m_gnWidget(new QWidget),
    m_areaWidget(new QWidget),
    m_buttonGroup(new QButtonGroup),
    m_nfilterFlag(103),
    m_strCode(""),
    m_strHyCode(""),
    m_strGnCode(""),
    m_strDyCode(""),
    m_strHyText(""),
    m_strGnText(""),
    m_strDyText("")
{
    createWg();
}

void BkListBottomNavigator::afterActive()
{
    if(m_buttonGroup)
    {
        if(m_buttonGroup->checkedId() != -1)
        {
            emit signalBkCategoryNavChange(m_nfilterFlag);
        }else
        {
            emit signalBkNavChange(m_nfilterFlag,m_strCode);
        }
    }
}

void BkListBottomNavigator::setFilterFlag(int nFilterFlg, bool nFlg)
{

    qDebug() << "BkListBottomNavigator::setFilterFlag" << "nFilterFlg: " << nFilterFlg <<"nFlg: " << nFlg;
    QString strHyText, strGnText, strDyText;

    if(!nFlg)
    {
        strHyText = QStringLiteral("行业板块"),
        strGnText = QStringLiteral("概念板块");
        strDyText = QStringLiteral("地域板块");
    }
    else
    {
        strHyText = m_strHyText,
        strGnText = m_strGnText;
        strDyText = m_strDyText;
    }


    if(100 == nFilterFlg)   //行业板块
    {
        m_hyBtn->setText(strHyText);
        m_hyBtn->setChecked(true);
        m_toolBtnMore->setChecked(true);

         setHyBtnChecked();

    }
    else if(101 == nFilterFlg)  //概念板块
    {
        m_gnBtn->setText(strGnText);
        m_gnBtn->setChecked(true);
        m_toolBtnMore1->setChecked(true);

        setGnBtnChecked();

    }
    else if(102 == nFilterFlg)  //地域板块
    {
        m_areaBtn->setText(strDyText);
        m_areaBtn->setChecked(true);
        m_toolBtnMore2->setChecked(true);

        setDyBtnChecked();

    }
    else if(103 == nFilterFlg)  //全部板块
    {
        m_allBkBtn->setChecked(true);

        if(m_hyBtn->isChecked() || m_toolBtnMore->isChecked()
                ||m_gnBtn->isChecked() || m_toolBtnMore1->isChecked()
                || m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
        {
            m_hyBtn->setChecked(false);
            m_gnBtn->setChecked(false);
            m_areaBtn->setChecked(false);
            m_toolBtnMore->setChecked(false);
            m_toolBtnMore1->setChecked(false);
            m_toolBtnMore2->setChecked(false);
        }
    }
}


void BkListBottomNavigator::createWg()
{
    initBtn();
    initConnect();
    initMenu();
    initTable();
    setCheckable();
}


void BkListBottomNavigator::initBtn()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);


    m_allBkBtn = new QToolButton;
    m_allBkBtn->setText(QStringLiteral("全部板块"));
    QToolButton *hyBkBtn = new QToolButton;
    hyBkBtn->setText(QStringLiteral("行业板块"));
    QToolButton *gnBkBtn = new QToolButton;
    gnBkBtn->setText(QStringLiteral("概念板块"));
    QToolButton *areaBkBtn = new QToolButton;
    areaBkBtn->setText(QStringLiteral("地区板块"));

    m_buttonGroup->addButton(m_allBkBtn, 0);

    m_hyBtn->setText(QStringLiteral("行业板块"));
    m_gnBtn->setText(QStringLiteral("概念板块"));
    m_areaBtn->setText(QStringLiteral("地域板块"));

    QHBoxLayout *hlayout = new  QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setSpacing(0);


    hlayout->addWidget(m_hyBtn);
    hlayout->addWidget(m_toolBtnMore);

    m_hyWidget->setLayout(hlayout);

    m_hyWidget->setObjectName("hywidget");

    QHBoxLayout *hlayout1 = new  QHBoxLayout;
    hlayout1->setMargin(0);
    hlayout1->setSpacing(0);

    hlayout1->addWidget(m_gnBtn);
    hlayout1->addWidget(m_toolBtnMore1);

    m_gnWidget->setLayout(hlayout1);

    m_gnWidget->setObjectName("gnwidget");


    QHBoxLayout *hlayout2 = new  QHBoxLayout;
    hlayout2->setMargin(0);
    hlayout2->setSpacing(0);

    hlayout2->addWidget(m_areaBtn);
    hlayout2->addWidget(m_toolBtnMore2);

    m_areaWidget->setLayout(hlayout2);

    m_areaWidget->setObjectName("areawidget");


    mainLayout->addWidget(m_allBkBtn);
    mainLayout->addWidget(m_hyWidget);
    mainLayout->addWidget(m_gnWidget);
    mainLayout->addWidget(m_areaWidget);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void BkListBottomNavigator::initConnect()
{
    connect(m_buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
          this, &BkListBottomNavigator::onbuttonGroupClicked);
    connect(m_hyBtn, &QToolButton::clicked, this, &BkListBottomNavigator::onHyBtnClicked);
    connect(m_gnBtn, &QToolButton::clicked, this, &BkListBottomNavigator::onGnBtnClicked);
    connect(m_areaBtn, &QToolButton::clicked, this, &BkListBottomNavigator::onAreaBtnClicked);

    connect(m_toolBtnMore, &QToolButton::clicked, this, &BkListBottomNavigator::onBtnMoreClicked);
    connect(m_toolBtnMore1, &QToolButton::clicked, this, &BkListBottomNavigator::onBtnMore1Clicked);
    connect(m_toolBtnMore2, &QToolButton::clicked, this, &BkListBottomNavigator::onBtnMore2Clicked);

    connect(m_hyDownListWidget, &DrowDownListWidget::clicked, [&]{
        qDebug() << "行业板块";
        QMenu * menu = qobject_cast<QMenu *>(m_hyDownListWidget->parent());
        if(nullptr == menu)
            return;
        menu->hide();

        emit signalBkCategoryNavChange(HZData::BlockIndustry);
        m_hyBtn->setText("行业板块");
        m_hyBtn->setChecked(true);
        m_toolBtnMore->setChecked(true);


        setHyBtnChecked();
    });

    connect(m_gnDownListWidget, &DrowDownListWidget::clicked, [&]{
        qDebug() << "全部概念";
        QMenu * menu = qobject_cast<QMenu *>(m_gnDownListWidget->parent());
        if(nullptr == menu)
            return;
        menu->hide();
        emit signalBkCategoryNavChange(HZData::BlockConcept);

        m_gnBtn->setText("概念板块");
        m_gnBtn->setChecked(true);
        m_toolBtnMore1->setChecked(true);

        setGnBtnChecked();
    });

    connect(m_areaDownListWidget, &DrowDownListWidget::clicked, [&]{
        qDebug() << "全部地域";
        QMenu * menu = qobject_cast<QMenu *>(m_areaDownListWidget->parent());
        if(nullptr == menu)
            return;
        menu->hide();
        emit signalBkCategoryNavChange(HZData::BlockRegion);

        m_areaBtn->setText("地域板块");
        m_areaBtn->setChecked(true);
        m_toolBtnMore2->setChecked(true);

        setDyBtnChecked();

    });

    connect(m_hyDownListWidget, SIGNAL(cellClicked(QString,QString)), this, SLOT(onHycellClicked(QString,QString)));
    connect(m_gnDownListWidget, SIGNAL(cellClicked(QString,QString)), this, SLOT(onGncellClicked(QString,QString)));
    connect(m_areaDownListWidget, SIGNAL(cellClicked(QString,QString)), this, SLOT(onAreacellClicked(QString,QString)));
}

void BkListBottomNavigator::initMenu()
{
    QMenu *menu = new QMenu;
    QMenu *menu1 = new QMenu;
    QMenu *menu2 = new QMenu;


    QVBoxLayout *layout = new QVBoxLayout;
    QVBoxLayout *layout1 = new QVBoxLayout;
    QVBoxLayout *layout2 = new QVBoxLayout;

    layout->setMargin(0);
    layout->setSpacing(0);
    layout1->setMargin(0);
    layout1->setSpacing(0);
    layout2->setMargin(0);
    layout2->setSpacing(0);


    layout->addWidget(m_hyDownListWidget);
    layout1->addWidget(m_gnDownListWidget);
    layout2->addWidget(m_areaDownListWidget);
    menu->setLayout(layout);
    menu1->setLayout(layout1);
    menu2->setLayout(layout2);

    m_toolBtnMore->setMenu(menu);
    m_toolBtnMore1->setMenu(menu1);
    m_toolBtnMore2->setMenu(menu2);
}

void BkListBottomNavigator::setCheckable()
{
    m_buttonGroup->setExclusive(true);
    QList<QAbstractButton *> list = m_buttonGroup->buttons();

    for(int i = 0; i < list.size(); i++)
    {
        list[i]->setCheckable(true);

    }
    if(!list.isEmpty())
    {
        list[0]->setChecked(true);
    }
    m_hyBtn->setCheckable(true);
    m_gnBtn->setCheckable(true);
    m_areaBtn->setCheckable(true);
    m_toolBtnMore->setCheckable(true);
    m_toolBtnMore1->setCheckable(true);
    m_toolBtnMore2->setCheckable(true);
}

void BkListBottomNavigator::initTable()
{
    m_hyDownListWidget->setTitle("全部行业");
    m_gnDownListWidget->setTitle("全部概念");
    m_areaDownListWidget->setTitle("全部地域");
    QVector<BlockFormatDataItem> vec;
    vec = Httpdata::instance()->getFormatDataFromBkList(HZData::BlockIndustry);
    m_hyDownListWidget->setData(vec);
    vec = Httpdata::instance()->getFormatDataFromBkList(HZData::BlockConcept);
    m_gnDownListWidget->setData(vec);
    vec = Httpdata::instance()->getFormatDataFromBkList(HZData::BlockRegion);
    m_areaDownListWidget->setData(vec);

}


void BkListBottomNavigator::showHyMenu()
{
    QPoint point = m_hyBtn->mapToGlobal(m_hyBtn->rect().bottomLeft());
    QMenu *menu = m_toolBtnMore->menu();
    menu->move(point);
    menu->show();
}

void BkListBottomNavigator::showGnMenu()
{
    QPoint point = m_gnBtn->mapToGlobal(m_gnBtn->rect().bottomLeft());
    QMenu *menu = m_toolBtnMore1->menu();
    menu->move(point);
    menu->show();
}

void BkListBottomNavigator::showAreaMenu()
{
    QPoint point = m_areaBtn->mapToGlobal(m_areaBtn->rect().bottomLeft());
    QMenu *menu = m_toolBtnMore2->menu();
    menu->move(point);
    menu->show();
}

void BkListBottomNavigator::setHyBtnChecked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(m_buttonGroup->checkedButton());
    if(btn)
    {
        m_buttonGroup->setExclusive(false);
        btn->setChecked(false);
        m_buttonGroup->setExclusive(true);
    }

    //判断另外两个控件是否选中
    if(m_toolBtnMore1->isChecked() || m_gnBtn->isChecked() ||
            m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
    {
        m_toolBtnMore1->setChecked(false);
        m_gnBtn->setChecked(false);
        m_toolBtnMore2->setChecked(false);
        m_areaBtn->setChecked(false);
    }
}

void BkListBottomNavigator::setGnBtnChecked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(m_buttonGroup->checkedButton());
    if(btn)
    {
        m_buttonGroup->setExclusive(false);
        btn->setChecked(false);
        m_buttonGroup->setExclusive(true);
    }

    //判断另外两个控件是否选中
    if(m_toolBtnMore->isChecked() || m_hyBtn->isChecked() ||
            m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
    {
        m_toolBtnMore->setChecked(false);
        m_hyBtn->setChecked(false);
        m_toolBtnMore2->setChecked(false);
        m_areaBtn->setChecked(false);
    }
}

void BkListBottomNavigator::setDyBtnChecked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(m_buttonGroup->checkedButton());

    if(btn)
    {
        m_buttonGroup->setExclusive(false);
        btn->setChecked(false);
        m_buttonGroup->setExclusive(true);
    }

    //判断另外两个控件是否选中
    if(m_toolBtnMore->isChecked() || m_hyBtn->isChecked() ||
            m_toolBtnMore1->isChecked() || m_gnBtn->isChecked())
    {
        m_toolBtnMore->setChecked(false);
        m_hyBtn->setChecked(false);
        m_toolBtnMore1->setChecked(false);
        m_gnBtn->setChecked(false);
    }
}

void BkListBottomNavigator::onbuttonGroupClicked(int nID)
{

    qDebug() << "nID = " << nID;
    switch (nID) {
    case 0:
        m_nfilterFlag = 103;
        break;
    case 1:
    case 2:
    case 3:
        m_nfilterFlag = 100 + nID - 1;
    default:
        break;
    }
    emit signalBkCategoryNavChange(m_nfilterFlag);

    if(m_toolBtnMore->isChecked() || m_hyBtn->isChecked() ||
        m_toolBtnMore1->isChecked() || m_gnBtn->isChecked() ||
            m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
    {
        m_toolBtnMore->setChecked(false);
        m_hyBtn->setChecked(false);
        m_toolBtnMore1->setChecked(false);
        m_gnBtn->setChecked(false);
        m_toolBtnMore2->setChecked(false);
        m_areaBtn->setChecked(false);
    }
}

void BkListBottomNavigator::onHyBtnClicked()
{
    QToolButton *sender = qobject_cast<QToolButton*>(this->sender());

    if(sender)
    {
        sender->setChecked(true);
        m_toolBtnMore->setChecked(true);
    }

    QString strText = sender->text();

    if(strText == QStringLiteral("行业板块"))
    {
        emit signalBkCategoryNavChange(HZData::BlockIndustry);
    }
    else
    {
        emit signalBkNavChange(HZData::BlockIndustry, m_strHyCode);
    }

    setHyBtnChecked();
}

void BkListBottomNavigator::onGnBtnClicked()
{
    QToolButton *sender = qobject_cast<QToolButton*>(this->sender());

    if(sender)
    {
        sender->setChecked(true);
        m_toolBtnMore1->setChecked(true);

    }

    QString strText = sender->text();

    if(strText == QStringLiteral("概念板块"))
    {
        emit signalBkCategoryNavChange(HZData::BlockConcept);
    }
    else
    {
        emit signalBkNavChange(HZData::BlockConcept, m_strGnCode);
    }

    setGnBtnChecked();
}

void BkListBottomNavigator::onAreaBtnClicked()
{
    QToolButton *sender = qobject_cast<QToolButton*>(this->sender());

    if(sender)
    {
        sender->setChecked(true);
        m_toolBtnMore2->setChecked(true);
    }


    QString strText = sender->text();

    if(strText == QStringLiteral("地域板块"))
    {
        emit signalBkCategoryNavChange(HZData::BlockRegion);
    }
    else
    {
        emit signalBkNavChange(HZData::BlockRegion, m_strDyCode);
    }

    setDyBtnChecked();
}

void BkListBottomNavigator::onBtnMoreClicked()
{

    showHyMenu();

    QToolButton *btn = qobject_cast<QToolButton*>(this->sender());
    if(btn)
    {
        btn->setChecked(true);
    }

    if(m_toolBtnMore1->isChecked() || m_gnBtn->isChecked() ||
            m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
    {
        m_toolBtnMore1->setChecked(false);
        m_gnBtn->setChecked(false);
        m_toolBtnMore2->setChecked(false);
        m_areaBtn->setChecked(false);
    }
}

void BkListBottomNavigator::onBtnMore1Clicked()
{
    showGnMenu();
    QToolButton *btn = qobject_cast<QToolButton*>(this->sender());
    if(btn)
    {
        btn->setChecked(true);
    }

    if(m_toolBtnMore->isChecked() || m_hyBtn->isChecked() ||
            m_toolBtnMore2->isChecked() || m_areaBtn->isChecked())
    {
        m_toolBtnMore->setChecked(false);
        m_hyBtn->setChecked(false);
        m_toolBtnMore2->setChecked(false);
        m_areaBtn->setChecked(false);
    }
}

void BkListBottomNavigator::onBtnMore2Clicked()
{
    showAreaMenu();

    QToolButton *btn = qobject_cast<QToolButton*>(this->sender());
    if(btn)
    {
        btn->setChecked(true);
    }

    if(m_toolBtnMore->isChecked() || m_hyBtn->isChecked() ||
            m_toolBtnMore1->isChecked() || m_gnBtn->isChecked())
    {
        m_toolBtnMore->setChecked(false);
        m_hyBtn->setChecked(false);
        m_toolBtnMore1->setChecked(false);
        m_gnBtn->setChecked(false);
    }
}

void BkListBottomNavigator::onHycellClicked(QString strName, QString strCode)
{
    QMenu * menu = qobject_cast<QMenu *>(m_hyDownListWidget->parent());
    if(nullptr == menu)
        return;
    menu->hide();

    m_strHyText = strName,

    m_nfilterFlag = HZData::BlockIndustry;
    m_strCode = strCode;
    m_strHyCode = strCode;
    emit signalBkNavChange(m_nfilterFlag, m_strCode);

    m_hyBtn->setText(strName);
    m_hyBtn->setChecked(true);
    m_toolBtnMore->setChecked(true);

    setHyBtnChecked();
}

void BkListBottomNavigator::onGncellClicked(QString strName, QString strCode)
{
    QMenu * menu = qobject_cast<QMenu *>(m_gnDownListWidget->parent());
    if(nullptr == menu)
        return;
    menu->hide();

    m_strGnText  = strName;


    m_nfilterFlag = HZData::BlockConcept;
    m_strCode = strCode;
    m_strGnCode = strCode;
    emit signalBkNavChange(m_nfilterFlag, m_strCode);

    m_gnBtn->setText(strName);
    m_gnBtn->setChecked(true);
    m_toolBtnMore1->setChecked(true);

    setGnBtnChecked();
}

void BkListBottomNavigator::onAreacellClicked(QString strName, QString strCode)
{
    QMenu * menu = qobject_cast<QMenu *>(m_areaDownListWidget->parent());
    if(nullptr == menu)
        return;
    menu->hide();

    m_strDyText  = strName;

    m_nfilterFlag = HZData::BlockRegion;
    m_strCode = strCode;
    m_strDyCode = strCode;
    emit signalBkNavChange(m_nfilterFlag, m_strCode);

    m_areaBtn->setText(strName);
    m_areaBtn->setChecked(true);
    m_toolBtnMore2->setChecked(true);

    setDyBtnChecked();
}















