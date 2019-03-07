#include "hzitemmodel.h"

HZItemModel::HZItemModel(const ItemModelInfo& itemInfo,QObject *parent):QAbstractItemModel(parent),m_ItemInfo(itemInfo),m_expandStockPos(-1){

    m_pRoot_ = new TreeNode;
    QList<QVariant> head;
    if(m_ItemInfo.fieldFieldTypeList.size() == m_ItemInfo.fieldNameList.size())
    {
        //QMap<int,int> m_FieldTypeToColIndex;
        //QMap<int,int> m_colIndexToFieldType;
        for(int i =0;i < m_ItemInfo.fieldFieldTypeList.size();i++)
        {
            m_nameToFieldTyp[m_ItemInfo.fieldNameList[i]] = m_ItemInfo.fieldFieldTypeList[i];
            m_FieldTypToName[m_ItemInfo.fieldFieldTypeList[i]] = m_ItemInfo.fieldNameList[i];
            m_FieldTypeToColIndex[m_ItemInfo.fieldFieldTypeList[i]] = i;
            m_colIndexToFieldType[i] = m_ItemInfo.fieldFieldTypeList[i];
            head.push_back(m_ItemInfo.fieldNameList[i]);
        }
        m_pRoot_->setData(head);
    }
}


void HZItemModel::updateData(const int isSortFlag,const QList<TreeNodePtr>& data)
{
    if(data.isEmpty())
        return;
    int listCount  = mRootEntrys.size();
    int dataCout = data.size();
    if(listCount == 0)
    {
        m_pRoot_->childs.clear();
        for(int i = 0; i < dataCout; i++){
            TreeNodePtr pNode = new TreeNode();
            pNode->setData(data[i]->alldata());
            pNode->setParent(m_pRoot_);
            pNode->childs = data[i]->childs;
            pNode->mIsUpdate = 1;
            m_pRoot_->appendNode(pNode);
            addEntryNode(pNode);
            if(m_FieldTypeToColIndex.find(m_ItemInfo.fieldKey) != m_FieldTypeToColIndex.end()){
               QVariant v =  pNode->data(m_FieldTypeToColIndex[m_ItemInfo.fieldKey]);
               if(v.isValid() && v.type() == QVariant::String)
               {
                   m_stockIndexMap[v.toString()] = i;
               }
            }
        }

    }else if(isSortFlag == 1  &&  m_stockIndexMap.isEmpty() == false)
    {
        //有展开数据时不更新排序
        if(m_expandStockPos == -1){
            for(int i = 0; i < dataCout; i++){
                TreeNodePtr pNode = data[i];
                if(m_FieldTypeToColIndex.find(m_ItemInfo.fieldKey) != m_FieldTypeToColIndex.end()){
                   QVariant v =  pNode->data(m_FieldTypeToColIndex[m_ItemInfo.fieldKey]);
                   if(v.isValid() && v.type() == QVariant::String)
                   {
                       m_stockIndexMap[v.toString()] = i;
                   }
                }
            }
        }
    }else if( m_stockIndexMap.isEmpty() == false)
    {
        for(int i = 0; i < dataCout; i++){
            if(m_FieldTypeToColIndex.find(m_ItemInfo.fieldKey) != m_FieldTypeToColIndex.end()){
               QVariant v =  data[i]->data(m_FieldTypeToColIndex[m_ItemInfo.fieldKey]);
               if(v.isValid() && v.type() == QVariant::String)
               {
                   TreeNodePtr pNode = mRootEntrys[m_stockIndexMap[v.toString()]];
                   pNode->setData(data[i]->alldata());
                   pNode->mIsUpdate = 1;
               }
            }
         }
    }
}

QVariant HZItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


        if (role == Qt::TextAlignmentRole)
             return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);

        if (role != Qt::DisplayRole)
             return QVariant( );


    TreeNodePtr node = (TreeNodePtr)index.internalPointer();
    return node->data(index.column());
}

Qt::ItemFlags HZItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant HZItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return m_pRoot_->data(section);

    return QVariant();
}

QModelIndex HZItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    TreeNodePtr parentItem;
    if (!parent.isValid())
        parentItem = m_pRoot_;
    else
        parentItem = static_cast<TreeNodePtr>(parent.internalPointer());

    if(parentItem && parentItem->childs.size() > row)
    {
        return createIndex(row, column, parentItem->childs[row]);
    }
    return QModelIndex();


    //if(!parent.isValid())  //parent为空，返回的是根节点的modelIndex，返回的同时，把节点数据指针(TreeNodePtr)保存在QModelIndex的internalPointer中，以便在其它函数中获取节点数据
    //{
    //    if((row >= 0) && (row < mRootEntrys.size()))
    //    {
    //        return createIndex(row,column,mRootEntrys.at(row));
    //    }
    //}else{
    //    TreeNodePtr parentNode = (TreeNodePtr)parent.internalPointer(); //返回子节点的modelIndex
    //    return createIndex(row,column,parentNode->childs[row]);
    //}
    //return QModelIndex();
}

QModelIndex HZItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNodePtr childItem = (TreeNodePtr)index.internalPointer();
    if(childItem == nullptr)
    {
        return QModelIndex();
    }
    TreeNodePtr parentItem = childItem->parent();

    if (parentItem == m_pRoot_)
        return QModelIndex();

    int nIndex = 0;
    if(parentItem && parentItem->parent())
    {
        nIndex = parentItem->parent()->childs.indexOf(parentItem);
    }
    return createIndex(nIndex, 0, parentItem);

    //TreeNodePtr node = (TreeNodePtr)index.internalPointer();
    //if(node->parent() == nullptr)
    //{
    //    return QModelIndex(); //根节点没有parent
    //}else{
    //    return createIndex(0,1,node->parent());
    //}

}

int HZItemModel::rowCount(const QModelIndex &parent) const
{
    TreeNodePtr parentItem = nullptr;
    if (!parent.isValid())
        parentItem = m_pRoot_;
    else
        parentItem = (TreeNodePtr)parent.internalPointer();

    if(parentItem == nullptr)
    {
        return 0;
    }
    return parentItem->childs.size();

    //if(parent.isValid()) //parent不为空，则要获取的行数是某个节点的子节点行数
    //{
    //    TreeNodePtr parentNode = (TreeNodePtr)parent.internalPointer(); //节点信息在index时被保存在QModelIndex的internalPointer中
    //    return parentNode->childs.size();
    //}
    //return mRootEntrys.size();  //否则返回的是根节点行数

}

int HZItemModel::columnCount(const QModelIndex &parent) const
{
    return m_ItemInfo.fieldFieldTypeList.size();
    //if (parent.isValid())
    //    return static_cast<TreeNodePtr>(parent.internalPointer())->m_itemData.size();
    //else
    //    return m_ItemInfo.fieldFieldTypeList.size();
}

int HZItemModel::getRow(const QModelIndex &index, TreeNodePtr parent,QString &strFildName)
{
    int row = -1;
    if (!index.isValid())
        return row;
    TreeNodePtr node = (TreeNodePtr)index.internalPointer();
    if(node->parent() == m_pRoot_)
    {
        parent = nullptr;
        if(m_FieldTypeToColIndex.find(m_ItemInfo.fieldKey) != m_FieldTypeToColIndex.end()){
           QVariant v =  node->data(m_FieldTypeToColIndex[m_ItemInfo.fieldKey]);
           if(v.isValid() && v.type() == QVariant::String)
           {
               auto it = m_stockIndexMap.find(v.toString());
               if(it != m_stockIndexMap.end())
               {
                   row= m_stockIndexMap[v.toString()];
                   strFildName = v.toString();
               }
           }
        }
    }else{
        parent = node->parent();
        auto& childList = node->parent()->childs;
        for(int i =0;i < childList.size();i++)
        {
            if(node == childList[i])
            {
                if(m_FieldTypeToColIndex.find(m_ItemInfo.fieldKey) != m_FieldTypeToColIndex.end()){
                   QVariant v =  node->data(m_FieldTypeToColIndex[m_ItemInfo.fieldKey]);
                   if(v.isValid() && v.type() == QVariant::String)
                   {

                           row = i;
                           strFildName = v.toString();
                           break;

                   }
                }
//                row = i;
//                break;
            }
        }

    }
    return row;
}

void HZItemModel::addEntryNode(TreeNodePtr node, TreeNodePtr parent)
{
    if(parent == NULL)
    {
        mRootEntrys.append(node);
    }else{
        node->setParent(parent);
        parent->appendNode(node);
    }
}

