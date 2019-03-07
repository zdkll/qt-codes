#include "indmenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IndMenuMaster w;
    w.show();

    return a.exec();
}
