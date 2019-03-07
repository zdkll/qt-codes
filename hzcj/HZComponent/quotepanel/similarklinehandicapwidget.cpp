#include "SimilarKlineHandicapWidget.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include "subtab.h"
#include "stockinfowidget.h"
#include "fundhandicap.h"
#include "financemarster.h"
#include "quotetabwidget.h"
#include "httpdata.h"
#include "newswidget.h"
#include "hzinfoadapter.h"
#include "handicapnewswidget.h"

SimilarKlineHandicapWidget::SimilarKlineHandicapWidget(QWidget *parent, QString instName)
        :StockDetailBaseWnd(parent,instName),
          m_nReqSelect(INVALID_REQID),
          m_nReqSimilar(INVALID_REQID),
          m_selectedTitleWidget(new SimilarKlineHandicapTitleWidget),
          m_similarTitleWidget(new SimilarKlineHandicapTitleWidget),
          m_selectedCentralWidget(new HandicapCentralWidget),
          m_similarCentralWidget(new HandicapCentralWidget),
          m_stackWidget(new QStackedWidget),
          m_newswidget(new HandicapNewsWidget),
          m_anncInfo(new HZAnncsInfo("")),
          m_newsInfo(new HZNewsInfo("")),
          m_researchReport(new HZResearchReport(""))
{
    m_similarTitleWidget->setObjectName("similar");
    m_selectedTitleWidget->setObjectName("select");
    m_similarCentralWidget->setObjectName("similar");
}

void SimilarKlineHandicapWidget::createWg()
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setMargin(0);
    pLayout->setSpacing(0);


    pLayout->addWidget(m_selectedTitleWidget);
    pLayout->addWidget(m_selectedCentralWidget);
    pLayout->addWidget(m_similarTitleWidget);
    pLayout->addWidget(m_similarCentralWidget);

    CSubTab *subTab = new CSubTab(this);
    subTab->setObjectName("subtab");
    subTab->addButton(QStringLiteral("新闻"), 0);
    subTab->addButton(QStringLiteral("公告"), 1);
    subTab->addButton(QStringLiteral("研报"), 2);

    subTab->getButton(0)->setChecked(true);

    connect(subTab, &CSubTab::currentButtonChanged, this, &SimilarKlineHandicapWidget::onBtnClicked);

    pLayout->addWidget(subTab);
    pLayout->addWidget(m_newswidget);
    m_newswidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    m_newswidget->setAdapter(m_newsInfo,QStringLiteral("个股新闻"));

    this->setLayout(pLayout);
}

void SimilarKlineHandicapWidget::afterActive()
{
    QVector<HZData::AllStock> vec;
    HZData::AllStock allStock;
    allStock.stock_code = m_strSelectedCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_strSelectedCode).toStdString();
    vec.push_back(allStock);

    m_selectedTitleWidget->updateData(vec);
    m_selectedCentralWidget->updateData(vec);
    allStock.stock_code = m_strSimilarCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_strSimilarCode).toStdString();
    vec.push_back(allStock);

    m_similarTitleWidget->updateData(vec);
    m_similarCentralWidget->updateData(vec);

    subDyna();

    QVector<QString> strVec;
    strVec.append(m_strSimilarCode);
    m_newswidget->setCode(strVec);
    m_newsInfo->setNextpage(0);
    m_newswidget->afterActive();
}

void SimilarKlineHandicapWidget::afterDeactive()
{
    cancelSub();


}

void SimilarKlineHandicapWidget::switchStocks(const QString &stock1, const QString &stock2)
{
    m_strSelectedCode = stock1;
    m_strSimilarCode = stock2;

    m_selectedTitleWidget->setStockCode(m_strSelectedCode);
    m_similarTitleWidget->setStockCode(m_strSimilarCode);


    if(this->isVisible())
    {
        afterDeactive();
        afterActive();
    }
}

void SimilarKlineHandicapWidget::slotsObjChange(const QString &obj)
{

}


void SimilarKlineHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;
        QVector<HZData::AllStock> vec;
        vec.push_back(stocks[0]);

        if(QString::fromStdString(stocks[0].stock_code) ==  m_strSelectedCode)
        {
            m_selectedTitleWidget->updateData(vec);
            m_selectedCentralWidget->updateData(vec);
        }
        else if(QString::fromStdString(stocks[0].stock_code) ==  m_strSimilarCode)
        {
            m_similarTitleWidget->updateData(vec);
            m_similarCentralWidget->updateData(vec);
        }
    }
}

void SimilarKlineHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_strSelectedCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&SimilarKlineHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReqSelect =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
    reqDynaInfo.stock_code.clear();
    reqDynaInfo.stock_code.push_back(m_strSimilarCode.toStdString());
    m_nReqSimilar =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);
}

void SimilarKlineHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReqSelect)
    {
        gHZDataController->cancelSub(m_nReqSelect);
        m_nReqSelect = INVALID_REQID;
    }

    if(INVALID_REQID != m_nReqSimilar)
    {
        gHZDataController->cancelSub(m_nReqSimilar);
        m_nReqSimilar = INVALID_REQID;
    }
}

void SimilarKlineHandicapWidget::OnStockCodeChange()
{

}

void SimilarKlineHandicapWidget::onBtnClicked(int id)
{
    QVector<QString> strVec;
    strVec.append(m_strSimilarCode);
    switch (id) {
    case 0:
        m_newsInfo->setNextpage(0);
        m_newsInfo->setCode(strVec);
        m_newswidget->setAdapter(m_newsInfo,QStringLiteral("个股新闻"));
        break;
    case 1:
        m_anncInfo->setNextpage(0);
        m_anncInfo->setCode(strVec);
        m_newswidget->setAdapter(m_anncInfo,QStringLiteral("个股公告"));
        break;
    case 2:
        m_researchReport->setNextpage(0);
        m_researchReport->setCode(strVec);
        m_newswidget->setAdapter(m_researchReport,QStringLiteral("个股研报"));
        break;
    default:
        break;
    }
    m_newswidget->afterDeactive();
    m_newswidget->afterActive();
}



