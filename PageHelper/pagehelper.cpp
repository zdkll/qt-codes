#include "pagehelper.h"

#include <iostream>
#include <pages.h>

PageHelper::PageHelper(QWidget *parent)
    :QStackedWidget(parent)
{
    m_Ioc.RegisterInherit<BasePage,HomePage,QWidget*>("HomePage");
    m_Ioc.RegisterInherit<BasePage,StockPage,QWidget*>("StockPage");
    m_Ioc.RegisterInherit<BasePage,DataPage,QWidget*>("DataPage");

    m_pageStrmp.emplace(Home_Page,"HomePage");
    m_pageStrmp.emplace(Stock_Page,"StockPage");
    m_pageStrmp.emplace(Data_Page,"DataPage");
}

void PageHelper::createPage(PageIdx idx,QWidget *parent )
{
    std::cout<<m_pageStrmp.find(idx)->second<<std::endl;

    BasePage *page = m_Ioc.Resolve<BasePage>(m_pageStrmp.find(idx)->second,parent);
    this->addWidget(page);
    m_pageMap.emplace(idx,page);
}

void PageHelper::setCurPage(PageIdx idx)
{
    this->setCurrentWidget(m_pageMap.find(idx)->second);
}

