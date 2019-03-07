#include <QSharedMemory>
#include "hzapplication.h"

int main(int argc, char *argv[])
{
    QSharedMemory shared_memory;
    shared_memory.setKey("shhzcjEx");
    if(shared_memory.attach())
    {
        return 0;
    }
    if(shared_memory.create(1))
    {
        HZApplication a;
        return a.initInstance(argc,argv);
    }
    return 0;
}
