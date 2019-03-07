#ifndef TREENODE_H
#define TREENODE_H
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>

class TreeNode;
typedef TreeNode* TreeNodePtr;

class TreeNode
{
public:
    explicit TreeNode();
    TreeNodePtr parent();
    void setParent(const TreeNodePtr p);  //设置父节点，根节点的父节点为NULL
    void appendNode(TreeNodePtr node);
    void removeNode(int row);
    QVariant data(int role = Qt::DisplayRole) const;
    QList<QVariant> alldata();
    void setData(const QList<QVariant> &data);
    QList<TreeNodePtr> childs;   //用于保存子节点指针，把childs放到public不好，仅仅是为了方便，不要学。

    TreeNodePtr mParent=nullptr;   //父节点
    QList<QVariant> m_itemData;     //存放每项的数据
    int mIsUpdate;//节点数据是否更新
};

#endif // TREENODE_H
