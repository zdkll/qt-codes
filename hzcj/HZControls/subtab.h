/********************************************************************
created: 2018/04/13
author: min.cheng dk.zhang
purpose:导航条，负责管理管理Buttons
*********************************************************************/
#ifndef CSUBTAB_H
#define CSUBTAB_H

#include <QMap>

#include "hzcontrols_global.h"
#include "basewidget.h"

class QButtonGroup;
class QPushButton;
class  QHBoxLayout;
class HZLine;
class SwitchPanel;
class HZCONTROLSSHARED_EXPORT CSubTab : public BaseWidget
{
    Q_OBJECT
public:
    //右侧扩展栏类型
    enum StretchLastType
    {
        None_Strecth  = 0,
        Empty_Strecth= 1,
        Button_Strecth= 2,
    };
    //根导航条还是子导航条
    enum CSubTabType
    {
        Root_Tab =  0,
        Child_Tab = 1
    };
    Q_ENUM(CSubTabType)
    Q_PROPERTY(CSubTabType csubTabType READ csubTabType WRITE setCSubTabType NOTIFY csubTypeChanged)

    CSubTab(QWidget *parent = 0,QString instName="");
    ~CSubTab();

    void setCSubTabType(CSubTabType tabType) ;
    CSubTabType csubTabType() const {return m_tabType;}

    //切换面板是否显示
    void setShowSwitchPanel(const bool &show);

    void setPacing(const int &space);

    void setContentsMargins(int left, int top, int right, int bottom);

    //管理Button
    void addLine(int w,int h);
    void addButton(const QString & name, const int  &id= -1,HZLine* pLine = nullptr);
    void addButtons(const QStringList &textList);

    void setButtons(const QStringList &textList);

    //添加右侧填充类型
    void setStretchLastType(StretchLastType stretchType);

    void setCurrentButton(const int &id);

    void clear();

    int currentButton()const {return m_currentBtnIdx;}

    QPushButton *getButton(int nIndex);

    HZLine* getLine(int nIndex);

    void setAllUnCheck();

    void setExclusiveEx(bool bFlg);

public slots:

signals:
    void csubTypeChanged(CSubTabType);
    //当前Button改变
    void currentButtonChanged(int id);

private slots:
    void buttonToggled(int id,bool checked);
    void slotsButtonClicked(int id);

    void switchBack();
    void switchForward();

private:
    void  createWg();

private:
    QVector<HZLine*> m_lineVec;
    QMap<int , HZLine*> m_lineMap;
    QMap<int , QPushButton*> m_buttonMap;
    QButtonGroup   * m_pButtonGroup;
    QHBoxLayout      *topHLayout;
    QHBoxLayout     * m_pMainLayout;
    CSubTabType       m_tabType;

    int                       m_currentBtnIdx;
    StretchLastType  m_stretchLastType;
};




#endif // CSUBTAB_H
