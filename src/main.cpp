#include <MorusMainWindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MorusMainWindow w;
    w.show();
    
    int res = a.exec();
    return res;
}
