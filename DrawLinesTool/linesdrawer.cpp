#include "linesdrawer.h"

#include <QEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QMessageBox>

#include "line.h"
#include "linesettingdialogs.h"
#include <QDebug>

using namespace DrawLine;


LinesDrawer::LinesDrawer(QWidget *parent)
    :QObject(parent),m_curLine(0)
    ,m_widget(parent),m_screenShot(0)
{
    //设置画线的静态属性
    AbstractLine::setGActivedColor(Qt::white);
    AbstractLine::setGFont(QFont());
}

LinesDrawer::~LinesDrawer()
{
    qDeleteAll(m_lines);
    m_lines.clear();
    if(m_screenShot){
        delete m_screenShot;
        m_screenShot = NULL;
    }
}

void LinesDrawer::resize(const QSize& size)
{
    m_size = size;

    AbstractLine::setGRect(QRect(0,0,size.width(),size.height()));
}

void LinesDrawer::resize(int w,int h)
{
    m_size.setWidth(w);
    m_size.setHeight(h);

    AbstractLine::setGRect(QRect(0,0,w,h));
}

void LinesDrawer::setLineTools(const QVector<LineTool> &linetools)
{
    m_lineTools = linetools;
}

LineTool LinesDrawer::lineTool(int typeId)
{
    for(int i=0;i<m_lineTools.size();i++){
        if(m_lineTools[i].typeId == typeId)
            return m_lineTools[i];
    }
    return LineTool();
}

void LinesDrawer::beginCommand(const Command &cmd)
{
    //当前命令是否完成
    if(m_cmd.isRunning()) return;

    m_cmd = cmd;

    //先过滤处理
    if(commandFilter(m_cmd))
        return endCommand();
}

void LinesDrawer::endCommand()
{
    //命令结束
    m_curLine = Q_NULLPTR;
    m_cmd.end();

    emit finishedCurCommand();
}

void LinesDrawer::penChanged(const QPen &pen)
{
    m_pen = pen;
}

bool LinesDrawer::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Paint){
        QPaintEvent *e = static_cast<QPaintEvent *>(event);
        this->paintLines(e);
    }else if(event->type() == QEvent::Resize){
        QResizeEvent *e = static_cast<QResizeEvent *>(event);
        this->resize(e->size());
    }else if(event->type() == QEvent::MouseButtonPress
             || event->type() == QEvent::MouseMove
             || event->type() == QEvent::MouseButtonRelease
             || event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(event->type() == QEvent::MouseButtonPress)
            return this->mousePress(e);
        else if(event->type() == QEvent::MouseMove)
            return  this->mouseMove(e);
        else if(event->type() == QEvent::MouseButtonRelease)
            return  this->mouseRelease(e);
        else if(event->type() == QEvent::MouseButtonDblClick)
            return this->mouseDbClick(e);
    }
    return QObject::eventFilter(obj,event);
}

void LinesDrawer::paintLines(QPaintEvent *)
{
    m_pixmap = QPixmap(m_size);
    m_pixmap.fill(Qt::transparent);

    QPainter pt(&m_pixmap);
    foreach (AbstractLine *line, m_lines) {
        line->paint(&pt);
    }
    pt.end();
}

bool LinesDrawer::mousePress(QMouseEvent *e)
{
    m_isPressed = true;
    m_pressPt = e->pos();

    if(e->button() == Qt::RightButton)
        return rightButtonPress(e);
    else if(e->button() == Qt::LeftButton)
        leftButtonPress(e);
    return false;
}

bool LinesDrawer::leftButtonPress(QMouseEvent *e)
{
    //恢复所有线状态
    if(!m_cmd.isRunning())
        resetAllLines();
    if(m_cmd.toolType == eNoneTool || m_cmd.toolType == eDelSingleLine){
        //选中激活某条线
        m_curLine = findActivedLine(m_pressPt);
        if(m_curLine)
            m_widget->update();

        return (m_curLine != Q_NULLPTR);
    }else {
        //创建新的线
        if(!m_cmd.running){
            m_curLine = LineFactory::instance()->createLine(m_cmd.toolType,m_pen);
            if(m_curLine){
                //特殊处理-文字，弹出属性窗口输入文字
                if(m_cmd.toolType == DrawLine::eTextNote){
                    if(!setLineProperty(m_curLine))
                    {
                        delete m_curLine;m_curLine = Q_NULLPTR;
                        endCommand();
                        return true;
                    }else{
                        m_curLine->msPressed(m_pressPt);
                        m_lines.append(m_curLine);
                        endCommand();
                        return true;
                    }
                }

                m_cmd.start();
                m_curLine->msPressed(m_pressPt);
                m_lines.append(m_curLine);
            }
        }
        return true;
    }
    return false;
}

bool LinesDrawer::rightButtonPress(QMouseEvent *)
{
    //恢复所有线状态
    if(!m_cmd.isRunning())
        resetAllLines();

    endCommand();//右键菜单直接取消
    AbstractLine *line = findActivedLine(m_pressPt); //选中激活某条线
    if(line){
        //显示右键菜单
        showMenu(line);
        return true;
    }
    return false;
}

bool LinesDrawer::mouseMove(QMouseEvent *e)
{
    //非按下移动鼠标时，表示选中标识点或线
    if(!m_isPressed)
        updateCursor(e->pos());

    //移动整条线 或者移动某点
    if(m_curLine){
        if(m_curLine->state() != AbstractLine::Normal){
            if(m_curLine->msMove(e->pos() - m_pressPt, m_isPressed)){
                m_widget->update();
                m_pressPt = e->pos();
                return true;
            }
        }
    }
    return false;
}

bool LinesDrawer::mouseRelease(QMouseEvent *e)
{
    m_isPressed = false;
    if(m_curLine){
        if(m_cmd.toolType == eDelSingleLine){
            m_lines.removeOne(m_curLine);
            delete m_curLine;
            endCommand();
            updateCursor(e->pos());
            m_widget->update();
            return  true;
        }
        //判断释放命令是否完成
        if(m_curLine->msRelease(e->pos()))
        {
            endCommand();
            m_widget->update();
            return true;
        }
    }

    return false;
}

//双击
bool LinesDrawer::mouseDbClick(QMouseEvent *e)
{
    m_isPressed = false;
    if(e->button() == Qt::RightButton)
        return false;

    if(m_cmd.toolType != eNoneTool){
        this->endCommand();
        return true;
    }
    if(m_curLine){
        setLineProperty(m_curLine);
    }
    return false;
}

AbstractLine *LinesDrawer::findActivedLine(const QPoint &pt)
{
    foreach (AbstractLine *line, m_lines) {
        if(line->actived(pt))
            return line;
    }
    return Q_NULLPTR;
}

bool LinesDrawer::commandFilter(const DrawLine::Command& cmd)
{
    switch (cmd.toolType) {
    case eDelAllLines: //删除所有Line
    {
        int button = QMessageBox::warning(NULL,QStringLiteral("删除所有线"),QStringLiteral("确定要删除所有画线吗？")
                                          ,QMessageBox::Yes|QMessageBox::No);
        if(button == QMessageBox::Yes)
            clearAllLines();
    }
        return true;
    case eScreenShot://截图
    {
        if(!m_screenShot)
            m_screenShot = new ScreenShot();
        m_screenShot->startScreenShot();
    }
        return true;
    default:
        break;
    }
    return false;
}

void LinesDrawer::resetAllLines()
{
    if(m_lines.size()>0){
        foreach (AbstractLine *line, m_lines)
            line->reset();
        m_widget->update();
    }
}

void LinesDrawer::clearAllLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
    m_widget->update();
}

void LinesDrawer::updateCursor(const QPoint& pt)
{
    foreach (AbstractLine *line, m_lines){
        AbstractLine::State state = line->pointAtLine(pt);
        if(state == AbstractLine::Revising){
            m_widget->setCursor(Qt::CrossCursor);
            return ;
        }else if(state == AbstractLine::Selecting){
            m_widget->setCursor(Qt::SizeAllCursor);
            return ;
        }
    }
    m_widget->unsetCursor();
}

bool LinesDrawer::setLineProperty(AbstractLine *line)
{
    //tilte
    DrawLine::LineTool lineTool = this->lineTool(line->typeId());
    if(lineTool.typeId == eNoneTool) return false;
    QString title = lineTool.text;
    switch (line->typeId()) {
    case DrawLine::eTextNote: //文字
    {
        TextNoteProperty property;
        property.text   = line->property(AbstractLine::Pt_Text).toString();
        property.color = line->color();
        property.fontSize = line->property(AbstractLine::Pt_FontSize).toInt();

        TextNoteProperty txtProperty;
        if(LineSettingDialog::getLineProperty(title,property,&txtProperty))
        {
            if(txtProperty.text.isEmpty()) return false;

            line->setColor(txtProperty.color);
            line->setProperty(AbstractLine::Pt_Text,txtProperty.text);
            line->setProperty(AbstractLine::Pt_FontSize,txtProperty.fontSize);

            m_widget->update();
            return true;
        }
    }
        break;
    case DrawLine::eUpArrow:     //文字
    case DrawLine::eDownArrow://箭头颜色
    {
        QColor color;
        if(LineSettingDialog::getLineProperty(title,line->color(),&color)){
            line->setColor(color);
            m_widget->update();
            return true;
        }
    }
        break;
    default://线属性
    {
        QPen pen;
        if(LineSettingDialog::getLineProperty(title,line->pen(),&pen))
        {
            line->setPen(pen);
            m_widget->update();
            return true;
        }
    }
        break;
    }
    return false;
}

bool LinesDrawer::showMenu(AbstractLine *line)
{
    return true;
}





