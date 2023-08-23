#include "im.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IM w;
    w.show();
    return a.exec();
}
