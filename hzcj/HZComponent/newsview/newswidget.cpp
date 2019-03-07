#include "newswidget.h"
#include <QVBoxLayout>

#include "hzinfomationdlg.h"
#include "newsmodel.h"

NewsWidget::NewsWidget(QWidget *parent):
    BaseWidget(parent),
    m_newsView(new NewsView)
{
    connect(m_newsView,SIGNAL(signalClickItem(const QString&)),this,SLOT(onRowClicked(const QString&)));
    connect(m_newsView, &NewsView::signalRequest,[=]{
           m_apdater->requestList();
    });
    createWg();
}

void NewsWidget::setAdapter(HZInfoAdapterInterface *apdater,const QString& titleText)
{
    m_titleText = titleText;
    m_apdater = apdater;
    apdater->setVisitor(m_newsView);
}

void NewsWidget::setCode(const QVector<QString> &codeVec)
{
    m_apdater->setCode(codeVec);
}

void NewsWidget::afterActive()
{
    m_apdater->setNextpage(0);
    m_apdater->requestList();
}

void NewsWidget::afterDeactive()
{
    m_newsView->clearAll();
}

void NewsWidget::createWg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_newsView);
}

void NewsWidget::onRowClicked(const QString &id)
{
    STInfoData tmpDatas;
    tmpDatas.datas = m_newsView->m_newsModel->getRecords();
    m_apdater->setFirstDatas(tmpDatas);
    m_apdater->setItemId(id);

    int type;
    if(m_titleText == QStringLiteral("个股新闻")) {
        type = HZInfomationDlg::News_Info;
    } else if(m_titleText == QStringLiteral("个股公告")) {
        type = HZInfomationDlg::Anncs_Info;
    } else if(m_titleText == QStringLiteral("个股研报")) {
        type = HZInfomationDlg::Research_Report;
    } else if(m_titleText == QStringLiteral("板块新闻")) {
        type = HZInfomationDlg::Plate_News;
    } else if(m_titleText == QStringLiteral("指数新闻")) {
        type = HZInfomationDlg::Index_News;
    }

//    HZInfomationDlg dlg(type, m_apdater);
//    dlg.exec();
    gInfoDlg->setVisiter(type,m_apdater);
}
