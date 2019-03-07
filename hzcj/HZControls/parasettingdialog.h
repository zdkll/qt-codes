#ifndef PARASETTINGDIALOG_H
#define PARASETTINGDIALOG_H
#include "popupdialog.h"
#include "hzcfg.h"
#include <QToolButton>
class QGridLayout;
class QSpinBox;
class QComboBox;
class QButtonGroup;

class PenStyleToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit PenStyleToolButton(QWidget *parent = 0);

    void createWg();

    void setDefalutWidth(int nWidth = 1);

    void setDefalutBtn();

private:
    int m_ndefaultWidth;

    QToolButton *m_defaultBtn;

signals:
    void setLineWidth(int nWidth);
};

class HZCONTROLSSHARED_EXPORT ParaSettingDialog:public PopupDialog
{
    Q_OBJECT
public:
    explicit ParaSettingDialog(QWidget *parent = nullptr, QString instname = "");

    void createWg();

    void setIndi(QString strIndi);

    void addButton(QPushButton* btn, const int id);


private:
    QGridLayout *m_pGridLayout;

    IndiItemMap m_inidItemList;

    IndiItemInfo m_itemInfo;
    IndiItemInfo m_defaultIndiInfo;

    QList<QSpinBox*> m_splist;
    QList<PenStyleToolButton*> m_toolBtnlist;
    QList<QWidget*> m_colorWglist;
    QButtonGroup* m_pButtonGroup;

    QString m_strIndi;

private slots:
    void onrestBtnClicked();
    void onsaveBtnClicked();
    void onButtonGroupClicked(int nId);
    void onsetLineWidth(int nWidth);
    void onvalueChanged(int nValue);
    void onWndClose();
    void test();

signals:
    void valueChanged(bool isChange);
};

#endif // PARASETTINGDIALOG_H
