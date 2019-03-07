#include "tablemodel.h"
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QDateTime>
#include <QTimer>



TableModel::TableModel(QObject *parent):
    QAbstractTableModel(parent)
{
//    QTimer *timer = new QTimer(this);
//    timer->setInterval(INTERVAL);
//    timer->start();
//    connect(timer, &QTimer::timeout, [this]{HzInfoCenter::getInstance()->reqHostNews(0, m_callback);});
    init();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_hostVec.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(m_hostVec.isEmpty())
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
        if(0 == index.column())
        {
            return QStringLiteral("·") + m_hostVec[index.row()].title;
        }
        else if(1 == index.column())
        {
            return QVariant(QDateTime::fromTime_t(m_hostVec[index.row()].time).toString("hh:mm"));
        }
        break;
    case Qt::TextAlignmentRole:
        if(1 == index.column())
        {
            return  QVariant(Qt::AlignCenter);
        }
        break;
    case UrlIdRole:
        return m_hostVec[index.row()].id;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal && 0 == section)
        return QStringLiteral("人民币中间价上调176点");
    if(section == 1)
        return QVariant();
    return QAbstractTableModel::headerData(section , orientation, role);
}

void TableModel::clearData()
{
    m_hostVec.clear();
    beginResetModel();
    endResetModel();
}

void TableModel::setRecords(const HostNews &news)
{
    m_hostVec += news;
    beginResetModel();
    endResetModel();
}

HostNews TableModel::getRecords() const
{
    return m_hostVec;
}


void TableModel::init()
{
    //getHotInfo();
}

//void TableModel::getHotInfo()
//{
//    m_callback = std::bind(&TableModel::onhotNewsArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

//    HzInfoCenter::getInstance()->reqHostNews(0, m_callback);
//}

//void TableModel::onhotNewsArrived(const InfoRet &errCode, HostNews &vec, int nRet)
//{
//    if(errCode.code == 0)
//    {

//        m_hostVec.append(vec);

//        if(m_hostVec.size() >= 50)
//        {
//            m_hostVec = m_hostVec.mid(0,50);

//            beginResetModel();
//            endResetModel();
//            return;
//        }
//        HzInfoCenter::getInstance()->reqHostNews(nRet, m_callback);

//    }
//}
