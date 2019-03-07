#include "historyboxwidget.h"
#include <QDebug>
#include <QApplication>
#include "historybox/historyboxmodel.h"
#include "historybox/historyboxview.h"
#include "hzauthenopt.h"
#include "localstocksource.h"
#include "hzdirhelper.h"
#include "zxstockopt.h"

//QMap<int,QString> CustomGroup::mGroupData;
HistoryBoxWidget::HistoryBoxWidget(QWidget *parent):
    BaseWidget(parent)
{
//    CustomGroup::init();
    createWg();
    init();
}

HistoryBoxWidget::~HistoryBoxWidget()
{
    qDebug() << "~HistoryBoxWidget";
    writeJson();
}

void HistoryBoxWidget::slotInput(QString strText)
{
    QString strtmp = strText;

    strtmp = strtmp.simplified();
    m_strInput = strtmp;
    if(strtmp.isEmpty()){
        strtmp = "";
        m_strInput = strtmp;
    }
    searchStock(strtmp);
}

void HistoryBoxWidget::resizeEvent(QResizeEvent *event)
{
    m_pTableView->resize(this->size());
    int nWidth = this->rect().width();
    m_pTableView->setColumnWidth(STOCK_CODE_COLUMN, nWidth*0.3);
    m_pTableView->setColumnWidth(STOCK_NAME_COLUMN, nWidth*0.3);
    m_pTableView->setColumnWidth(STOCK_TYPE_COLUMN, nWidth*0.3);
    m_pTableView->setColumnWidth(BUTTON_ADD_COLUMN, 20);
}

void HistoryBoxWidget::createWg()
{
    m_pTableView = new HistoryBoxView(this);

    m_pModel = new HistoryBoxModel(this);

    m_pTableView->setModel(m_pModel);

    connect(m_pTableView, &QTableView::clicked, this, &HistoryBoxWidget::onBtnClicked);
}

void HistoryBoxWidget::searchStock(const QString &searchText)
{
    searchFromServer(searchText);
}

void HistoryBoxWidget::searchFromServer(const QString &searchText)
{
    HZData::ReqQueryCode codeQuery = { HZData::SearchType::FuzzyQuery, searchText.toLower().toStdString()};
    OnCodeInfoArrived codeSearchFunc = std::bind(&HistoryBoxWidget::seachrArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    DataController::instance()->requestCodeInfo(codeQuery ,codeSearchFunc);
}

void HistoryBoxWidget::seachrArrived(ReqId id, QVector<HZData::CodeInfo> codes, ErrCode errCode)
{
    Q_UNUSED(id)
    if(0 == errCode)
    {
        if(m_strInput.isEmpty())
        {
            readJson();
        }
        else
        {
            qSort(codes.begin(), codes.end(), [=](const HZData::CodeInfo &first,const HZData::CodeInfo &second){
                return first.code < second.code;
            });
            updateData(codes);
        }

    }
}

bool HistoryBoxWidget::isSelfStock(const QString &strCode)
{

    ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

    if(zxStockOpt->isHas(strCode))
    {
        return true;
    }
    else
    {
        return false;
    }


    return false;
}

void HistoryBoxWidget::init()
{
    HZData::CodeInfo code1;
    code1.code = "sh000001";
    code1.name = "上证指数";
    code1.type = "SHI";
    HZData::CodeInfo code2;
    code2.code = "sz399001";
    code2.name = "深证成指";
    code2.type = "SZI";
    HZData::CodeInfo code3;
    code3.code = "sz399006";
    code3.name = "创业板指";
    code3.type = "SZI";

    m_defaultCode.append(code1);
    m_defaultCode.append(code2);
    m_defaultCode.append(code3);

//    updateData(m_defaultCode);
    readJson();
}

void HistoryBoxWidget::readJson()
{
    QString strPath = qApp->applicationDirPath() +"/cfg/search.json";
    //读json
    QJsonDocument doc;
    QVector<HZData::CodeInfo> codeVec;
    if(HzDirHelper::readJson(doc, strPath))
    {
        QJsonArray array = doc.array();
        for(int i = 0; i < array.size(); i++)
        {
            HZData::CodeInfo code;
            QJsonObject obj = array[i].toObject();
            code.code = obj.value("code").toString().toStdString();
            code.name = obj.value("name").toString().toStdString();
            code.type = obj.value("type").toString().toStdString();
            m_historyCode.append(code);
        }
    }
    if(m_historyCode.size() > 10)
        m_historyCode.resize(10);
    codeVec = m_defaultCode + m_historyCode;
    qSort(codeVec.begin(), codeVec.end(), [=](const HZData::CodeInfo &first,const HZData::CodeInfo &second){
        return first.code < second.code;
    });
    updateData(codeVec);
}

void HistoryBoxWidget::writeJson()
{
//    QVector<HZData::CodeInfo> list;
//    HZData::CodeInfo code1;
//    code1.code = "sh000001";
//    code1.name = "上证指数";
//    code1.type = "SHI";
//    HZData::CodeInfo code2;
//    code2.code = "sz399001";
//    code2.name = "深证成指";
//    code2.type = "SZI";
//    HZData::CodeInfo code3;
//    code3.code = "sz399006";
//    code3.name = "创业板指";
//    code3.type = "SZI";
//    list.append(code1);
//    list.append(code2);
//    list.append(code3);

    QString strPath = qApp->applicationDirPath() +"/cfg/search.json";

    /* 写json */
    //构建json数组
    QJsonArray array;

    //构建json对象
    foreach (HZData::CodeInfo code, m_historyCode) {
        QJsonObject obj;
        obj.insert("code", QString::fromStdString(code.code));
        obj.insert("name", QString::fromStdString(code.name));
        obj.insert("type", QString::fromStdString(code.type));
        array.append(QJsonValue(obj));
    }
    //构建json文档
    QJsonDocument doc;
    doc.setArray(array);
    HzDirHelper::saveJson(doc, strPath);
}

void HistoryBoxWidget::updateData(const QVector<HZData::CodeInfo> &codes)
{
    QList<stockRecord> record;
    foreach (HZData::CodeInfo codeinfo, codes) {

        stockRecord stock;
        stock.strCode = QString::fromStdString(codeinfo.code);
        stock.strName = QString::fromStdString(codeinfo.name);

        QString strType = QString::fromStdString(codeinfo.type).toUpper();
        if(gGroupNames.contains(strType))
        {
            stock.strType = gGroupNames[strType];
        }
        else
        {
            stock.strType = "其它";
        }
        stock.bChecked = isSelfStock(stock.strCode);
        record.push_back(stock);
    }

    m_pModel->updateData(record);
}

void HistoryBoxWidget::insertHisCode(const HZData::CodeInfo &code)
{
    if(code.code == "sh000001" || code.code == "sz399001"
            || code.code == "sz399006")
    {
        return;
    }
    QVector<HZData::CodeInfo>::iterator it = m_historyCode.begin();

    while (it != m_historyCode.end())
    {
        if(code.code == it->code)
        {
            it = m_historyCode.erase(it);
        }
        else
        {
            it++;
        }
    }

    m_historyCode.prepend(code);
    if(m_historyCode.size() > 10)
        m_historyCode.resize(10);
}

#include <QMessageBox>

void HistoryBoxWidget::onBtnClicked(const QModelIndex index)
{

    if(index.column() == BUTTON_ADD_COLUMN)
    {
        HZData::CodeInfo code;
        stockRecord record = index.data(MY_USERDATA_ROLE).value<stockRecord>();

        code.code = record.strCode.toStdString();
        code.name = record.strName.toStdString();
        code.type = gGroupNames.key(record.strType).toStdString();
//        m_historyCode.prepend(code);
//        if(m_historyCode.size() > 10)
//            m_historyCode.resize(10);

        insertHisCode(code);

        bool bChecked = record.bChecked;
        QString strCode = record.strCode;

        ZXStockOpt *zxStockOpt =  ZXStockOpt::instance();

        bool ok = false;
        if(!bChecked)
        {
            ok = zxStockOpt->delZXStock(strCode, false);
        }
        else
        {
            if(ZXStockOpt::instance()->getZXStock().size() == 100)
            {
                m_pModel->setData(index, false, Qt::CheckStateRole);
                 QMessageBox::information(nullptr, "提示", "目前只可添加100个自选股", QMessageBox::Ok);
                 return;
            }
            ok = zxStockOpt->addZXStock(strCode, false);
        }

        if(ok)
        {
            emit stateChange();
        }




    }
}
