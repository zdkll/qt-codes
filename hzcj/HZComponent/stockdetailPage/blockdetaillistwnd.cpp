#include "blockdetaillistwnd.h"
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>

#include "httpdata.h"
#include "datamodeldef.h"
#include "navilistview.h"

BlockDetaillistWnd::BlockDetaillistWnd(QWidget *parent, QString instName)
    :StockBaseDetailListWnd(parent,instName)
{
    m_pButtonGroup = new QButtonGroup(this) ;
    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotsButtonClicked(int)));
}

void BlockDetaillistWnd::slotsObjChange(const QString &obj)
{
    setStockCode(obj);
}

void BlockDetaillistWnd::slotsButtonClicked(int id)
{
    if(id == bk_detial_sort_up) //升序
    {
        m_pView->sortIndicatorChanged(HZData::block_rate,true);
    }else if(id == bk_detail_sort_down) //降序
    {
        m_pView->sortIndicatorChanged(HZData::block_rate,false);
    }
}

void BlockDetaillistWnd::slotsSwitchUpDownStock(bool isUp, int step)
{
    m_pView->switchToUpDownStock(isUp,step);
}

NaviListView *BlockDetaillistWnd::createView()
{
    return new NaviBlockListView(this);
}

void BlockDetaillistWnd::OnStockCodeChange()
{
    m_pView->setStockCode(m_stockCode);
    m_filter = (HZData::FilterFlag)Httpdata::instance()->getfilterByBkCode(m_stockCode);
    m_pView->setFilterFlag(m_filter);
    QString text = Httpdata::instance()->filterCodeToName(m_filter);
    m_pLabel->setText(text);
}

QWidget *BlockDetaillistWnd::createTitle()
{
    QGridLayout* pGrid = new QGridLayout;
    pGrid->setSpacing(0);
    pGrid->setMargin(0);

    m_pLabel = new QLabel;
    m_pLabel->setObjectName("optionLabel");
    m_pLabel->setAlignment(Qt::AlignCenter);
    m_pLabel->setText(QStringLiteral(Default_Item_String));
    m_pLabel->setFixedSize(210,26);
    pGrid->addWidget(m_pLabel,0,0,2,1);

    QPushButton* spButton =  new QPushButton(this);
    spButton->setCheckable(true);
    spButton->setObjectName("upBt");
    m_pButtonGroup->addButton(spButton,bk_detial_sort_up); //升序
    spButton->setFixedWidth(22);
    spButton->setFixedHeight(6);
    pGrid->addWidget(spButton,0,1,1,1,Qt::AlignBottom);
    pGrid->setVerticalSpacing(1);
    spButton =  new QPushButton(this);
    spButton->setCheckable(true);
    spButton->setObjectName("downBt");
    m_pButtonGroup->addButton(spButton,bk_detail_sort_down); //降序
    m_pButtonGroup->button(bk_detail_sort_down)->setChecked(true);
    spButton->setFixedWidth(22);
    spButton->setFixedHeight(6);
    pGrid->addWidget(spButton,1,1,1,1,Qt::AlignTop);
    m_pButtonGroup->setExclusive(true);

    QWidget* pWnd = new QWidget;
    pWnd->setLayout(pGrid);
    pWnd->setObjectName("titleWnd");
    return pWnd;
}

QWidget *BlockDetaillistWnd::createBody()
{
    m_pView = createView();
    addWidget(m_pView,m_pView);
    connect(m_pView,&NaviBlockListView::selectedItem,this,&BlockDetaillistWnd::signalObjChange);
    return getStackWnd();
}
