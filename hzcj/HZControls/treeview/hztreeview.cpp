#include "HZTreeView.h"
#include <QScrollBar>
#include <QHeaderView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QDebug>
#include "treenode.h"
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>




HZTreeView::HZTreeView(HZItemModel * model,int nFreeze,QWidget * parent):
    QTreeView(parent),
    m_nFreeezeField(nFreeze)
{

    if(m_nFreeezeField > model->columnCount())
        m_nFreeezeField = model->columnCount();

    m_model = model;
    setModel(model);
    frozenTreeView = new QTreeView(this);
    this->setMouseTracking(true);
    frozenTreeView->setMouseTracking(true);
    header()->setDefaultAlignment(Qt::AlignCenter);      //列头文字默认居中对齐
    frozenTreeView->header()->setDefaultAlignment(Qt::AlignCenter);      //列头文字默认居中对齐

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    frozenTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    init();
    connect(header(), &QHeaderView::sectionResized, this, &HZTreeView::updateSectionWidth);
    connect(frozenTreeView->header(), &QHeaderView::sectionResized, this, &HZTreeView::updateselfGeometry);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged, frozenTreeView->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(frozenTreeView->verticalScrollBar(), &QAbstractSlider::valueChanged, verticalScrollBar(), &QAbstractSlider::setValue);

//    this->verticalScrollBar()->setStyleSheet(strTmp);

//    frozenTreeView->installEventFilter(this);

//    connect(header(), SIGNAL(QHeaderView::sortIndicatorChanged),this, SLOT(QTreeView::sortByColumn));

    connect(frozenTreeView, SIGNAL(expanded(const QModelIndex&)), this, SLOT(slotExpanded(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex&)), frozenTreeView, SLOT(slotExpanded(const QModelIndex &)));


    connect(frozenTreeView, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(slotcollapsed(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(slotcollapsed(const QModelIndex &)));

    //添加双击信号
    connect(this, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(GetRowAndCol(QModelIndex)));
    connect(frozenTreeView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(GetRowAndCol(QModelIndex)));


    //添加点击表头信号
    connect(header(), &QHeaderView::sectionClicked, this, &HZTreeView::SectionClick);
    connect(frozenTreeView->header(), &QHeaderView::sectionClicked, this, &HZTreeView::SectionClick);

    //添加右菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    frozenTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),
    this, SLOT(slotCustomContextMenu(const QPoint &)));

    connect(frozenTreeView,SIGNAL(customContextMenuRequested(const QPoint &)),
    this, SLOT(slotCustomContextMenu(const QPoint &)));
    //connect(frozenTreeView, SIGNAL(entered(const QModelIndex &)), this, SLOT(slotentered(const QModelIndex &)));
   // connect(frozenTreeView, SIGNAL(viewportEntered()), this, SLOT(sloviewportEntered()));
}

void HZTreeView::slotExpanded(const QModelIndex&index)
{
    this->expand(index);
}



void HZTreeView::slotcollapsed(const QModelIndex &index)
{
    this->collapse(index);
}

void HZTreeView::slotentered(const QModelIndex &index)
{
    this->entered(index);
}

void HZTreeView::slotviewportEntered()
{
    this->viewportEntered();
}


HZTreeView::~HZTreeView()
{
      delete frozenTreeView;
}

void HZTreeView::init()
{
      frozenTreeView->setModel(model());
      frozenTreeView->setSelectionModel(selectionModel());
      frozenTreeView->setFocusPolicy(Qt::NoFocus);
      this->setFocusPolicy(Qt::NoFocus);
      //frozenTreeView->setStyleSheet("QTreeView { border: none;}");
      frozenTreeView->setStyleSheet("QTreeView { border: none;"
                                     "selection-background-color: #999}"); //for demo purposes
      viewport()->stackUnder(frozenTreeView);

      //设置树形结构不可以双击展开
      frozenTreeView->setExpandsOnDoubleClick(false);
      this->setExpandsOnDoubleClick(false);
      frozenTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
      for (int col = m_nFreeezeField; col < this->model()->columnCount(); ++col)
            frozenTreeView->setColumnHidden(col, true);
      for(int i =0;i < m_nFreeezeField;i++)
      {
            frozenTreeView->setColumnWidth(i, columnWidth(i));
      }
      frozenTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      frozenTreeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      frozenTreeView->show();



      updateFrozenTableGeometry();
      setHorizontalScrollMode(ScrollPerPixel);
      setVerticalScrollMode(ScrollPerPixel);
      frozenTreeView->setVerticalScrollMode(ScrollPerPixel);
}

void HZTreeView::SetRightMenuItems(QList<MenuItem> lists)
{
    if(!m_ItemLists.isEmpty())
        m_ItemLists.clear();
    m_ItemLists = lists;
}


void HZTreeView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
//    if (logicalIndex == 0){
//          frozenTreeView->setColumnWidth(0, newSize);
//          updateFrozenTableGeometry();
//    }


    if (logicalIndex < m_nFreeezeField){
          for(int i =0;i < m_nFreeezeField;i++)
          {
              if(i == logicalIndex)
              {
                  frozenTreeView->setColumnWidth(i, newSize);
                  break;
              }
          }
          updateFrozenTableGeometry();
      }
}


void HZTreeView::updateselfGeometry(int logicalIndex, int oldSize, int newSize)
{
    //frozenTreeView->setGeometry(1, 1, newSize,
    //    viewport()->height() + header()->height());
    //this->setColumnWidth(0, newSize);


    updateFrozenTableGeometry();
    for(int i =0;i < m_nFreeezeField;i++)
    {
        if(i == logicalIndex)
        {
            this->setColumnWidth(i, newSize);
            break;
        }
    }
}

void HZTreeView::slotCustomContextMenu(const QPoint &pos)
{
    QTreeView* treeView = qobject_cast<QTreeView*>(sender());  // sender()是QObejct的函数，用来获取连接时信号的发射指针

        if (!treeView)
        {
            return;
        }

        QModelIndex index = treeView->indexAt(pos);

        m_ItemLists.clear();
        MenuItem item1, item2;
        item1.name = QStringLiteral("添加自选股");
        item1.type = 0;
        item2.name = QStringLiteral("删除自选股");
        item2.type = 1;
        m_ItemLists.push_back(item1);
        m_ItemLists.push_back(item2);



        if (index.isValid())
        {
            // 创建右键菜单
            if(!m_ItemLists.isEmpty())
            {
                QMenu menu;
                for(int i = 0; i < m_ItemLists.size(); i++)
                {
                    menu.addAction(m_ItemLists[i].name);
                }
                connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(OnActionTriggered(QAction*)));
                menu.exec(QCursor::pos());
            }
        }



//        if (index.isValid())
//        {
//            // 创建右键菜单

//            QMenu menu;
//            menu.addAction("Show");
//            menu.addAction("delete");

//            menu.exec(QCursor::pos());  //QCursor::pos()让menu的位置在鼠标点击的的位置，
//        }
}

void HZTreeView::GetRowAndCol(const QModelIndex &index)
{

    TreeNodePtr node = nullptr;
    QString strFildName;
    int row = m_model->getRow(index, node, strFildName);
    emit OnItemDbClicked(row, node, strFildName);

}

void HZTreeView::SectionClick(int logicalIndex)
{
//    QString strContent = (QString("Header Number is %1")).arg(QString::number(logicalIndex, 10));
//    QMessageBox::information(NULL, "header", strContent, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    int i = logicalIndex;
    emit OnHeaderClicked(logicalIndex);
}

void HZTreeView::OnActionTriggered(QAction *Act)
{
        QAction *act = Act;
        QString strName = act->text();
//        QMessageBox::information(NULL, "MENUItem", strName, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    emit OnRightClicked(m_ItemLists, strName);
}



void HZTreeView::resizeEvent(QResizeEvent * event)
{
      QTreeView::resizeEvent(event);
      updateFrozenTableGeometry();
 }

void HZTreeView::updateFrozenTableGeometry()
{
//    frozenTreeView->setGeometry(1, 1, columnWidth(0),
//        viewport()->height() + header()->height());
     int width = 0;
     for(int i =0;i < m_nFreeezeField;i++)
     {
         width += columnWidth(i);
     }
     //frozenTreeView->setGeometry(header()->width() + frameWidth(),
     //                            frameWidth(), width,viewport()->height() + header()->height());

     frozenTreeView->setGeometry(1,
                                 1, width,viewport()->height() + header()->height());
}

QModelIndex HZTreeView::moveCursor(CursorAction cursorAction,
                                          Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTreeView::moveCursor(cursorAction, modifiers);
    if (this->isExpanded(current)) {
        frozenTreeView->expand(current);
    }
    else {
        frozenTreeView->collapse(current);
    }
    return current;
}

//void HZTreeView::mouseDoubleClickEvent(QMouseEvent *event)
//{
//    QAbstractItemView::mouseDoubleClickEvent(event);
//}





//
//
//void HZTreeView::scrollTo (const QModelIndex & index, ScrollHint hint){
//    if (index.column() > 0)
//        QTreeView::scrollTo(index, hint);
//}
//


//void HZTreeView::slotPushButton()
//{
//    QItemSelectionModel * model = this->selectionModel();
//    QModelIndexList list = model->selectedIndexes();
//    qDebug() << list.size();
//}

void HZTreeView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    frozenTreeView->setItemDelegate(delegate);
    QTreeView::setItemDelegate(delegate);
}
