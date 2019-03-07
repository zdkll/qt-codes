#include <QCoreApplication>

#include <QProcess>

int main(int argc, char *argv[])
{
    bool ret = QProcess::startDetached("shhzcjEx.exe --ppapi-flash-path=./pepflashplayer.dll --ppapi-flash-version=29.0.0.140");

    return ret?0:1;
}
