#include "render.h"
#include <qpoint.h>
#include <qpen.h>

#include "xaxis.h"
#include "yaxis.h"
#include "basechart.h"

void Render::drawIndicator(QPainter &dc,const IndiRspResult &data, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y,UpDownEqualClr clr)
{
    for(unsigned int i=0;i < data.vecOutput.size();i++)
    {
        const IndiData& item = data.vecOutput[i];
        const IndiResult& resultStyle = data.Calcinfo.base_info.results[i];
        if(resultStyle.isVisible)
        {
            if(resultStyle.drawType ==  LINESTICK && item.pBlock)
            {
                IndiLineStick(dc,resultStyle,item.pBlock,x,y,clr);
            }else if(resultStyle.drawType == VOLSTICK && item.pBlock)
            {
                IndiKlineVolStick(dc,resultStyle,item.pBlock,x,y,clr);
            }
            else if(resultStyle.drawType == ENTITYRECTSTICK && item.pBlock)
            {
                IndiEntityRectStick(dc,resultStyle,item.pBlock,x,y,clr);
            }
            else if(COLORSTICK == resultStyle.drawType && item.pBlock) //彩色棒状
            {
                IndiColorStick(dc,resultStyle,item.pBlock,x,y,clr);
            }else if(CMFBSTICK == resultStyle.drawType && item.pBlock)
            {
                IndiCmfbStick(dc,resultStyle,item.pBlock,x,y,clr);
            }else if(ENTITYRECTSTICKEX == resultStyle.drawType  && item.blockList.isEmpty() == false)
            {
                IndiCptxRedGreenStick(dc,resultStyle,item.blockList,x,y,clr);
            }else if(BUYSELLSTICK == resultStyle.drawType)
            {
                IndiBuySellStick(dc,resultStyle,item.pBlock,x,y,clr);
            }else if(DUOKONGSTICK == resultStyle.drawType)
            {
                IndiDuoKongStick(dc,resultStyle,item.pBlock,x,y,clr);
            }else if(CIRCLESTICK == resultStyle.drawType )
            {
                IndiCircleStick(dc,resultStyle,item.pBlock,x,y,clr);
            }
        }
    }

}

void Render::IndiColorStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y,UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0;
        double xPos = -1,yPos;
        int nEndPos =  x->getCount()-1;
        QVector<QLine> upVec,downVec;
        QRect AreaRc = y->getRc();
        //double midPos = AreaRc.top()+AreaRc.height()/2;
        double midPos =y->yCanvasValueToPt(0);
        double mid = 0/*y->yCanvasPTyToValue(midPos)*/;
        QLine line;
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT && val != 0)
            {
                yPos = y->yCanvasValueToPt(val);
                xPos = x->xCanvasIndexToPTx(nEndPos);
                line.setLine(xPos,midPos,xPos,yPos);
                if(val > mid)
                {
                    upVec.push_back(line);
                }else
                {
                    downVec.push_back(line);
                }
            }
        }
        if(upVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.upClr);
            dc.setPen(pen);
            dc.drawLines(upVec);
        }
        if(downVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.downClr);
            dc.setPen(pen);
            dc.setBrush(QBrush(clr.downClr));
            dc.drawLines(downVec);
        }
    }
}

void Render::IndiLineStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock,std::shared_ptr<XAxis> x,std::shared_ptr<YAxis> y,UpDownEqualClr clr)
{
    if(pBlock)
    {
        QPen     pen;
        pen.setColor(resultStyle.clrMap[0]);
        pen.setWidth(resultStyle.linewidth);
        dc.setPen(pen);
        QVector<QPointF> vecPt;
        double val = 0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        QRect rc =  y->getChart()->getRc();
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            //qDebug() << "Y rc: " << "left: " << y->getRc().left() << "right: " << y->getRc().right() << "top: " <<y->getRc().top() << "bottom: " << y->getRc().bottom();
            //qDebug() << "block[]：" << j << "value: " << val;
            if(val != INVALID_MEMBLOCKDATA_FLOAT  && val != -1)
            {
                val = y->yCanvasValueToPt(val);
                xPos = x->xCanvasIndexToPTx(nEndPos);
                if(rc.contains(xPos,val))
                    vecPt.push_back(QPointF(xPos,val));
            }
        }
        if(vecPt.isEmpty() == false)
        {
            dc.drawPolyline(vecPt.data(),vecPt.size());
        }
    }
}

void Render::IndiKlineVolStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        double fWidth = x->getSingleAreaWidth()* CandleRate;
        QVector<QRectF> upVec,downVec;
        QRect AreaRc = y->getRc();
        int k_status = KlineStatusDown;
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            //qDebug() << "Y rc: " << "left: " << y->getRc().left() << "right: " << y->getRc().right() << "top: " <<y->getRc().top() << "bottom: " << y->getRc().bottom();
            //qDebug() << "block[]：" << j << "value: " << val;
            if(val != INVALID_MEMBLOCKDATA_FLOAT && val != -1)
            {
                val = y->yCanvasValueToPt(val);
                if(val == 0)
                    continue;
                xPos = x->xCanvasIndexToPTx(nEndPos);
                QRect rc = AreaRc;
                rc.setLeft(xPos-fWidth/2.0);
                rc.setRight(rc.left() + fWidth);
                rc.setTop(val);
                if(x->getChart()->getStatusFromIndex(nEndPos,k_status))
                {
                    if(k_status == KlineStatusDown)
                    {
                        downVec.push_back(rc);
                    }else
                    {
                        upVec.push_back(rc);
                    }
                }
            }
        }
        if(upVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.upClr);
            dc.setPen(pen);
            dc.drawRects(upVec);
        }
        if(downVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.downClr);
            dc.setPen(pen);
            dc.setBrush(QBrush(clr.downClr));
            dc.drawRects(downVec);
        }
    }
}

void Render::IndiEntityRectStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        double fWidth = x->getSingleAreaWidth()* CandleRate;
        QVector<QRectF> outVec;
        QRect AreaRc = y->getRc();
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT && val != -1 && val != 0)
            {
                val = y->yCanvasValueToPt(val);
                if(val == 0)
                    continue;
                xPos = x->xCanvasIndexToPTx(nEndPos);
                QRect rc = AreaRc;
                rc.setLeft(xPos-fWidth/2.0);
                rc.setRight(rc.left() + fWidth);
                rc.setTop(val);
                rc.setBottom(y->yCanvasValueToPt(0.0));
                dc.fillRect(rc,resultStyle.clrMap[0]);
            }
        }
    }
}

void Render::IndiCmfbStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0,yPos;
        double xPos = -1;
        QVector<QPointF> gVec;
        QVector<QPointF> dVec;
        const DataKLine& data =  x->getChart()->getScreenData();
        for(int j = pBlock->GetCount()-1;j >= 0;j--)
        {
            val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT && j < data.m_highPrice.size()
                    && j < data.m_lowPrice.size())
            {
                xPos = x->xCanvasIndexToPTx(j);
                if(val >= data.m_highPrice[j]) //分
                {
                    yPos = y->yCanvasValueToPt(data.m_highPrice[j]);
                    dVec.push_back(QPointF(xPos,yPos-20));

                }else if(val <= data.m_lowPrice[j]) //集
                {
                    yPos = y->yCanvasValueToPt(data.m_lowPrice[j]);
                    gVec.push_back(QPointF(xPos,yPos+20));
                }
            }
        }
        if(gVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(resultStyle.clrMap[0]);
            dc.setPen(pen);
            QRectF rc;
            for(int i =0;i < gVec.size();i++)
            {
                rc.setBottom(gVec[i].y());
                rc.setTop(rc.bottom()-20);
                rc.setLeft(gVec[i].x()-10);
                rc.setRight(gVec[i].x()+10);
                dc.drawText(rc,Qt::AlignHCenter|Qt::AlignBottom,QStringLiteral("集"));
            }
        }

        if(dVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(resultStyle.clrMap[0]);
            dc.setPen(pen);
            QRectF rc;
            for(int i =0;i < dVec.size();i++)
            {
                rc.setTop(dVec[i].y());
                rc.setBottom(rc.top()+20);
                rc.setLeft(dVec[i].x()-10);
                rc.setRight(dVec[i].x()+10);
                dc.drawText(rc,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("分"));
            }
        }
    }
}

void Render::IndiCptxRedGreenStick(QPainter &dc, const IndiResult &resultStyle, QVector<MemBlock *> blockList, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(!blockList.isEmpty())
    {
        double topVal = 0.0,downVal=0.0,midVal = 0.0;
        bool bUp = false;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        double fWidth = x->getSingleAreaWidth()* CandleRate;
        if(resultStyle.linewidth != -1)
        {
            fWidth = resultStyle.linewidth;
        }
        QRect AreaRc = y->getRc();
        for(int j = blockList[0]->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            topVal = blockList[0]->GetNumberAt(j);
            if(topVal != INVALID_MEMBLOCKDATA_FLOAT)
            {
                downVal = blockList[1]->GetNumberAt(j);
                midVal = blockList[2]->GetNumberAt(j);
                if(topVal == midVal)
                    bUp=true;
                else if(downVal = midVal)
                    bUp = false;

                topVal = y->yCanvasValueToPt(topVal);
                downVal = y->yCanvasValueToPt(downVal);
                xPos = x->xCanvasIndexToPTx(nEndPos);
                QRect rc = AreaRc;
                rc.setLeft(xPos-fWidth/2.0);
                rc.setRight(rc.left() + fWidth);
                rc.setTop(topVal);
                rc.setBottom(downVal);
                if(bUp)
                {
                    dc.fillRect(rc,resultStyle.clrMap[0]);
                }else
                {
                    dc.fillRect(rc,resultStyle.clrMap[1]);
                }
            }
        }
    }
}

void Render::IndiBuySellStick(QPainter &dc, const IndiResult &resultStyle, MemBlock* pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0,yPos;
        double xPos = -1;
        QPointF pt;
        const DataKLine& data =  x->getChart()->getScreenData();
        QPen     spen;
        spen.setColor(resultStyle.clrMap[1]);
        QPen     bpen;
        bpen.setColor(resultStyle.clrMap[0]);
        QFont font;
        font.setBold(true);
        font.setPixelSize(14);
        font.setFamily("SimSun");
        dc.setFont(font);
        QRectF rc;
        for(int j = pBlock->GetCount()-1;j >= 0;j--)
        {
            val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT && j < data.m_highPrice.size()
                    && j < data.m_lowPrice.size())
            {
                xPos = x->xCanvasIndexToPTx(j);
                if(val >= data.m_highPrice[j]) //卖
                {
                    dc.setPen(spen);
                    yPos = y->yCanvasValueToPt(data.m_highPrice[j]);
                    pt = QPointF(xPos,yPos-20);
                    rc.setTop(pt.y());
                    rc.setBottom(rc.top()+20);
                    rc.setLeft(pt.x()-10);
                    rc.setRight(pt.x()+10);
                    dc.drawText(rc,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("S"));

                }else if(val <= data.m_lowPrice[j]) //买
                {
                    dc.setPen(bpen);
                    yPos = y->yCanvasValueToPt(data.m_lowPrice[j]);
                    pt = QPointF(xPos,yPos+20);
                    rc.setBottom(pt.y());
                    rc.setTop(rc.bottom()-20);
                    rc.setLeft(pt.x()-10);
                    rc.setRight(pt.x()+10);
                    dc.drawText(rc,Qt::AlignHCenter|Qt::AlignBottom,QStringLiteral("B"));
                }
            }
        }
    }
}

void Render::IndiDuoKongStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        QPen     pen;
        pen.setColor(resultStyle.clrMap[0]);
        pen.setWidth(resultStyle.linewidth);
        dc.setPen(pen);

        QFont font;
        font.setPixelSize(14);
        font.setFamily("SimSun");
        dc.setFont(font);

        QVector<QPointF> vecPt;
        double val  = 0.0;
        double valTemp = 0.0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        QRect rc =  y->getChart()->getRc();

        QRect textRc;
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            valTemp = val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT)
            {
                val = y->yCanvasValueToPt(val);
                xPos = x->xCanvasIndexToPTx(nEndPos);
                if(rc.contains(xPos,val))
                    vecPt.push_back(QPointF(xPos,val));
                if(valTemp < 90.0 && valTemp > 50.0) //空
                {
                    textRc.setLeft(xPos-10);
                    textRc.setRight(xPos+10);
                    textRc.setTop(val);
                    textRc.setBottom(val+20);
                    dc.drawText(textRc,Qt::AlignHCenter|Qt::AlignTop,QStringLiteral("空"));
                }else if(valTemp > 10.0 && valTemp < 50.0) //多
                {
                    textRc.setLeft(xPos-10);
                    textRc.setRight(xPos+10);
                    textRc.setBottom(val);
                    textRc.setTop(val-20);
                    dc.drawText(textRc,Qt::AlignHCenter|Qt::AlignBottom,QStringLiteral("多"));
                }
            }
        }
        if(vecPt.isEmpty() == false)
        {
            dc.drawPolyline(vecPt.data(),vecPt.size());
        }
    }
}

void Render::IndiCircleStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y, UpDownEqualClr clr)
{
    if(pBlock)
    {
        QPen     pen;
        pen.setColor(resultStyle.clrMap[0]);
        pen.setStyle(Qt::SolidLine);
        dc.setPen(pen);
        double val = 0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        QRect rc =  y->getChart()->getRc();
        double nWidth = resultStyle.linewidth/2;
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            //qDebug() << "Y rc: " << "left: " << y->getRc().left() << "right: " << y->getRc().right() << "top: " <<y->getRc().top() << "bottom: " << y->getRc().bottom();
            //qDebug() << "block[]：" << j << "value: " << val;
            if(val != INVALID_MEMBLOCKDATA_FLOAT  && val != -1)
            {
                val = y->yCanvasValueToPt(val);
                xPos = x->xCanvasIndexToPTx(nEndPos);
                if(rc.contains(xPos,val))
                    dc.drawEllipse(QPoint(xPos,val),2 ,2 );
            }
        }
    }
}

void Render::IndiMinVolStick(QPainter &dc, const IndiResult &resultStyle, MemBlock *pBlock, std::shared_ptr<XAxis> x, std::shared_ptr<YAxis> y,UpDownEqualClr clr)
{
    if(pBlock)
    {
        double val = 0;
        double xPos = -1;
        int nEndPos =  x->getCount()-1;
        QVector<QLineF> upVec,downVec;
        int k_status = KlineStatusDown;
        QPointF pt1,pt2;
        double top = y->getRc().top()+1;
        pt1.setY(y->getRc().bottom());
        for(int j = pBlock->GetCount()-1;j >= 0;j--,nEndPos--)
        {
            val = pBlock->GetNumberAt(j);
            if(val != INVALID_MEMBLOCKDATA_FLOAT && val != -1)
            {
                val = y->yCanvasValueToPt(val);
                if(val == 0)
                    continue;
                else if(val < top)
                    val = top;
                xPos = x->xCanvasIndexToPTx(nEndPos);
                pt1.setX(xPos);
                pt2.setX(xPos);
                pt2.setY(val);
                if(x->getChart()->getStatusFromIndex(nEndPos,k_status))
                {
                    if(k_status == KlineStatusDown)
                    {
                        downVec.push_back(QLineF(pt1,pt2));
                    }else
                    {
                        upVec.push_back(QLineF(pt1,pt2));
                    }
                }
            }
        }
        if(upVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.upClr);
            dc.setPen(pen);
            dc.drawLines(upVec);
        }
        if(downVec.isEmpty() == false)
        {
            QPen     pen;
            pen.setColor(clr.downClr);
            dc.setPen(pen);
            dc.setBrush(QBrush(clr.downClr));
            dc.drawLines(downVec);
        }
    }

}

//int Render::interpolationKline(const DataKLine &input, int num,double interval,DataKLine &output)
//{
//    output.resize(num);
//    int ret = -1;
//    gobs_1D_interpolation_regular((double*)input.m_updateTime.data(),input.m_updateTime.size(),0,1,(double*)output.m_updateTime.data(),num,0,interval,0,input.m_updateTime.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_highPrice.data(),input.m_highPrice.size(),0,1,(double*)output.m_highPrice.data(),num,0,interval,0,input.m_highPrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_openPrice.data(),input.m_openPrice.size(),0,1,(double*)output.m_openPrice.data(),num,0,interval,0,input.m_openPrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_lowPrice.data(),input.m_lowPrice.size(),0,1,(double*)output.m_lowPrice.data(),num,0,interval,0,input.m_lowPrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_closePrice.data(),input.m_closePrice.size(),0,1,(double*)output.m_closePrice.data(),num,0,interval,0,input.m_closePrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_volume.data(),input.m_volume.size(),0,1,(double*)output.m_volume.data(),num,0,interval,0,input.m_volume.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_turnover.data(),input.m_turnover.size(),0,1,(double*)output.m_turnover.data(),num,0,interval,0,input.m_turnover.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_preClosePrice.data(),input.m_preClosePrice.size(),0,1,(double*)output.m_preClosePrice.data(),num,0,interval,0,input.m_preClosePrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    gobs_1D_interpolation_regular((double*)input.m_averPrice.data(),input.m_averPrice.size(),0,1,(double*)output.m_averPrice.data(),num,0,interval,0,input.m_averPrice.size(),LINEARINTER,&ret);
//    if(ret != 0)
//        return ret;
//    return ret;
//}


