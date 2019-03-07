#include "treenode.h"


TreeNode::TreeNode()
{
}

TreeNodePtr TreeNode::parent()
{
    return mParent;
}

void TreeNode::setParent(TreeNodePtr p)
{
    this->mParent = p;
}

void TreeNode::appendNode(TreeNodePtr node)
{
    childs.append(node);
}

void TreeNode::removeNode(int row)
{
    childs.removeAt(row);
}

QVariant TreeNode::data(int role) const {
    if(!m_itemData.isEmpty())
        return m_itemData.value(role);
    return QVariant();
}

QList<QVariant> TreeNode::alldata()
{
    return m_itemData;
}


void TreeNode::setData(const QList<QVariant> &data)
{
    m_itemData = data;      //存放每项目的数据
    mIsUpdate = true;
}


