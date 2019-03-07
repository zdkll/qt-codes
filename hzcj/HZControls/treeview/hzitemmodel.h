#ifndef HZITEMMODEL_H
#define HZITEMMODEL_H
#include <QAbstractItemModel>
#include "treenode.h"
#include <QList>

struct ItemModelInfo
{
    QList<int> fieldFieldTypeList;  //标题类型
    QList<QString> fieldNameList;   //标题名称
    int fieldKey;                   //用于标记一行的字段key
};

class HZItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit HZItemModel(const ItemModelInfo& itemInfo,QObject *parent=0);

    void updateData(const int isSortFlag, const QList<TreeNodePtr>& data);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int getRow(const QModelIndex& index,TreeNodePtr parent,QString &strFildName);
signals:

public slots:

private:
     void addEntryNode(TreeNodePtr node,TreeNodePtr parent=0); //用于向树添加节点
private:
    QList<TreeNodePtr> mRootEntrys;      //用于保存根节点
    TreeNodePtr m_pRoot_;
    ItemModelInfo m_ItemInfo;
    QMap<QString,int> m_nameToFieldTyp;     //标题名称与对应类型映射
    QMap<int,QString> m_FieldTypToName;     //标题类型与名称映射
    QMap<int,int> m_FieldTypeToColIndex;    //标题类型对应下标
    QMap<int,int> m_colIndexToFieldType;    //标题下表对应类型
    QMap<QString,int> m_stockIndexMap;   //股票代码对应行号
    int m_expandStockPos;
};



#endif // HZITEMMODEL_H
