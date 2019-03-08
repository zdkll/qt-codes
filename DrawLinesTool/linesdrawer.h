#ifndef LINESDRAWER_H
#define LINESDRAWER_H

#include <QWidget>
#include <QPixmap>

#include "line.h"
#include "drawlinepublic.h"
#include "poscalculator.h"
#include "screenshot.h"

//线条绘制管理器：受DrawLinePanel 面板控制，绘制管理Line
class QPaintEvent;
class QMouseEvent;
class LinesDrawer :public QObject
{
    Q_OBJECT
public:
    LinesDrawer(QWidget *parent);
    ~LinesDrawer();

    void resize(const QSize& size);
    void resize(int w,int h);

    void setLineTools(const QVector<DrawLine::LineTool>& linetools);
    DrawLine::LineTool lineTool(int typeId);


    QPixmap pixmap()const {return m_pixmap;}

    //开始和结束命令
    void beginCommand(const DrawLine::Command& cmd);
    void endCommand();

signals:
    void finishedCurCommand();

public slots:
    void penChanged(const QPen& pen);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void paintLines(QPaintEvent *e);

    //截取处理串口鼠标事件
    bool mousePress(QMouseEvent *e);
    //
    bool leftButtonPress(QMouseEvent *e);
    bool rightButtonPress(QMouseEvent *e);

    bool mouseMove(QMouseEvent *e);
    bool mouseRelease(QMouseEvent *e);
    bool mouseDbClick(QMouseEvent *e);

     AbstractLine *findActivedLine(const QPoint& pt);
    bool commandFilter(const DrawLine::Command& cmd);

    void resetAllLines();
    void clearAllLines();//清除所有画线
    void updateCursor(const QPoint& pt);//移动鼠标位置时更新鼠标状态

    //参数设置面板
    bool setLineProperty(AbstractLine *line);
    bool showMenu(AbstractLine *line);

private:
    QPixmap  m_pixmap;
    QSize      m_size;
    QPen       m_pen;
    DrawLine::Command m_cmd;
    AbstractLine             *m_curLine;
    QList<AbstractLine*> m_lines;
    QVector<DrawLine::LineTool>      m_lineTools;

    bool        m_isPressed;
    QPoint    m_pressPt;

    //截图工具
    ScreenShot  *m_screenShot;

    //坐标计算类
    PosCalculator m_pCaltor;
    QWidget        *m_widget;
};




#endif // LINESDRAWER_H
