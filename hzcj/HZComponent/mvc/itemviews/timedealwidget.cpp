#include "timedealwidget.h"

#define Cache_Row_Count                           50
#define SLCache_Row_Count                      10
#define Max_ReqCount                                 200
#define Sub_Count                                       5

#include <QScrollBar>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QLabel>

#include "httpdata.h"
#include "../itemmodels/timedealmodel.h"
#include "../itemdelegates/timedealitemdelegates.h"



///AllTimeDealWidget-----
AllTimeDealWidget::AllTimeDealWidget(QWidget *parent)
    :BaseTimeDealWidget(parent,"alltimedealwidget")
{
    initWg();
    this->resize(1000,800);
    m_dataScope.first = m_dataScope.second = -1;
    m_reqRecrd.reqId = INVALID_REQID;
}

AllTimeDealWidget::~AllTimeDealWidget()
{

}

void AllTimeDealWidget::initStylePropoerty()
{
    QFont tickDataFont;
    tickDataFont.setFamily(fontFamily());
    tickDataFont.setPixelSize(fontPointSize());
    tickDataFont.setBold(true);
    tickDataFont.setStyleStrategy(QFont::NoAntialias);


    QVector<QStyledItemDelegate*> itemDelegates(3);
    AllTickColTimeDelegate *colTimeDelegate = new AllTickColTimeDelegate(this);
    colTimeDelegate->setColor(colTimeColor());
    colTimeDelegate->setFont(tickDataFont);
    itemDelegates[0] = colTimeDelegate;

    AllTickColPriceDelegate *colPriceDelegate = new AllTickColPriceDelegate(this);
    colPriceDelegate->setColor(colPriceColor());
    colPriceDelegate->setFont(tickDataFont);
    itemDelegates[1] =  colPriceDelegate;

    AllTickColVolumeDelegate *colVolDelegate = new AllTickColVolumeDelegate(this);
    colVolDelegate->setUpColor(colVolUpColor());
    colVolDelegate->setDownColor(colVolDownColor());
    colVolDelegate->setUpArrowPix(upArrowPix());
    colVolDelegate->setDownArrowPix(downArrowPix());
    colVolDelegate->setFont(tickDataFont);
    itemDelegates[2] = colVolDelegate;

    for(int i =0;i<m_tableViews.size();i++){
        m_tableViews[i]->setItemDelegateForColumn(0,itemDelegates[0]);
        m_tableViews[i]->setItemDelegateForColumn(1,itemDelegates[1]);
        m_tableViews[i]->setItemDelegateForColumn(2,itemDelegates[2]);
    }
}

void AllTimeDealWidget::reqTickData()
{
    if(m_reqRecrd.reqId != INVALID_REQID)
        return;

    int index           = m_dataScope.first;
    int scroll_value =  m_scrollBar->value();
    if(scroll_value>=m_dataScope.first + Cache_Row_Count || index==0)
        return;

    int   count = Max_ReqCount+1;
    posTickRequest(index,count,true);
}

void AllTimeDealWidget::initWg()
{
    TimeDealTitleWg *wg = new TimeDealTitleWg(this);
    wg->setFixedHeight(30);
    wg->setObjectName("titleWg");
    this->setTitleWg(wg);
    QList<float> stretchFactors;
    stretchFactors<<0.33f<<0.33f;

    for(int i=0;i<4;i++){
        TimeDealTableView *view = new TimeDealTableView (this);
        view->setStretchFactors(stretchFactors);

        addTableView(view);
    }

    connect(m_scrollBar,&QScrollBar::sliderPressed,this,&AllTimeDealWidget::scrollPressed);
    connect(m_scrollBar,&QScrollBar::actionTriggered,this,&AllTimeDealWidget::scrollActionTriggered);
    connect(m_scrollBar,&QScrollBar::valueChanged,this,&AllTimeDealWidget::scrollValueChanged);
}


void AllTimeDealWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    //大小改变，从当前index 数据按顺序填充表
    this->setData(m_zhubi.ticks.mid(m_scrollBar->value()));

    reqTickData();

    //只更新显示 page 大小和滚动条 总的范围
    updateScrollBarPageAndRange();
}

void AllTimeDealWidget::keyPressEvent(QKeyEvent  *e)
{
    int d = 0;
    if(e->key() == Qt::Key_Up){
        d = -m_scrollBar->singleStep();
    }
    if(e->key() == Qt::Key_Down){
        d = m_scrollBar->singleStep();
    }
    if(e->key() == Qt::Key_PageUp){
        d = -m_scrollBar->pageStep();
    }
    if(e->key() == Qt::Key_PageDown){
        d = m_scrollBar->pageStep();
    }
    if(d!=0){
        int value = m_scrollBar->value() +d;
        value  = value<m_scrollBar->minimum()?m_scrollBar->minimum():value;
        value  = value>m_scrollBar->maximum()?m_scrollBar->maximum():value;
        m_scrollBar->setValue(value);
    }
    return BaseTimeDealWidget::keyPressEvent(e);
}

void AllTimeDealWidget::wheelEvent(QWheelEvent *e)
{
    //+-1
    int numSteps = e->angleDelta().y()/8/15;

    int value = m_scrollBar->value();
    value -= numSteps*m_scrollBar->singleStep();

    value  = value<m_scrollBar->minimum()?m_scrollBar->minimum():value;
    value  = value>m_scrollBar->maximum()?m_scrollBar->maximum():value;
    m_scrollBar->setValue(value);
}

void AllTimeDealWidget::init()
{
    m_dataScope.first = m_dataScope.second = -1;

    int reqCount = currentMaxRowCount() + Cache_Row_Count;

    posTickRequest(-1,reqCount,true);
}

void  AllTimeDealWidget::startSub()
{
    //开始订阅需要把上次订阅取消
    cancelSub();

    HZData::ReqTickInfo reqTickInfo;
    reqTickInfo.stock_code = m_stockCode;

    reqTickInfo.update_time= -1;
    reqTickInfo.count    = Sub_Count;
    reqTickInfo.is_desc= true;

    //从最新往前拉
    OnTickArrived func = std::bind(&AllTimeDealWidget::onSubTickArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    m_subReqId = DataController::instance()->subTick(reqTickInfo,func);
}

void AllTimeDealWidget::scrollPressed()
{
    if(currentRowCount()<currentMaxRowCount()){
        alignScrollBar();
    }
}

void AllTimeDealWidget::scrollActionTriggered()
{
    if(currentRowCount()<currentMaxRowCount()){
        alignScrollBar();
    }
}

void  AllTimeDealWidget::scrollValueChanged(int value)
{
    if(m_zhubi.ticks.size()>0)
        this->setData(m_zhubi.ticks.mid(value));

    reqTickData();

    if(currentRowCount()<currentMaxRowCount()){
        alignScrollBar();
    }
}

ReqId AllTimeDealWidget::posTickRequest(const int &index,const uint &count,const bool &is_desc)
{
    OnTickArrived func = std::bind(&AllTimeDealWidget::onTickArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    HZData::ReqTickInfo  reqTickInfo;
    reqTickInfo.stock_code = m_stockCode;
    reqTickInfo.update_time= (index == -1)?-1:m_zhubi.ticks[index].update_time;

    reqTickInfo.count         = count;
    reqTickInfo.is_desc      = is_desc;
    ReqId reqId = DataController::instance()->requestTick(reqTickInfo,func);

    //保存请求记录
    m_reqRecrd.reqId = reqId;
    m_reqRecrd.index = index;
    m_reqRecrd.count = count;
    m_reqRecrd.is_desc = is_desc;

    return reqId;
}

void  AllTimeDealWidget::onTickArrived(const  ReqId  &reqId,const  HZData::Zhubi &zhubi ,const  ErrCode &errCode)
{
    //    qDebug()<<"save data reqId:"<<reqId
    //           <<m_reqRecrd.index<<m_reqRecrd.count<<"recv totalCount:"<<zhubi.totalCount
    //          <<"recv count:"<<zhubi.ticks.size();

    //1 保存数据 m_zhubi
    saveZhubiData(m_reqRecrd,zhubi);

    //2 更新显示数据
    if(!m_isInitialed){
        int  index = m_zhubi.totalCount - currentMaxRowCount();
        index = index<0?0:index;

        if(m_zhubi.ticks.size()>0)
            this->setData(m_zhubi.ticks.mid(index));

        //更新滚动条
        updateScrollBar(index);
    }else{
        int index = m_scrollBar->value();
        this->setData(m_zhubi.ticks.mid(index));
    }

    m_reqRecrd.reqId = INVALID_REQID;

    if(!m_isInitialed){
        startSub();
        m_isInitialed = true;
    }
    if(m_zhubi.ticks.size()>0)
        reqTickData();
}

void AllTimeDealWidget::onSubTickArrived(const  ReqId  &,const  HZData::Zhubi &zhubi,const  ErrCode & )
{
    int totalCount = zhubi.totalCount;

    //有最新数据
    if(m_dataScope.second < totalCount-1)
    {
        m_zhubi.ticks.resize(totalCount);
        int newCount = totalCount - m_dataScope.second-1;
        if(newCount <1)
            return;

        int idx = zhubi.ticks.size() - newCount;
        for(;idx<zhubi.ticks.size();idx++){
            m_zhubi.ticks.replace(m_dataScope.second+1,zhubi.ticks[idx]);
            m_dataScope.second++;
        }
        m_zhubi.totalCount     = totalCount;

        int index = m_scrollBar->value();
        //当期显示到最后-更新显示
        if(m_scrollBar->value() == m_scrollBar->maximum()){
            int maxRowCount = currentMaxRowCount();
            int currentRowCount = totalCount<maxRowCount?totalCount:maxRowCount;
            index = totalCount - currentRowCount;

            this->setData(m_zhubi.ticks.mid(index));
        }
        updateScrollBar(index);
    }
    if(totalCount < m_zhubi.totalCount )
    {
        //如果数据量变少，在交易日切换时候清除逐笔数据了
        //重新拉取数据
        init();
    }
}

int AllTimeDealWidget::saveZhubiData(const ReqRecord &recrd,const HZData::Zhubi &zhubi)
{
    int totalCount = zhubi.totalCount;
    if(m_zhubi.totalCount != totalCount){
        m_zhubi.totalCount = totalCount;
        m_zhubi.ticks.resize(totalCount);
    }

    int   count     = zhubi.ticks.size();
    if(count < 1)
        return 0;

    int   start       = recrd.index;
    bool desc     = recrd.is_desc;

    //倒拉
    if(desc)
        start = (start == -1)?(totalCount - count ):(start - count+1);

    //    qDebug()<<"save  data total count:"<<totalCount
    //           <<"start:"<<start<<" count:"<<count;

    for(int i=0;i<count;i++){
        m_zhubi.ticks.replace(start+i,zhubi.ticks[i]);
    }

    int end = start +count-1;
    if(m_dataScope.second ==  -1){
        m_dataScope.first        = start;
        m_dataScope.second  = end;
    }else{
        m_dataScope.first        =  m_dataScope.first <start?m_dataScope.first:start;
        m_dataScope.second  =  m_dataScope.second >(end)?m_dataScope.second:end;
    }

    //qDebug()<<"save data scope:"<<m_dataScope;
    return count;
}

void AllTimeDealWidget::updateScrollBarPageAndRange()
{
    //总个数
    int count = m_zhubi.totalCount;

    //当前显示范围
    int rowCount = currentRowCount();
    m_scrollBar->setPageStep(rowCount);

    int max = (count-rowCount);

    m_scrollBar->setRange(0,max);
}

void AllTimeDealWidget::updateScrollBar(const int &value)
{
    int totalCount = m_zhubi.totalCount;
    int pageStep  = currentRowCount();
    m_scrollBar->setPageStep(pageStep);

    int max = (totalCount-pageStep);

    m_scrollBar->setRange(0,max);

    m_scrollBar->setValue(value);
}

void AllTimeDealWidget::alignScrollBar()
{
    int totalCount = m_zhubi.totalCount;
    int maxRowCount = currentMaxRowCount();
    int count = totalCount<maxRowCount?totalCount:maxRowCount;

    m_scrollBar->setPageStep(count);
    m_scrollBar->setValue(totalCount-count);
    m_scrollBar->setRange(0, totalCount-count);
}

///SLTimeDealWidget------------------------
SLTimeDealWidget::SLTimeDealWidget(QWidget *parent )
    :BaseTimeDealWidget(parent,"sltimedealwidget")
{
    initWg();
}

SLTimeDealWidget::~SLTimeDealWidget()
{

}

void SLTimeDealWidget::setStockCode(const string &stockcode, const QString &)
{
    m_stockCode = stockcode;
    m_isInitialed  = false;
}

void SLTimeDealWidget::initStylePropoerty()
{
    QFont tickDataFont;
    tickDataFont.setFamily(fontFamily());
    tickDataFont.setPixelSize(fontPointSize());
    //tickDataFont.setBold(true);
    tickDataFont.setStyleStrategy(QFont::NoAntialias);

    QVector<QStyledItemDelegate*> itemDelegates(3);

    //时间
    SLTickColTimeDelegate *colTimeDelegate = new SLTickColTimeDelegate(this);
    colTimeDelegate->setColor(colTimeColor());
    colTimeDelegate->setFont(tickDataFont);
    colTimeDelegate->setGridLineColor(gridLineColor());
    itemDelegates[0] = colTimeDelegate;
    //价格
    SLTickColPriceDelegate *colPriceDelegate = new SLTickColPriceDelegate(this);
    colPriceDelegate->setColor(colPriceColor());
    colPriceDelegate->setFont(tickDataFont);
    colPriceDelegate->setGridLineColor(gridLineColor());
    itemDelegates[1] =  colPriceDelegate;
    //成交量
    SLTickColVolumeDelegate *colVolDelegate = new SLTickColVolumeDelegate(this);
    colVolDelegate->setUpColor(colVolUpColor());
    colVolDelegate->setDownColor(colVolDownColor());
    colVolDelegate->setFont(tickDataFont);
    colVolDelegate->setUpArrowPix(upArrowPix());
    colVolDelegate->setDownArrowPix(downArrowPix());
    colVolDelegate->setGridLineColor(gridLineColor());
    itemDelegates[2] = colVolDelegate;

    for(int i =0;i<m_tableViews.size();i++){
        m_tableViews[i]->setItemDelegateForColumn(0,itemDelegates[0]);
        m_tableViews[i]->setItemDelegateForColumn(1,itemDelegates[1]);
        m_tableViews[i]->setItemDelegateForColumn(2,itemDelegates[2]);
    }
}

void SLTimeDealWidget::init()
{
    //初始化
    this->clearData();

    int reqCount = currentMaxRowCount() + SLCache_Row_Count;

    posTickRequest(-1,reqCount,true);
}

void SLTimeDealWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    int curMaxRowCount =currentMaxRowCount();

    int dataSize = m_zhubi.ticks.size();
    int index      = dataSize - curMaxRowCount;
    index           = index<0?0:index;
    if(dataSize>0){
        this->setData(m_zhubi.ticks.mid(index));
        //倒拉之前数据
        reqTickData();
    }
}

void SLTimeDealWidget::reqTickData()
{
    if(m_reqRecrd.reqId != INVALID_REQID)
        return;
    int dataSize               = m_zhubi.ticks.size() ;
    int curMaxRowCount = currentMaxRowCount();

    if(dataSize >= curMaxRowCount || dataSize >= m_zhubi.totalCount)
        return;

    int count = curMaxRowCount - dataSize;

    posTickRequest(0,count,true);
}

void SLTimeDealWidget::initWg()
{
    this->setShowVerticalScrollBar(false);

    TimeDealTableView *view1 = new TimeDealTableView(this);
    view1->horizontalHeader()->hide();
    view1->setShowGrid(false);
    addTableView(view1);

    QList<float> stretchFactors;
    stretchFactors<<0.33f<<0.35f;
    view1->setStretchFactors(stretchFactors);
}

ReqId  SLTimeDealWidget::posTickRequest(const int &index,const uint &count,const bool &is_desc)
{
    OnTickArrived func = std::bind(&SLTimeDealWidget::onTickArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    HZData::ReqTickInfo      reqTickInfo;
    reqTickInfo.stock_code = m_stockCode;
    reqTickInfo.update_time =  index == -1?index:m_zhubi.ticks[index].update_time;
    reqTickInfo.count    = count;
    reqTickInfo.is_desc= is_desc;
    ReqId reqId = DataController::instance()->requestTick(reqTickInfo,func);

    //保存请求记录
    m_reqRecrd.reqId =  reqId;
    m_reqRecrd.index = index;
    m_reqRecrd.count =  count;
    m_reqRecrd.is_desc = is_desc;

    return reqId;
}

void SLTimeDealWidget::onTickArrived(const  ReqId  &,const  HZData::Zhubi &zhubi ,const  ErrCode &errCode)
{
    if(errCode != 0) return;

    m_zhubi.totalCount = zhubi.totalCount;
    m_zhubi.ticks = zhubi.ticks+m_zhubi.ticks;

    int dataCount = m_zhubi.ticks.size();
    if(dataCount>0){
        int rowCount = dataCount<currentMaxRowCount()?dataCount:currentMaxRowCount();
        int index       = dataCount - rowCount;
        this->setData(m_zhubi.ticks.mid(index));
    }

    //开始订阅
    if(!m_isInitialed){
        startSub();
        m_isInitialed = true;
    }

    m_reqRecrd.reqId = INVALID_REQID;
    if(dataCount>0)
        reqTickData();
}

void SLTimeDealWidget::startSub()
{
    //开始订阅需要把上次订阅取消
    cancelSub();

    //从最新往前拉
    OnTickArrived func = std::bind(&SLTimeDealWidget::onSubTickArrived,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);

    HZData::ReqTickInfo reqTickInfo;
    reqTickInfo.stock_code = m_stockCode;
    //如果当前没有数据，从最新开始倒拉一个
    reqTickInfo.update_time=  -1;
    reqTickInfo.count    =  Sub_Count;
    reqTickInfo.is_desc=   true;

    m_subReqId = DataController::instance()->subTick(reqTickInfo,func);
}

void SLTimeDealWidget::onSubTickArrived(const  ReqId  & ,const  HZData::Zhubi &zhubi,const  ErrCode &errCode)
{
    int totalCount = zhubi.totalCount;

    //有最新一笔数据
    if(totalCount > m_zhubi.totalCount )
    {
        //订阅前没数据
        int newCount = totalCount - m_zhubi.totalCount;
        int idx             = zhubi.ticks.size() - newCount;
        //保存数据
        m_zhubi.ticks.append(zhubi.ticks.mid(idx));
        m_zhubi.totalCount     = totalCount;
        int dataSize = m_zhubi.ticks.size();

        //更新显示
        int maxRowCount     = currentMaxRowCount();
        int currentRowCount = dataSize<maxRowCount?dataSize:maxRowCount;
        int index = dataSize - currentRowCount;

        this->setData(m_zhubi.ticks.mid(index));
    }
    if(totalCount < m_zhubi.totalCount )
    {
        //如果数据量变少，在交易日切换时候清除逐笔数据了
        cancelSub();
        m_isInitialed = false;
        init();
    }
}



