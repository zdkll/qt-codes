#include "FundChangeWidget.h"
#include <QBoxLayout>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>
#include "fundchangeview.h"
#include "fundchangemodel.h"

#define Cache_Row_Count  50
#define Max_ReqCount  200
#define Sub_Count 20

FundChangeWidget::FundChangeWidget(QWidget *parent):
    BaseWidget(parent),
    m_FundChangeView(new FundChangeView),
    m_subReqId(INVALID_REQID),
    m_isInitialed(false),
    m_nRowCount(-1)
{
    createWg();

    connect(m_FundChangeView, &FundChangeView::clickRow, [&](const QString strCode){
        emit clickRow(strCode);
        qDebug() << strCode;
    });
}

void FundChangeWidget::afterActive()
{
    init();
}

void FundChangeWidget::afterDeactive()
{
    m_isInitialed = false;
    cancelSub();

}

void FundChangeWidget::switchToUpDownStock(bool isUp, int step)
{
    int rowCount = m_FundChangeView->rowCount();
    if(rowCount<1) return;
    QModelIndex index = m_FundChangeView->currentIndex();
    int cur_row = 0;
    if(index.isValid()){  //没有选中默认从第0行切换
        cur_row = index.row();
    }

    int value = m_verScrollBar->value();


    int new_row = cur_row;
    if(new_row == 0 && isUp && index.isValid())
    {
        value -= step;
        value = value < m_verScrollBar->minimum()?m_verScrollBar->minimum():value;
        m_verScrollBar->setValue(value);
    }
    else if(new_row == 0 && isUp && !index.isValid())
    {
        value -= step;
        value = value < m_verScrollBar->minimum()?m_verScrollBar->minimum():value;
        m_verScrollBar->setValue(value);
        new_row = rowCount - 1;
    }
    else if(new_row == rowCount - 1 && !isUp)
    {
        value += step;
        value = value > m_verScrollBar->maximum()?m_verScrollBar->maximum():value;
        m_verScrollBar->setValue(value);
    }
    else
    {
        if(isUp){
            new_row = cur_row-step;
            value -= step;
            if(new_row<0)
                new_row = rowCount - abs(new_row)%rowCount;
        }else{
            new_row = cur_row+step;
            value += step;
            if(new_row>=rowCount)
                new_row = new_row%rowCount;
        }

        m_verScrollBar->setValue(value);
    }

    QModelIndex new_index = m_FundChangeView->model()->index(new_row, 0);

    m_FundChangeView->setCurrentIndexEx(new_index);


    FundData data = new_index.data(MYDATAROLE).value<FundData>();

    emit clickRow(data.strCode);
}

void FundChangeWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int index = m_verScrollBar->value();
    int totalCount = m_fundInfo.total_cnt;
    int maxRowCount = m_FundChangeView->maxRowCount();

    if(index == m_verScrollBar->maximum())
    {
        int screenRowCount =  totalCount < maxRowCount?totalCount:maxRowCount;

        index = totalCount - screenRowCount;

        m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(index));

        updateScrollBar(index);
    }
    else
    {
        if(-1 == m_nRowCount)
            return;

        int endPos = m_nRowCount + index;

        int nStart = endPos - maxRowCount;

        if(nStart < 0)
        {
            nStart = 0;
        }

        m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(nStart));
    }

    reqFundData();
}

void FundChangeWidget::keyPressEvent(QKeyEvent *e)
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
    BaseWidget::keyPressEvent(e);
}

void FundChangeWidget::wheelEvent(QWheelEvent *e)
{
    //+-1
    int numSteps = e->angleDelta().y()/8/15;
    int value = m_verScrollBar->value();
    value -= numSteps*m_verScrollBar->singleStep();

    value = value < m_verScrollBar->minimum()?m_verScrollBar->minimum():value;
    value = value > m_verScrollBar->maximum()?m_verScrollBar->maximum():value;
    m_verScrollBar->setValue(value);
}

void FundChangeWidget::showEvent(QShowEvent *e)
{
    //    init();
}

void FundChangeWidget::keyReleaseEvent(QKeyEvent *e)
{

    if(e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        QModelIndex index = m_FundChangeView->currentIndex();

        FundData data = index.data(MYDATAROLE).value<FundData>();

        emit clickRow(data.strCode);
    }
}

void FundChangeWidget::scrollPressed()
{
    if(m_FundChangeView->rowCount() < m_FundChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void FundChangeWidget::scrollActionTriggered()
{
    if(m_FundChangeView->rowCount() < m_FundChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void FundChangeWidget::scrollValueChanged(int value)
{
    if(m_fundInfo.shortMotions.size() > 0)
    {
        m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(value));
    }

    reqFundData();

    if(m_FundChangeView->rowCount() < m_FundChangeView->maxRowCount())
    {
        alignScrollBar();
    }
}

void FundChangeWidget::createWg()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    m_verScrollBar = new QScrollBar(Qt::Vertical);
    mainLayout->addWidget(m_FundChangeView);
    mainLayout->addWidget(m_verScrollBar);

    connect(m_verScrollBar, &QScrollBar::sliderPressed, this, &FundChangeWidget::scrollPressed);
    connect(m_verScrollBar, &QScrollBar::actionTriggered, this, &FundChangeWidget::scrollActionTriggered);
    connect(m_verScrollBar, &QScrollBar::valueChanged, this, &FundChangeWidget::scrollValueChanged);
}

void FundChangeWidget::init()
{
    //清除显示数据
    m_FundChangeView->clearData();

    m_dataRange.first = m_dataRange.second = -1;

    int reqCount = m_FundChangeView->maxRowCount() + Cache_Row_Count;

    postFundRequest(-1, reqCount, true);
}

void FundChangeWidget::startSub()
{
    cancelSub();

    ReqShortMotionInfo req;
    req.type = SMCT_Money;
    req.index = -1;
    req.count_range = Sub_Count;
    req.is_desc = true; //反拉

    OnShortMotionInfoArrived fun = std::bind(&FundChangeWidget::onSubDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    m_subReqId = gHZDataController->subShortMotionInfo(req, fun);
}

void FundChangeWidget::cancelSub()
{
    if(INVALID_REQID != m_subReqId)
    {
        gHZDataController->cancelSub(m_subReqId);
        m_subReqId = INVALID_REQID;
    }
}

void FundChangeWidget::reqFundData()
{
    if(m_reqRecrd.reqId != INVALID_REQID)
        return;

    int index = m_dataRange.first;//起始位置
    int value = m_verScrollBar->value(); //滚动条位置
    if(value >= m_dataRange.first + Cache_Row_Count || index == 0)
        return;

    int count = Max_ReqCount+1;

    postFundRequest(index, count, true);
}

ReqId FundChangeWidget::postFundRequest(const int &index, const int &count, const bool &is_desc)
{
    ReqShortMotionInfo req;
    req.type = SMCT_Money;
    req.index = (index==-1)?-1:(index - 1);
    req.count_range =  count;
    req.is_desc = is_desc; //反拉

    OnShortMotionInfoArrived fun = std::bind(&FundChangeWidget::onReqDataArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    ReqId reqId = gHZDataController->requestShortMotionInfo(req, fun);

    m_reqRecrd.reqId = reqId;
    m_reqRecrd.index = index;
    m_reqRecrd.count = count;
    m_reqRecrd.is_desc = is_desc;

    return reqId;
}

void FundChangeWidget::onReqDataArrived(ReqId id, const ShortMotionInfo &FundInfo, ErrCode errCode)
{

    if(0 != errCode)
        return;
    //1 保存数据
    saveFundData(m_reqRecrd,FundInfo);

    //2 更新显示数据
    if(!m_isInitialed){
        int  index = m_fundInfo.total_cnt - m_FundChangeView->maxRowCount();
        index = index<0?0:index;

        if(m_fundInfo.shortMotions.size()>0)
        {
            m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(index)); 
        }

        //更新滚动条
        updateScrollBar(index);
    }else{
        int index = m_verScrollBar->value();
        m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(index));
    }

    m_reqRecrd.reqId = INVALID_REQID;

    if(!m_isInitialed){
        startSub();
        m_isInitialed = true;
    }
    if(m_fundInfo.shortMotions.size()>0)
        reqFundData();
}

void FundChangeWidget::onSubDataArrived(ReqId id, const ShortMotionInfo &FundInfo, ErrCode errCode)
{
    if(0 != errCode)
        return;

    int totalCount = FundInfo.total_cnt;

    //日志
//    QString strPath = qApp->applicationDirPath() + "/test.txt";
//    QFile file(strPath);

//    file.open(QIODevice::Append | QIODevice::Text);
//    QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss");

//    QTextStream out(&file);

//    out << endl;
//    out << "newCount = "  << totalCount  - m_dataRange.second - 1
//        << " m_dataRange.second= "<< m_dataRange.second<< " " << strTime ;


//    file.close();


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
            int maxRowCount = m_FundChangeView->maxRowCount();
            int currentRowCount = totalCount<maxRowCount?totalCount:maxRowCount;
            index = totalCount - currentRowCount;

            m_FundChangeView->upDateData(m_fundInfo.shortMotions.mid(index));
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

void FundChangeWidget::updateScrollBarPageAndRange()
{
    //总个数
    int count = m_fundInfo.total_cnt;

    //当前显示范围
    int rowCount = m_FundChangeView->maxRowCount();
    m_verScrollBar->setPageStep(rowCount);

    int max = (count-rowCount);

    m_verScrollBar->setRange(0,max);
}

void FundChangeWidget::alignScrollBar()
{
    int totalCount = m_fundInfo.total_cnt;
    int maxRowCount = m_FundChangeView->maxRowCount();
    int count = totalCount<maxRowCount?totalCount:maxRowCount;

    m_verScrollBar->setPageStep(count);
    m_verScrollBar->setValue(totalCount-count);
    m_verScrollBar->setRange(0, totalCount-count);
}

void FundChangeWidget::updateScrollBar(const int &value)
{
    int totalCount = m_fundInfo.total_cnt;
    int pageStep  = m_FundChangeView->rowCount();
    m_verScrollBar->setPageStep(pageStep);

    int max = (totalCount-pageStep);

    m_verScrollBar->setRange(0,max);

    m_verScrollBar->setValue(value);

    m_nRowCount = m_FundChangeView->rowCount();

    qDebug() << "m_verScrollBar->value():" << m_verScrollBar->value();
}

int FundChangeWidget::saveFundData(const DataRecord &recrd, const ShortMotionInfo &FundInfo)
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



