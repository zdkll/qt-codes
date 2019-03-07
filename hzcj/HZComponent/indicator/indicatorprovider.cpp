#include "indicatorprovider.h"
#include "CalcCenter.h"
#include "hzcfg.h"
#include "chartmodle.h"
#include "hzobject.h"
#include "xaxis.h"
#include "yaxis.h"
#include "basechart.h"

IndiLocalProvider::IndiLocalProvider(BaseChart* pchart):IndicatorProvider(pchart)
{
   m_indiId = -1;
   m_id= -1;
}

IndiLocalProvider::~IndiLocalProvider()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for(unsigned int i=0;i < m_result.vecOutput.size();i++)
    {
        m_result.vecOutput[i].ReleaseBlcok();
    }
}

int IndiLocalProvider::query(const std::string &req)
{
    QString key;
    MessageParams params;
    if(HZObject::parseMsg(req.c_str(),key,params))
    {
        if(SwitchIndicatorInfo == key)
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                for(unsigned int i=0;i < m_result.vecOutput.size();i++)
                {
                    m_result.vecOutput[i].ReleaseBlcok();
                }
                m_result.vecOutput.clear();
            }

            int indiId = -1;
            int offset = -1;
            int count = -1;
            QString obj = "";
            int period = -1;
            for(auto it = params.begin();it != params.end();it++)
            {
                if(it->first == "id")
                {
                    indiId = it->second.toInt();
                }else if(it->first == "offset")
                {
                    offset = it->second.toInt();
                }else if(it->first == "count")
                {
                    count = it->second.toInt();
                }
                else if(it->first == "obj")
                {
                    obj = it->second;
                }else if(it->first == "period")
                {
                    period = it->second.toInt();
                }
            }
            if(indiId == -1 || offset == -1 || count == -1 || period == -1 || obj == "")
            {
                return -1;
            }
            return calc(indiId,offset,count,obj,period);
        }
    }
    return -1;
}

void IndiLocalProvider::cancle(int id)
{
    if(id != -1)
    {
        CIndiCalcCenter::getInstance()->Cancle(id);
        m_id = -1;
    }
}

void IndiLocalProvider::copyData(IndiRspResult &data, int offset, int count)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    data = m_result;
    data.vecOutput.clear();
    for(unsigned int i=0;i < m_result.vecOutput.size();i++)
    {
        if(m_result.vecOutput[i].pBlock)
        {
            //qDebug() << "IndiLocalProvider::copyData reuslt_ name" << m_result.vecOutput[i].name.c_str();
            //qDebug() << "IndiLocalProvider::copyData reuslt_ size" << m_result.vecOutput[i].pBlock->GetCount();
            int end = m_result.vecOutput[i].pBlock->GetCount()/* + offset*/;
            if(end >= 0)
            {
                int begin = end - count;
                if(begin < 0)
                {
                    begin = 0;
                    //count = count < totalSize?count:totalSize;
                    count = end - begin;
                }
                if(count > 0)
                {
                    IndiData item;
                    item.name = m_result.vecOutput[i].name;
                    item.pBlock = MemBlock::SubBlock(m_result.vecOutput[i].pBlock,begin,count);
                    //qDebug() << "IndiLocalProvider::copyData ret name" << item.name.c_str();
                    //qDebug() << "IndiLocalProvider::copyData ret size" << item.pBlock->GetCount();

                    data.vecOutput.push_back(item);
                }
            }
        }
    }
}

void IndiLocalProvider::getIndiBaseInfo(IndiItemInfo &base_info,int indiId)
{
    auto indiInfo = Hzcfg::getInstance()->getIndiIndexMap();
    auto iter = indiInfo.find(indiId);
    if(iter != indiInfo.end())
    {
        m_result.Calcinfo.base_info = iter->second;
    }
}

void IndiLocalProvider::OnIndiCalc(IndiRspResult &result)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for(unsigned int i=0;i < m_result.vecOutput.size();i++)
        {
            m_result.vecOutput[i].ReleaseBlcok();
        }
        m_result.vecOutput.clear();
        m_result =  result;
        sendDataResponse(result.taskId,-1);
    }
}

int IndiLocalProvider::calc(int indiId, int offset, int count,const QString& obj,int period)
{
    if(m_id != -1)
         cancle(m_id);
    int id = -1;
    DataProvider* pMainProvider = m_chart->getXAxis()->getChart()->getProvider();
    RestorationType drType =  m_chart->getXAxis()->getChart()->getDrType();
    IndiCalcData* pKlineData = pMainProvider->getDataCache(drType)->pInidCalcData;
    if(pKlineData)
    {
        const IndiIndexMap &indiMap = Hzcfg::getInstance()->getIndiIndexMap();
        auto it = indiMap.find(indiId);
        if(it != indiMap.end())
        {
            IndiReqParam param;
            IndiCalcBaseInfo info;
            info.base_info = it->second;
            info.objName = obj.toStdString();
            info.period = period;
            param.Calcinfo = info;
            param.pfun = std::bind(&IndiLocalProvider::OnIndiCalc,this,std::placeholders::_1);
            param.ptrData = getData(pKlineData,count,offset);
            if(param.ptrData)
            {
                CIndiCalcCenter::getInstance()->AsynCalc(param,m_id);
                id =m_id;
            }
            m_obj = obj;
            m_period = period;
            //qDebug() << "calc indi: " << indiId;
        }
    }
    return id;
}

std::shared_ptr<IndiCalcData> IndiLocalProvider::getData(IndiCalcData* pKlineData,int count,int offset)
{
    std::shared_ptr<IndiCalcData> retPtrData = nullptr;
    if(pKlineData)
    {
        int end = pKlineData->pTime->GetCount() + offset;
        if(end >= 0)
        {
            int begin = end - count;
            if(begin < 0)
            {
                begin = 0;
                count = end - begin;
                //count = count < totalSize?count:totalSize;
            }
            if(count > 0)
            {
                retPtrData = std::shared_ptr<IndiCalcData>(new IndiCalcData);
                retPtrData->pTime = MemBlock::SubBlock(pKlineData->pTime,begin,count);
                retPtrData->pHigh = MemBlock::SubBlock(pKlineData->pHigh,begin,count);
                retPtrData->pLow = MemBlock::SubBlock(pKlineData->pLow,begin,count);
                retPtrData->pClose = MemBlock::SubBlock(pKlineData->pClose,begin,count);
                retPtrData->pOpen = MemBlock::SubBlock(pKlineData->pOpen,begin,count);
                retPtrData->pVolume = MemBlock::SubBlock(pKlineData->pVolume,begin,count);
                retPtrData->pAmount = MemBlock::SubBlock(pKlineData->pAmount,begin,count);
                retPtrData->pTickCount = MemBlock::SubBlock(pKlineData->pTickCount,begin,count);
            }
        }


//            int count = pKlineData->pTime->GetCount();
//            int begin =0;
//            retPtrData = std::shared_ptr<IndiCalcData>(new IndiCalcData);
//            retPtrData->pTime = MemBlock::SubBlock(pKlineData->pTime,begin,count);
//            retPtrData->pHigh = MemBlock::SubBlock(pKlineData->pHigh,begin,count);
//            retPtrData->pLow = MemBlock::SubBlock(pKlineData->pLow,begin,count);
//            retPtrData->pClose = MemBlock::SubBlock(pKlineData->pClose,begin,count);
//            retPtrData->pOpen = MemBlock::SubBlock(pKlineData->pOpen,begin,count);
//            retPtrData->pVolume = MemBlock::SubBlock(pKlineData->pVolume,begin,count);
//            retPtrData->pAmount = MemBlock::SubBlock(pKlineData->pAmount,begin,count);
//            retPtrData->pTickCount = MemBlock::SubBlock(pKlineData->pTickCount,begin,count);
    }
    return retPtrData;
}

IndicatorSrvProvider::IndicatorSrvProvider(BaseChart *pchart):IndicatorProvider(pchart)
{
}

IndicatorSrvProvider::~IndicatorSrvProvider()
{
    if(m_subId != -1)
        DataController::instance()->cancelSub(m_subId);
    for(auto it = m_reqIdMap.begin();it != m_reqIdMap.end();it++)
    {
        DataController::instance()->cancelSub(it.key());
    }
}

int IndicatorSrvProvider::query(const string &req)
{
    QString key;
    MessageParams params;
    const QVector<double>& screenTimeList = m_chart->getXAxis()->getChart()->getScreenData().m_upsertTime;
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        if(HZObject::parseMsg(req.c_str(),key,params) && timeList.isEmpty() == false)
        {
            if(SwitchIndicatorInfo == key)
            {
                HZData::ReqZhibiaoInfo req;
                for(auto it = params.begin();it != params.end();it++)
                {
                    if(it->first == "id")
                    {
                        req.ztype = (HZData::ZhibiaoType)it->second.toInt();
                    }
                    else if(it->first == "obj")
                    {
                        req.stock_code = it->second.toStdString();
                    }
                }
                RestorationType drType =  m_chart->getXAxis()->getChart()->getDrType();
                qDebug() << "m_reqInfo.ztype" << m_reqInfo.ztype;
                qDebug() << "req.ztype" << req.ztype;
                qDebug() <<  "m_reqInfo.stock_code" <<  m_reqInfo.stock_code.c_str();
                qDebug() << "req.stock_code" << req.stock_code.c_str();
                qDebug() << "m_reqInfo.start_time" << m_reqInfo.start_time;
                qDebug() << "m_reqInfo.end_time" << m_reqInfo.end_time;
                qDebug() << "timeList[0]" << timeList[0];
                qDebug() << "timeList[timeList.size()-1]" << timeList[timeList.size()-1];

                auto fun = std::bind(&IndicatorSrvProvider::onIndiCallBack,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
                if(m_reqInfo.ztype == req.ztype && m_reqInfo.stock_code == req.stock_code
                   && m_reqInfo.start_time <= screenTimeList[0] && m_reqInfo.end_time >= screenTimeList[screenTimeList.size()-1]
                   && drType == m_drType && m_result.vecOutput.size() > 0)
                {
//                    if(m_chart->getXAxis()->getoffset() == 0)
//                    {
//                        req.start_time = timeList[timeList.size()-1];
//                        req.end_time = timeList[timeList.size()-1];
//                        qDebug() << "sub req end data : begin: " <<  req.start_time << " end: " << req.end_time;
//                        m_subId =DataController::instance()->requestZhibiaoInfo(req,fun);
//                        return -1;
//                    }else
                    {
                        sendDataResponse(-1,-1);
                    }
                }else
                {
                    //请求当前屏幕
                    cancle(-1);
                    m_reqInfo = req;
                    int beginTime = 0;
                    int endTime = timeList.size()-1;
                    m_reqInfo.stock_code = req.stock_code;
                    m_reqInfo.start_time = timeList[beginTime];
                    m_reqInfo.end_time = timeList[endTime];
                    m_drType=drType;

                    qDebug() << "req curr : begin: " <<  m_reqInfo.start_time << " end: " << m_reqInfo.end_time;
                    //当数据量很大是分割成多批次请求数据 目前这个功能待优化
                    int interval = 2000;
                    int reqId = -1;
                    for(int i=beginTime;i <= endTime;)
                    {
                        if(i+interval < endTime)
                        {
                            req.start_time = timeList[i];
                            req.end_time = timeList[i+interval];
                            qDebug() << "req end data  A: begin: " <<  req.start_time << " end: " << req.end_time;
                            reqId =DataController::instance()->requestZhibiaoInfo(req,fun);
                             qDebug() << "req end data  A id: " << reqId << " i= " << i;
                            i+= (interval+1);
                            m_reqIdMap[reqId] = reqId;
                        }else
                        {
                            req.start_time = timeList[i];
                            req.end_time = timeList[endTime];
                            qDebug() << "req end data B: begin: " <<  req.start_time << " end: " << req.end_time;
                            reqId =DataController::instance()->requestZhibiaoInfo(req,fun);
                            qDebug() << "req end data  B id: " << reqId << " i= " << i;
                            m_reqIdMap[reqId] = reqId;
                            break;
                        }
                    }
//                    req.start_time = timeList[0];
//                    req.end_time = timeList[timeList.size()-1];
//                     qDebug() << "req end data : begin: " <<  req.start_time << " end: " << req.end_time;
//                    ReqId reqId =DataController::instance()->requestZhibiaoInfo(req,fun);
//                     m_reqIdMap[reqId] = reqId;
                }
            }
        }
    }
    return -1;
}

void IndicatorSrvProvider::cancle(int id)
{
    if(m_subId != -1)
        DataController::instance()->cancelSub(m_subId);
    for(auto it = m_reqIdMap.begin();it != m_reqIdMap.end();it++)
    {
        DataController::instance()->cancelSub(it.key());
    }
    m_subId = -1;
    m_reqIdMap.clear();
    m_data = HZData::ZhibiaoInfo();
    m_reqInfo =HZData::ReqZhibiaoInfo();
    for(unsigned int i=0;i < m_result.vecOutput.size();i++)
    {
        m_result.vecOutput[i].ReleaseBlcok();
    }
    m_result = IndiRspResult();
}

void IndicatorSrvProvider::copyData(IndiRspResult &data, int offset, int count)
{
    data = m_result;
    data.vecOutput.clear();
    for(unsigned int i=0;i < m_result.vecOutput.size();i++)
    {
        if(m_result.vecOutput[i].pBlock)
        {
            int end = m_result.vecOutput[i].pBlock->GetCount() + offset;
            if(end >= 0)
            {
                int begin = end - count;
                if(begin < 0)
                {
                    begin = 0;
                    count = end - begin;
                }
                if(count > 0)
                {
                    IndiData item;
                    item.name = m_result.vecOutput[i].name;
                    item.pBlock = MemBlock::SubBlock(m_result.vecOutput[i].pBlock,begin,count);
                    data.vecOutput.push_back(item);
                }
            }
        }else if(m_result.vecOutput[i].type == IndiDataSignal)
        {
            IndiData item;
            item.name = m_result.vecOutput[i].name;
            item.type = IndiDataSignal;
            for(int j =0;j < m_result.vecOutput[i].blockList.size();j++)
            {
                MemBlock* pBlock = m_result.vecOutput[i].blockList[j];
                int end = pBlock->GetCount() + offset;
                if(end >= 0)
                {
                    int begin = end - count;
                    if(begin < 0)
                    {
                        begin = 0;
                        count = end - begin;
                    }
                    if(count > 0)
                    {

                        pBlock = MemBlock::SubBlock(pBlock,begin,count);
                        item.blockList.push_back(pBlock);
                    }
                }
            }
            data.vecOutput.push_back(item);
        }
    }




//    QVector<double> timeList = m_chart->getXAxis()->getChart()->getScreenData().m_upsertTime;
//    switch(m_data.ztype)
//    {
//        case HZData::JDCJ: // 见底出击
//        {
//            data.Calcinfo.objName = m_reqInfo.stock_code;
//            data.Calcinfo.base_info = IndiItemInfo();
//            getIndiBaseInfo(data.Calcinfo.base_info,m_data.ztype);

//            if(m_data.dotInfo.isEmpty() == false)
//            {
//                //短期趋势
//                IndiData shotData;
//                shotData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
//                shotData.name = QString(QStringLiteral("短期趋势")).toStdString();

//                //长期趋势
//                IndiData longData;
//                longData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
//                longData.name = QString(QStringLiteral("长期趋势")).toStdString();

//                //中期趋势
//                IndiData midData;
//                midData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
//                midData.name  = QString(QStringLiteral("中期趋势")).toStdString();

//                int pos = -1;
//                int j1 =0,j2=0,j3=0;
//                for(int i =0;i < timeList.size();i++)
//                {
//                    //短期
//                    pos = indexOfZbDotVec(timeList[i],1,j1);
//                    if(pos != -1)
//                    {
//                       j1 = pos;
//                       shotData.pBlock->SetNumberAt(i,0.61f);
//                       if(pos == m_data.dotInfo.size()-1)
//                       {
//                           break;
//                       }
//                    }

//                     //长期
//                     pos = indexOfZbDotVec(timeList[i],3,j3);
//                     if(pos != -1)
//                     {
//                         j3 = pos;
//                          longData.pBlock->SetNumberAt(i,1.21f);
//                          if(pos == m_data.dotInfo.size()-1)
//                          {
//                              break;
//                          }
//                     }

//                     //中期
//                     pos = indexOfZbDotVec(timeList[i],2,j2);
//                     if(pos != -1)
//                     {
//                         j2=pos;
//                         midData.pBlock->SetNumberAt(i,1.21f);
//                         if(pos == m_data.dotInfo.size()-1)
//                         {
//                             break;
//                         }
//                     }
//               }
//               data.vecOutput.push_back(shotData);
//               data.vecOutput.push_back(longData);
//               data.vecOutput.push_back(midData);
//            }
//        }
//            break;
//        case HZData::CMFB: // 筹码分布
//        {
//           data.Calcinfo.objName = m_reqInfo.stock_code;
//           data.Calcinfo.base_info = IndiItemInfo();
//           getIndiBaseInfo(data.Calcinfo.base_info,m_data.ztype);

//           QVector<double> hight = m_chart->getXAxis()->getChart()->getScreenData().m_highPrice;
//           QVector<double> low = m_chart->getXAxis()->getChart()->getScreenData().m_lowPrice;

//           if(m_data.dotInfo.isEmpty() == false)
//           {
//               //集
//               IndiData g;
//               g.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT);
//               g.name = QString(QStringLiteral("集")).toStdString();

//               //分
//               IndiData d;
//               d.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT);
//               d.name = QString(QStringLiteral("分")).toStdString();

//               int pos = -1;
//               int j1 =0,j2=0;
//               double distance = 0.0f;
//               double value = 0.0f;
//               for(int i =0;i < timeList.size();i++)
//               {
//                   //集
//                   distance = (hight[i]-low[i])/3;
//                   pos = indexOfZbDotVec(timeList[i],0,j1);
//                   if(pos != -1)
//                   {
//                      j1 = pos;
//                      value = low[i]-distance;
//                      g.pBlock->SetNumberAt(i,value);
//                      if(pos == m_data.dotInfo.size()-1)
//                      {
//                          break;
//                      }
//                   }

//                    //分
//                    pos = indexOfZbDotVec(timeList[i],1,j2);
//                    if(pos != -1)
//                    {
//                        j2 = pos;
//                        value = hight[i]+distance;
//                         d.pBlock->SetNumberAt(i,value);
//                         if(pos == m_data.dotInfo.size()-1)
//                         {
//                             break;
//                         }
//                    }
//              }
//              data.vecOutput.push_back(g);
//              data.vecOutput.push_back(d);
//           }
//       }
//            break;
//        case HZData::QSGD: // 趋势拐点
//        {
//        }
//            break;
//        case HZData::HLQJ: // 黄蓝区间
//        {
//        }
//            break;
//        case HZData::ZJKP: // 庄家控盘
//        {
//        }
//            break;
//        case HZData::CPTX: // 操盘提醒
//        {
//        }
//            break;
//        case HZData::HPM:	// 海平面
//        {
//        }
//            break;
//        case HZData::XQQX:	// 心情曲线
//        {
//        }
//            break;
//        case HZData::DQKJ:	// 多空区间
//        {
//        }
//            break;
//        case HZData::DBGJ:	// 底部根基
//        {
//        }
//            break;
//        case HZData:: DBDJ:	// 顶部搭建
//        {
//        }
//            break;
//    };
}

void IndicatorSrvProvider::getIndiBaseInfo(IndiItemInfo &base_info, int indiId)
{
    IndiItemInfo retBaseinfo;
    switch(m_data.ztype)
    {
        case HZData::JDCJ: // 见底出击
        {
            getJdcjBaseInfo(base_info);
        }
            break;
        case HZData::CMFB: // 筹码分布
        {
            getCmfbBaseInfo(base_info);
        }
            break;
        case HZData::QSGD: // 趋势拐点
        {
        }
            break;
        case HZData::HLQJ: // 黄蓝区间
        {
            getHlqjBaseInfo(base_info);
        }
            break;
        case HZData::ZJKP: // 庄家控盘
        {
        }
            break;
        case HZData::CPTX: // 操盘提醒
        {
            getCptxBaseInfo(base_info);
        }
            break;
        case HZData::HPM:	// 海平面
        {
            getHpmBaseInfo(base_info);
        }
            break;
        case HZData::XQQX:	// 心情曲线
        {
            getXqqxBaseInfo(base_info);
        }
            break;
        case HZData::DQKJ:	// 多空区间
        {
        }
            break;
        case HZData::DBGJ:	// 底部根基
        {
        }
            break;
        case HZData:: DBDJ:	// 顶部搭建
        {
        }
            break;
    };
}

void IndicatorSrvProvider::onIndiCallBack(ReqId id, const HZData::ZhibiaoInfo & info, ErrCode error)
{
    if(error != 0)
        sendErrorResponse(id,error,DataController::instance()->errString().c_str());
    auto it  = m_reqIdMap.find(id);
    if(it != m_reqIdMap.end())
    {
        if(!info.colInfo.isEmpty())
            qDebug() << "info.colInfo ret: begin: " << info.colInfo[0].upserttime_time << " END   " <<info.colInfo[info.colInfo.size()-1].upserttime_time << " id " <<  id;
        if(!info.dotInfo.isEmpty())
            qDebug() << "info.dotInfo ret: begin: " <<info.dotInfo[0].upserttime_time << " END   " <<info.dotInfo[info.dotInfo.size()-1].upserttime_time << " id " <<  id;
        m_data.ztype = info.ztype;
        m_reqIdMap.erase(it);
        m_tempData[id] = info;
        if(m_reqIdMap.isEmpty())
        {
            for(auto it=m_tempData.begin();it != m_tempData.end();it++)
            {
                m_data.colInfo.append(it.value().colInfo);
                m_data.dotInfo.append(it.value().dotInfo);
            }
            m_tempData.clear();
            convertIndiData(false);
            sendDataResponse(-1,-1);
            return;
        }
    }else if(m_subId == id)
    {
        if(!info.colInfo.isEmpty())
        {
            if(m_data.colInfo.isEmpty())
                m_data.colInfo.append(info.colInfo);
            else if(m_data.colInfo[m_data.colInfo.size()-1].upserttime_time == info.colInfo[info.colInfo.size()-1].upserttime_time)
            {
                m_data.colInfo[m_data.colInfo.size()-1] = info.colInfo[info.colInfo.size()-1];
            }
        }
        if(!info.dotInfo.isEmpty())
        {
            if(m_data.dotInfo.isEmpty())
                m_data.dotInfo.append(info.dotInfo);
            else if(m_data.dotInfo[m_data.dotInfo.size()-1].upserttime_time == info.dotInfo[info.dotInfo.size()-1].upserttime_time)
            {
                m_data.dotInfo[m_data.dotInfo.size()-1] = info.dotInfo[info.dotInfo.size()-1];
            }
        }
        convertIndiData(true);
        sendDataResponse(-1,-1);
    }
}

int IndicatorSrvProvider::indexOfZbDotVec(double time,int type,int from)
{
    int pos  = -1;
    for(int i =from;i < m_data.dotInfo.size();i++)
    {
        if(time == m_data.dotInfo[i].upserttime_time)
        {
            if(type == -1 || type == m_data.dotInfo[i].stype)
            {
                pos = i;
                break;
            }
        }
    }
    return pos;
}

int IndicatorSrvProvider::indexOfZbColumnVec(double time,int from)
{
    int pos  = -1;
    for(int i =from;i < m_data.colInfo.size();i++)
    {
        if(time == m_data.colInfo[i].upserttime_time)
        {
            pos = i;
            break;
        }
    }
    return pos;
}

void IndicatorSrvProvider::proceJdcj(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData &&  m_data.dotInfo.isEmpty() == false)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        int pos = -1;
        int j1=0,j2=0,j3=0;
        int i =0;

        //短期趋势
        IndiData shotData;
        shotData.name = QString(QStringLiteral("短期趋势")).toStdString();

        //长期趋势
        IndiData longData;
        longData.name = QString(QStringLiteral("长期趋势")).toStdString();

        //中期趋势
        IndiData midData;
        midData.name  = QString(QStringLiteral("中期趋势")).toStdString();


        if(bSub)
        {
            i = timeList.size()-1;
            j1 = j2 = j3= m_data.dotInfo.size()-5>0?m_data.dotInfo.size()-5:0;
            shotData.pBlock =  m_result.vecOutput[0].pBlock;
            longData.pBlock = m_result.vecOutput[1].pBlock;
            midData.pBlock = m_result.vecOutput[2].pBlock;
        }else
        {
            shotData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
            longData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
            midData.pBlock = MemBlock::Alloc(timeList.size(), 0.0);
        }
        for(;i < timeList.size();i++)
        {
            //短期
            pos = indexOfZbDotVec(timeList[i],1,j1);
            if(pos != -1)
            {
               j1 = pos;
               shotData.pBlock->SetNumberAt(i,0.61f);
               if(pos == m_data.dotInfo.size()-1)
               {
                   break;
               }
            }

             //长期
             pos = indexOfZbDotVec(timeList[i],3,j2);
             if(pos != -1)
             {
                 j2 = pos;
                  longData.pBlock->SetNumberAt(i,1.21f);
                  if(pos == m_data.dotInfo.size()-1)
                  {
                      break;
                  }
             }

             //中期
             pos = indexOfZbDotVec(timeList[i],2,j3);
             if(pos != -1)
             {
                 j3=pos;
                 midData.pBlock->SetNumberAt(i,1.21f);
                 if(pos == m_data.dotInfo.size()-1)
                 {
                     break;
                 }
             }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(shotData);
           m_result.vecOutput.push_back(longData);
           m_result.vecOutput.push_back(midData);
       }
    }
}

void IndicatorSrvProvider::proceCmfb(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData && m_data.dotInfo.isEmpty() == false)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        const QVector<double>& hight = pKlineData->pData->m_highPrice;
        const QVector<double>& low = pKlineData->pData->m_lowPrice;
        //集
        IndiData g;
        g.name = QString(QStringLiteral("集")).toStdString();

        //分
        IndiData d;
        d.name = QString(QStringLiteral("分")).toStdString();

        int pos = -1;
        int j1 =0;
        int i =0;
        if(bSub)
        {
            i = timeList.size()-1;
            j1 = m_data.dotInfo.size()-1;
            g.pBlock = m_result.vecOutput[0].pBlock;
            d.pBlock = m_result.vecOutput[1].pBlock;
        }else
        {
            g.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT);
            d.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT);
        }
        QRect rc = m_chart->getXAxis()->getChart()->getChartMainArea();
        double distance = m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom()-20);
        distance -= m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom());
        double value = 0.0f;
        for(;i < timeList.size();i++)
        {
            //集
            pos = indexOfZbDotVec(timeList[i],0,j1);
            if(pos != -1)
            {
               j1 = pos;
               value = low[i]-distance;
               g.pBlock->SetNumberAt(i,value);
               if(pos == m_data.dotInfo.size()-1)
               {
                   break;
               }
            }

             //分
             pos = indexOfZbDotVec(timeList[i],1,j1);
             if(pos != -1)
             {
                  j1 = pos;
                  value = hight[i]+distance;
                  d.pBlock->SetNumberAt(i,value);
                  if(pos == m_data.dotInfo.size()-1)
                  {
                      break;
                  }
             }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(g);
           m_result.vecOutput.push_back(d);
       }
    }
}

void IndicatorSrvProvider::proceCptx(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData &&  m_data.dotInfo.isEmpty() == false)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        if(timeList.isEmpty())
            return;
        int j1=0,j2=0,j3=0,j4=0;
        int i =0;

        //波段进场
        IndiData bdjc;
        bdjc.name = QString(QStringLiteral("波段进场")).toStdString();

        //反弹进场
        IndiData ftjc;
        ftjc.name = QString(QStringLiteral("反弹进场")).toStdString();

        //超跌进场
        IndiData cdjc;
        cdjc.name  = QString(QStringLiteral("超跌进场")).toStdString();

        //红绿区间
        IndiData hlqj;
        hlqj.type = IndiDataList;
        hlqj.name  = QString(QStringLiteral("红绿区间")).toStdString();

        if(bSub)
        {
            i = timeList.size()-1;
            j1 = j2 = j3= m_data.dotInfo.size()-5>0?m_data.dotInfo.size()-5:0;
            j4 = m_data.colInfo.size()-5>0?m_data.colInfo.size()-5:0;
            bdjc.pBlock =  m_result.vecOutput[0].pBlock;
            ftjc.pBlock = m_result.vecOutput[1].pBlock;
            cdjc.pBlock = m_result.vecOutput[2].pBlock;
            hlqj.blockList = m_result.vecOutput[3].blockList;
        }else
        {
            bdjc.pBlock = MemBlock::Alloc(timeList.size(), 50.0f);
            ftjc.pBlock = MemBlock::Alloc(timeList.size(),50.0f);
            cdjc.pBlock = MemBlock::Alloc(timeList.size(), 50.0f);
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的上坐标
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的下坐标
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的下坐标
        }
        double v1=0.0,v2=0.0;
        int dotPos =-1;
        int colPos = -1;
        for(;i < timeList.size();i++)
        {
            if(dotPos != m_data.dotInfo.size()-1)
            {
                //波段进场
                dotPos = indexOfZbDotVec(timeList[i],0,j1);
                if(dotPos != -1)
                {
                   j1 = dotPos;
                   bdjc.pBlock->SetNumberAt(i,80.0f);
                }

                 //反弹进场
                 dotPos = indexOfZbDotVec(timeList[i],1,j2);
                 if(dotPos != -1)
                 {
                     j2 = dotPos;
                      ftjc.pBlock->SetNumberAt(i,80.0f);
                 }

                 //超跌进场
                 dotPos = indexOfZbDotVec(timeList[i],2,j3);
                 if(dotPos != -1)
                 {
                     j3=dotPos;
                     cdjc.pBlock->SetNumberAt(i,80.0f);
                 }
            }

            if(colPos != m_data.colInfo.size()-1)
            {
                //绿红区间
                colPos = indexOfZbColumnVec(timeList[i],j4);
                if(colPos != -1)
                {
                    j4=colPos;
                    v1 = m_data.colInfo[colPos].high/1000.0f;
                    v2 = m_data.colInfo[colPos].low/1000.0f;
                    hlqj.blockList[0]->SetNumberAt(i,v1);
                    hlqj.blockList[1]->SetNumberAt(i,v2);
                    if(m_data.colInfo[colPos].color_type == 0)
                    {
                        hlqj.blockList[2]->SetNumberAt(i,v1);
                    }else
                    {
                       hlqj.blockList[2]->SetNumberAt(i,v2);
                    }
                }
            }
            if(colPos  == m_data.colInfo.size()-1 && dotPos ==  m_data.dotInfo.size()-1)
            {
                break;
            }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(bdjc);
           m_result.vecOutput.push_back(ftjc);
           m_result.vecOutput.push_back(cdjc);
           m_result.vecOutput.push_back(hlqj);
       }
    }
}

void IndicatorSrvProvider::proceHlqj(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData &&  m_data.dotInfo.isEmpty() == false)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        const QVector<double>& hight = pKlineData->pData->m_highPrice;
        const QVector<double>& low = pKlineData->pData->m_lowPrice;

        if(timeList.isEmpty())
            return;
        int j1=0,j2=0;
        int i =0;

        //买卖点
        IndiData bsPoint;
        bsPoint.name = QString(QStringLiteral("买卖点")).toStdString();

        //黄蓝区间
        IndiData hlqj;
        hlqj.type = IndiDataList;
        hlqj.name  = QString(QStringLiteral("黄蓝区间")).toStdString();

        if(bSub)
        {
            i = timeList.size()-1;
            j1 = m_data.dotInfo.size()-5>0?m_data.dotInfo.size()-5:0;
            j2 = m_data.colInfo.size()-5>0?m_data.colInfo.size()-5:0;
            bsPoint.pBlock =  m_result.vecOutput[0].pBlock;
            hlqj.blockList = m_result.vecOutput[1].blockList;
        }else
        {
            bsPoint.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT);//买卖点
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的上坐标
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的下坐标
            hlqj.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//如果当前点是上坐标就填入上坐标数据，否则反之
        }
        double v1=0.0,v2=0.0;
        int dotPos =-1;
        int colPos = -1;

        QRect rc = m_chart->getXAxis()->getChart()->getChartMainArea();
        double distance = m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom()-20);
        distance -= m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom());

        for(;i < timeList.size();i++)
        {
            if(dotPos != m_data.dotInfo.size()-1)
            {
                //买卖点
                dotPos = indexOfZbDotVec(timeList[i],-1,j1);
                if(dotPos != -1)
                {
                   j1 = dotPos;
                   if(m_data.dotInfo[dotPos].stype == 0) //买
                   {
                       bsPoint.pBlock->SetNumberAt(i,low[i]-distance);
                   }else //卖
                   {
                       bsPoint.pBlock->SetNumberAt(i,hight[i]+distance);
                   }
                }
            }

            if(colPos != m_data.colInfo.size()-1)
            {
                //黄蓝区间
                colPos = indexOfZbColumnVec(timeList[i],j2);
                if(colPos != -1)
                {
                    j2=colPos;
                    v1 = m_data.colInfo[colPos].high/1000.0f;
                    v2 = m_data.colInfo[colPos].low/1000.0f;
                    hlqj.blockList[0]->SetNumberAt(i,v1);
                    hlqj.blockList[1]->SetNumberAt(i,v2);
                    if(m_data.colInfo[colPos].color_type == 0)
                    {
                        hlqj.blockList[2]->SetNumberAt(i,v1);
                    }else
                    {
                       hlqj.blockList[2]->SetNumberAt(i,v2);
                    }
                }
            }
            if(colPos  == m_data.colInfo.size()-1 && dotPos ==  m_data.dotInfo.size()-1)
            {
                break;
            }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(bsPoint);
           m_result.vecOutput.push_back(hlqj);
       }
    }
}

void IndicatorSrvProvider::proceHpm(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        if(timeList.isEmpty())
            return;
        int j1=0,j2=0;
        int i =0;

        //底部
        IndiData bottomData;
        bottomData.name = QString(QStringLiteral("底部")).toStdString();

        //顶部
        IndiData topData;
        topData.name = QString(QStringLiteral("顶部")).toStdString();

        //海平面
        IndiData hpmData;
        hpmData.name = QString(QStringLiteral("海平面")).toStdString();

        //紫绿柱状
        IndiData zhlzz;
        zhlzz.type = IndiDataList;
        zhlzz.name  = QString(QStringLiteral("紫绿柱状")).toStdString();

        if(bSub)
        {
            i = timeList.size()-1;
            j1= m_data.dotInfo.size()-5>0?m_data.dotInfo.size()-5:0;
            j2 = m_data.colInfo.size()-5>0?m_data.colInfo.size()-5:0;
            bottomData.pBlock =  m_result.vecOutput[0].pBlock;
            topData.pBlock =  m_result.vecOutput[1].pBlock;
            hpmData.pBlock =  m_result.vecOutput[2].pBlock;
            zhlzz.blockList = m_result.vecOutput[3].blockList;
        }else
        {
            bottomData.pBlock = MemBlock::Alloc(timeList.size(), 10.0f);//底部
            topData.pBlock = MemBlock::Alloc(timeList.size(), 90.0f);//顶部
            hpmData.pBlock = MemBlock::Alloc(timeList.size(), 45.0f);//海平面

            zhlzz.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的上坐标
            zhlzz.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//柱状点的下坐标
            zhlzz.blockList.push_back(MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT));//如果当前点是上坐标就填入上坐标数据，否则反之
        }
        double v1=0.0,v2=0.0;
        int dotPos = -1;
        int colPos = -1;

        QRect rc = m_chart->getXAxis()->getChart()->getChartMainArea();
        double distance = m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom()-20);
        distance -= m_chart->getXAxis()->getChart()->getYAxis()->yCanvasPTyToValue(rc.bottom());

        for(;i < timeList.size();i++)
        {
            if(dotPos != m_data.dotInfo.size()-1)
            {
                //底部
                dotPos = indexOfZbDotVec(timeList[i],-1,j1);
                if(dotPos != -1)
                {
                   j1 = dotPos;
                   if(m_data.dotInfo[dotPos].stype == 1) //多 底部
                   {
                       bottomData.pBlock->SetNumberAt(i,20.0f);
                   }else if(m_data.dotInfo[dotPos].stype == 0) //空 顶部
                   {
                       topData.pBlock->SetNumberAt(i,80.0f);
                   }
                }
            }

            if(colPos != m_data.colInfo.size()-1)
            {
                //柱形颜色
                colPos = indexOfZbColumnVec(timeList[i],j2);
                if(colPos != -1)
                {
                    j2=colPos;
                    v1 = m_data.colInfo[colPos].high/1000.0f;
                    v2 = m_data.colInfo[colPos].low/1000.0f;
                    zhlzz.blockList[0]->SetNumberAt(i,v1);
                    zhlzz.blockList[1]->SetNumberAt(i,v2);
                    if(m_data.colInfo[colPos].color_type == 0)
                    {
                       zhlzz.blockList[2]->SetNumberAt(i,v1);
                    }else
                    {
                       zhlzz.blockList[2]->SetNumberAt(i,v2);
                    }
                }
            }
            if(colPos  == m_data.colInfo.size()-1 && dotPos ==  m_data.dotInfo.size()-1)
            {
                break;
            }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(bottomData);
           m_result.vecOutput.push_back(topData);
           m_result.vecOutput.push_back(hpmData);
           m_result.vecOutput.push_back(zhlzz);
       }
    }
}

void IndicatorSrvProvider::proceXqqx(bool bSub)
{
    KlineData * pKlineData = m_chart->getXAxis()->getChart()->getProvider()->getDataCache();
    if(pKlineData->pData && m_data.colInfo.isEmpty() == false)
    {
        const QVector<double>& timeList =pKlineData->pData->m_upsertTime;
        if(timeList.isEmpty())
            return;
        int j1=0;
        int i =0;

        //心情曲线
        IndiData xqqxData;
        xqqxData.name = QString(QStringLiteral("心情曲线")).toStdString();

        //贪婪
        IndiData tlData;
        tlData.name = QString(QStringLiteral("贪婪")).toStdString();

        //恐惧
        IndiData kjData;
        kjData.name = QString(QStringLiteral("恐惧")).toStdString();

        //强弱线
        IndiData qrData;
        qrData.name = QString(QStringLiteral("恐惧")).toStdString();

        if(bSub)
        {
            i = timeList.size()-1;
            j1= m_data.dotInfo.size()-5>0?m_data.dotInfo.size()-5:0;
            xqqxData.pBlock =  m_result.vecOutput[0].pBlock;
        }else
        {
            xqqxData.pBlock = MemBlock::Alloc(timeList.size(), INVALID_MEMBLOCKDATA_FLOAT); //心情曲线
            tlData.pBlock = MemBlock::Alloc(timeList.size(), 80.0f);//贪婪
            kjData.pBlock = MemBlock::Alloc(timeList.size(), 20.0f);//恐惧
            qrData.pBlock = MemBlock::Alloc(timeList.size(), 50.0f);//强弱线
        }
        double v1=0.0,v2=0.0;
        int pos = -1;

        for(;i < timeList.size();i++)
        {
            //心情曲线
            pos = indexOfZbColumnVec(timeList[i],j1);
            if(pos != -1)
            {
               j1 = pos;
               xqqxData.pBlock->SetNumberAt(i,m_data.colInfo[pos].high/1000.0f);
               if(pos == m_data.colInfo.size()-1)
                   break;
            }
       }
       if(!bSub)
       {
           m_result.vecOutput.push_back(xqqxData);
           m_result.vecOutput.push_back(tlData);
           m_result.vecOutput.push_back(kjData);
           m_result.vecOutput.push_back(qrData);
       }
    }
}

void IndicatorSrvProvider::convertIndiData(bool bSub)
{
    m_result.Calcinfo.objName = m_reqInfo.stock_code;
    m_result.Calcinfo.base_info = IndiItemInfo();
    getIndiBaseInfo(m_result.Calcinfo.base_info,m_data.ztype);
    switch(m_data.ztype)
    {
        case HZData::JDCJ: // 见底出击
        {
            proceJdcj(bSub);
        }
            break;
        case HZData::CMFB: // 筹码分布
        {
            proceCmfb(bSub);
        }
            break;
        case HZData::QSGD: // 趋势拐点
        {
        }
            break;
        case HZData::HLQJ: // 黄蓝区间
        {
            proceHlqj(bSub);
        }
            break;
        case HZData::ZJKP: // 庄家控盘
        {
        }
            break;
        case HZData::CPTX: // 操盘提醒
        {
            proceCptx(bSub);
        }
            break;
        case HZData::HPM:	// 海平面
        {
            proceHpm(bSub);
        }
            break;
        case HZData::XQQX:	// 心情曲线
        {
            proceXqqx(bSub);
        }
            break;
        case HZData::DQKJ:	// 多空区间
        {
        }
            break;
        case HZData::DBGJ:	// 底部根基
        {
        }
            break;
        case HZData:: DBDJ:	// 顶部搭建
        {
        }
            break;
    };
}

void IndicatorSrvProvider::getJdcjBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 1;
    retBaseinfo.id = HZData::JDCJ;
    retBaseinfo.name = "JDCJ";
    retBaseinfo.screenname = QString(QStringLiteral("见底出击")).toStdString();
    //短期趋势
    IndiResult info;
    info.screenname =QString(QStringLiteral("短期趋势")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::yellow);
    info.drawType = LINESTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //长期趋势
    info=IndiResult();
    info.screenname = QString(QStringLiteral("长期趋势")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor("#C71585");
    info.drawType = ENTITYRECTSTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;


    //中期趋势
    info=IndiResult();
    info.screenname = QString(QStringLiteral("中期趋势")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::red);
    info.drawType = LINESTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;
    base_info = retBaseinfo;
}

void IndicatorSrvProvider::getCmfbBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 0;
    retBaseinfo.id = HZData::CMFB;
    retBaseinfo.name = "CMFB";
    retBaseinfo.screenname = QString(QStringLiteral("筹码分布")).toStdString();
    //集
    IndiResult info;
    info.screenname =QString(QStringLiteral("集")).toStdString();
    info.bShowName = false;
    info.clrMap[0] = QColor(Qt::yellow);
    info.drawType = CMFBSTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //分
    info=IndiResult();
    info.screenname = QString(QStringLiteral("分")).toStdString();
    info.bShowName = false;
    info.clrMap[0] = QColor(Qt::green);
    info.drawType = CMFBSTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    base_info = retBaseinfo;
}

void IndicatorSrvProvider::getCptxBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 1;
    retBaseinfo.id = HZData::CPTX;
    retBaseinfo.name = "CPTX";
    retBaseinfo.screenname = QString(QStringLiteral("操盘提醒")).toStdString();
    //波段进场
    IndiResult info;
    info.screenname =QString(QStringLiteral("波段进场")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::white);
    info.drawType = LINESTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //反弹进场
    info=IndiResult();
    info.screenname = QString(QStringLiteral("反弹进场")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::yellow);
    info.drawType = LINESTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;


    //超跌进场
    info=IndiResult();
    info.screenname = QString(QStringLiteral("超跌进场")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor("#C71585");
    info.drawType = LINESTICK;
    info.isVisible = true;
    info.linewidth = 1;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //红绿区间
    info=IndiResult();
    info.screenname = QString(QStringLiteral("红绿区间")).toStdString();
    info.bShowName = false;
    info.drawType = ENTITYRECTSTICKEX;
    info.linewidth = -1;
    info.isVisible = true;
    info.clrMap[0] = QColor("#e62727");
    info.clrMap[1] = QColor("#0aaa38");
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    base_info = retBaseinfo;
}

void IndicatorSrvProvider::getHlqjBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 1;
    retBaseinfo.id = HZData::HLQJ;
    retBaseinfo.name = "HLQJ";
    retBaseinfo.screenname = QString(QStringLiteral("黄蓝区间")).toStdString();
    //买卖点
    IndiResult info;
    info.screenname =QString(QStringLiteral("买卖点")).toStdString();
    info.bShowName = false;
    info.clrMap[0] = QColor("#e62727"); //买
    info.clrMap[1] = QColor("#0aaa38"); //卖
    info.drawType = BUYSELLSTICK;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //黄蓝区间
    info=IndiResult();
    info.screenname = QString(QStringLiteral("黄蓝区间")).toStdString();
    info.bShowName = false;
    info.drawType = ENTITYRECTSTICKEX;
    info.linewidth = -1;
    info.isVisible = true;
    info.clrMap[0] = QColor(Qt::yellow);
    info.clrMap[1] = QColor(Qt::blue);
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    base_info = retBaseinfo;
}

void IndicatorSrvProvider::getHpmBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 1;
    retBaseinfo.id = HZData::HPM;
    retBaseinfo.name = "HPM";
    retBaseinfo.screenname = QString(QStringLiteral("海平面")).toStdString();
    //底部
    IndiResult info;
    info.screenname =QString(QStringLiteral("底部")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor("#e62727");
    info.drawType = DUOKONGSTICK;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //顶部
    info = IndiResult();
    info.screenname =QString(QStringLiteral("顶部")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor("#0aaa38");
    info.drawType = DUOKONGSTICK;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //海平面
    info = IndiResult();
    info.screenname =QString(QStringLiteral("海平面")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::blue);
    info.drawType = LINESTICK;
    info.linewidth = 1;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;


    //紫绿柱状
    info=IndiResult();
    info.screenname = QString(QStringLiteral("紫绿柱状")).toStdString();
    info.bShowName = false;
    info.drawType = ENTITYRECTSTICKEX;
    info.isVisible = true;
    info.linewidth = 1;
    info.clrMap[0] = QColor("#C71585");
    info.clrMap[1] = QColor("#0aaa38");
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    base_info = retBaseinfo;
}

void IndicatorSrvProvider::getXqqxBaseInfo(IndiItemInfo &base_info)
{
    IndiItemInfo retBaseinfo;
    retBaseinfo.bLocalCalc = false;
    retBaseinfo.drawArea = 1;
    retBaseinfo.id = HZData::XQQX;
    retBaseinfo.name = "XQQX";
    retBaseinfo.screenname = QString(QStringLiteral("心情曲线")).toStdString();
    //心情曲线
    IndiResult info;
    info.screenname =QString(QStringLiteral("心情曲线")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::blue);
    info.drawType = LINESTICK;
    info.linewidth = 2;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //贪婪
    info = IndiResult();
    info.screenname =QString(QStringLiteral("贪婪")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::yellow);
    info.drawType = LINESTICK;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    //恐惧
    info = IndiResult();
    info.screenname =QString(QStringLiteral("恐惧")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor(Qt::red);
    info.drawType = LINESTICK;
    info.isVisible = true;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;


    //强弱线
    info = IndiResult();
    info.screenname =QString(QStringLiteral("强弱线")).toStdString();
    info.bShowName = true;
    info.clrMap[0] = QColor("#0aaa38");
    info.drawType = CIRCLESTICK;
    info.isVisible = true;
    info.linewidth = 4;
    retBaseinfo.results.push_back(info);
    retBaseinfo.resultMap[info.screenname] = info;

    base_info = retBaseinfo;
}

