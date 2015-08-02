#include "analyser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Analyser w;
    w.show();

    return a.exec();
}
