#ifndef BKLISTBOTTOMNAVIGATOR_H
#define BKLISTBOTTOMNAVIGATOR_H
/********************************************************************
created:2018.10
author: juntang.xie
purpose: 板块底部导航
*********************************************************************/
#include "basewidget.h"
#include "gwdefine.h"

DECLARE_MAP(QString, QString, INDUSTRY)

#define ITEMWIDTH 88
#define ITEMHEIGHT 23



class QTableWidget;
class QButtonGroup;
class QToolButton;
class MyToolButton;
class DrowDownListWidget;
class BkListBottomNavigator : public BaseWidget
{
     Q_OBJECT
public:
    explicit BkListBottomNavigator(QWidget *parent = 0,QString instName="");

    //激活对象
    virtual void afterActive();

    //根据设置板块id 跳转到对应的板块页面
    void setFilterFlag(int nFilterFlg, bool nFlg = false);

signals:
    /**
     * @brief 板块分类导航切换信号
     * @param[in] filterFlag 排行分类标记
     */
    void signalBkCategoryNavChange(int filterFlag);

    /**
     * @brief 板块切换信号
     * @param[in] filterFlag 排行分类标记
     * @param[in] bkCode 板块code
     */
    void signalBkNavChange(int filterFlag,const QString& bkCode);

public slots:

private:
    void createWg();
    void initBtn();
    void initConnect();
    void initMenu();
    void setCheckable();
    void initTable();

    void showHyMenu();
    void showGnMenu();
    void showAreaMenu();

    void setHyBtnChecked();
    void setGnBtnChecked();
    void setDyBtnChecked();

private slots:
    void onbuttonGroupClicked(int nID);
    void onHyBtnClicked();
    void onGnBtnClicked();
    void onAreaBtnClicked();
    void onBtnMoreClicked();
    void onBtnMore1Clicked();
    void onBtnMore2Clicked();

    void onHycellClicked(QString strName,QString strCode);
    void onGncellClicked(QString strName,QString strCode);
    void onAreacellClicked(QString strName,QString strCode);

private:
    DrowDownListWidget *m_hyDownListWidget;
    DrowDownListWidget *m_gnDownListWidget;
    DrowDownListWidget *m_areaDownListWidget;

    MyToolButton *m_toolBtnMore;    //行业板块右侧按钮
    MyToolButton *m_toolBtnMore1;   //概念板块右侧按钮
    MyToolButton *m_toolBtnMore2;   //地域板块右侧按钮

    QToolButton *m_hyBtn;    //行业板块按钮
    QToolButton *m_gnBtn;    //概念板块按钮
    QToolButton *m_areaBtn;  //地域板块按钮
    QToolButton *m_allBkBtn;    //全部板块按钮



    QWidget *m_hyWidget;    //行业分类背景窗口
    QWidget *m_gnWidget;    //概念分类背景窗口
    QWidget *m_areaWidget;  //地区分类背景窗口
    QButtonGroup *m_buttonGroup;
    int m_nfilterFlag;
    QString m_strCode;
    QString m_strHyCode;
    QString m_strGnCode;
    QString m_strDyCode;

    QString m_strHyText;
    QString m_strGnText;
    QString m_strDyText;
};

#endif // BKLISTBOTTOMNAVIGATOR_H
