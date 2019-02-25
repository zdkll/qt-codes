#include "widget.h"
#include "ui_widget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *btnLayout = new QHBoxLayout;

    QButtonGroup *btnGroup = new QButtonGroup(this);
    QPushButton   *homeBtn = new QPushButton("Home");
    btnGroup->addButton(homeBtn,Home_Page);
    QPushButton   *stockBtn = new QPushButton("Stock");
    btnGroup->addButton(stockBtn,Stock_Page);
    QPushButton   *dataBtn = new QPushButton("Data");
    btnGroup->addButton(dataBtn,Data_Page);
    btnLayout->addWidget(homeBtn);
    btnLayout->addWidget(stockBtn);
    btnLayout->addWidget(dataBtn);
    btnLayout->addStretch(1);

    mainLayout->addLayout(btnLayout,0);

    m_pageHelper = new PageHelper(this);
    mainLayout->addWidget(m_pageHelper,1);

    this->setLayout(mainLayout);

    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotButtonClicked(int id)
{
    if(m_pageIdxMap.find(PageIdx(id)) == m_pageIdxMap.end())
        m_pageHelper->createPage(PageIdx(id));

    m_pageHelper->setCurPage(PageIdx(id));
}

