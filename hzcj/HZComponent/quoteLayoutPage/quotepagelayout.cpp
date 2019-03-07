#include "quotepagelayout.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "quotepanelwnd.h"
#include "stockdetailbasewnd.h"

QuotePageLayout::QuotePageLayout(BaseWidget* pOwnerWnd, QObject *parent):
    QObject(parent),
    m_ownerWnd(pOwnerWnd)
{

}

void QuotePageLayout::layout(StockDetailBaseWnd *midWnd, StockDetailBaseWnd *midBottomWnd, StockDetailBaseWnd *leftWnd, QuotePanelWnd *rightWnd)
{
   m_midWnd = midWnd;
   m_midBottomWnd = midBottomWnd;
   m_leftWnd = leftWnd;
   m_rightWnd = rightWnd;

   QHBoxLayout* pLayout = new QHBoxLayout;
   pLayout->setMargin(0);
   pLayout->setSpacing(0);
   m_leftWnd->setFixedWidth(QuoteLeftWndWidth);
   pLayout->addWidget(m_leftWnd);

   QVBoxLayout* pMidLayout = new QVBoxLayout;
   pMidLayout->setSpacing(0);
   pMidLayout->setMargin(0);
   pMidLayout->addWidget(m_midWnd);
   m_midBottomWnd->setFixedHeight(QuoteBottomWndHight);
   pMidLayout->setSpacing(1);
   pMidLayout->addWidget(m_midBottomWnd);
   QWidget* pMid = new QWidget;
   pMid->setLayout(pMidLayout);
   pLayout->setSpacing(1);
   pLayout->addWidget(pMid);
   pLayout->setSpacing(1);

   m_rightWnd->setFixedWidth(QuoteRightWndWidth);
   pLayout->addWidget(m_rightWnd);
   m_ownerWnd->setLayout(pLayout);
}

void QuotePageLayout::slotsHideLeftWnd(bool bHide)
{
    if(m_leftWnd)
    {
        m_leftWnd->hideWnd(bHide);
    }
}

void QuotePageLayout::slotsHideMidBottomWnd(bool bHide)
{
    if(m_midBottomWnd)
    {
        m_midBottomWnd->hideWnd(bHide);
    }
}

void QuotePageLayout::slotsHideRightWnd(bool bHide)
{
    if(m_rightWnd)
    {
        m_rightWnd->hideWnd(bHide);
    }
}
