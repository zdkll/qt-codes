/********************************************************************
created: 2018.9.29
author: juntang.xie
purpose: 添加股票对话框中间窗体容器
*********************************************************************/
#ifndef HISTORYBOXWIDGET_H
#define HISTORYBOXWIDGET_H
#include "basewidget.h"
#include "datacontroller.h"
#pragma execution_character_set("utf-8")
/*!
 * \brief The CustomGroup class 指定分组
 */
//class CustomGroup
//{
//public:
//    static void init()
//    {
//        mGroupData[10] = "股票";
//        mGroupData[11] = "基金";
//        mGroupData[12] = "债券";
//        mGroupData[13] = "指数";
//        mGroupData[14] = "权证";
//        mGroupData[15] = "利率";
//        mGroupData[16] = "期货";
//        mGroupData[17] = "现货";
//        mGroupData[18] = "理财产品";
//        mGroupData[19] = "远期";
//        mGroupData[20] = "期权";
//        mGroupData[50] = "基金业绩基准";
//        mGroupData[51] = "信用风险缓释工具";
//        mGroupData[52] = "非公开定向工具";
//        mGroupData[53] = "经济价格指数";
//        mGroupData[54] = "数据源2";
//        mGroupData[55] = "数据源1";
//        mGroupData[56] = "服务端";

//        return;
//        //}
//    }


//    /**
//     * @brief getGroupNameByGroupId 通过组ID获取组名
//     * @param id 组ID
//     * @return
//     * @note 如果查找不到，则返回“其它”
//     */
//    static QString getGroupNameByGroupId(int id){
//        if(mGroupData.contains(id)) {
//            return mGroupData[id];
//        }
//        return "其它";
//    }

//private:
//    static QMap<int,QString> mGroupData;
//};

static const QHash<QString,QString> gGroupNames = {
    {"SHES", "沪A"},
    {"SZ1", "深A"},
    {"SZ2", "深A"},
    {"SZ3", "深A"},
    {"SZ4", "深B"},
    {"SZI", "指数"},
    {"SHI", "指数"},
    {"BK", " 板块"},
};

class HistoryBoxView;
class HistoryBoxModel;
class HistoryBoxWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit HistoryBoxWidget(QWidget *parent = nullptr);
    ~HistoryBoxWidget();

public slots:
    void slotInput(QString strText);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void createWg();
    void searchStock(const QString& searchText);
    void searchFromServer(const QString &searchText);

    void seachrArrived(ReqId id, QVector<HZData::CodeInfo> codes, ErrCode errCode);

    bool isSelfStock(const QString &strCode);

    void init();
    void readJson();
    void writeJson();

    void updateData(const QVector<HZData::CodeInfo> &codes);

    void insertHisCode(const HZData::CodeInfo &code);

signals:
    void stateChange();

private slots:
    void onBtnClicked(const QModelIndex index);

private:
    HistoryBoxView *m_pTableView;
    HistoryBoxModel *m_pModel;
    QString m_strInput;
    QVector<HZData::CodeInfo> m_defaultCode;
    QVector<HZData::CodeInfo> m_historyCode;
};

#endif // HISTORYBOXWIDGET_H
