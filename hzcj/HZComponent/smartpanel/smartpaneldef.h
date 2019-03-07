#ifndef SMARTPANELDEF_H
#define SMARTPANELDEF_H
#include <QtCore>

#include <datamodeldefs/stockinfodef.h>


/**
 * @brief The CompanyType enum ��������
 */
enum CompanyType
{
    LongHuBang=1,
    FenHongSongZhuan,
    CaiBao
};

/**
 * @brief The EMPageType enum ҳ����
 */
enum EMPageType{
    CompanyEvent_Page,
    SmartPlate_Page //�����춯
};

/**
 * @brief The DrawType enum ��������
 */
enum DrawType{
    Draw_Date, //���ڱ�־ ��ʽ��yyyy-MM-dd
    Draw_Time, //����ʱ���־
    Draw_Content, // ����������־ ����չʾ�����ֻչʾ����
    Draw_ContentEx, // ����������־ ����չʾ
    Draw_None // ֹͣ��־(space)
};

/**
 * @brief The PanelItem struct ��˾����
 */
struct PanelItem
{
    DrawType type;  //draw graph
    int contentEx;// content ex
    int msgType;    //1��������2���ֺ���ת��3���Ʊ�
    QString code;   //stockcode
    QString name;   //stockname
    QString title;  //title
    QString content;//descript

    bool smFlags=false; //�������ת��־--�Ʊ������
    qreal change=DBL_MAX; //�ǵ���
    qreal chg =DBL_MAX;   //�ǵ���
    qreal real_chg = DBL_MAX;
    qreal buy=DBL_MAX;//	������������λ��Ԫ
    qreal sell=DBL_MAX;//	�������������λ��Ԫ
    qreal net_buy=DBL_MAX;//	������������λ��Ԫ
    QString reason="";//	�������ϰ�ԭ��
    QString record_date=UINT_MAX;//	��Ȩ�Ǽ��գ���ʽ��YYYYMMDD
    QString dividend_date=UINT_MAX;//	��Ȩ��Ϣ�գ���ʽ��YYYYMMDD
    QString detail="";//	�ֺ���ת����
    quint32 date=UINT_MAX; //	�Ʊ��������ڣ���ʽ��YYYYMMDD
    QString url="";//	�Ʊ�H5����ҳ��ַ
    quint32 time=UINT_MAX;//	���ݸ���ʱ���
    QString typeTwoMsg="";

    UINT32                    update_time = INVALID_UINT32;//����ʱ��
    UINT32                    last_price = INVALID_UINT32;//�ּ�
    UINT64                    turnover = INVALID_UINT64;//�ɽ���
    UINT64                    volume  = INVALID_UINT64; //�ɽ���
};

#endif // SMARTPANELDEF_H
