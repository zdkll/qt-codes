#include "drawlinepanel.h"

#define    LineTools_Group     "LineTools"
#define    AssisTools_Group   "AssisTools"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>
#include <QComboBox>
#include <QFile>

#include <QDebug>

#include "controls.h"

using namespace DrawLine;

DrawLinePanel::DrawLinePanel(QWidget *parent)
    :QDialog(parent),m_pen(Qt::white)
    ,m_currentIdx(-1)
{
    createPanel();
    m_pen.setWidth(1);
    m_pen.setStyle(Qt::SolidLine);
    m_pen.setColor(Qt::white);

    this->setPen(m_pen);
}

void DrawLinePanel::setPen(const QPen &pen)
{
    m_pen = pen;
    m_colorCbx->setColor(pen.color());
    m_lineStyleToolBtn->setPenWidth(pen.width());
    m_lineWidthToolBtn->setPenStyle(pen.style());
}

LineTool DrawLinePanel::lineTool(LineToolType typeId)
{
    for(int i=0;i<m_lineTools.size();i++){
        if(m_lineTools[i].typeId == typeId)
            return m_lineTools[i];
    }
    return LineTool();
}

void DrawLinePanel::finishedCommand()
{
    if(m_currentIdx>=0){
        m_btnGroup->button(m_currentIdx)->setChecked(false);
    }
    //结束当前命令
    m_command.end();
    m_currentIdx = -1;
}

void DrawLinePanel::buttonClicked(int id)
{
    if(m_currentIdx>=0){
        m_btnGroup->button(m_currentIdx)->setChecked(false);
    }
    qDebug()<<"click id:"<<id;
    m_currentIdx = id;
    if(m_currentIdx<0 || m_currentIdx>=m_lineTools.size()) return;
    //画线工具命令

    m_command.toolType = m_lineTools[id].typeId;
    qDebug()<<m_lineTools[id].typeId<<m_lineTools[id].text;
    emit startCommand(m_command);
}

void DrawLinePanel::colorChanged(const QColor &color)
{
    m_pen.setColor(color);
    emit penChanged(m_pen);
}

void DrawLinePanel::penStyleChanged(Qt::PenStyle penStyle)
{
    m_pen.setStyle(penStyle);
    emit penChanged(m_pen);
}

void DrawLinePanel::penWidthChanged(int width)
{
    m_pen.setWidth(width);
    emit penChanged(m_pen);
}

bool DrawLinePanel::loadInfos()
{
    QFile file(":/resource/ToolButtons.json");
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QJsonDocument doc =QJsonDocument::fromJson(file.readAll());
    if(doc.isNull()) return false;
    QJsonObject rootObj = doc.object();

    QJsonArray toolsArray =  rootObj.value(LineTools_Group).toArray();

    for(int i=0;i<toolsArray.size();i++){
        LineTool lineTool;
        lineTool.typeId     = (DrawLine::LineToolType)toolsArray[i].toObject().value("id").toInt();
        lineTool.text  = toolsArray[i].toObject().value("text").toString();
        lineTool.icon = toolsArray[i].toObject().value("icon").toString();
        lineTool.group = LineTools_Group;
        m_lineTools.append(lineTool);
    }

    toolsArray =  rootObj.value(AssisTools_Group).toArray();
    for(int i=0;i<toolsArray.size();i++){
        LineTool lineTool;
        lineTool.typeId     = (DrawLine::LineToolType)toolsArray[i].toObject().value("id").toInt();
        lineTool.text  = toolsArray[i].toObject().value("text").toString();
        lineTool.icon = toolsArray[i].toObject().value("icon").toString();
        lineTool.group = AssisTools_Group;
        m_lineTools.append(lineTool);
    }
    qDebug()<<m_lineTools.size();
    return true;
}

void DrawLinePanel::createPanel()
{
    if(!loadInfos()) return;

    m_btnGroup = new QButtonGroup(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    //1 画线类型面板
    mainLayout->addWidget(createLineToolsGroup(LineTools_Group));

    //2 辅助工具
    QLabel *assisLabel = new QLabel(QStringLiteral("辅助工具"),this);
    mainLayout->addWidget(assisLabel,0,Qt::AlignCenter);

    //3 辅助工具框
    mainLayout->addWidget(createLineToolsGroup(AssisTools_Group));
    m_btnGroup->setExclusive(false);

    //4 颜色，线型，粗细
    mainLayout->addWidget(createLineStylePanel());
    mainLayout->stretch(1);

    this->setLayout(mainLayout);

    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));
}

QWidget *DrawLinePanel::createLineToolsGroup(const QString &group)
{
    QString iconPath = ":/resource/icons";
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(2);
    gridLayout->setContentsMargins(0,0,0,0);
    int row = 0,col = 0;
    int idx = 0;
    for(int i=0;i<m_lineTools.size();i++){
        if(m_lineTools[i].group == group){
            row = idx/4;
            col  =  idx%4;
            QToolButton *toolBtn = createBtn(m_lineTools[i],iconPath);
            gridLayout->addWidget(toolBtn,row,col);
            m_btnGroup->addButton(toolBtn,i);
            idx++;
        }
    }

    QWidget *wg = new QWidget;
    wg->setLayout(gridLayout);
    return wg;
}

QWidget *DrawLinePanel::createLineStylePanel()
{
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(1,1);
    gridLayout->setHorizontalSpacing(4);
    //颜色
    QLabel *colorLabel = new QLabel(QStringLiteral("颜色"));
    gridLayout->addWidget(colorLabel,0,0);
    m_colorCbx = new ColorComboBox();
    gridLayout->addWidget(m_colorCbx,0,1);
    connect(m_colorCbx,SIGNAL(colorChanged(QColor)),this,SLOT(colorChanged(QColor)));

    //线型
    QLabel *lineStyleLabel = new QLabel(QStringLiteral("线型"));
    gridLayout->addWidget(lineStyleLabel,1,0);
    m_lineStyleToolBtn = new PenStyleToolButton(PenStyleToolButton::PenStyle);
    gridLayout->addWidget(m_lineStyleToolBtn,1,1);
    connect(m_lineStyleToolBtn,SIGNAL(penStyleChanged(Qt::PenStyle)),this,SLOT(penStyleChanged(Qt::PenStyle)));

    //粗细
    QLabel *lineWidthLabel = new QLabel(QStringLiteral("粗细"));
    gridLayout->addWidget(lineWidthLabel,2,0);
    m_lineWidthToolBtn = new PenStyleToolButton(PenStyleToolButton::PenWidth);
    gridLayout->addWidget(m_lineWidthToolBtn,2,1);
    connect(m_lineWidthToolBtn,SIGNAL(penWidthChanged(int)),this,SLOT(penWidthChanged(int)));

    QWidget *wg = new QWidget;
    wg->setLayout(gridLayout);
    return wg;
}

QToolButton *DrawLinePanel::createBtn(const LineTool &LineTool,const QString& iconPath)
{
    QToolButton *toolBtn = new QToolButton;
    toolBtn->setCheckable(true);
    toolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBtn->setToolTip(LineTool.text);
    toolBtn->setText(LineTool.text);

    toolBtn->setIcon(QIcon(iconPath+"/"+LineTool.icon));
    return toolBtn;
}
