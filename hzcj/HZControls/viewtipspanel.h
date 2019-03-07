#ifndef VIEWTIPSPANEL_H
#define VIEWTIPSPANEL_H
/********************************************************************
created:
author: min.cheng
purpose: 可用于资讯，自选等需要在数据为空或其他状态下显示不同提示图标的场景，支持外部配置QSS样式
         传入，内部只做布局（固定布局）；支持图标点击返回对应信号。


    使用例子：创建今日机会页面

    //配置 无数据和数据错误的样式
    QVector<TipsItem> tipList;
    TipsItem tipEmpty;
    tipEmpty.btObjName = "dataEmptyBt";
    tipEmpty.descLabelName = "dataEmptyLable";
    tipEmpty.descText = QStringLiteral("当前暂无数据");
    tipEmpty.type = ChanceDataIsEmpty;  //这个类型自己外部随意定
    tipList.push_back(tipEmpty);
    TipsItem tipErr;
    tipErr.btObjName = "dataErrBt";
    tipErr.descLabelName = "dataErrLable";
    tipErr.descText = QStringLiteral("操作失败，稍后重试");
    tipErr.type = ChanceDataIsErr; //这个类型自己外部随意定
    tipList.push_back(tipErr);

    m_pInfoTipsPanel =new ViewTipsPanel(this);
    //设置外部QSS样式列表
    m_pInfoTipsPanel->setTipItems(tipList);
    //将今日机会窗口放入其内部堆栈窗口中统一管理
    m_pInfoTipsPanel->setNormalWnd(createListView(),ChanceDataNoraml);
    //设置好以后创建改提示窗口
    m_pInfoTipsPanel->createWg();
    //获取提示窗口的内部堆栈窗口用于外部布局
    pLayout->addWidget(m_pInfoTipsPanel);

    使用例子：切换页面例子
    if(ret.code != 0) //数据错误
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsErr);
         return;
    }else if(data.todayChance.isEmpty()) //数据为空
    {
        m_pInfoTipsPanel->switchType(ChanceDataIsEmpty);
        return;
    }else //正常是显示
    {
        m_pInfoTipsPanel->switchType(ChanceDataNoraml);
    }

*********************************************************************/

#include "hzcontrols_global.h"
#include "basewidget.h"
#include <QMap>
class QStackedWidget;
class QButtonGroup;
class QPushButton;

struct TipsItem
{
   int type;
   QString btObjName;
   QString descLabelName;
   QString descText;
};

class HZCONTROLSSHARED_EXPORT  ViewTipsPanel: public BaseWidget
{
    Q_OBJECT
public:
    ViewTipsPanel(QWidget *parent = 0,QString instName="");

    /**
     * @brief 调用createWg前设置，表示需要几种状态
     * @param[in] list 样式类型和文本以及QSS配置
     * @param[in] width图片样式宽度
     * @param[in] hight图片样式高度
     */
    void setTipItems(const QVector<TipsItem>& list,int width = 84,int hight = 84);

    /**
     * @brief 用于设置正常页面的窗口，可以辅助根据不同状态来切换
     * @param[in] pWnd 正常显示时的窗口
     * @param[in] type 正常窗口类型
     */
    void setNormalWnd(QWidget* pWnd,int type);

    /**
     * @brief 设置过类型和正常窗口时创建
     */
    void createWg();

    /**
     * @brief 返回内部窗口，用于外部布局
     */
    QWidget* getStackWnd();

    /**
     * @brief 切换显示的页面，包括切换到正常窗口显示
     * @param[in] type 窗口类型
     */
    void switchType(int type);

    int getCurrType();
signals:
    /**
     * @brief 获取不同类型窗口返回的单击事件，不包括正常窗口的点击
     */
    void clickItem(int type);
public slots:
    void slotsButtonClicked(int type);
private:
    QButtonGroup   * m_pButtonGroup = nullptr;
    QVector<TipsItem> m_itemList;
    QMap<int,QWidget*> m_typeIndexMap;
    QStackedWidget* m_statck = nullptr;
    QWidget* m_pWnd = nullptr;
    int m_nWidth;
    int m_nHight;
    int m_currType=-1;
};

#endif // VIEWTIPSPANEL_H
