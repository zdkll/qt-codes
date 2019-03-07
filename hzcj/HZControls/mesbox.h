#ifndef MESBOX_H
#define MESBOX_H
#include "popupdialog.h"
#include <QMessageBox>
#include <QDialogButtonBox>


class QLabel;
class QPushButton;
class MsgView :public QDialog
{
    Q_OBJECT
public:
    MsgView(QWidget *parent = nullptr, const QString &text = "",
              QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    void setDefaultButton(QPushButton *button);
    void setDefaultButton(QMessageBox::StandardButton button);

    QMessageBox::StandardButton standardButton(QAbstractButton *button) const;

    void setStyle(int nStyle);

    void translateUI();
    int execReturnCode(QAbstractButton *button);

private:

    QPushButton *msgBtn;

    QDialogButtonBox *m_pButtonBox;

public slots:
    void onButtonClicked(QAbstractButton *button);

Q_SIGNALS:
    void btnClicked(int nRet);
};

#include <QEventLoop>
class HZCONTROLSSHARED_EXPORT MesBox : public PopupDialog
{
    Q_OBJECT
public:
    explicit MesBox(QWidget *parent = nullptr, const QString &title = tr("Tip"), const QString &text = "",
                            QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);
    ~MesBox();

    MsgView *m_MsgView;

    void setStyle(int nStyle);


public slots:
    void onButtonClicked(int nRet);
};


class HZCONTROLSSHARED_EXPORT MessageBox
{

public:
    /**
     * @brief 普通提示框
     * @param[in] title 标题
     * @param[in] text  内容
     * @param[in] buttons 按钮组
     * @param[in] defaultButton 默认焦点
     * @retval 返回值为点击按钮
     */

    static int showInformation(QWidget *parent = nullptr, const QString &title = "",
                                                const QString &text = "", QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    /**
     * @brief 询问提示框
     * @param[in] title 标题
     * @param[in] text  内容
     * @param[in] buttons 按钮组
     * @param[in] defaultButton 默认焦点
     * @retval 返回值为点击按钮
     */
    static int showQuestion(QWidget *parent = nullptr, const QString &title = "",
                                             const QString &text = "", QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                             QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    /**
     * @brief 错误提示框
     * @param[in] title 标题
     * @param[in] text  内容
     * @param[in] buttons 按钮组
     * @param[in] defaultButton 默认焦点
     * @retval 返回值为点击按钮
     */
    static int showError(QWidget *parent = nullptr, const QString &title = "",
                                          const QString &text = "", QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                          QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    /**
     * @brief 警告提示框
     * @param[in] title 标题
     * @param[in] text  内容
     * @param[in] buttons 按钮组
     * @param[in] defaultButton 默认焦点
     * @retval 返回值为点击按钮
     */
    static int showWarning(QWidget *parent = nullptr, const QString &title = "",
                                            const QString &text = "", QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    /**
     * @brief 关于提示框
     * @param[in] title 标题
     * @param[in] text  内容
     * @param[in] buttons 按钮组
     * @param[in] defaultButton 默认焦点
     * @retval 返回值为点击按钮
     */
    static int showAbout(QWidget *parent = nullptr, const QString &title = "",
                                             const QString &text = "", QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};


#endif // MESBOX_H
