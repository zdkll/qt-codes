#include "ShortTermChangeWidget.h"
#include <QBoxLayout>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include "ShortTermChangeview.h"

#define Cache_Row_Count  50
#define Max_ReqCount  200
#define Sub_Count 30

ShortTermChangeWidget::ShortTermChangeWidget(QWidget *parent):
    BaseWidget(parent),
    m_shortTermChangeView(new ShortTermChangeView),
    m_subReqId(INVALID_REQID),
    m_isInitialed(false)
{
    createWg();
    init();

    connect(m_shortTermChangeView, &ShortTermChangeView::clickedRow, [&](const QString strCode){
        emit clickedRow(strCode);
    });
}

ShortTermChangeWidget::~ShortTermChangeWidget()
{
    cancelSub();
}

void ShortTermChangeWidget::afterActive()
{
    init();
}

void ShortTermChangeWidget::afterDeactive()
{
    m_isInitialed = false;
}

void ShortTermChangeWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int value = m_verScrollBar->value();
    m_shortTermChangeView->upDateData(m_fundInfo.shortMotions.mid(value));

    reqFundData();

    updateScrollBarPageAndRange();
}

void ShortTermChangeWidget::keyPressEvent(QKeyEvent *e)
{
    int d = 0;
    if(e->key() == Qt::Key_Up)
    {
        d = -m_verScrollBar->singleStep();
    }
    if(e->key() == Qt::Key_Down)
    {
        d = m_verScrollBar->singleStep();
    }
    if(e->key() == Qt::Key_PageUp)
    {
        d = -m_verScrollBar->pageStep();
    }
    if(e->key() == Qt::Key_PageDown)
    {
        d = m_verScrollBar->pageStep();
    }

    if(d != 0)
    {
        int value = m_verScrollBar->value() + d;

        value = value < m_verScrollBar->minimum()?m_verScrollBar->minimum():value;
        value = value > m_verScrollBar->maximum()?m_verScrollBar->maximum():value;
        m_verScrollBar->setValue(value);
    }
}

void ShortTermChangeWidget::wheelEvent(QWheelEvent *e)
{
    //+-1
    int numSteps = e->angleDelta().y()/8/15;
    int value = m_verScrollBar->value();
    value -= numSteps*m_verScrollBar->singleStep();

    value = value < m_verScrollBar->minimum()?m_verScrollBar->minimum():value;
    value = value > m_verScrollBar->maximum()?m_verScrollBar->maximum():value;
    m_verScrollBar->setValue(value);
}

void ShortTermChangeWidget::showEvent(QShowEvent *e)
{
//    init();
}

void ShortTermChangeWidget::scrollPressed()
{
    if(m_shortTermChangeView->rowCount() < m_shortTermChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void ShortTermChangeWidget::scrollActionTriggered()
{
    if(m_shortTermChangeView->rowCount() < m_shortTermChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void ShortTermChangeWidget::scrollValueChanged(int value)
{
    if(m_fundInfo.shortMotions.size() > 0)
    {
        m_shortTermChangeView->upDateData(m_fundInfo.shortMotions.mid(value));
    }

    reqFundData();

    if(m_shortTermChangeView->rowCount() < m_shortTermChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void ShortTermChangeWidget::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    m_verScrollBar = new QScrollBar(Qt::Vertical);
    mainLayout->addWidget(m_shortTermChangeView);
    mainLayout->addWidget(m_verScrollBar);

    connect(m_verScrollBar, &QScrollBar::sliderPressed, this, &ShortTermChangeWidget::scrollPressed);
    connect(m_verScrollBar, &QScrollBar::actionTriggered, this, &ShortTermChangeWidget::scrollActionTriggered);
    connect(m_verScrollBar, &QScrollBar::valueChanged, this, &ShortTermChangeWidget::scrollValueChanged);
}

void ShortTermChangeWidget::init()
{
    //清除显示数据
    m_shortTermChangeView->clearData();

    m_dataRange.first = m_dataRange.second = -1;

    int reqCount = m_shortTermChangeView->maxRowCount() + Cache_Row_Count;

    postFundRequest(-1, reqCount, true);
}

void ShortTermChangeWidget::startSub()
{
    cancelSub();

    ReqShortMotionInfo req;
    req.type = SMCT_Total;
    req.index = -1;
    req.count_range = Sub_Count;
    req.is_desc = true; //反拉

    OnShortMotionInfoArrived fun = std::bind(&ShortTermChangeWidget::onSubDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_subReqId = gHZDataController->subShortMotionInfo(req, fun);
}

void ShortTermChangeWidget::cancelSub()
{
    if(INVALID_REQID != m_subReqId)
    {
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void ShortTermChangeWidget::reqFundData()
{
    if(m_reqRecrd.reqId != INVALID_REQID)
        return;

    int index = m_dataRange.first;
    int value = m_verScrollBar->value();
    if(value >= m_dataRange.first + Cache_Row_Count || index == 0)
        return;

    int count = Max_ReqCount+1;

    postFundRequest(index, count, true);
}

ReqId ShortTermChangeWidget::postFundRequest(const int &index, const int &count, const bool &is_desc)
{
    ReqShortMotionInfo req;
    req.type = SMCT_Total;
    req.index = (index==-1)?-1:(index - 1);
    req.count_range =  count;
    req.is_desc = is_desc; //反拉

    OnShortMotionInfoArrived fun = std::bind(&ShortTermChangeWidget::onReqDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    ReqId reqId = gHZDataController->requestShortMotionInfo(req, fun);

    m_reqRecrd.reqId = reqId;
    m_reqRecrd.index = index;
    m_reqRecrd.count = count;
    m_reqRecrd.is_desc = is_desc;

    return reqId;
}

void ShortTermChangeWidget::onReqDataArrived(ReqId id, const ShortMotionInfo &FundInfo, ErrCode errCode)
{

    if(0 != errCode)
        return;
    //1 保存数据
    saveFundData(m_reqRecrd,FundInfo);

    //2 更新显示数据
    if(!m_isInitialed){
        int  index = m_fundInfo.total_cnt - m_shortTermChangeView->maxRowCount();
        index = index<0?0:index;

        if(m_fundInfo.shortMotions.size()>0)
        {
            m_shortTermChangeView->upDateData(m_fundInfo.shortMotions.mid(index));
        }

        //更新滚动条
        updateScrollBar(index);
    }else{
        int index = m_verScrollBar->value();
        m_shortTermChangeView->upDateData(m_fundInfo.shortMotions.mid(index));
    }

    m_reqRecrd.reqId = INVALID_REQID;

    if(!m_isInitialed){
        startSub();
        m_isInitialed = true;
    }
    if(m_fundInfo.shortMotions.size()>0)
        reqFundData();
}

void ShortTermChangeWidget::onSubDataArrived(ReqId id, const ShortMotionInfo &FundInfo, ErrCode errCode)
{
    if(0 != errCode)
        return;

    int totalCount = FundInfo.total_cnt;

        //日志
    QString strPath = qApp->applicationDirPath() + "/test111.txt";
    QFile file(strPath);

    file.open(QIODevice::Append | QIODevice::Text);
    QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss");

    QTextStream out(&file);

    out << endl;
    out << "newCount = "  << totalCount  - m_dataRange.second - 1
        << " m_dataRange.second= "<< m_dataRange.second<< " " << strTime ;


    file.close();


    //有最新数据
    if(m_dataRange.second < totalCount-1)
    {
        m_fundInfo.shortMotions.resize(totalCount);
        int newCount = totalCount - m_dataRange.second-1;
        if(newCount <1)
            return;



        int idx = FundInfo.shortMotions.size() - newCount;
        if(idx < 0)
            return;
        for(;idx<FundInfo.shortMotions.size();idx++){
            m_fundInfo.shortMotions.replace(m_dataRange.second+1,FundInfo.shortMotions[idx]);
            m_dataRange.second++;
        }
        m_fundInfo.total_cnt     = totalCount;

        int index = m_verScrollBar->value();
        //当期显示到最后-更新显示
        if(index == m_verScrollBar->maximum()){
            int maxRowCount = m_shortTermChangeView->maxRowCount();
            int currentRowCount = totalCount<maxRowCount?totalCount:maxRowCount;
            index = totalCount - currentRowCount;

            m_shortTermChangeView->upDateData(m_fundInfo.shortMotions.mid(index));
        }
        updateScrollBar(index);
    }
    if(totalCount < m_fundInfo.total_cnt )
    {
        //如果数据量变少，在交易日切换时候清除逐笔数据了
        //重新拉取数据
        init();
    }
}

void ShortTermChangeWidget::updateScrollBarPageAndRange()
{
    //总个数
    int count = m_fundInfo.total_cnt;

    //当前显示范围
    int rowCount = m_shortTermChangeView->maxRowCount();
    m_verScrollBar->setPageStep(rowCount);

    int max = (count-rowCount);

    m_verScrollBar->setRange(0,max);
}

void ShortTermChangeWidget::alignScrollBar()
{
    int totalCount = m_fundInfo.total_cnt;
    int maxRowCount = m_shortTermChangeView->maxRowCount();
    int count = totalCount<maxRowCount?totalCount:maxRowCount;

    m_verScrollBar->setPageStep(count);
    m_verScrollBar->setValue(totalCount-count);
    m_verScrollBar->setRange(0, totalCount-count);
}

void ShortTermChangeWidget::updateScrollBar(const int &value)
{
    int totalCount = m_fundInfo.total_cnt;
    int pageStep  = m_shortTermChangeView->rowCount();
    m_verScrollBar->setPageStep(pageStep);

    int max = (totalCount-pageStep);

    m_verScrollBar->setRange(0,max);

    m_verScrollBar->setValue(value);

    qDebug() << "m_verScrollBar->value():" << m_verScrollBar->value();
}

int ShortTermChangeWidget::saveFundData(const ShortDataRecord &recrd, const ShortMotionInfo &FundInfo)
{
    int totalCount = FundInfo.total_cnt;
    if(m_fundInfo.total_cnt != totalCount){
        m_fundInfo.total_cnt = totalCount;
        m_fundInfo.shortMotions.resize(totalCount);
    }

    int   count     = FundInfo.shortMotions.size();
    if(count < 1)
        return 0;

    int   start       = recrd.index;
    bool desc     = recrd.is_desc;

    //倒拉
    if(desc)
        start = (start == -1)?(totalCount - count ):(start - count);

    for(int i=0;i<count;i++){
        m_fundInfo.shortMotions.replace(start+i,FundInfo.shortMotions[i]);
    }

    int end = start +count-1;
    if(m_dataRange.second ==  -1){
        m_dataRange.first        = start;
        m_dataRange.second  = end;
    }else{
        m_dataRange.first        =  m_dataRange.first <start?m_dataRange.first:start;
        m_dataRange.second  =  m_dataRange.second >(end)?m_dataRange.second:end;
    }

    return count;
}



