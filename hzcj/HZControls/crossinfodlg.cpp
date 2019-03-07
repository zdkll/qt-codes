#include "crossinfodlg.h"

#include <QStackedWidget>
#include <QBoxLayout>
#include <QWidget>
#include <QLabel>



VBoxWidget::VBoxWidget(QWidget *parent )
    :QWidget(parent),layout( new QVBoxLayout)
{
    this->setWindowFlags(Qt::Tool |Qt::Popup);
    this->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);
}

void VBoxWidget::addLabel()
{
    QLabel *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
void VBoxWidget::addLabel(const QString &text)
{
    QLabel *label = new QLabel(text,this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}

void VBoxWidget::addValLabel()
{
    QLabel *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    m_labels.push_back(label);
    layout->addWidget(label);
}
void VBoxWidget::addValLabel(const QString &text)
{
    QLabel *label = new QLabel(text,this);
    label->setAlignment(Qt::AlignCenter);
    m_labels.push_back(label);
    layout->addWidget(label);
}

void VBoxWidget::setValblText(const int &index,const QString &valStr)
{
    if(0<=index && index<m_labels.size()){
        m_labels[index]->setText(valStr);
    }
}



//=============================
CrossInfoDlg::CrossInfoDlg(QWidget *parent)
    :QDialog(parent),m_parentWg(parent)
{
    createDlg();
}
void CrossInfoDlg::createDlg()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_statckWg = new QStackedWidget(this);
    m_statckWg->setContentsMargins(0,0,0,0);

    VBoxWidget *w1 = new VBoxWidget(this);
    m_statckWg->addWidget(w1);

    VBoxWidget *w2 = new VBoxWidget(this);
    m_statckWg->addWidget(w2);

    mainLayout->addWidget(m_statckWg);

    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    this->setLayout(mainLayout);
}

VBoxWidget *CrossInfoDlg::widgetAt(const int &index)
{
    if(0<=index &&index<m_statckWg->count())
        return  static_cast<VBoxWidget *>(m_statckWg->widget(index));
    return 0;
}

void CrossInfoDlg::setCurrentIndex(const int &index)
{
    m_statckWg->setCurrentIndex(index);
}

void CrossInfoDlg::adjustPosition()
{
    QPoint pos(0,0);
    if(m_parentWg){
        pos = m_parentWg->mapToGlobal(QPoint(0,40));
    }
    this->move(pos);
}
