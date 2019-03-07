/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef HZINFOMATIONDLG_H
#define HZINFOMATIONDLG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "vlistmaster.h"
#include "vlistdef.h"
#include "hzinfoadapterinterface.h"
#include "hzinfoadapter.h"
#include "infomation_export.h"


class INFOMATION_EXPORT HZInfomationDlg : public QDialog
{
    Q_OBJECT
public:
    enum {
        Host_News=0,//热门资讯
        Flash_News,//7*24
        News_Info,//个股新闻
        Anncs_Info,//个股公告
        Research_Report,//研报
        Major_Meeting,//重大会议
        Financial_Report,//财报发布
        Plate_News, //板块新闻
        Index_News //指数新闻
    };

    static HZInfomationDlg *instance();
    ~HZInfomationDlg();

    ///
    /// \brief setVisiter
    /// \param type
    /// \param adapter
    ///
    void setVisiter(int type, const HZInfoAdapterInterface *adapter);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //void paintEvent(QPaintEvent *e);
//    bool eventFilter(QObject *, QEvent *);
    void hideEvent(QHideEvent *event);

    void updateList(const STInfoData& lst);
    void updateContent(const STDetailDescript& content);

private:
    HZInfoAdapterInterface *mSubject{nullptr};
    QPushButton *m_closeBtn;
    QLabel *m_title;
    QFrame *m_titleFrame;
    HZVListMaster *m_master;
    bool m_dragWindow;
    QPoint m_mousePoint;
    QString m_tmpPath;
    QUrl mUrl;

    HZInfomationDlg(QWidget *parent = 0);
    void init();
};

///
/// \brief The HZInfomationDlgEx class
///
class HZInfomationDlgEx : public QDialog
{
    Q_OBJECT
public:
    static HZInfomationDlgEx *instance();
    ~HZInfomationDlgEx();
    void setPageInfo(const QString &title, const QString &url);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    //void paintEvent(QPaintEvent *e);
    void hideEvent(QHideEvent *event);

private:
    HZInfomationDlgEx(QWidget *parent = 0);
    QPushButton *m_closeBtn;
    QLabel *m_title;
    QFrame *m_titleFrame;
    bool m_dragWindow;
    QPoint m_mousePoint;
    HZInfoWebPageMarster *mWebView;

    void initSingleWebPage();
};


#define gInfoDlg HZInfomationDlg::instance()
#define gInfoDlgEx HZInfomationDlgEx::instance()
#endif // HZINFOMATIONDLG_H
