#include "pagefactory.h"
#include "homepage.h"
#include "webmainpage.h"
#include "optionstockpage.h"
#include "quotelistpage.h"
#include "blocklistpage.h"
#include "stockdetailpage.h"
#include "datacenterpage.h"
#include "strategycenterpage.h"
#include "infomasterpage.h"

#include "test_page.h"

PageFactory::PageFactory(QObject *parent) : QObject(parent)
{
   m_ioc.RegisterInherit<BasePage, HomePage, QWidget*, QString>("HomePage");
   m_ioc.RegisterInherit<BasePage, WebMainPage, QWidget*, QString>("WebMainPage");
   m_ioc.RegisterInherit<BasePage, OptionStockPage, QWidget*, QString>("OptionStockPage");
   m_ioc.RegisterInherit<BasePage, QuoteListPage, QWidget*, QString>("QuoteListPage");
   m_ioc.RegisterInherit<BasePage, BlockListPage, QWidget*, QString>("BlockListPage");
   m_ioc.RegisterInherit<BasePage, StockDetailPage, QWidget*, QString>("StockDetailPage");
   m_ioc.RegisterInherit<BasePage, DataCenterPage, QWidget*, QString>("DataCenterPage");
   m_ioc.RegisterInherit<BasePage, StrategyCenterPage, QWidget*, QString>("StrategyCenterPage");
   m_ioc.RegisterInherit<BasePage, InfoMasterPage, QWidget*, QString>("InfoMasterPage");

   //m_ioc.RegisterInherit<BasePage, test_page, QWidget*, QString>("test_page");
}

IocContainer *PageFactory::getIoc()
{
    static PageFactory isntance;
    return &(isntance.m_ioc);
}
