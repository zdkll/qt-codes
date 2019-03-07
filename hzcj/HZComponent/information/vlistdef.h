/********************************************************************
created: 2018,9,13
author: xuzhehu
purpose:
*********************************************************************/
#ifndef ITEMDEF_H
#define ITEMDEF_H

#include <hzinfodata.h>
#include <QMetaType>

#define VLISTITEM_CONTENT_HEIGHT 19
#define VLISTITEM_SYSTEM_MARGIN 10
#define VLISTITEM_LEFT_SPACE 18
#define VLISTITEM_RIGHT_SPACE 26
#define VLISTITEM_TOP_SPACE 19/2
#define VLISTITEM_BOTTON_SPACE 10/2
#define VLIST_WIDTH_SIZE 260
#define VLIST_HEIGHT_SIZE 81

#define CUSTOM_ROLE_ITEM        Qt::UserRole+1
#define CUSTOM_ROLE_ITEM_ROLE   Qt::UserRole+2
#define CUSTOM_ROLE_ITEM_IID    Qt::UserRole+3

typedef struct tagVListData{
    QString id;     //id
    QString body;   //标题
    QString times;  //时间
    QString desc;   //摘要
    QString tag;    //状态
    QString source; //来源
    int type =  UnknownTag;
    QString url;//详情页地址
    bool operator ==(const tagVListData &item) const {
        return item.body==this->body && item.times == this->times && this->id == item.id;
    }
}STVListData;

typedef struct tagInfoData
{
    QList<STVListData> datas;
}STInfoData;

/**
  * @brief tagReaderDatas 详情结构
  * @memberof title 标题
  * @memberof source 来源
  * @memberof time 时间
  * @memberof content 内容
  **/
typedef struct tagDetailDescript
{
    QString title;
    QString source;
    QString time;
    QString description;
    QString content;
}STDetailDescript;

Q_DECLARE_METATYPE(STVListData)
Q_DECLARE_METATYPE(STInfoData)
Q_DECLARE_METATYPE(STDetailDescript)
#endif // ITEMDEF_H
