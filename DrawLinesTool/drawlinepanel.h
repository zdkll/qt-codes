#ifndef DRAWLINEPANEL_H
#define DRAWLINEPANEL_H

#include <unordered_map>
#include <QDialog>
#include <QPen>
#include "drawlinepublic.h"

class QButtonGroup;
class QToolButton;
class QComboBox;
class ColorComboBox;
class PenStyleToolButton;
//画线工具控制面板
//负责生成命令，传给主界面->画线管理->具体操作执行
class DrawLinePanel : public QDialog
{
    Q_OBJECT
public:
    DrawLinePanel(QWidget *parent = 0);
    ~DrawLinePanel(){}

    DrawLine::Command command()const {return m_command;}
    void    setPen(const QPen&pen);
    QPen pen()const {return m_pen;}

    DrawLine::LineTool lineTool(DrawLine::LineToolType typeId);
    QVector<DrawLine::LineTool> lineTools()const{return m_lineTools;}

public slots:
    //完成当前命令
    void finishedCommand();

signals:
    void startCommand(const DrawLine::Command&);//发送命令
    void penChanged(const QPen& pen);

protected slots:
    void buttonClicked(int);

    void colorChanged(const QColor& color);
    void penStyleChanged(Qt::PenStyle);
    void penWidthChanged(int);

private:
    //加载工具按钮信息
    bool loadInfos();
    //创建面板
    void createPanel();

    //子面板
    QWidget *createLineToolsGroup(const QString& group);
    QWidget *createLineStylePanel();
    QToolButton  *createBtn(const DrawLine::LineTool& LineTool,const QString& iconPath);
    QComboBox *createLineWidthCbx();

    QButtonGroup               *m_btnGroup;
    int                                     m_currentIdx;
    QVector<DrawLine::LineTool>        m_lineTools;

    DrawLine::Command     m_command; //当前命令
    QPen                               m_pen;   //当前画笔样式

    ColorComboBox      *m_colorCbx;
    PenStyleToolButton *m_lineStyleToolBtn,*m_lineWidthToolBtn;
};

#endif // DRAWLINEPANEL_H
