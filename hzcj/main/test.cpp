#include "test.h"

#include <QDebug>

#include "mvc/sorter.h"

QString  toFormatString(const double &in)
{
    double out = in;

    double val = abs(in);

    QString suffix;
    if(1e4<= val && val<1e8)
    {
        out = out/1e4f;
        suffix = QStringLiteral("万");
    }else if(1e8<= val && val<1e12)
    {
        out = out/1e8f;
        suffix = QStringLiteral("亿");
    }else if(1e12<= val)
    {
        out = out/1e12f;
        suffix = QStringLiteral("万亿");
    }

    int  eval = log10(abs(out));
    eval = 2-eval<0?0:2-eval;
    return QString::number(out,'f',eval)+suffix;
}
QTime t;

//void wait(int sec){
//    QTime t;
//    t.start();
//    while(t.elapsed()<1000*sec) ;
//}

Test::Test()
{
    m_reqBatchInfo.count_ange =m_reqBatchInfo.index = 0;
}


void Test::reqStockBatch()
{
    m_reqBatchInfo.filter = HZData::BlockIndustry;
    //    m_reqBatchInfo.filter_detail ;
    m_reqBatchInfo.index = 0;
    m_reqBatchInfo.sort_field = HZData::block_tot_value;
    m_reqBatchInfo.is_desc = false;
    m_reqBatchInfo.count_ange = 500;
    m_reqBatchInfo.mask = HZData::SLStock_Mask;

    if(!m_file.isOpen()){
        m_file.setFileName(QStringLiteral("C:/StockData/行业板块.txt"));
        m_file.open(QIODevice::WriteOnly|QIODevice::Text);

        m_outStream.setDevice(&m_file);
    }
    m_outStream<<" \t"<<QStringLiteral("板块")<<" \t"<<QStringLiteral("名称")<<" \t"<<QStringLiteral("总市值")<<endl;
    OnStockDynaArrived  stockDynaArrived = std::bind(&Test::onStockBatchDataArrived
                                                     ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                                     ,std::placeholders::_4);

    gHZDataController->requestBatchDyna(m_reqBatchInfo,stockDynaArrived);
}

void Test::onStockBatchDataArrived(ReqId  reqId, char *data, int num, ErrCode errCode)
{
    //收到板块列表
    qDebug()<<"recv StockBatch num:"<<num;
    if(errCode != 0 || num<1) {
        if(m_file.isOpen())m_file.close();
        return;
    }
    HZData::SLStock *slStocks = (HZData::SLStock *)data;
    m_stockCodes.resize(num);
    for(int i=0;i<num;i++){
//        m_outStream<< (1+i)<<"\t"
//                   <<QString::fromStdString(slStocks[i].stock_name)<<endl;
        m_stockCodes[i].stock_code = slStocks[i].stock_code;
    }

    //开始请求板块
    m_index = 0;
    reqBkStockBatch();
}


void Test::reqBkStockBatch()
{
    if(m_index>=m_stockCodes.size()) {
         if(m_file.isOpen())m_file.close();
        return;
    }

    m_reqBatchInfo.filter = HZData::BlockIndustry;
    m_reqBatchInfo.filter_detail  = m_stockCodes[m_index].stock_code ;
    m_reqBatchInfo.index = 0;
    m_reqBatchInfo.sort_field = HZData::tot_value;
    m_reqBatchInfo.is_desc = false;
    m_reqBatchInfo.count_ange = 500;
    m_reqBatchInfo.mask = HZData::Stock_Mask;


    OnStockDynaArrived  stockDynaArrived = std::bind(&Test::onBkStockBatchDataArrived
                                                     ,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                                                     ,std::placeholders::_4);

    gHZDataController->requestBatchDyna(m_reqBatchInfo,stockDynaArrived);
}

void Test::onBkStockBatchDataArrived(ReqId  reqId, char *data, int num, ErrCode errCode)
{
    //收到板块列表
    qDebug()<<"recv BkStockBatch num:"<<num;
    if(errCode != 0 || num<1) {
        qDebug()<<QString::fromStdString(gHZDataController->errString());
        if(m_file.isOpen())m_file.close();
        return;
    }
    HZData::Stock *stocks = (HZData::Stock *)data;

    m_outStream<<(m_index+1)<<"\t"
               <<QString::fromStdString(stocks[0].stock_name)<<endl;
    for(int i=1;i<num;i++){
        m_outStream<<"\t"<< i<<"\t"
                   <<QString::fromStdString(stocks[i].stock_name)<<"\t"<<toFormatString(stocks[i].tot_value)<<endl;
    }

    //开始请求板块
    m_index ++;
    reqBkStockBatch();
}


