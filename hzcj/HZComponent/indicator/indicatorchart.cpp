#include "indicatorchart.h"
#include <qdebug.h>

#include "indicatorprovider.h"
#include "imagelayer.h"
#include "xaxis.h"
#include "yaxis.h"
#include "hzobject.h"
#include "render.h"
#include "chartmodle.h"
#include "basecanvas.h"
#include "quoteformat.h"
#include "parasettingdialog.h"
#include "charttitle.h"
#include "hzcfg.h"
#include "ChartFactory.h"

IndicatorChart::IndicatorChart(ImageLayer *layer,bool bDrawBk,bool bDrawMainArea)
    :BaseChart(layer),
      m_bDrawBk(bDrawBk),
      m_bDrawMainArea(bDrawMainArea)
{
    m_bSwitchIndi = false;
    m_indiId = -1;
    m_reqId = -1;
    connect(this,SIGNAL(signalDataResponse(int,int)),this,SLOT(soltDataResponse(int,int)),Qt::QueuedConnection);
}

IndicatorChart::~IndicatorChart()
{
    releaseIndiData(m_result);
}

void IndicatorChart::createProvider(const QString& provider)
{
    BaseChart* pThis =this;
    if(provider.isEmpty())
    {
        m_provider = ChartFactory::getIoc()->ResolveShared<IndicatorProvider>("IndiLocalProvider",pThis);
    }else
    {
        std::string strClass = provider.toStdString();
        m_provider = ChartFactory::getIoc()->ResolveShared<IndicatorProvider>(strClass,pThis);
    }
}

void IndicatorChart::switchParam(const QString &msg)
{
    if(isShowEx())
    {
        if(paraseCmd(msg))
        {
            resetData();
            clearDrawData();
            cacIndicator();
        }
    }
}

void IndicatorChart::setParam(const QString &msg)
{
    if(isShowEx())
    {
        paraseCmd(msg);
    }
}

void IndicatorChart::redraw()
{
    BaseCanvas* pBaseCanvas = (BaseCanvas*)m_imageLayer->getWnd();
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawText(painter);
    drawLine(painter);
    painter.drawRect(m_rc);
    UpDownEqualClr clr;
    clr.downClr = pBaseCanvas->getDownClr();
    clr.upClr = pBaseCanvas->getUpClr();
    clr.equalClr = pBaseCanvas->getEqualClr();
    Render::drawIndicator(painter,m_result,m_xAxis,m_yAxis,clr);
}

void IndicatorChart::mainChartXaixsChnage(const QString& msg)
{
    if(isShowEx())
    {
        QString key;
        MessageParams params;
        if(HZObject::parseMsg(msg,key,params))
        {
            if(SwitchStockInfo == key)
            {
                QString obj = "";
                int period = -1;
                for(auto it = params.begin();it != params.end();it++)
                {
                    if(it->first == "obj")
                    {
                        obj = it->second;
                    }else if(it->first == "period")
                    {
                        period = it->second.toInt();
                    }
                }
                if(period == -1 || obj == "")
                {
                    return ;
                }
                m_obj = obj;
                m_period = period;
                cacIndicator();
            }else if(ChartUiClear == key)
            {
                resetData();
                clearDrawData();
            }
        }
    }
}

void IndicatorChart::dataResponse(int id,int type)
{
    //qDebug() << "IndicatorChart::dataResponse req id: " << id << "indi id: " << m_indiId;
    emit signalDataResponse(id,type);
}

void IndicatorChart::errorResponse(int id, int ncode, const QString &errReuslt)
{
    if(m_bDrawMainArea)
    {
        BaseChart* pChart = this;
        m_observerMgr.Notify(false,m_indiId,pChart); //通知主图重叠图像，并刷新
    }
}

void IndicatorChart::setIndi(int id)
{
    m_indiId = id;
}

void IndicatorChart::setIndiInfo(const IndiChartInfo &info)
{
    m_indiId = info.indiId;
    m_obj = info.obj;
    m_period = info.period;
}

void IndicatorChart::switchIndi(const IndiChartInfo &info)
{
    if(isShowEx())
    {
        m_bSwitchIndi =true;
        setIndiInfo(info);
        resetData();
        clearDrawData();
        cacIndicator();
    }
}

int IndicatorChart::getIndiId()
{
    return m_indiId;
}

Events<IndiAxisRefreshCallBack> &IndicatorChart::getObserverMgr()
{
    return m_observerMgr;
}


void IndicatorChart::createTitleTextInfo(std::vector<TitleTextItem>& itemList,int index)
{
    BaseCanvas* pCanvas = (BaseCanvas*)m_imageLayer->getWnd();
    QColor titleClr = QColor(pCanvas->getContainerTitleTextClr());
    double value =  -1;

    //指标名称 例如 趋向指数DMI
    TitleTextItem indiScreen;
    indiScreen.str = m_result.Calcinfo.base_info.screenname.c_str();
    indiScreen.clr = titleClr;
    indiScreen.nInterval = TitleTextInternal;
    itemList.push_back(indiScreen);


    int outSize =  m_result.Calcinfo.base_info.results.size();
    //指标名称和参数 例如 MACD(26.9.12)
    for(unsigned int i=0;i < outSize;i++)
    {
        value =  -1;
        const IndiResult& resultStyle = m_result.Calcinfo.base_info.results[i];
        TitleTextItem item;
        item.fixedWidth = 70;
        item.clr = resultStyle.clrMap[0];
        item.str = resultStyle.screenname.c_str();
        item.str += ":";
        MemBlock* pBlock = nullptr;
        if(i < m_result.vecOutput.size())
            pBlock = m_result.vecOutput[i].pBlock;
        if(pBlock)
        {
            int num = pBlock->GetCount();
            int offset = m_xAxis->getCount() - num;
            if(resultStyle.bShowName)
            {
                if(index < 0)
                {
                    index =0;
                }else if(index >= num)
                {
                    index = num-1;
                }
                int pos = index - offset;
                if(pos < num && pos >= 0)
                {
                     value = pBlock->GetNumberAt(pos);
                }
                if (value != INVALID_MEMBLOCKDATA_FLOAT&& value != -1)
                {
                    item.str += leftTipInfoFormat(value);//QString::number(value,'f',2);
                    item.nInterval = TitleTextInternal;
                }
                else
                {
                    item.str += Default_Item_String;
                    item.nInterval = TitleTextInternal;
                }
                itemList.push_back(item);
            }
        }else if(resultStyle.bShowName)
        {
            item.str += Default_Item_String;
            item.nInterval = TitleTextInternal;
            itemList.push_back(item);
        }
    }
}

void IndicatorChart::soltDataResponse(int id,int type)
{
    IndiRspResult data;
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    double value = 0;
    m_provider->copyData(data,m_xAxis->getoffset(),m_xAxis->getChart()->getScreenData().m_updateTime.size());
    releaseIndiData(m_result);
    m_result = data;
    if(!m_result.vecOutput.empty())
    {
        for(unsigned int i=0;i < m_result.vecOutput.size();i++)
        {
            MemBlock* pBlock = m_result.vecOutput[i].pBlock;
            if(pBlock)
            {
                int num = pBlock->GetCount();
                for(int j =0;j < num;j++)
                {
                    value = pBlock->GetNumberAt(j);
                    if (value != INVALID_MEMBLOCKDATA_FLOAT&& value != -1)
                    {
                        if (value > m_chartMax)
                        {
                            m_chartMax = value;
                            m_maxIndex = j;
                        }
                        if (value < m_chartmin)
                        {
                            m_chartmin = value;
                            m_minIndex = j;
                        }
                    }
                }
            }else if(m_result.vecOutput[i].blockList.isEmpty() == false)
            {
                for(int k =0;k < m_result.vecOutput[i].blockList.size();k++)
                {
                    pBlock =  m_result.vecOutput[i].blockList[k];
                    int num = pBlock->GetCount();
                    for(int j =0;j < num;j++)
                    {
                        value = pBlock->GetNumberAt(j);
                        if (value != INVALID_MEMBLOCKDATA_FLOAT&& value != -1)
                        {
                            if (value > m_chartMax)
                            {
                                m_chartMax = value;
                                m_maxIndex = j;
                            }
                            if (value < m_chartmin)
                            {
                                m_chartmin = value;
                                m_minIndex = j;
                            }
                        }
                    }
                }
            }
        }
    }
    if(!m_bDrawMainArea)
    {
        qDebug() <<  "m_indiId: " << m_indiId;
        qDebug() << "max: " << m_chartMax;
        qDebug() << "min: " << m_chartmin;
        m_yAxis->recalc();
        redraw();
    }else
    {
        qDebug() <<  "m_indiId: " << m_indiId;
        qDebug() << "max: " << m_chartMax;
        qDebug() << "min: " << m_chartmin;
        BaseChart* pChart = this;
        m_observerMgr.Notify(m_bSwitchIndi,m_indiId,pChart); //通知主图重叠图像，并刷新
    }
    if(!isShowCross())
    {
        m_titleItemList.clear();
        createTitleTextInfo(m_titleItemList,INT_MAX);
        if(m_title)
            m_title->updateTextItem(m_titleItemList);
    }else if(m_result.vecOutput.empty())
    {
        m_titleItemList.clear();
        if(m_title)
            m_title->updateTextItem(m_titleItemList);
    }
    if(m_bSwitchIndi)
    {
        m_bSwitchIndi = false;
    }
    paintImmet();
}

void IndicatorChart::soltButtonParamSet()
{
    const IndiIndexMap &indiMap = Hzcfg::getInstance()->getIndiIndexMap();
    auto it = indiMap.find(m_indiId);
    if(it != indiMap.end())
    {
        ParaSettingDialog dlg;
        connect(&dlg, &ParaSettingDialog::valueChanged, this,&IndicatorChart::soltValueChanged);
        dlg.setIndi(it->second.name.c_str());
        dlg.exec();
    }
}

void IndicatorChart::soltValueChanged(bool isChange)
{
    m_bSwitchIndi  = true;
    cacIndicator();
}

void IndicatorChart::resetData()
{
    //清理缓存数据
    if(m_reqId != -1)
    {
        m_provider->cancle(m_reqId);
        m_reqId=-1;
    }
    releaseIndiData(m_result);
    m_yAxis->resetDefaultValue();
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
}

void IndicatorChart::clearDrawData()
{
    m_imageLayer->clear(m_type);
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
    QPainter painter(drawLayer.pix);
    drawText(painter);
    drawLine(painter);
}

bool IndicatorChart::paraseCmd(const QString &msg)
{
    bool bRet = false;
    QString key;
    MessageParams params;
    if(HZObject::parseMsg(msg,key,params))
    {
        if(SwitchIndicatorInfo == key)
        {
            int id = -1;
            QString market = "";
            QString obj = "";
            int period = -1;
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "id")
                {
                    id = it->second.toInt();
                }
                if(it->first == "market")
                {
                    market = it->second;
                }else if(it->first == "obj")
                {
                    obj = it->second;
                }else if(it->first == "period")
                {
                    period = it->second.toInt();
                }
            }
            //qDebug() << "query indi: " << "m_indiId: " << id << "m_market: " << market.data() << "m_obj: " << obj.data() <<  "m_period: " << period;
            if(id == -1 || period == -1 || obj == "")
            {
                return bRet;
            }
            if(m_indiId != -1 && m_indiId != id)
                m_bSwitchIndi  = true;
            m_indiId = id;
            m_obj = obj;
            m_period = period;
            bRet = true;
        }
    }
    return bRet;
}

bool IndicatorChart::mouseMoveEx(QMouseEvent *event)
{
    if(m_xAxis && event && m_title)
    {
        QPoint pos = event->pos();
        int index = m_xAxis->getCurrCrossXIndex();
        if(index != -1)
        {
            QRect rc = getChartMainArea();
            if(pos.x() >= rc.left() && pos.x() < rc.right() ||
               pos.x() > rc.left() && pos.x() <= rc.right())
            {
                index = m_xAxis->xCanvasPTxToIndex(pos.x());
                if(index != -1)
                {
                    m_titleItemList.clear();
                    createTitleTextInfo(m_titleItemList,index);
                    m_title->updateTextItem(m_titleItemList);
                }
            }
        }
    }
    return true;
}

bool IndicatorChart::keyPressEx(QKeyEvent *event)
{
    if(m_xAxis && m_title && m_title->isShowEx())
    {
        int index  = m_xAxis->getCurrCrossXIndex();
        if(Qt::Key_Left == event->key() && index-1 >= 0)
        {
            m_titleItemList.clear();
            createTitleTextInfo(m_titleItemList,index-1);
            m_title->updateTextItem(m_titleItemList);
        }
        if(Qt::Key_Right == event->key())
        {
            m_titleItemList.clear();
            createTitleTextInfo(m_titleItemList,index+1);
            m_title->updateTextItem(m_titleItemList);
        }
    }
    return true;
}

void IndicatorChart::onResize()
{
    BaseChart::onResize();
    if(!isShowCross())
    {
        m_titleItemList.clear();
        createTitleTextInfo(m_titleItemList,INT_MAX);
        if(m_title)
            m_title->updateTextItem(m_titleItemList);
    }
    if(!m_bDrawMainArea)
    {
        m_yAxis->recalc();
        redraw();
        BaseChart* pChart = this;
        m_observerMgr.Notify(true,m_indiId,pChart); //通知主图重叠图像，并刷新
    }
}

void IndicatorChart::drawText(QPainter &painter)
{
    if(m_bDrawBk)
    {
        BaseCanvas* pCanvas = (BaseCanvas*)m_imageLayer->getWnd();
        QRect chartRc = getChartMainArea();
        double row = 2;
        double gridHeight = chartRc.height()/2;
        QPoint pt1(chartRc.topLeft()),pt2(chartRc.topRight());
        QRect leftRc = getLeftAxisArea(),
              rightRc = getRightAxisArea();

        QPen pen;
        QFont ft;
        ft.setFamily(pCanvas->getYaxisFontFamily());
        ft.setPixelSize(pCanvas->getYaxisFontSize());
        ft.setStyleStrategy(QFont::NoAntialias);
        painter.setFont(ft);
        pen.setWidth(1);
        double tempVal = DBL_MAX;
        double topOffset = gridHeight/2.0;
        double bottomOffset = gridHeight/2.0;
        for(int i = 0;i <= row;i++)
        {
            if(abs(chartRc.bottom() - pt1.y()) < gridHeight/2)
            {
                pt1.setY(chartRc.bottom());
                pt2.setY(chartRc.bottom());
            }
            tempVal = m_yAxis->yCanvasPTyToValue(pt1.y());
            if(i == 0)
            {
                topOffset = 3;
                bottomOffset = 10;
            }else if(i == row)
            {
                topOffset = 10;
                bottomOffset = 3;
            }else
            {
                topOffset = 10;
                bottomOffset = 10;
            }
            //绘制左边坐标轴
            QRect tempRc = leftRc;
            tempRc.setTop(pt1.y()-topOffset);
            tempRc.setBottom(pt1.y() + bottomOffset);
            pen.setColor(pCanvas->getEqualClr());
            painter.setPen(pen);
            painter.drawText(tempRc,Qt::AlignCenter,leftTipInfoFormat(tempVal));

            //绘制右边坐标轴
            tempRc = rightRc;
            tempRc.setTop(pt2.y()-topOffset);
            tempRc.setBottom(pt2.y() + bottomOffset);
            painter.drawText(tempRc,Qt::AlignCenter,rightTipInfoFormat(tempVal));

            pt1.setY(pt1.y() + gridHeight);
            pt2.setY(pt2.y() + gridHeight);
        }
    }
}

void IndicatorChart::drawLine(QPainter &painter)
{
    if(m_bDrawBk)
    {
//        QPen pen(ZB_BK_CLR, 1, Qt::DotLine);
//        painter.setPen(pen);
//        QRect rc = getChartMainArea();
//        int nWidth = rc.height() / INDCHART_SEPARATE;

//        painter.drawLine(rc.left(), rc.top()+1, rc.right(), rc.top()+1);
//        for(int i = 1; i < INDCHART_SEPARATE; i++)
//        {
//            painter.drawLine(rc.left(), rc.top() + nWidth*i, rc.right(), rc.top() + nWidth*i);
//        }
//        painter.drawLine(rc.left(), rc.bottom()-1, rc.right(), rc.bottom()-1);

        QRect rc = getChartMainArea();
        BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
        QPen pen(pWnd->getGridClr(),1);
        painter.setPen(pen);
        painter.drawRect(rc);
        painter.drawLine(rc.left(),rc.top()+rc.height()/2,rc.right(),rc.top()+rc.height()/2);
    }
}

void IndicatorChart::afterDeactive()
{
    if(m_reqId != -1)
    {
        m_provider->cancle(m_reqId);
        m_reqId = -1;
    }
}

QString IndicatorChart::leftTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX)
    {
        if(value > 1000)
        {
            retString = HzQuoteFormat::StringUnitFormat::toFormatString(value);

        }else
        {
           retString = QString::number(value, 'f', 2);
        }
    }
    return retString;
}

QString IndicatorChart::rightTipInfoFormat(double value)
{
    QString retString = Default_Item_String;
    if(value != DBL_MAX)
    {
        if(value > 10000)
        {
            retString = HzQuoteFormat::StringUnitFormat::toFormatString(value);

        }else
        {
           retString = QString::number(value, 'f', 2);
        }
    }
    return retString;
}

void IndicatorChart::getMaxMinRangeIndex(int &maxPos, int &minPos)
{
    maxPos = m_maxIndex;
    minPos = m_minIndex;
}

void IndicatorChart::getMaxMinRange(double &max, double &min)
{
    if(m_bDrawMainArea)
    {
        getXAxis()->getChart()->getMaxMinRange(max,min);
    }else
    {
        max = m_chartMax;
        min = m_chartmin;
    }
}

void IndicatorChart::getMaxMinRangeByMainArea(double &max, double &min)
{
    max = m_chartMax;
    min = m_chartmin;
}

bool IndicatorChart::isShowCross()
{
    bool bRet = false;
    if(m_xAxis)
    {
        if(m_xAxis->getCurrCrossXIndex() != -1)
        {
            bRet  = true;
        }
    }
    return bRet;
}

void IndicatorChart::cacIndicator()
{
    m_chartMax = DBL_MIN;
    m_chartmin = DBL_MAX;
    //计算前初始化样式
    m_provider->getIndiBaseInfo(m_result.Calcinfo.base_info,m_indiId);
    if(m_xAxis && m_xAxis->getChart()->getScreenData().m_updateTime.isEmpty() == false)
    {
        if(m_reqId != -1)
            m_provider->cancle(m_reqId);

        if(m_indiId != -1)
        {
            MessageParams params;
            params["id"] = QString::number(m_indiId);
            params["offset"] = QString::number(m_xAxis->getoffset());
            params["count"] = QString::number( m_xAxis->getChart()->getScreenData().m_updateTime.size()+IndiDependNum);
            params["period"] = QString::number(m_period);
            params["obj"] = m_obj;
            QString msg = HZObject::createMsg(SwitchIndicatorInfo,params);
            //qDebug() << "req indi calc: "  << "id: " <<   m_indiId << "offset: " << m_xAxis->getoffset() << "count: " << m_xAxis->getCount()+IndiDependNum;
            m_reqId = m_provider->query(msg.toStdString());
            if(m_reqId ==-1)
            {
                m_titleItemList.clear();
                if(m_title)
                {
                    createTitleTextInfo(m_titleItemList,INT_MAX);
                    m_title->updateTextItem(m_titleItemList);
                }
                paintImmet();
            }
        }
    }else
    {
        m_titleItemList.clear();
        if(m_title)
        {
            createTitleTextInfo(m_titleItemList,INT_MAX);
            m_title->updateTextItem(m_titleItemList);
        }
        paintImmet();
    }
}

void IndicatorChart::releaseIndiData(IndiRspResult& data)
{
    for(unsigned i = 0;i < data.vecOutput.size();i++)
    {
        data.vecOutput[i].ReleaseBlcok();
    }
    data.vecOutput.clear();
}

void IndicatorChart::onCreate()
{
    m_yAxis = std::shared_ptr<YAxis>(new YAxis(this));
}
