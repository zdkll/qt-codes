/********************************************************************
created:2018.12
author: jdxchina
purpose:
*********************************************************************/
#ifndef WIDGET_H
#define WIDGET_H

#include <QFrame>
#include <QtWidgets>
#include <QVector>
#include <QList>
#include <QWidgetAction>
#include <QMenu>
#include <QPixmap>
#include "infopage_export.h"
#include "hzcalandarctrl.h"

///
/// \brief The CalandarBtn class
/// 用于弹出自定义控件的按钮
///
class HZINFOPAGE_EXPORT CustomCldBtn : public QPushButton
{
    Q_OBJECT
public:
    CustomCldBtn(const QString &text, bool forward = true, const QString &indicate=":/skin/default/img/infopage/menu_down.png",
                 const QString &icon=":/skin/default/img/infopage/titleicon.png", QWidget *parent=nullptr)
        :QPushButton(parent), m_text(text), m_iconPath(icon), m_indicatePath(indicate),m_forward(forward)
    {
        setObjectName("CustomCldBtn");
    }

    ~CustomCldBtn(){}

    /**
     * @brief setStrText 设置按钮显示文本
     * @param text
     */
    void setStrText(const QString &text);


protected:
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

signals:
    void signalClicked(const QPoint &pos);

public slots:
    void slotMenuVisible(bool visible);

private:
    bool m_forward{true};
    QPoint m_hoverPos;
    QString m_text;
    QString m_iconPath;
    QString m_indicatePath;
    bool m_isHover{false};
};


///
/// \brief The CalandarBtn class
/// 水平日历方向按钮
///
class HZINFOPAGE_EXPORT CalandarBtn : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool forward READ forward WRITE setForward)
public:
    CalandarBtn(bool forward, /*const QString& propertyName,*/ const QString &text="", QWidget *parent=nullptr)
        :QPushButton(text, parent),m_forward(forward){
        //setProperty(propertyName.toUtf8(), forward);
    }

    ~CalandarBtn(){}

    bool forward(){ return m_forward;}
    void setForward(bool frw) { m_forward = frw;}

protected:
    void paintEvent(QPaintEvent *e);

private:
    bool m_forward;//0 left
};

///
/// \brief The CalandarBarFrame class
/// <!组件1>
///
class HZINFOPAGE_EXPORT CalandarBarFrame: public QFrame
{
    Q_OBJECT
public:
    CalandarBarFrame(QWidget *parent=nullptr);

    /**
     * @brief initDate 初始化日期
     * @param dates
     */
    void initDate(const QVector<quint64> &dates);

    /**
     * @brief timeScroll 时间滚动
     * @param forward True 表示日期加一天
     */
    void timeScroll(bool forward);

    /**
     * @brief setStartDay 设置从指定日期开始滚动，默认为今天，且选中
     * @param day yyyyMMdd
     * @param select 是否选中
     */
    void setStartDay(const QString &day, bool select = true);

    /**
     * @brief currentDate 获取当前选中日期
     * @return 查找不到返回空
     */
    QString currentDate() const;

    /**
     * @brief weekOf
     * @param time
     * @return
     */
    static QString weekOf(quint32 time);

signals:
    void signalCalandarClicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);

private:
    struct CalandarData
    {
        quint32 time;   //当天日期
        int index;      //当前位置在容器里的位置
        int start;      //所有时间节点里的起始时间
        int end;        //所有时间节点里的结束时间
    };
    struct CalandarDataDetail
    {
        quint32 date;   //time_t
        bool flags;     //是否标记
    };

    QList<CalandarDataDetail> m_dates; //时间列表
    CalandarData *m_calandarInfo;     //日志信息结构
    QPoint m_pressPos;               //按下时的位置
    int m_visualItems;               //可视化项数
    bool m_isPressed;
    int m_paintForward{true};//true right->left false left->right
    QPixmap m_pixmap;

    void test();
    void resetFlags(int index, bool statu=true);
    bool isToDay(int index) const;
};


///
/// \brief The CustomCalandarBtnMarster class
/// \note 一个按钮和按钮被点击后弹出的日历框组合的封装组件
/// <!组件2>
///
class HZINFOPAGE_EXPORT CustomCalandarBtnMarster : public QFrame
{
    Q_OBJECT
public:
    enum{Hide_Today, Show_ToDay};

    /**
     * @brief CustomCalandarBtnMarster
     * @param forward true-left 表示左右按钮的动态属性值（该值须被传入，默认为true，表示左侧的按钮的动态属性值）
     * @param hidetoday true 表示隐藏 ‘转到今天’的按钮 ，false表示显示
     * @param parent
     */
    CustomCalandarBtnMarster(bool forward=true, bool hidetoday=false,QWidget *parent=nullptr);

    /**
     * @brief setStrText 设置按钮文本
     * @param text
     */
    void setStrText(const QString &text);

    /**
     * @brief addValidTimes 设置有效数据
     * @param times
     */
    void addValidTimes(const QVector<quint64> &times);

signals:
    /*某个日历被触发时向外抛出的信号*/
    void signalCalandarBarClicked(quint64 time);

protected:
    bool eventFilter(QObject *watched, QEvent *event);


private:
    bool m_forward{true};
    QWidgetAction *m_menuAct;
    HZCalanderCtrl *m_calandarCtrl;
    QMenu *m_cldMenu;
    CustomCldBtn *m_calandarBtn;
};


///
/// \brief The CalandarBarMaster class
/// \note 一个水平日历滚动栏和（右侧）一个按钮和该按钮被点击后弹出的日历框组合的封装组件
/// <!组件3>
///
class HZINFOPAGE_EXPORT CalandarBarMaster : public QFrame
{
    Q_OBJECT
public:
    explicit CalandarBarMaster(bool hidetoday=false,QWidget *parent = 0);
    ~CalandarBarMaster();

    /**
     * @brief getCurrentDate 获取当前时间
     * @return
     */
    QString getCurrentDate() const;

    /**
     * @brief setCurrentDate
     */
    void setCurrentDate(const quint64 time);

    /**
     * @brief setValidTimes 设置当前有效时间
     * @param times 时间戳列表
     * （日历弹出框和日历滚动栏上的时间是同步的）
     */
    void setValidTimes(const QVector<quint64> &times);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
    /**
     * @brief signalCalandarBarClicked 某个日历被触发时向外抛出的信号
     */
    void signalCalandarBarClicked();

private:
    CalandarBarFrame *m_calandarBar;
    HZCalanderCtrl *m_calandarCtrl;
    QPushButton *m_leftBtn, *m_rightBtn;
    CustomCldBtn *m_calandarBtn;
    QWidgetAction *m_menuAct;
    QMenu *m_cldMenu{nullptr};
};
#endif // WIDGET_H
