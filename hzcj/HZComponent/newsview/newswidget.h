/********************************************************************
created:2018.9
author: juntang.xie
purpose:派生自BaseWidget 新闻公告外部容器
*********************************************************************/
#ifndef NEWSWIDGET_H
#define NEWSWIDGET_H
#include "basewidget.h"
#include "newsview.h"

class  NewsWidget:public BaseWidget
{
    Q_OBJECT
public:
    explicit NewsWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置获取数据类型
     * @param[in] apdater 子类对象
     * @retval
     */
    void setAdapter(HZInfoAdapterInterface *apdater,const QString& titleText);
    /**
     * @brief   设置股票代码
     * @param[in] codeVec 股票代码列表
     * @retval
     */
    void setCode(const QVector<QString> &codeVec);

    //对象激活
    void afterActive();

    //取消激活
    void afterDeactive();
private:
    void createWg();
private slots:
    void onRowClicked(const QString& id);
private:
    QString m_titleText;
    QVector<QString> m_objList;
    NewsView *m_newsView;
    HZInfoAdapterInterface *m_apdater;
};

#endif // NEWSWIDGET_H
