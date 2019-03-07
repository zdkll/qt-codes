#ifndef IMAGELAYER_H
#define IMAGELAYER_H
/********************************************************************
created:  2018/4/3
author: min.cheng
purpose: 提供图层管理相关功能，支持自动添加图层，多图层叠加显示
*********************************************************************/
#include <qwidget.h>
#include <qpixmap.h>
#include <qlist.h>
#include <QRect>

struct ImgLayerInfo
{
    bool bHide = false;
    int type;
    QRect rc;
    QPixmap *pix;
    QColor fillColor;
    ImgLayerInfo():pix(nullptr){}
};

class ImageLayer
{
public:
    explicit ImageLayer(QWidget* wnd);

    ~ImageLayer();

    /**
     * @brief 图层创建
     * @param[in] rc图层在窗口中的位置
     * @param[in] type 图层类型
     * @param[in] fillColor 填充图层背景色 指定透明色可以实现图层叠加显示
     * @retval 创建成功的图层
     */
    ImgLayerInfo createLayout(QRect rc,QColor fillColor=Qt::transparent);

    /**
     * @brief  根据类型获取图层
     * @param[in] type 图层类型
     * @retval 如果图层不存在，则返回的类型为无效
     */
    ImgLayerInfo getLayout(int type);

    /**
     * @brief 从图层管理器中删除指定图层
     * @param[in] type 指定图层类型
     */
    void remove(int type);

    /**
     * @brief  清除指定图层上的内容
     * @param[in]  type 指定图层类型
     */
    void clear(int type);

    /**
     * @brief  隐藏图层
     * @param[in]  type 指定图层类型
     */
    void hide(int type,bool bshow);
    void hide(QRect rc,int type,bool bshow);

    /**
     * @brief 重置图层区域大小
     * @param[in] rc 新图层区域
     * @param[in] type 指定图层类型
     */
    void resize(QRect rc,int type);

    /**
     * @brief  叠加绘制各个图层内容到窗口中
     */
    void repaint();

    /**
     * @brief  获取图层管理器所在的窗口
     */
    QWidget* getWnd();

    /**
     * @brief 清理图层上面的内容
     */
    void clearAll();
 private:
    int m_index;
    QWidget* m_wnd;
    std::map<int,ImgLayerInfo> m_layMap;
    std::map<int,int> m_layIndeMap; //key=type, value=layVec_下标
    QList<ImgLayerInfo> m_layVec;
};


#endif // IMAGELAYER_H
