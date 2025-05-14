#include <QApplication>
#include "hash_gui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HashGui window;
    window.show();
    return app.exec();
}