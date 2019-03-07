#ifndef TEST_PAGE_H
#define TEST_PAGE_H
#include "hzcomponent_global.h"
#include "basepage.h"

class HZCOMPONENTSHARED_EXPORT  test_page : public BasePage
{
public:
    test_page(QWidget *parent = 0,QString instName="");
};

#endif // TEST_PAGE_H
