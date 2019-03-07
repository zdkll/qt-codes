#include "lhlisthandicapwidget.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include "lhpartswidget.h"
#include "lhbuysellview.h"
#include "minchartwidget.h"
#include "pubmsgdef.h"
#include "scrollwidget.h"
#include "httpdata.h"

LhListHandicapWidget::LhListHandicapWidget(QWidget *parent):
    BaseWidget(parent),
    m_nReq(INVALID_REQID),
    m_titleWidget(new LhTitieWidget),
    m_minChartWidget(new MinChartWidget),
    m_dateTime(-1)
{
    this->setFixedWidth(708);
    connect(m_titleWidget, &LhTitieWidget::currentIndex, this, &LhListHandicapWidget::onCurrentIndex);
    createWg();
}

LhListHandicapWidget::~LhListHandicapWidget()
{
    deleteWidget();
}

void LhListHandicapWidget::afterActive()
{
    HZData::AllStock allStock;

    allStock.stock_code = m_stockCode.toStdString();
    allStock.stock_name = Httpdata::instance()->getCodeNameByCode(m_stockCode).toStdString();

    m_titleWidget->updateData(allStock);

    subDyna();
    reqStockLhbDetial();
    m_minChartWidget->afterActive();
}

void LhListHandicapWidget::afterDeactive()
{
    cancelSub();
    m_minChartWidget->afterDeactive();
    m_titleWidget->afterDeactive();
}

void LhListHandicapWidget::setDateTime(qint64 date)
{
    m_dateTime = date;
}


void LhListHandicapWidget::slotsBuySellViewClickRow(const QString &strID)
{
    std::map<QString,QString> msg;
    msg.emplace(HZ_CMD,ROOT_SWITCH_PAGE);
    msg.emplace(WINNERS_LIST_ID,strID);    //营业部ID
    msg.emplace(CHILD_WND_ID,QString::number(7));                  //可选
    msg.emplace(HZ_PAGE_ID,QString::number(data_center_page));
    HZObject::sendMessage(ROOT_FRAME,msg);
}

void LhListHandicapWidget::onCurrentIndex(int index)
{
    deleteWidget();
    for(int i = 0; i < m_lhDetailList[index].data.size(); i++)
    {
        LhbBottomWidget* widget = new LhbBottomWidget;
        m_bottomLayout->addWidget(widget);
        widget->updateLhData(m_lhDetailList[index].data[i]);
        m_lhbWidgetVec.append(widget);

        connect(widget, &LhbBottomWidget::clickRow, this, &LhListHandicapWidget::slotsBuySellViewClickRow);
    }
}

void LhListHandicapWidget::OnStockCodeChange()
{
    m_titleWidget->setStockCode(m_stockCode);
//    m_riseFallWidget->setStockCode(m_stockCode);
    m_minChartWidget->setStockCode(m_stockCode);
//    reqStockLhbDetial();
}

void LhListHandicapWidget::createWg()
{
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    mainlayout->addWidget(m_titleWidget);

    mainlayout->addWidget(m_minChartWidget);

    ScrollWidget *srcollwidget = new ScrollWidget;
    srcollwidget->scrollArea()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    srcollwidget->scrollArea()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_bottomWidget = new QWidget;

    m_bottomWidget->setObjectName("bottomwidget");

    m_bottomLayout = new QVBoxLayout;
    m_bottomLayout->setSpacing(0);
    m_bottomLayout->setMargin(0);

    m_bottomWidget->setLayout(m_bottomLayout);
    srcollwidget->setWidget(m_bottomWidget);
    mainlayout->addWidget(srcollwidget);

    this->setLayout(mainlayout);
}

void LhListHandicapWidget::subDyna()
{
    cancelSub();
    HZData::ReqOptionalDynaInfo reqDynaInfo;
    reqDynaInfo.stock_code.push_back(m_stockCode.toStdString());
    OnStockDynaArrived stockDynaArrived = std::bind(&LhListHandicapWidget::onStockOptionalDataArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    m_nReq =  gHZDataController->subOptionalDyna(reqDynaInfo,stockDynaArrived);

}

void LhListHandicapWidget::cancelSub()
{
    if(INVALID_REQID != m_nReq)
    {
        gHZDataController->cancelSub(m_nReq);
        m_nReq = INVALID_REQID;
    }
}

void LhListHandicapWidget::onStockOptionalDataArrived(ReqId id, char *data, int num, ErrCode errCode)
{
    Q_UNUSED(id)
    //回调通知刷新其他小版块数据
    if(errCode == 0)
    {
        HZData::AllStock *stocks = (HZData::AllStock*)data;

        m_titleWidget->updateData(stocks[0]);
    }
}

void LhListHandicapWidget::reqStockLhbDetial()
{
    StockLhbDetialCallBack fun = std::bind(&LhListHandicapWidget::onLhbDataArrived, this, std::placeholders::_1, std::placeholders::_2);
    HzInfoCenter::getInstance()->reqStockLhbDetial(m_dateTime, m_stockCode, fun);

}

void LhListHandicapWidget::onLhbDataArrived(const InfoRet &infoRet, const StockLhbDetialList &data)
{

    deleteWidget();

    if(0 != infoRet.code)
        return;
    m_lhDetailList = data;

    m_titleWidget->updateLhData(m_lhDetailList);

    if(m_lhDetailList.isEmpty())
        return;

    if(m_lhDetailList.isEmpty())
        return;
    for(int i = 0; i < m_lhDetailList[0].data.size(); i++)
    {
        LhbBottomWidget* widget = new LhbBottomWidget;
        m_bottomLayout->addWidget(widget);
        widget->updateLhData(m_lhDetailList[0].data[i]);
        m_lhbWidgetVec.append(widget);

        connect(widget, &LhbBottomWidget::clickRow, this, &LhListHandicapWidget::slotsBuySellViewClickRow);
    }
}

void LhListHandicapWidget::deleteWidget()
{
    foreach (LhbBottomWidget* widget, m_lhbWidgetVec) {
        delete widget;
    }

    m_lhbWidgetVec.clear();
}
