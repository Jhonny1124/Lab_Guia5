#include "bomberman.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Bomberman w;
    w.show();
    return a.exec();
}
