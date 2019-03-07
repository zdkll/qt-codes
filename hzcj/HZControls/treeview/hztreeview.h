/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef HZTreeView_H
#define HZTreeView_H
#include <QTreeView>
#include "hzitemmodel.h"
class QStandardItemModel;
//! [Widget definition]

struct MenuItem{
    QString name;
    int type;
};


class HZTreeView : public QTreeView
{
     Q_OBJECT

public:
      HZTreeView(HZItemModel * model,int nFreeze,QWidget * parent = nullptr);
      ~HZTreeView();
      void init();

      void SetRightMenuItems(QList<MenuItem> lists);

      void setItemDelegate(QAbstractItemDelegate *delegate);
protected:
      void resizeEvent(QResizeEvent *event) override;
      QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
//      void mouseDoubleClickEvent(QMouseEvent *event);

      //void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible) override;

private slots:
      void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
      //void updateSectionHeight(int logicalIndex, int oldSize, int newSize);
      void slotExpanded(const QModelIndex&index);
      void slotcollapsed(const QModelIndex &index);
      void slotentered(const QModelIndex &index);
      void slotviewportEntered();
      void updateselfGeometry(int logicalIndex, int oldSize, int newSize);
      void slotCustomContextMenu(const QPoint &);
public slots:
      void GetRowAndCol(const QModelIndex &index);
      void SectionClick(int logicalIndex);
      void OnActionTriggered(QAction* Act);

Q_SIGNALS:
      /**
       * @brief  双节每行时发出的信号
       * @param[out] row 所在行号
       * @param[out] node 树形结构返回每一行的数据
       * @param[out] type 字段名称
       * @retval 返回类型为void
       */
      void OnItemDbClicked(int row, TreeNodePtr node, QString strFildName);
      /**
       * @brief  点击表头时发出信号
       * @param[out] index 返回对应列的索引
       * @retval 返回类型为void
       */
      void OnHeaderClicked(int index);
      /**
       * @brief  右键时发出的信号
       * @param[out] lists  返回菜单列表
       * @param[out] strAct 对应菜单的名称
       * @retval 如果图层不存在，则返回的类型为无效
       */
      void OnRightClicked(QList<MenuItem> &lists, QString strAct);

private:
    void updateFrozenTableGeometry();
private:
    QTreeView *frozenTreeView;
    int m_nFreeezeField;
    bool m_mark;
    HZItemModel *m_model;

    QList<MenuItem> m_ItemLists;   //保存右键菜单显示内容
};

#endif // HZTreeView_H
