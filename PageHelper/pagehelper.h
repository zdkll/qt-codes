#ifndef PAGEHELPER_H
#define PAGEHELPER_H

#include <QStackedWidget>

#include <ioccontainter.h>
#include <publicdef.h>
#include "pages.h"

class PageHelper : public QStackedWidget
{
public:
    PageHelper(QWidget *parent = Q_NULLPTR);

   void createPage(PageIdx,QWidget *parent = 0);

   void setCurPage(PageIdx idx);
private:
     IocContainter  m_Ioc;
     std::unordered_map<PageIdx,std::string> m_pageStrmp;
     std::unordered_map<PageIdx,BasePage *> m_pageMap;
};

#endif // PAGEHELPER_H
