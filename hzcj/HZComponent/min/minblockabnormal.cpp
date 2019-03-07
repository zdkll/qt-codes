#include "minblockabnormal.h"
#include <QDateTime>

#include "imagelayer.h"
#include "xaxis.h"
#include "yaxis.h"
#include "selfstockpanel.h"
#include "mincanvas.h"

MinBlockAbnormal::MinBlockAbnormal(ImageLayer *layer):
    Minchart(layer)
{
}

MinBlockAbnormal::~MinBlockAbnormal()
{

}

void MinBlockAbnormal::afterActive()
{
    Minchart::afterActive();
    HZData::ReqBlockAbnormalInfo req;
    req.block_code = "";
    req.count_range = 200;
    OnBlockAbnormalInfoArrived fun = std::bind(&MinBlockAbnormal::OnBlockAbnormal,this,std::placeholders::_1, std::placeholders::_2,std::placeholders::_3);
    m_subId = DataController::instance()->subBlockAbnormalInfo(req,fun);
}

void MinBlockAbnormal::afterDeactive()
{
    Minchart::afterDeactive();
    DataController::instance()->cancelSub(m_subId);
}

void MinBlockAbnormal::getMaxMinRange(double &max, double &min)
{
    Minchart::getMaxMinRange(max,min);
    max += max*0.01;
    min -= min*0.01;
}

void MinBlockAbnormal::errorResponse(int id, int ncode, const QString &errReuslt)
{
    m_data = HZData::BlockAbnormalInfo();
    m_imageLayer->clear(m_bkAbnormalLayer); //清理掉板块异动数据数据
    Minchart::errorResponse(id,ncode,errReuslt);
}

void MinBlockAbnormal::onResize()
{
    Minchart::onResize();
    if(m_bkAbnormalLayer != -1)
        m_imageLayer->resize(m_wndDrawRc,m_bkAbnormalLayer);
    drawBlock();
    paintImmet();
}

void MinBlockAbnormal::onCreate()
{
    Minchart::onCreate();
    auto info = m_imageLayer->createLayout(m_wndDrawRc);
    m_bkAbnormalLayer = info.type;
}

void MinBlockAbnormal::OnBlockAbnormal(ReqId id, const HZData::BlockAbnormalInfo &data, ErrCode err)
{
    if(id == m_subId)
    {
        m_data = data;
        drawBlock();
        paintImmet();
    }
}
qint64 rmoveSecond(qint64 t)
{
    QDateTime dateTime;
    dateTime.setTime_t(t);
    QString str = dateTime.toString("yyyy/MM/dd hh:mm");
    dateTime = QDateTime::fromString(str,"yyyy/MM/dd hh:mm");
    return dateTime.toTime_t();
}

void MinBlockAbnormal::drawBlock()
{
    m_imageLayer->clear(m_bkAbnormalLayer);
    if(m_vKlineData.m_data.isEmpty())
        return;
    bool upPos = true;
    ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_bkAbnormalLayer);
    QPainter painter(drawLayer.pix);

    int index = -1;
    QRect rc;
    QPoint pt1;
    QPoint pt2;
    QVector<QPoint> pts;
    QVector<QLine> lines;
    QVector<QString> strList;
    QVector<QRect> rcList;
    qint64 time = 0;
    QVector<double>& upsetTime = m_vKlineData.m_upsertTime;
    QMap<int64_t,int64_t> upDrawDateMap; // 去除重复绘图
    QMap<int64_t,int64_t> downDrawDateMap; // 去除重复绘图
    for(int i = 0;i < m_data.blockAbnormals.size();i++)
    {
        HZData::BlockAbnormal& item = m_data.blockAbnormals[i];
        if(!item.isEmpty())
        {
            time =  rmoveSecond(item[0].update_time);
            index = upsetTime.indexOf(time);
            if(upPos && upDrawDateMap.contains(time) == false ||
               upPos == false && downDrawDateMap.contains(time) == false)
            {
                if(index != -1)
                {
                    pt1.setX(m_xAxis->xCanvasIndexToPTx(index));
                    pt1.setY(m_yAxis->yCanvasValueToPt(m_vKlineData.m_closePrice[index]));
                    pts.push_back(pt1);
                    pt2 = pt1;
                    if(upPos)
                    {
                        upDrawDateMap[time] = time;
                        pt2.setY(pt2.y()-25);
                        rc.setBottom(pt2.y());
                        rc.setTop(pt2.y()-20);
                    }else
                    {
                        downDrawDateMap[time] = time;
                        pt2.setY(pt2.y()+25);
                        rc.setTop(pt2.y());
                        rc.setBottom(rc.top()+25);
                    }
                    rc.setLeft(pt2.x());
                    rc.setRight(pt2.x()+65);
                    if(rc.right() > m_rc.right())
                    {
                        rc.setRight(pt2.x());
                        rc.setLeft(rc.right()-65);
                    }
                    rcList.push_back(rc);
                    strList.push_back(item[0].stock_name.c_str());
                    lines.push_back(QLine(pt1,pt2));
                    upPos = !upPos;
                }
            }
        }
    }

     MinCanvas* pWnd = (MinCanvas*)m_imageLayer->getWnd();
     QPen pen;
     pen.setColor(pWnd->getAbNormalLineClr());
     QBrush brush(pWnd->getAbNormalRcBkClr());
     painter.setBrush(brush);
     painter.setPen(pen);
     painter.drawRects(rcList);
     painter.drawLines(lines);

     pen.setColor(pWnd->getAbNormalTextClr());
     painter.setPen(pen);
     brush.setColor(pWnd->getAbNormalPtBkClr());
     painter.setBrush(brush);
     QFont font;
     font.setPixelSize(pWnd->getAbNormalFontSize());
     font.setFamily(pWnd->getAbNormalFontFamily());
     font.setStyleStrategy(QFont::NoAntialias);
     painter.setFont(font);
     for(int i = 0;i < pts.size();i++)
     {
         painter.drawEllipse(pts[i],3,3);
         painter.drawText(rcList[i],Qt::AlignCenter,strList[i]);
     }
}


