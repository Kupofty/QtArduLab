#include "app_controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppController w;

    w.setWindowTitle("QtArduinoLab");
    w.show();
    return a.exec();
}
