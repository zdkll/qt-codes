#ifndef LINESETTINGDIALOGS_H
#define LINESETTINGDIALOGS_H

#include <unordered_map>
#include <QDialog>
#include <QPen>
#include <QVariant>

class ColorComboBox;
class PenStyleToolButton;
struct TextNoteProperty
{
    TextNoteProperty(){}
    TextNoteProperty(const QString& txt,const QColor &cr,int size)
        :text(txt),color(cr),fontSize(size){}
    QString text;
    QColor color;
    int        fontSize;
};

class QDialogButtonBox;
class QPlainTextEdit;
class QSpinBox;
//设置对话框
class LineSettingDialog : public QDialog
{
    Q_OBJECT
public:
    LineSettingDialog(const QString& title,QWidget *parent = 0);

    //获取设置属性
    static bool   getLineProperty(const QString& title,const QPen& pen_in, QPen *pen_out,QWidget *parent = 0);
    static bool   getLineProperty(const QString& title,const QColor& color_in,QColor *color_out,QWidget *parent = 0);
    static bool   getLineProperty(const QString& title,const TextNoteProperty& property_in,TextNoteProperty *property_out,QWidget *parent = 0);

    virtual void createDialog() = 0;
protected:
    QDialogButtonBox *createDialogButtonBox();
};

//颜色 -线型- 粗细设置
class PenSettingDialog : public LineSettingDialog
{
    Q_OBJECT
public:
    PenSettingDialog(const QString& title,const QPen& pen,QWidget *parent = Q_NULLPTR);
    void createDialog();
    QPen pen()const {return m_pen;}

protected:
    void accept();

private:
    QPen m_pen;
    ColorComboBox *m_colorCbx;
    PenStyleToolButton *m_lineStyleToolBtn,*m_lineWidthToolBtn;
};

//填充颜色设置
class ColorSettingDialog : public LineSettingDialog
{
    Q_OBJECT
public:
    ColorSettingDialog(const QString& title,const QColor& color,QWidget *parent = Q_NULLPTR);
    void createDialog();
    QColor color()const {return m_color;}

protected:
    void accept();

private:
    QColor m_color;
    ColorComboBox *m_colorCbx;
};

//文本-颜色-字号设置
class TextColorSizeSettingDialog : public LineSettingDialog
{
    Q_OBJECT
public:
    TextColorSizeSettingDialog(const QString& title,const TextNoteProperty& ,QWidget *parent = Q_NULLPTR);
    void createDialog();
    TextNoteProperty txtNoteProperty() const {return m_txtNodeProperty;}

protected:
    void accept();

private:
    TextNoteProperty  m_txtNodeProperty;
    QPlainTextEdit      *m_textEdit;
    ColorComboBox  *m_colorCbx;
    QSpinBox             *m_sizeSpbox;
};


#endif // LINESETTINGDIALOGS_H
