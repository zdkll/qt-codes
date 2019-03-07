#include "seachstockwidget.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include "searchstockview.h"



SeachStockWidget::SeachStockWidget(QWidget *parent):
    BaseWidget(parent)
{
    this->setFixedSize(230, 220);
    createWg();
//    this->setFocus();
}


void SeachStockWidget::createWg()
{
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setSpacing(1);
    m_pMainLayout->setMargin(1);

    m_searchView = new SearchStockView(this);

    m_searchView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_lineEdit = new QLineEdit(this);

//    m_lineEdit->setFocusPolicy(Qt::StrongFocus);
    m_lineEdit->setFocus();


    m_lineEdit->setFixedSize(230, 26);
    QPushButton *pSearchBtn = new QPushButton(this);
    pSearchBtn->setCursor(Qt::PointingHandCursor);
    pSearchBtn->setFixedSize(14, 14);
    pSearchBtn->setToolTip(QStringLiteral("搜索"));
    m_closeBtn = new QPushButton(this);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    m_closeBtn->setFixedSize(12, 12);
    m_closeBtn->setToolTip(QStringLiteral("删除"));
    pSearchBtn->setObjectName("search");
    m_closeBtn->setObjectName("close");
    QMargins margins = m_lineEdit->textMargins();
    m_lineEdit->setTextMargins(6+pSearchBtn->width(), margins.top(), 8+m_closeBtn->width(), margins.bottom());

    m_lineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

    QHBoxLayout *pSearchLayout = new QHBoxLayout;
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(6, 0, 8, 0);
    pSearchLayout->addWidget(pSearchBtn);
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(m_closeBtn);

    m_lineEdit->setLayout(pSearchLayout);


    m_closeBtn->hide();

    connect(m_lineEdit, &QLineEdit::textChanged, this, &SeachStockWidget::slotInput);
    connect(m_closeBtn, &QPushButton::clicked , this, &SeachStockWidget::onCloseBtnClicked);
    connect(m_searchView, &SearchStockView::rowClicked, this, &SeachStockWidget::onRowClicked);

    m_pWidget = new QWidget(this);
    m_pWidget->setObjectName("body");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    QLabel *label1 = new QLabel("输入代码/简拼/名称");
    QLabel *label2 = new QLabel("双击或者按回车(Enter)添加");

    layout->addSpacing(68);
    layout->addWidget(label1, 0, Qt::AlignCenter);
    layout->addSpacing(14);
    layout->addWidget(label2, 0, Qt::AlignCenter);
    layout->addStretch();

    m_pWidget->setLayout(layout);


    m_pMainLayout->addWidget(m_lineEdit);

    m_pMainLayout->addWidget(m_pWidget);

    m_pMainLayout->addWidget(m_searchView);
    m_searchView->hide();
}

void SeachStockWidget::searchFromServer(const QString &searchText)
{
    HZData::ReqQueryCode codeQuery = { HZData::SearchType::FuzzyQuery, searchText.toLower().toStdString()};
    OnCodeInfoArrived codeSearchFunc = std::bind(&SeachStockWidget::seachrArrived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    DataController::instance()->requestCodeInfo(codeQuery ,codeSearchFunc);
}

void SeachStockWidget::seachrArrived(ReqId id, QVector<HZData::CodeInfo> codes, ErrCode errCode)
{
    Q_UNUSED(id)
    if(0 == errCode)
    {

        qSort(codes.begin(), codes.end(), [=](const HZData::CodeInfo &first,const HZData::CodeInfo &second){
            return first.code < second.code;
        });
        updateData(codes);
    }


}

void SeachStockWidget::updateData(const QVector<HZData::CodeInfo> &codes)
{

    QList<SearchStockData> lists;
    foreach (HZData::CodeInfo codeinfo, codes) {

        SearchStockData stock;
        stock.strCode = QString::fromStdString(codeinfo.code);
        stock.strName = QString::fromStdString(codeinfo.name);

        lists.push_back(stock);
    }
    m_searchView->upDateData(lists);

    m_searchView->selectRow(0);
//    m_searchView->setFocus();

}

void SeachStockWidget::clickUp()
{
    int curRow = qMax(m_searchView->currentIndex().row() - 1, (0));
    m_searchView->selectRow(curRow);
}

void SeachStockWidget::clickDown()
{
    int curRow = qMin(m_searchView->currentIndex().row() + 1, m_searchView->model()->rowCount());
    m_searchView->selectRow(curRow);

}

void SeachStockWidget::clickReturn()
{
    if(m_searchView->model()->rowCount() <= 0)
        return;
    QModelIndex index = m_searchView->currentIndex();

    QString strCode = index.data(CODEROLE).toString();

    if(strCode.isEmpty())
        return;
    emit rowClicked(strCode);

    m_lineEdit->clear();
}

void SeachStockWidget::keyPressEvent(QKeyEvent *e)
{
    BaseWidget::keyPressEvent(e);

    switch(e->key())
    {
        case Qt::Key_Up:
            clickUp();
            break;
        case Qt::Key_Down:
            clickDown();
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            clickReturn();
            break;
        default:
            QWidget::keyPressEvent(e);
    }
}

void SeachStockWidget::slotInput(const QString &strText)
{
    if(strText.isEmpty()){
        m_closeBtn->hide();
        m_pWidget->show();
        m_searchView->hide();
        ((SearchStockModel*)m_searchView->model())->clearData();
    }
    else{
        m_closeBtn->show();
        m_pWidget->hide();
        m_searchView->show();
        searchFromServer(strText);
    }

}

void SeachStockWidget::onCloseBtnClicked()
{
    m_lineEdit->clear();
    m_lineEdit->setFocus();
}

void SeachStockWidget::onRowClicked(const QString &strCode)
{
    emit rowClicked(strCode);
    m_lineEdit->clear();
}
