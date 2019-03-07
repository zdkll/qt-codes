#include "linesettingdialogs.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QSpinBox>

#include "controls.h"

LineSettingDialog::LineSettingDialog(const QString& title,QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(title);
}

bool LineSettingDialog::getLineProperty(const QString &title, const QPen &pen_in, QPen *pen_out, QWidget *parent)
{
    PenSettingDialog penSettingDialog(title,pen_in,parent);
    penSettingDialog.createDialog();
    if(penSettingDialog.exec()){
        *pen_out = penSettingDialog.pen();
        return true;
    }else
        return false;
    return false;
}

bool LineSettingDialog::getLineProperty(const QString &title, const QColor &color_in, QColor *color_out, QWidget *parent)
{
    ColorSettingDialog colorSettingDialog(title,color_in,parent);
    colorSettingDialog.createDialog();
    if(colorSettingDialog.exec()){
        *color_out = colorSettingDialog.color();
        return true;
    }else
        return false;
    return false;
}

bool LineSettingDialog::getLineProperty(const QString &title, const TextNoteProperty &property_in, TextNoteProperty *property_out, QWidget *parent)
{
    TextColorSizeSettingDialog txtCrSizeSettingDialog(title,property_in,parent);
    txtCrSizeSettingDialog.createDialog();
    if(txtCrSizeSettingDialog.exec()){
        *property_out = txtCrSizeSettingDialog.txtNoteProperty();
        return true;
    }else
        return false;
    return false;
}

QDialogButtonBox *LineSettingDialog::createDialogButtonBox()
{
    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    btnBox->addButton(QStringLiteral("确定"),QDialogButtonBox::AcceptRole);
    btnBox->addButton(QStringLiteral("取消"),QDialogButtonBox::RejectRole);

    connect(btnBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(btnBox,SIGNAL(rejected()),this,SLOT(reject()));

    return btnBox;
}


///PenSettingDialog---
PenSettingDialog::PenSettingDialog(const QString& title,const QPen &pen, QWidget *parent)
    :LineSettingDialog(title,parent),m_pen(pen)
{

}

void PenSettingDialog::createDialog()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QGridLayout *gridLayout = new QGridLayout;
    QLabel *lineColorLbl = new QLabel(QStringLiteral("颜色"));
    m_colorCbx = new ColorComboBox();
    gridLayout->addWidget(lineColorLbl,0,0);
    gridLayout->addWidget(m_colorCbx,1,0);

    QLabel *lineStyleLbl = new QLabel(QStringLiteral("线型"));
    m_lineStyleToolBtn = new PenStyleToolButton(PenStyleToolButton::PenStyle);
    gridLayout->addWidget(lineStyleLbl,0,1);
    gridLayout->addWidget(m_lineStyleToolBtn,1,1);

    QLabel *lineWidthLbl = new QLabel(QStringLiteral("粗细"));
    m_lineWidthToolBtn = new PenStyleToolButton(PenStyleToolButton::PenWidth);
    gridLayout->addWidget(lineWidthLbl,0,2);
    gridLayout->addWidget(m_lineWidthToolBtn,1,2);

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(createDialogButtonBox());
    this->setLayout(mainLayout);

    //设置属性
    m_colorCbx->setColor(m_pen.color());
    m_lineStyleToolBtn->setPenStyle(m_pen.style());
    m_lineWidthToolBtn->setPenWidth(m_pen.width());
}

void PenSettingDialog::accept()
{
    m_pen.setColor(m_colorCbx->color());
    m_pen.setStyle(m_lineStyleToolBtn->penStyle());
    m_pen.setWidth(m_lineWidthToolBtn->penWidth());
    LineSettingDialog::accept();
}




///ColorSettingDialog----
ColorSettingDialog::ColorSettingDialog(const QString& title,const QColor &color, QWidget *parent)
    :LineSettingDialog(title,parent),m_color(color)
{

}

void ColorSettingDialog::createDialog()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *colorLayout = new QHBoxLayout;
    QLabel *colorLabel = new QLabel(QStringLiteral("颜色:"));
    m_colorCbx = new ColorComboBox();
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(m_colorCbx);

    mainLayout->addLayout(colorLayout);
    mainLayout->addWidget(createDialogButtonBox());

    this->setLayout(mainLayout);

    //初始化
    m_colorCbx->setColor(m_color);
}


void ColorSettingDialog::accept()
{
    m_color =  m_colorCbx->color();
    LineSettingDialog::accept();
}


///TextColorSizeSettingDialog---
TextColorSizeSettingDialog::TextColorSizeSettingDialog(const QString& title,const TextNoteProperty& txtNoteProperty, QWidget *parent)
    :LineSettingDialog(title,parent),m_txtNodeProperty(txtNoteProperty)
{

}

void TextColorSizeSettingDialog::createDialog()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    m_textEdit = new QPlainTextEdit(this);
    mainLayout->addWidget(m_textEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    QLabel *crLabel = new QLabel(QStringLiteral("颜色:"));
    m_colorCbx = new ColorComboBox();
    bottomLayout->addWidget(crLabel);
    bottomLayout->addWidget(m_colorCbx);

    QLabel *sizeLabel = new QLabel(QStringLiteral("字体大小:"));
    m_sizeSpbox = new QSpinBox();
    m_sizeSpbox->setRange(8,24);
    m_sizeSpbox->setValue(14);
    bottomLayout->addWidget(sizeLabel);
    bottomLayout->addWidget(m_sizeSpbox);

    bottomLayout->addWidget(createDialogButtonBox());

    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    //初始化
    m_textEdit->setPlainText(m_txtNodeProperty.text);
    m_sizeSpbox->setValue(m_txtNodeProperty.fontSize);
    m_colorCbx->setColor(m_txtNodeProperty.color);
}

void TextColorSizeSettingDialog::accept()
{
    m_txtNodeProperty.text = m_textEdit->toPlainText();
    m_txtNodeProperty.fontSize = m_sizeSpbox->value();
    m_txtNodeProperty.color = m_colorCbx->color();

    LineSettingDialog::accept();
}


