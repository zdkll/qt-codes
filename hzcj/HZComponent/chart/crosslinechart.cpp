#include "crosslinechart.h"
#include "imagelayer.h"
#include "xaxis.h"
#include "yaxis.h"
#include <QDateTime>
#include <qdebug.h>
#include <qfontmetrics.h>
#include "basecanvas.h"
#include "charttitle.h"
CrossLineChart::CrossLineChart(ImageLayer *layer):BaseChart(layer)
{

}

bool CrossLineChart::enterEventEx(QEvent *event)
{
    if(isShowEx())
    {
      redraw();
    }
    return true;
}

bool CrossLineChart::leaveEventEx(QEvent *event)
{
    if(isShowEx())
    {
        m_imageLayer->clear(m_type);
        m_imageLayer->getWnd()->update();
    }
    return true;
}

bool CrossLineChart::moveCrossLine(const QPoint& pos,int index)
{
    BaseCanvas* pWnd = (BaseCanvas*)m_imageLayer->getWnd();
    bool bRet = false;
    QRect drawRc = getChartMainArea();
    std::shared_ptr<ChartTitle> pTitle = m_xAxis->getChart()->getTitle();
    if(pTitle)
    {
        drawRc.setTop(drawRc.top()+pTitle->getRc().height());
    }
    if(drawRc.contains(pos))
    {
        m_imageLayer->clear(m_type);
        ImgLayerInfo drawLayer =  m_imageLayer->getLayout(m_type);
        QPainter painter;
        painter.begin(drawLayer.pix);
        QPen pen;
        pen.setStyle(Qt::PenStyle::DashDotLine);
        pen.setWidth(1);
        pen.setColor(QColor(pWnd->getCrossLineClr()));
        painter.setPen(pen);
        //绘制垂直十字线
        QPoint pt1(pos.x(),drawRc.top());
        QPoint pt2(pos.x(),drawRc.bottom());
        painter.drawLine(pt1,pt2);

        //绘制水平十字线
        QPoint pt3(drawRc.left(),pos.y());
        QPoint pt4(drawRc.right(),pos.y());
        painter.drawLine(pt3,pt4);

        //绘制分时和均线与垂直十字线交叉点
        if(index  == -1)
              index = m_xAxis->xCanvasPTxToIndex(pos.x());
        const DataKLine &data = m_xAxis->getChart()->getScreenData();
        std::shared_ptr<YAxis> mainYaxis = m_xAxis->getChart()->getYAxis();
        if(index != -1 && index >= 0 && mainYaxis && m_xAxis->getChart()->isDrawCrossLintPt())
        {
           if(index < data.m_closePrice.size())
           {
               QPoint closePt;
               closePt.setX(pos.x());
               closePt.setY(mainYaxis->yCanvasValueToPt(data.m_closePrice[index]));
               QColor clr = QColor(pWnd->getCrossLineClostPtClr());
               QPen pen;
               pen.setColor(clr);
               QBrush brush(clr);
               painter.setPen(clr);
               painter.setBrush(brush);
               painter.drawEllipse(closePt,3,3);
               painter.setBrush(Qt::NoBrush);
           }
           if(index < data.m_averPrice.size() && m_xAxis->getChart()->isDrawAvgLine())
           {
               QPoint avgPt;
               avgPt.setX(pos.x());
               avgPt.setY(mainYaxis->yCanvasValueToPt(data.m_averPrice[index]));
               QColor clr = QColor(pWnd->getCrossLineAvgPtClr());
               QPen pen;
               pen.setColor(clr);
               QBrush brush(clr);
               painter.setPen(clr);
               painter.setBrush(brush);
               painter.drawEllipse(avgPt,3,3);
               painter.setBrush(Qt::NoBrush);
           }
        }

        //绘制时间轴区域 X轴时间提示框
        QString formatStr = "yyyy-MM-dd";
        if(m_xAxis->getChart())
        {
            m_xAxis->getChart()->getFormatTimeStr(formatStr);
        }
        qint64 time = m_xAxis->xCanvasIndexToTime(index);
        QString strTime = "--:--";
        if(time != -1)
        {
            QDateTime dateTime;
            dateTime.setTime_t(time);
            strTime = dateTime.toString(formatStr);
        }

        QFont font;
        font.setFamily(pWnd->getCrossLineFontFamily());
        font.setPixelSize(pWnd->getCrossLineFontSize());
        font.setStyleStrategy(QFont::NoAntialias);
        QFontMetrics fm(font);
        int width = fm.boundingRect(strTime).width()+5;
        if(width < YaxisKlineMinWidth)
            width = YaxisKlineMinWidth;

        QRect xTipRc = getChartMainArea();
        xTipRc.setTop(xTipRc.bottom() - TimeXaxisHeight);
        xTipRc.setLeft(pos.x()-width/2);
        xTipRc.setRight(xTipRc.left()+ width);


        pen.setStyle(Qt::PenStyle::SolidLine);
        pen.setWidth(1);
        pen.setColor(QColor(pWnd->getCrossLineClr()));
        painter.setPen(pen);
        painter.fillRect(xTipRc,QColor(pWnd->getCrossLineRcBkClr()));
        painter.drawRect(xTipRc);

        painter.setFont(font);
        pen.setColor(QColor(pWnd->getCrossLineTextClr()));
        painter.setPen(pen);
        painter.drawText(xTipRc,Qt::AlignCenter,strTime);


        //绘制各个chart的Y轴左右侧提示框
        if(m_yAxis && m_yAxis->getRc().contains(pos))
        {
            double price = m_yAxis->yCanvasPTyToValue(pos.y());

            QRect yRightTipRect = m_yAxis->getChart()->getRightAxisArea();
            if(!yRightTipRect.isEmpty())
            {
                QString strRightPrice = Default_Item_String;
                yRightTipRect.setTop(pos.y() - TimeXaxisHeight/2);
                yRightTipRect.setBottom(pos.y() + TimeXaxisHeight/2);
                if(m_yAxis->getChart())
                {
                    strRightPrice = m_yAxis->getChart()->leftTipInfoFormat(price);
                }
                pen.setColor(QColor(pWnd->getCrossLineClr()));
                painter.setPen(pen);
                painter.fillRect(yRightTipRect,QColor(pWnd->getCrossLineRcBkClr()));
                painter.drawRect(yRightTipRect);
                pen.setColor(QColor(pWnd->getCrossLineTextClr()));
                painter.setPen(pen);
                painter.drawText(yRightTipRect,Qt::AlignCenter,strRightPrice);
            }

            QRect yLeftTipRect = m_yAxis->getChart()->getLeftAxisArea();
            if(!yLeftTipRect.isEmpty())
            {
                QString strLeftPrice=Default_Item_String;
                yLeftTipRect.setTop(pos.y() - TimeXaxisHeight/2);
                yLeftTipRect.setBottom(pos.y() + TimeXaxisHeight/2);
                if(m_yAxis->getChart())
                {
                    strLeftPrice = m_yAxis->getChart()->rightTipInfoFormat(price);
                }
                pen.setColor(QColor(pWnd->getCrossLineClr()));
                painter.setPen(pen);
                painter.fillRect(yLeftTipRect,QColor(pWnd->getCrossLineRcBkClr()));
                painter.drawRect(yLeftTipRect);
                pen.setColor(QColor(pWnd->getCrossLineTextClr()));
                painter.setPen(pen);
                painter.drawText(yLeftTipRect,Qt::AlignCenter,strLeftPrice);
            }
        }
        painter.end();
        m_imageLayer->getWnd()->update();
        bRet = true;
    }
    return bRet;
}

bool CrossLineChart::keyPressEx(QKeyEvent *event)
{
    if (m_xAxis && m_yAxis && isShowEx())
	{
		QPoint pos;
		int index = m_xAxis->getCurrCrossXIndex();
		if (index > m_xAxis->getCount())
		{
			pos = QCursor().pos();
            pos = m_imageLayer->getWnd()->mapFromGlobal(pos);
			index = m_xAxis->xCanvasPTxToIndex(pos.x());
		}
        qDebug() << "index: " << index;
		if (index != -1)
		{
			if (Qt::Key_Left == event->key())
			{
				index = index - 1;
				if (index < 0)
					index = 0;
                pos.setX(m_xAxis->xCanvasIndexToPTx(index/*,false*/));
				double price = m_xAxis->xGetPriceByIndex(index);
				if (price > 0)
				{
                    pos.setY(qRound(m_xAxis->getChart()->getYAxis()->yCanvasValueToPt(price)));
				}
                if (moveCrossLine(pos,index))
				{
					m_xAxis->setCurrCrossXIndex(index);
				}
			}
			if (Qt::Key_Right == event->key())
			{
				index = index + 1;
				if (index >= m_xAxis->getCount())
					index = m_xAxis->getCount() - 1;
                pos.setX(m_xAxis->xCanvasIndexToPTx(index));
				double price = m_xAxis->xGetPriceByIndex(index);
				if (price > 0)
				{
                    pos.setY(qRound(m_xAxis->getChart()->getYAxis()->yCanvasValueToPt(price)));
				}
                if (moveCrossLine(pos,index))
				{
					m_xAxis->setCurrCrossXIndex(index);
				}
			}
		}
	}
    return true;
}

bool CrossLineChart::mouseMoveEx(QMouseEvent *event)
{
    if(event && isShowEx())
    {
        QPoint pos = event->pos();
        if(moveCrossLine(pos))
        {
            m_xAxis->setCurrCrossXIndex(m_xAxis->xCanvasPTxToIndex(pos.x()));
            m_currCrossPt = pos;
        }else
        {
            m_xAxis->setCurrCrossXIndex(-1);
        }
    }
    return true;
}

void CrossLineChart::redraw()
{
    if(m_xAxis && m_yAxis)
    {
        int index  = m_xAxis->getCurrCrossXIndex();
        if(index != -1)
        {
            QPoint pos;
            pos.setX(m_xAxis->xCanvasIndexToPTx(index));
            double price  =  m_xAxis->xGetPriceByIndex(index);
            if(price > 0)
            {
                pos.setY(qRound(m_yAxis->yCanvasValueToPt(price)));
            }
            moveCrossLine(pos);
        }
    }
}

void CrossLineChart::onShow(bool bshow)
{
    BaseChart::onShow(bshow);
    if(bshow)
    {
        QPoint pos = QCursor().pos();
        pos = m_imageLayer->getWnd()->mapFromGlobal(pos);
        if(moveCrossLine(pos))
        {
            m_xAxis->setCurrCrossXIndex(m_xAxis->xCanvasPTxToIndex(pos.x()));
            m_currCrossPt = pos;
        }
    }
}

void CrossLineChart::mainChartXaixsChnage()
{
    if(isShowEx())
        redraw();
}
